/*
 * Copyright (C) 2011-2013 The Android Open Source Project
 * Copyright (C) 2016 The CyanogenMod Project
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

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <cutils/klog.h>
#include <batteryservice/BatteryService.h>
#include <cutils/android_reboot.h>
#include "healthd/healthd.h"
#include "minui/minui.h"

#define ARRAY_SIZE(x)          (sizeof(x)/sizeof(x[0]))

#define BACKLIGHT_PATH         "/sys/class/leds/lcd-backlight/brightness"
#define BACKLIGHT_ON_LEVEL     100
#define CHARGING_ENABLED_PATH  "/sys/class/power_supply/battery/charging_enabled"
#define STR_LEN                8

#define RED_LED_PATH           "/sys/class/leds/led:rgb_red/brightness"
#define GREEN_LED_PATH         "/sys/class/leds/led:rgb_green/brightness"
#define BLUE_LED_PATH          "/sys/class/leds/led:rgb_blue/brightness"

#define LOGV(x...) do { KLOG_DEBUG("charger", x); } while (0)
#define LOGE(x...) do { KLOG_ERROR("charger", x); } while (0)
#define LOGW(x...) do { KLOG_WARNING("charger", x); } while (0)

enum {
    RED_LED = 0x01 << 0,
    GREEN_LED = 0x01 << 1,
    BLUE_LED = 0x01 << 2,
};

struct led_ctl {
    int color;
    const char *path;
};

struct led_ctl leds[3] =
    {{RED_LED, RED_LED_PATH},
    {GREEN_LED, GREEN_LED_PATH},
    {BLUE_LED, BLUE_LED_PATH}};

struct soc_led_color_mapping {
    int soc;
    int color;
};

struct soc_led_color_mapping soc_leds[3] = {
    {15, RED_LED},
    {90, RED_LED | GREEN_LED},
    {100, GREEN_LED},
};

static int set_tricolor_led(int on, int color)
{
    int fd, i;
    char buffer[10];

    for (i = 0; i < (int)ARRAY_SIZE(leds); i++) {
        if ((color & leds[i].color) && (access(leds[i].path, R_OK | W_OK) == 0)) {
            fd = open(leds[i].path, O_RDWR);
            if (fd < 0) {
                LOGE("Could not open led node %d\n", i);
                continue;
            }
            if (on)
                snprintf(buffer, sizeof(int), "%d\n", 255);
            else
                snprintf(buffer, sizeof(int), "%d\n", 0);

            if (write(fd, buffer, strlen(buffer)) < 0)
                LOGE("Could not write to led node\n");
            if (fd >= 0)
                close(fd);
        }
    }
    return 0;
}

static int set_battery_soc_leds(int soc)
{
    int i, color;
    static int old_color = 0;

    for (i = 0; i < (int)ARRAY_SIZE(soc_leds); i++) {
        if (soc <= soc_leds[i].soc)
            break;
    }
    color = soc_leds[i].color;
    if (old_color != color) {
        set_tricolor_led(0, old_color);
        set_tricolor_led(1, color);
        old_color = color;
        LOGV("soc = %d, set led color 0x%x\n", soc, soc_leds[i].color);
    }
    return 0;
}

void healthd_board_mode_charger_draw_battery(
                struct android::BatteryProperties *batt_prop)
{
    char cap_str[STR_LEN];
    int x, y;
    int str_len_px;
    static int char_height = -1, char_width = -1;

    if (char_height == -1 && char_width == -1)
        gr_font_size(gr_sys_font(), &char_width, &char_height);
    snprintf(cap_str, (STR_LEN - 1), "%d%%", batt_prop->batteryLevel);
    str_len_px = gr_measure(gr_sys_font(), cap_str);
    x = (gr_fb_width() - str_len_px) / 2;
    y = (gr_fb_height() + char_height) / 2;
    gr_color(0, 0, 0, 255);
    gr_text(gr_sys_font(), x, y, cap_str, 0);
}

void healthd_board_mode_charger_battery_update(
                struct android::BatteryProperties *batt_prop)
{
    static int old_soc = 0;
    int soc = 0;

    if (batt_prop && batt_prop->batteryLevel >= 0) {
        soc = batt_prop->batteryLevel;
    }

    if (old_soc != soc) {
        old_soc = soc;
        set_battery_soc_leds(soc);
    }
}

void healthd_board_mode_charger_set_backlight(bool enable)
{
    int fd;
    char buffer[10];

    if (access(BACKLIGHT_PATH, R_OK | W_OK) != 0) {
        LOGW("Backlight control not support\n");
        return;
    }

    memset(buffer, '\0', sizeof(buffer));
    fd = open(BACKLIGHT_PATH, O_RDWR);
    if (fd < 0) {
        LOGE("Could not open backlight node : %s\n", strerror(errno));
        goto cleanup;
    }
    LOGV("Enabling backlight\n");
    snprintf(buffer, sizeof(buffer), "%d\n", enable ? BACKLIGHT_ON_LEVEL : 0);
    if (write(fd, buffer,strlen(buffer)) < 0) {
        LOGE("Could not write to backlight node : %s\n", strerror(errno));
        goto cleanup;
    }
cleanup:
    if (fd >= 0)
        close(fd);
}

void healthd_board_mode_charger_init()
{
    int ret;
    char buff[8] = "\0";
    int charging_enabled = 0;
    int fd;

    /* check the charging is enabled or not */
    fd = open(CHARGING_ENABLED_PATH, O_RDONLY);
    if (fd < 0)
        return;
    ret = read(fd, buff, sizeof(buff));
    close(fd);
    if (ret > 0 && sscanf(buff, "%d\n", &charging_enabled)) {
        /* if charging is disabled, reboot and exit power off charging */
        if (charging_enabled)
            return;
        LOGW("android charging is disabled, exit!\n");
        android_reboot(ANDROID_RB_RESTART, 0, 0);
    }
}

void healthd_board_init(struct healthd_config*)
{
    // use defaults
}

int healthd_board_battery_update(
                struct android::BatteryProperties*)
{
    // return 0 to log periodic polled battery status to kernel log
    return 1;
}
