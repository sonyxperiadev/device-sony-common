/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (C) 2014 The  Linux Foundation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "lights.sony"

#include <cutils/log.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>

#include <linux/msm_mdp.h>

#include <sys/ioctl.h>
#include <sys/types.h>

#include <cutils/properties.h>
#include <hardware/lights.h>

/******************************************************************************/

#define DEFAULT_LOW_PERSISTENCE_MODE_BRIGHTNESS 255

static pthread_once_t g_init = PTHREAD_ONCE_INIT;
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t g_lcd_lock = PTHREAD_MUTEX_INITIALIZER;
static struct light_state_t g_notification;
static int g_last_backlight_mode = BRIGHTNESS_MODE_USER;
static struct light_state_t g_battery;
static short backlight_bits = 8;

char const*const RED_LED_FILE
		= "/sys/class/leds/led:rgb_red/brightness";

char const*const GREEN_LED_FILE
		= "/sys/class/leds/led:rgb_green/brightness";

char const*const BLUE_LED_FILE
		= "/sys/class/leds/led:rgb_blue/brightness";

char const*const LCD_FILE
		= "/sys/class/leds/lcd-backlight/brightness";

char const*const LCD_MAX_FILE
		= "/sys/class/leds/lcd-backlight/max_brightness";

char const*const RED_BLINK_FILE
		= "/sys/class/leds/led:rgb_red/blink";

char const*const GREEN_BLINK_FILE
		= "/sys/class/leds/led:rgb_green/blink";

char const*const BLUE_BLINK_FILE
		= "/sys/class/leds/led:rgb_blue/blink";

char const*const DISPLAY_FB_DEV_PATH
		= "/dev/graphics/fb0";

char const*const BACKLIGHT_STATUS_PROPERTY
		= "sys.backlight_on";

/**
 * device methods
 */
static int read_int(char const* path);

void init_globals(void)
{
	// init the mutex
	pthread_mutex_init(&g_lock, NULL);
	pthread_mutex_init(&g_lcd_lock, NULL);

	backlight_bits = (read_int(LCD_MAX_FILE) == 4095 ? 12 : 8);
}

static int
write_int(char const* path, int value)
{
	int fd;
	static int already_warned = 0;

	fd = open(path, O_WRONLY);
	if (fd >= 0) {
		char buffer[20] = {0,};
		int bytes = snprintf(buffer, sizeof(buffer), "%d\n", value);
		ssize_t amt = write(fd, buffer, (size_t)bytes);
		close(fd);
		return amt == -1 ? -errno : 0;
	} else {
		if (already_warned == 0) {
			ALOGE("write_int failed to open %s\n", path);
			already_warned = 1;
		}
		return -errno;
	}
}

static int
read_int(char const* path)
{
	static int already_warned = 0;
	int fd;

	fd = open(path, O_RDONLY);
	if (fd >= 0) {
		char read_str[10] = {0,0,0,0,0,0,0,0,0,0};
		ssize_t err = read(fd, &read_str, sizeof(read_str));
		close(fd);
		return err < 2 ? -errno : atoi(read_str);
	} else {
		if (already_warned == 0) {
			ALOGE("read_int failed to open %s\n", path);
			already_warned = 1;
		}
		return -errno;
	};
}

static int
is_lit(struct light_state_t const* state)
{
	return state->color & 0x00ffffff;
}

static int
rgb_to_brightness(struct light_state_t const* state)
{
	int color = state->color & 0x00ffffff;
	return ((77 * ((color >> 16) & 0x00ff))
			+ (150 * ((color >> 8) & 0x00ff)) + (29 * (color & 0x00ff))) >> 8;
}

static int
set_light_backlight(struct light_device_t* dev,
		struct light_state_t const* state)
{
	int err = 0;
	int brightness = rgb_to_brightness(state);
	unsigned int lpEnabled = state->brightnessMode == BRIGHTNESS_MODE_LOW_PERSISTENCE;
	char prop[PROPERTY_VALUE_MAX];

	if(!dev) {
		return -1;
	}

	pthread_mutex_lock(&g_lcd_lock);

#ifdef LOW_PERSISTENCE_DISPLAY
	// If we're not in lp mode and it has been enabled or if we are in lp mode
	// and it has been disabled send an ioctl to the display with the update
	if ((g_last_backlight_mode != state->brightnessMode && lpEnabled) ||
			(!lpEnabled && g_last_backlight_mode == BRIGHTNESS_MODE_LOW_PERSISTENCE)) {
		int fd = -1;
		fd = open(DISPLAY_FB_DEV_PATH, O_RDWR);
		if (fd >= 0) {
			if ((err = ioctl(fd, MSMFB_SET_PERSISTENCE_MODE, &lpEnabled)) != 0) {
				ALOGE("%s: Failed in ioctl call to %s: %s\n", __FUNCTION__, DISPLAY_FB_DEV_PATH,
						strerror(errno));
				err = -1;
			}
			close(fd);

			brightness = DEFAULT_LOW_PERSISTENCE_MODE_BRIGHTNESS;
		} else {
			ALOGE("%s: Failed to open %s: %s\n", __FUNCTION__, DISPLAY_FB_DEV_PATH,
					strerror(errno));
			err = -1;
		}
	}


	g_last_backlight_mode = state->brightnessMode;
#endif

	if (!err) {
		if (backlight_bits > 8)
			brightness = brightness << (backlight_bits - 8);

		err = write_int(LCD_FILE, brightness);

		if (!err) {
			memset(prop, 0x0, PROPERTY_VALUE_MAX);
			property_get(BACKLIGHT_STATUS_PROPERTY, prop, "0");
			if ((strcmp(prop, "0") == 0) && (brightness > 0)) {
				property_set(BACKLIGHT_STATUS_PROPERTY, "1");
				ALOGI("%s: %s = 1 - brightness: %d", __FUNCTION__,
						BACKLIGHT_STATUS_PROPERTY, brightness);
			} else {
				if ((strcmp(prop, "1") == 0) && (brightness == 0)) {
					property_set(BACKLIGHT_STATUS_PROPERTY, "0");
					ALOGI("%s: %s = 0 - brightness: %d", __FUNCTION__,
							BACKLIGHT_STATUS_PROPERTY, brightness);
				}
			}
		}
	}

	pthread_mutex_unlock(&g_lcd_lock);
	return err;
}

static int
set_speaker_light_locked(struct light_device_t* dev,
		struct light_state_t const* state)
{
	int red, green, blue;
	int blink;
	int onMS, offMS;
	unsigned int colorRGB;

	if(!dev) {
		return -1;
	}

	switch (state->flashMode) {
		case LIGHT_FLASH_TIMED:
			onMS = state->flashOnMS;
			offMS = state->flashOffMS;
			break;
		case LIGHT_FLASH_NONE:
		default:
			onMS = 0;
			offMS = 0;
			break;
	}

	colorRGB = state->color;

#if 0
	ALOGD("set_speaker_light_locked mode %d, colorRGB=%08X, onMS=%d, offMS=%d\n",
			state->flashMode, colorRGB, onMS, offMS);
#endif

	red = (colorRGB >> 16) & 0xFF;
	green = (colorRGB >> 8) & 0xFF;
	blue = colorRGB & 0xFF;

	if (onMS > 0 && offMS > 0) {
		/*
		 * if ON time == OFF time
		 *   use blink mode 2
		 * else
		 *   use blink mode 1
		 */
		if (onMS == offMS)
			blink = 2;
		else
			blink = 1;
	} else {
		blink = 0;
	}

	if (blink) {
		if (red) {
			if (write_int(RED_BLINK_FILE, blink))
				write_int(RED_LED_FILE, 0);
	}
		if (green) {
			if (write_int(GREEN_BLINK_FILE, blink))
				write_int(GREEN_LED_FILE, 0);
	}
		if (blue) {
			if (write_int(BLUE_BLINK_FILE, blink))
				write_int(BLUE_LED_FILE, 0);
	}
	} else {
		write_int(RED_LED_FILE, red);
		write_int(GREEN_LED_FILE, green);
		write_int(BLUE_LED_FILE, blue);
	}

	return 0;
}

static void
handle_speaker_battery_locked(struct light_device_t* dev)
{
	if (is_lit(&g_battery)) {
		set_speaker_light_locked(dev, &g_battery);
	} else {
		set_speaker_light_locked(dev, &g_notification);
	}
}

static int
set_light_battery(struct light_device_t* dev,
		struct light_state_t const* state)
{
	if(!dev) {
		return -1;
	}

	pthread_mutex_lock(&g_lock);
	g_battery = *state;
	handle_speaker_battery_locked(dev);
	pthread_mutex_unlock(&g_lock);
	return 0;
}

static int
set_light_notifications(struct light_device_t* dev,
		struct light_state_t const* state)
{
	if(!dev) {
		return -1;
	}

	pthread_mutex_lock(&g_lock);
	g_notification = *state;
	handle_speaker_battery_locked(dev);
	pthread_mutex_unlock(&g_lock);
	return 0;
}

/** Close the lights device */
static int
close_lights(struct light_device_t *dev)
{
	if (dev) {
		free(dev);
	}
	return 0;
}


/******************************************************************************/

/**
 * module methods
 */

/** Open a new instance of a lights device using name */
static int open_lights(const struct hw_module_t* module, char const* name,
		struct hw_device_t** device)
{
	int (*set_light)(struct light_device_t* dev,
			struct light_state_t const* state);

	if (0 == strcmp(LIGHT_ID_BACKLIGHT, name))
		set_light = set_light_backlight;
	else if (0 == strcmp(LIGHT_ID_BATTERY, name))
		set_light = set_light_battery;
	else if (0 == strcmp(LIGHT_ID_NOTIFICATIONS, name))
		set_light = set_light_notifications;
	else
		return -EINVAL;

	pthread_once(&g_init, init_globals);

	struct light_device_t *dev = malloc(sizeof(struct light_device_t));

	if(!dev)
		return -ENOMEM;

	memset(dev, 0, sizeof(*dev));

	dev->common.tag = HARDWARE_DEVICE_TAG;
#ifdef LOW_PERSISTENCE_DISPLAY
	dev->common.version = LIGHTS_DEVICE_API_VERSION_2_0;
#else
	dev->common.version = 0;
#endif
	dev->common.module = (struct hw_module_t*)module;
	dev->common.close = (int (*)(struct hw_device_t*))close_lights;
	dev->set_light = set_light;

	*device = (struct hw_device_t*)dev;
	return 0;
}

static struct hw_module_methods_t lights_module_methods = {
	.open =  open_lights,
};

/*
 * The lights Module
 */
struct hw_module_t HAL_MODULE_INFO_SYM = {
	.tag = HARDWARE_MODULE_TAG,
	.version_major = 1,
	.version_minor = 0,
	.id = LIGHTS_HARDWARE_MODULE_ID,
	.name = "Sony lights Module",
	.author = "Google, Inc.",
	.methods = &lights_module_methods,
};
