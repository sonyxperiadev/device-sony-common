/*
 * Copyright (C) 2016 Adam Farden
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

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOG_TAG "Simple PowerHAL"
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

int sysfs_write(char *path, char *s)
{
    char buf[80];
    int len;
    int ret = 0;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return -1 ;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);

        ret = -1;
    }

    close(fd);

    return ret;
}

static void power_init(struct power_module *module)
{
    ALOGI("Simple PowerHAL is alive!.");
}

static void power_hint(struct power_module *module, power_hint_t hint,
                            void *data)
{
    switch (hint) {
        case POWER_HINT_VSYNC:
            break;

        case POWER_HINT_INTERACTION:
            // When touching the screen, pressing buttons etc.
            break;

        case POWER_HINT_LOW_POWER:
            // When we want to save battery.
            if (data) {
                ALOGI("Low power mode enabled.");
            }
            break;

        default:
            break;
    }
}

static void set_interactive(struct power_module *module, int on)
{
    // set interactive means change governor, cpufreqs etc
    // for when device is awake and ready to be used.

    if (!on) {
        ALOGI("Device is asleep.");
    } else {
        ALOGI("Device is awake.");
    }
}

void set_feature(struct power_module *module, feature_t feature, int state)
{
#ifdef TAP_TO_WAKE_NODE
    if (feature == POWER_FEATURE_DOUBLE_TAP_TO_WAKE) {
            ALOGI("Double tap to wake is %s.", state ? "enabled" : "disabled");
            sysfs_write(TAP_TO_WAKE_NODE, state ? "1" : "0");
        return;
    }
#endif
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

struct power_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = POWER_MODULE_API_VERSION_0_3,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = POWER_HARDWARE_MODULE_ID,
        .name = "Simple Power HAL",
        .author = "Adam Farden",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .powerHint = power_hint,
    .setInteractive = set_interactive,
    .setFeature = set_feature,
};
