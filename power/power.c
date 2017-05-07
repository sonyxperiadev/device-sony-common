/*
 * Copyright (C) 2017 AngeloGioacchino Del Regno <kholk11@gmail.com>
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
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

#include <cutils/properties.h>
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#include "power.h"

#define LOG_TAG "RQBalance-PowerHAL"

static struct rqbalance_params *rqb;
static int hal_init_ok = false;

/* Remove this when all platforms will be migrated? */
static bool param_perf_supported = false;

/* Extension library support */
static void *ext_library;
lock_acq_t perf_lock_acquire;
lock_rel_t perf_lock_release;

#define UNUSED __attribute__((unused))

/*
 * sysfs_write - Write string to sysfs path
 *
 * \param path - Path to the sysfs file
 * \param s    - String to write
 * \return Returns success (true) or failure (false)
 */
static bool sysfs_write(char *path, char *s)
{
    char buf[80];
    int len;
    int fd = open(path, O_WRONLY);
    bool ret = true;

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return false ;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);

        ret = false;
    }

    close(fd);

    return ret;
}

/*
 * rqb_param_string - Get power mode string
 *
 * \param pwrmode - RQBalance Power Mode (from enum rqb_pwr_mode_t)
 * \param compat - Switch for compatibility string
 * \return Returns compat or new power mode string
 */
static char* rqb_param_string(rqb_pwr_mode_t pwrmode, bool compat)
{
    char* type_string;
    char* compat_string;

    switch (pwrmode) {
        case POWER_MODE_BATTERYSAVE:
            type_string = "batterysave";
            compat_string = "low";
            break;
        case POWER_MODE_BALANCED:
            type_string = "balanced";
            compat_string = "normal";
            break;
        case POWER_MODE_PERFORMANCE:
            type_string = "performance";
            compat_string = "performance";
            break;
        default:
            return "unknown";
    }

    if (compat)
        return compat_string;

    return type_string;
}

/*
 * print_parameters - Print PowerHAL RQBalance parameters to ALOG
 *
 * \param pwrmode - RQBalance Power Mode (from enum rqb_pwr_mode_t)
 */
static void print_parameters(rqb_pwr_mode_t pwrmode)
{
    char* mode_string = rqb_param_string(pwrmode, false);
    struct rqbalance_params *cur_params = &rqb[pwrmode];

    ALOGI("Parameters for %s mode:", mode_string);
    ALOGI("Minimum cores:       %s", cur_params->min_cpus);
    ALOGI("Maximum cores:       %s", cur_params->max_cpus);
    ALOGI("Upcore thresholds:   %s", cur_params->up_thresholds);
    ALOGI("Downcore thresholds: %s", cur_params->down_thresholds);
    ALOGI("Balance level:       %s", cur_params->balance_level);
}

/*
 * parse_rqbalance_params - Parse parameters for RQBalance power modes
 *
 * TODO: Is there any more-human-readable way for error checking?
 * \param pwrmode - RQBalance Power Mode (from enum rqb_pwr_mode_t)
 * \return Returns success (true) or failure (false)
 */
static bool parse_rqbalance_params(rqb_pwr_mode_t pwrmode)
{
    int ret = -1;
    size_t sz;
    char* mode_string = rqb_param_string(pwrmode, true);
    char* prop_string = NULL;
    struct rqbalance_params *cur_params = &rqb[pwrmode];

    if (cur_params == NULL)
        goto fail;

    /* Allocate a decent amount of memory, to make any
     * property string to fit */
    sz = sizeof(char)*((sizeof(PROP_MIN_CPUS)*2)+sizeof(mode_string));
    prop_string = (char*)malloc(sz);

    snprintf(prop_string, sz, PROP_MAX_CPUS, mode_string);
    ret = property_get(prop_string, cur_params->max_cpus, "0");
    if (!ret)
        goto freemem;

    snprintf(prop_string, sz, PROP_MIN_CPUS, mode_string);
    ret = property_get(prop_string, cur_params->min_cpus, "0");
    if (!ret)
        goto freemem;

    snprintf(prop_string, sz, PROP_UPCORE_THRESH, mode_string);
    ret = property_get(prop_string, cur_params->up_thresholds, "0");
    if (!ret)
        goto freemem;

    snprintf(prop_string, sz, PROP_DNCORE_THRESH, mode_string);
    ret = property_get(prop_string, cur_params->down_thresholds, "0");
    if (!ret)
        goto freemem;

    snprintf(prop_string, sz, PROP_BALANCE_LVL, mode_string);
    ret = property_get(prop_string, cur_params->balance_level, "0");
    if (!ret)
        goto freemem;

freemem:
    free(prop_string);
    if (ret)
        return true;
fail:
    ALOGE("FATAL: RQBalance %s parameters parsing error!!!", mode_string);
    return false;
}

/*
 * _set_power_mode - Writes power configuration to the RQBalance driver
 *
 * \param rqparm - RQBalance Power Mode parameters struct
 */
void __set_power_mode(struct rqbalance_params *rqparm)
{
    sysfs_write(SYS_MAX_CPUS, rqparm->max_cpus);
    sysfs_write(SYS_MIN_CPUS, rqparm->min_cpus);
    sysfs_write(SYS_UPCORE_THRESH, rqparm->up_thresholds);
    sysfs_write(SYS_DNCORE_THRESH, rqparm->down_thresholds);
    sysfs_write(SYS_BALANCE_LVL, rqparm->balance_level);

    return;
}

/*
 * set_power_mode - Writes power configuration to the RQBalance driver
 *
 * \param mode - RQBalance Power Mode (from enum rqb_pwr_mode_t)
 */
void set_power_mode(rqb_pwr_mode_t mode)
{
    char* mode_string = rqb_param_string(mode, false);

    if (mode == POWER_MODE_PERFORMANCE && !param_perf_supported)
        return;

    ALOGI("Setting %s mode", mode_string);

    __set_power_mode(&rqb[mode]);
}

/*
 * power_init - Initializes the PowerHAL structs and configurations
 */
static void power_init(struct power_module *module UNUSED)
{
    int ret, dbg_lvl;
    char ext_lib_path[127];
    char propval[2];
    struct rqbalance_params *rqbparm;

    ALOGI("Initializing PowerHAL...");

    rqb = malloc(sizeof(struct rqbalance_params) * POWER_MODE_MAX);
    assert(rqb != NULL);

    /* Initialize all parameters */
    ret = parse_rqbalance_params(POWER_MODE_BATTERYSAVE);
    if (!ret)
        goto general_error;

    ret = parse_rqbalance_params(POWER_MODE_BALANCED);
    if (!ret)
        goto general_error;

    ret = parse_rqbalance_params(POWER_MODE_PERFORMANCE);
    if (!ret) {
        ALOGW("No performance parameters. Going on.");
    } else {
        param_perf_supported = true;
    }

    hal_init_ok = true;

    property_get(PROP_DEBUGLVL, propval, "0");
    dbg_lvl = atoi(propval);

    if (dbg_lvl > 0) {
        ALOGW("WARNING: Starting in debug mode");
        print_parameters(POWER_MODE_BATTERYSAVE);
        print_parameters(POWER_MODE_BALANCED);
        if (param_perf_supported)
            print_parameters(POWER_MODE_PERFORMANCE);
    } else {
        ALOGI("Loading with debug off. To turn on, set %s", PROP_DEBUGLVL);
    }

    /* Init thermal_max_cpus and default profile */
    rqbparm = &rqb[POWER_MODE_BALANCED];
    sysfs_write(SYS_THERM_CPUS, rqbparm->max_cpus);
    set_power_mode(POWER_MODE_BALANCED);

    ALOGI("Initialized successfully.");

    /* Get librqbalance (or others) support */
    ret = property_get(PROP_EXTLIB, ext_lib_path, NULL);
    if (!ret) {
        ALOGI("No PowerHAL Extension Library (%s) specified. Going on.",
                PROP_EXTLIB);
        return;
    }

    ext_library = dlopen(ext_lib_path, RTLD_NOW);
    if (ext_library == NULL)
        return;

    perf_lock_acquire = (lock_acq_t)dlsym(ext_library, "perf_lock_acq");
    if (perf_lock_acquire == NULL) {
        ALOGE("Oops! Cannot find perf_lock_acq function in %s", ext_lib_path);
        goto extlib_error;
    }

    perf_lock_release = (lock_rel_t)dlsym(ext_library, "perf_lock_rel");
    if (perf_lock_release == NULL) {
        ALOGE("Oops! Cannot find perf_lock_rel function in %s", ext_lib_path);
        goto extlib_error;
    }

    ALOGI("Extension library support detected!");

    return;

general_error:
    ALOGE("PowerHAL initialization FAILED.");
extlib_error:
    if (ext_library != NULL)
        dlclose(ext_library);

    return;
}

/*
 * power_hint - Passes hints on power requirements from userspace
 *
 * \param module - This HAL's info sym struct
 * \param hint - Power hint (from power_hint_t)
 * \param data - Any kind of supplementary variable relative to the hint
 */
static void power_hint(struct power_module *module UNUSED, power_hint_t hint,
                            void *data)
{
    if (!hal_init_ok)
        return;

    switch (hint) {
        case POWER_HINT_VSYNC:
            break;

        case POWER_HINT_LOW_POWER:
            if (data) {
                set_power_mode(POWER_MODE_BATTERYSAVE);
            } else {
                set_power_mode(POWER_MODE_BALANCED);
            }
            break;

        case POWER_HINT_VR_MODE:
            if (data && param_perf_supported) {
                set_power_mode(POWER_MODE_PERFORMANCE);
            } else {
                set_power_mode(POWER_MODE_BALANCED);
            }
            break;

        case POWER_HINT_LAUNCH:
            if (data && param_perf_supported) {
                set_power_mode(POWER_MODE_PERFORMANCE);
            } else {
                set_power_mode(POWER_MODE_BALANCED);
            }
            break;

        default:
            break;
    }
}

/*
 * set_interactive - Performs power management actions for awake/sleep
 *
 * \param module - This HAL's info sym struct
 * \param on - 1: System awake 0: System asleep
 */
static void set_interactive(struct power_module *module UNUSED, int on)
{
    if (!hal_init_ok)
        return;

    if (!on) {
        ALOGI("Device is asleep.");
        set_power_mode(POWER_MODE_BATTERYSAVE);
    } else {
        ALOGI("Device is awake.");
        set_power_mode(POWER_MODE_BALANCED);
    }
}

/*
 * set_feature - Manages extra features
 *
 * \param module - This HAL's info sym struct
 * \param feature - Extra feature (from feature_t)
 * \param state - 1: enable 0: disable
 */
void set_feature(struct power_module *module UNUSED, feature_t feature, int state)
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
        .name = "RQBalance-based Power HAL",
        .author = "AngeloGioacchino Del Regno",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .powerHint = power_hint,
    .setInteractive = set_interactive,
    .setFeature = set_feature,
};
