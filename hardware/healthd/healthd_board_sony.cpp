/*
 * Copyright (C) 2011-2013 The Android Open Source Project
 * Copyright (c) 2015-2017 The Linux Foundation. All rights reserved.
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
#include <sys/reboot.h>
#include "healthd/healthd.h"
#include "minui/minui.h"

#define ARRAY_SIZE(x)                 (sizeof(x)/sizeof(x[0]))

#define BACKLIGHT_PATH                "/sys/class/leds/lcd-backlight/brightness"
#define CHARGING_ENABLED_PATH         "/sys/class/power_supply/battery/charging_enabled"

#define RED_LED_PATH                  "/sys/class/leds/led:rgb_red/brightness"
#define GREEN_LED_PATH                "/sys/class/leds/led:rgb_green/brightness"
#define BLUE_LED_PATH                 "/sys/class/leds/led:rgb_blue/brightness"

#define USB_MAX_CURRENT_PATH          "/sys/class/power_supply/usb/current_max"
#define USB_TYPEC_MODE_PATH           "/sys/class/power_supply/usb/typec_mode"
#define CHARGER_TYPE_PATH             "/sys/class/power_supply/usb/type"

#define BMS_READY_PATH                "/sys/class/power_supply/bms/soc_reporting_ready"

#define CHGR_TAG                      "charger_sony"

#define BACKLIGHT_ON_LEVEL            100
#define BACKLIGHT_OFF_LEVEL           0
#define STR_LEN                       8
#define USB500_UA                     500000
#define WAIT_BMS_READY_TIMES_MAX      200
#define WAIT_BMS_READY_INTERVAL_USEC  200000

#define LOGV(tag, x...) do { KLOG_DEBUG(tag, x); } while (0)
#define LOGE(tag, x...) do { KLOG_ERROR(tag, x); } while (0)
#define LOGW(tag, x...) do { KLOG_WARNING(tag, x); } while (0)

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

static int read_file(char const* path, char* buff, ssize_t size)
{
    int fd, rc;

    fd = open(path, O_RDONLY);
    if (fd < 0)
        return fd;

    rc = read(fd, buff, size - 1);
    if (rc < 0) {
        close(fd);
        return rc;
    }

    close(fd);
    buff[rc] = '\0';

    return rc;
}

static int read_int_from_file(char const* path, int* value)
{
    int rc;
    char buff[32];

    rc = read_file(path, buff, sizeof(buff));
    if (rc < 0)
        return rc;

    sscanf(buff, "%d\n", value);

    return rc;
}

static int write_file(char const*path, const char *buff, ssize_t size)
{
    int fd, rc = -1;

    fd = open(path,O_WRONLY);
    if (fd < 0)
        return fd;

    rc = write(fd, buff, size);
    close(fd);

    return rc > 0 ? 0 : -1;
}

static int write_file_int(char const* path, int value)
{
    int rc = -1, bytes;
    char buff[32];

    bytes = snprintf(buff, (sizeof(buff) - 1), "%d\n", value);
    buff[bytes] = '\0';
    rc = write_file(path, buff, bytes + 1);

    return rc;
}

static int set_tricolor_led(int on, int color)
{
    int rc, i;
    char buffer[10];

    for (i = 0; i < (int)ARRAY_SIZE(leds); i++) {
        if ((color & leds[i].color) && (access(leds[i].path, R_OK | W_OK) == 0)) {
            rc = write_file_int(leds[i].path, on ? 255 : 0);
            if (rc < 0)
                return rc;
        }
    }
    return 0;
}

static int set_battery_soc_leds(int soc)
{
    int i, color, rc;
    static int old_color = 0;

    for (i = 0; i < (int)ARRAY_SIZE(soc_leds); i++) {
        if (soc <= soc_leds[i].soc)
            break;
    }
    color = soc_leds[i].color;
    if (old_color != color) {
        rc = set_tricolor_led(0, old_color);
        if (rc < 0)
           LOGE(CHGR_TAG, "Error in setting old_color on tricolor_led\n");
        rc = set_tricolor_led(1, color);
        if (rc < 0)
           LOGE(CHGR_TAG, "Error in setting color on tricolor_led\n");
        if (!rc) {
           old_color = color;
           LOGV(CHGR_TAG, "soc = %d, set led color 0x%x\n", soc, soc_leds[i].color);
        }
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

void healthd_board_mode_charger_set_backlight(bool en)
{
    int rc;

    if (access(BACKLIGHT_PATH, R_OK | W_OK) != 0) {
        LOGW(CHGR_TAG, "Backlight control not support\n");
        return;
    }

    rc = write_file_int(BACKLIGHT_PATH, en ? BACKLIGHT_ON_LEVEL : BACKLIGHT_OFF_LEVEL);
    if (rc < 0) {
        LOGE(CHGR_TAG, "Could not write to backlight node : %s\n", strerror(errno));
        return;
    }
    LOGV(CHGR_TAG, "set backlight status to %d\n", en);
}

void healthd_board_mode_charger_init()
{
    int ret;
    char buff[40] = "\0";
    char *pos;
    int charging_enabled = 0;
    int bms_ready = 0;
    int wait_count = 0;
    int fd;
    bool usb_type_is_sdp = false, typec_default_src = false;

    /* check the charging is enabled or not */
    ret = read_int_from_file(CHARGING_ENABLED_PATH, &charging_enabled);
    if (ret >= 0) {
         LOGW(CHGR_TAG, "android charging is %s\n",
                 !!charging_enabled ? "enabled" : "disabled");
         /* if charging is disabled, reboot and exit power off charging */
         if (!charging_enabled)
             reboot(RB_AUTOBOOT);
    }
    ret = read_file(CHARGER_TYPE_PATH, buff, sizeof(buff));
    if (ret >= 0) {
        /* get rid of the new line charcter */
        buff[strcspn(buff, "\n")] = '\0';
        if (!strcmp(buff, "USB"))
            usb_type_is_sdp = true;
    }
    memset(buff, 0, sizeof(buff));
    ret = read_file(USB_TYPEC_MODE_PATH, buff, sizeof(buff));
    if (ret >= 0) {
        if (strcmp(buff, "Source attached (default current)"))
            typec_default_src = true;
    }
    if (usb_type_is_sdp && typec_default_src) {
        /*
         * Request 500mA input current when a SDP is connected and it's
         * acting as a default source.
         * PD capable source which could charge the device with USB_PD
         * charger type is not included here.
         */
        ret = write_file_int(USB_MAX_CURRENT_PATH, USB500_UA);
        if (ret == 0)
            LOGW(CHGR_TAG, "Force input current to 500mA with SDP inserted!\n");
    }
    fd = open(BMS_READY_PATH, O_RDONLY);
    if (fd < 0)
            return;
    while (1) {
        ret = read(fd, buff, (sizeof(buff) - 1));
        if (ret > 0) {
            buff[ret] = '\0';
            sscanf(buff, "%d\n", &bms_ready);
        } else {
            LOGE(CHGR_TAG, "read soc-ready failed, ret=%d\n", ret);
            break;
        }
        if ((bms_ready > 0) || (wait_count++ > WAIT_BMS_READY_TIMES_MAX))
            break;
        usleep(WAIT_BMS_READY_INTERVAL_USEC);
        lseek(fd, 0, SEEK_SET);
    }
    close(fd);
    LOGV(CHGR_TAG, "Checking BMS SoC ready done %d!\n", bms_ready);
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
