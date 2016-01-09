/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (C) 2013 Sony Mobile Communications AB
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
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <hardware/lights.h>

static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

enum {
	LED_SHARED_ATTENTION,
	LED_SHARED_NOTIFICATION,
	LED_SHARED_BATTERY,
	LED_SHARED_BLUETOOTH,
	_LED_SHARED_CNT,
};

static struct light_state_t g_led_shared[_LED_SHARED_CNT];

enum led_ident {
	LED_RED,
	LED_GREEN,
	LED_BLUE,
	LED_BACKLIGHT,
	LED_BKLT_MDSS
};

static struct led_desc {
	int max_brightness;
	const char *max_brightness_s;
	const char *brightness;
	const char *blink;
	const char *pwm;
	const char *step;
} led_descs[] = {
	[LED_BACKLIGHT] = {
		.max_brightness = 0,
		.max_brightness_s = "/sys/class/leds/wled:backlight/max_brightness",
		.brightness = "/sys/class/leds/wled:backlight/brightness",
	},
	[LED_BKLT_MDSS] = {
		.max_brightness = 0,
		.max_brightness_s = "/sys/class/leds/lcd-backlight/max_brightness",
		.brightness = "/sys/class/leds/lcd-backlight/brightness",
	},
	[LED_RED] = {
		.max_brightness = 0,
		.max_brightness_s = "/sys/class/leds/led:rgb_red/max_brightness",
		.brightness = "/sys/class/leds/led:rgb_red/brightness",
		.blink = "/sys/class/leds/led:rgb_red/blink",
		.pwm = "/sys/class/leds/led:rgb_red/duty_pcts",
		.step = "/sys/class/leds/led:rgb_red/ramp_step_ms",
	},
	[LED_GREEN] = {
		.max_brightness = 0,
		.max_brightness_s = "/sys/class/leds/led:rgb_green/max_brightness",
		.brightness = "/sys/class/leds/led:rgb_green/brightness",
		.blink = "/sys/class/leds/led:rgb_green/blink",
		.pwm = "/sys/class/leds/led:rgb_green/duty_pcts",
		.step = "/sys/class/leds/led:rgb_green/ramp_step_ms",
	},
	[LED_BLUE] = {
		.max_brightness = 0,
		.max_brightness_s = "/sys/class/leds/led:rgb_blue/max_brightness",
		.brightness = "/sys/class/leds/led:rgb_blue/brightness",
		.blink = "/sys/class/leds/led:rgb_blue/blink",
		.pwm = "/sys/class/leds/led:rgb_blue/duty_pcts",
		.step = "/sys/class/leds/led:rgb_blue/ramp_step_ms",
	},
};

struct light {
	char name[256];
	struct light_device_t dev;
	int shared_which;
	struct light *next;
};

static struct light *g_light_head;

#define PWM_PATTERN_LEN 16

static const float pwm_patterns[PWM_PATTERN_LEN][PWM_PATTERN_LEN] = {
	{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
	{0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
	{0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
	{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
	{0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0},
	{0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
	{0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

#define container_of(ptr, type, member) \
  ((type *)( (char *)(ptr) - offsetof(type,member)))
#define ll_light(dev) container_of(dev, struct light, dev)

static struct light *ll_find(const char *name)
{
	struct light *light;

	pthread_mutex_lock(&g_lock);
	light = g_light_head;
	while (light != NULL) {
		if (!strcmp(light->name, name)) {
			pthread_mutex_unlock(&g_lock);
			return light;
		}
		light = light->next;
	}
	pthread_mutex_unlock(&g_lock);
	return NULL;
}

static void ll_add(struct light *light)
{
	pthread_mutex_lock(&g_lock);
	light->next = g_light_head;
	g_light_head = light;
	pthread_mutex_unlock(&g_lock);
}

static void ll_remove(struct light *light)
{
	struct light *prev;
	struct light *iter;

	pthread_mutex_lock(&g_lock);
	iter = g_light_head;
	prev = NULL;
	while (iter != light) {
		prev = iter;
		iter = iter->next;
	}

	if (iter != NULL) {
		if (prev == NULL)
			g_light_head = iter->next;
		else
			prev->next = iter->next;
	}

	pthread_mutex_unlock(&g_lock);
}

static int write_string(const char *path, const char *value)
{
	static int already_warned = 0;
	int rc;
	int fd;

	fd = open(path, O_WRONLY);
	if (fd < 0) {
		if (already_warned == 0) {
			ALOGE("write_string failed to open %s\n", path);
			already_warned = 1;
		}
		return -1;
	}

	rc = write(fd, value, strlen(value));
	close(fd);
	if (rc < 0)
		return -1;

	return 0;
}

static int write_int(const char *path, int value)
{
	char buffer[20];

	snprintf(buffer, sizeof(buffer), "%d\n", value);
	return write_string(path, buffer);
}

static int read_int(const char *path)
{
	static int already_warned = 0;
	char buffer[12];
	int fd, rc;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		if (already_warned == 0) {
			ALOGE("read_int failed to open %s\n", path);
			already_warned = 1;
		}
		return -1;
	}

	rc = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (rc <= 0)
		return -1;

	buffer[rc] = 0;

	return strtol(buffer, 0, 0);
}

static void write_led_scaled(enum led_ident id, int brightness,
		int pwm_pattern_index, unsigned int duration)
{
	int max_brightness = read_int(led_descs[id].max_brightness_s);
	int scaled;

	if (brightness > max_brightness)
		scaled = max_brightness;
	else
		scaled = brightness;
#ifdef HAS_DIM_BACKLIGHT
		if (id == LED_BACKLIGHT)
			scaled = brightness == 0 ? 0 : ((brightness + 255) / 2);
#endif

	if (pwm_pattern_index >= 0 && led_descs[id].pwm) {
		int i;
		int pwm_pattern_values[PWM_PATTERN_LEN];
		char pwm_pattern[256];
		for (i = 0; i < PWM_PATTERN_LEN; i++) {
			pwm_pattern_values[i] = (int)(pwm_patterns[pwm_pattern_index][i] * scaled);
		}

		sprintf(pwm_pattern, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			pwm_pattern_values[0], pwm_pattern_values[1], pwm_pattern_values[2], pwm_pattern_values[3],
			pwm_pattern_values[4], pwm_pattern_values[5], pwm_pattern_values[6], pwm_pattern_values[7],
			pwm_pattern_values[8], pwm_pattern_values[9], pwm_pattern_values[10], pwm_pattern_values[11],
			pwm_pattern_values[12], pwm_pattern_values[13], pwm_pattern_values[14], pwm_pattern_values[15]);
		write_string(led_descs[id].pwm, pwm_pattern);
		write_int(led_descs[id].step, duration);
		write_int(led_descs[id].blink, 1);
	} else {
		write_int(led_descs[id].brightness, scaled);
	}
}

static int is_lit(struct light_state_t const* state)
{
	return state->color & 0x00ffffff;
}

static int rgb_to_brightness(struct light_state_t const* state)
{
	int color = state->color & 0x00ffffff;
	return ((77*((color>>16)&0x00ff))
			+ (150*((color>>8)&0x00ff)) + (29*(color&0x00ff))) >> 8;
}

static int set_light_backlight(struct light_device_t *dev, struct light_state_t const *state)
{
	int brightness = rgb_to_brightness(state);

	pthread_mutex_lock(&g_lock);
	write_led_scaled(LED_BACKLIGHT, brightness, -1, 0);
	pthread_mutex_unlock(&g_lock);

	return 0;
}

static int set_light_mdss(struct light_device_t *dev, struct light_state_t const *state)
{
	int brightness = rgb_to_brightness(state);

	pthread_mutex_lock(&g_lock);
	write_led_scaled(LED_BKLT_MDSS, brightness, -1, 0);
	pthread_mutex_unlock(&g_lock);

	return 0;
}

static int calc_pattern(int on, int off, int *duration)
{
	int index;

	index = on * 15 / (on + off);
	*duration = on / (index + 1);
	return index;
}

static int set_light_shared(struct light_device_t *dev, struct light_state_t const* state)
{
	struct light *light = ll_light(dev);
	int pwm_pattern_index;
	int duration;
	int i;

	pthread_mutex_lock(&g_lock);
	g_led_shared[light->shared_which] = *state;

	for (i = 0; i < _LED_SHARED_CNT; ++i) {
		state = &g_led_shared[i];
		if (is_lit(state))
			break;
	}

	if (state->flashMode != LIGHT_FLASH_NONE && state->flashOffMS != 0) {
		ALOGD("led [%d,%d] = %08x\n",
				state->flashOnMS, state->flashOffMS,
				state->color);
		pwm_pattern_index = calc_pattern(state->flashOnMS,
				state->flashOffMS, &duration);
	} else {
		ALOGD("led [solid] = %08x\n", state->color);
		duration = 0;
		pwm_pattern_index = -1;
	}

	write_led_scaled(LED_RED, (state->color >> 16) & 0xFF, pwm_pattern_index, duration);
	write_led_scaled(LED_GREEN, (state->color >> 8) & 0xFF, pwm_pattern_index, duration);
	write_led_scaled(LED_BLUE, (state->color) & 0xFF, pwm_pattern_index, duration);

	pthread_mutex_unlock(&g_lock);
	return 0;
}

static int close_lights(struct light_device_t *dev)
{
	struct light *light = ll_light(dev);
	ALOGI("closing light \"%s\"\n", light->name);
	ll_remove(light);
	free(light);
	return 0;
}

static int open_lights(const struct hw_module_t* module,
		char const* name, struct hw_device_t** device)
{
	struct light_state_t light_off = { 0 };
	struct light *light;
	struct stat buf;
	int shared_which;
	int (*set_light)(struct light_device_t* dev,
					 struct light_state_t const *state);

	light = ll_find(name);
	if (light != NULL) {
		ALOGW("light \"%s\" already opened, returning old handle\n", name);
		*device = (struct hw_device_t *)&light->dev;
		return 0;
	}

	if (strcmp(name, LIGHT_ID_BACKLIGHT) == 0) {
		if (stat(led_descs[LED_BACKLIGHT].brightness, &buf) == 0)
			set_light = set_light_backlight;
		else
			set_light = set_light_mdss;
		shared_which = -1;
	} else if (strcmp(name, LIGHT_ID_BATTERY) == 0) {
		set_light = set_light_shared;
		shared_which = LED_SHARED_BATTERY;
	} else if (strcmp(name, LIGHT_ID_BLUETOOTH) == 0) {
		set_light = set_light_shared;
		shared_which = LED_SHARED_BLUETOOTH;
	} else if (strcmp(name, LIGHT_ID_ATTENTION) == 0) {
		set_light = set_light_shared;
		shared_which = LED_SHARED_ATTENTION;
	} else if (strcmp(name, LIGHT_ID_NOTIFICATIONS) == 0) {
		set_light = set_light_shared;
		shared_which = LED_SHARED_NOTIFICATION;
	} else {
		ALOGI("don't have light \"%s\"\n", name);
		return -EINVAL;
	}

	ALOGI("registering light \"%s\"\n", name);

	light = calloc(1, sizeof(struct light));
	if (light == NULL)
		return -ENOMEM;

	light->dev.common.tag		= HARDWARE_DEVICE_TAG;
	light->dev.common.version	= 0;
	light->dev.common.module	= (struct hw_module_t*)module;
	light->dev.common.close	= (int (*)(struct hw_device_t*))close_lights;
	light->dev.set_light		= set_light;
	light->shared_which		= shared_which;
	strncpy(light->name, name, sizeof(light->name));
	light->name[sizeof(light->name) - 1] = 0;

	ll_add(light);

	if (set_light != set_light_backlight)
		(* set_light)(&light->dev, &light_off);

	*device = (struct hw_device_t *)&light->dev;

	return 0;
}

static struct hw_module_methods_t lights_module_methods = {
	.open = open_lights,
};

struct hw_module_t HAL_MODULE_INFO_SYM = {
	.tag = HARDWARE_MODULE_TAG,
	.version_major = 1,
	.version_minor = 0,
	.id = LIGHTS_HARDWARE_MODULE_ID,
	.name = "Sony lights module",
	.author = "Bjorn Andersson <bjorn.andersson@sonymobile.com>",
	.methods = &lights_module_methods,
};
