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
#include <sys/socket.h>
#include <sys/un.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

#include <private/android_filesystem_config.h>
#include <cutils/properties.h>
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#include "power.h"
#include "rqbalance_halext.h"

#define LOG_TAG "RQBalance-PowerHAL"

static struct rqbalance_params *rqb;
static int hal_init_ok = false;
static rqb_pwr_mode_t cur_pwrmode;

/* Remove this when all platforms will be migrated? */
static bool param_perf_supported = true;

/* PowerServer */
static int sock;
static int clientsock;
static struct sockaddr_un server_addr;
static pthread_t powerserver_thread;
static bool psthread_run = true;

/* XML Configuration support */
extern int parse_xml_data(char* filepath,
            char* node, struct rqbalance_params *therqb);

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
        return false;
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
            compat_string = "perf";
            break;
        case POWER_MODE_OMXDECODE:
            type_string = "video_decoding";
            compat_string = "vdec";
            break;
        case POWER_MODE_OMXENCODE:
            type_string = "video_encoding";
            compat_string = "venc";
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
 * _set_power_mode - Writes power configuration to the RQBalance driver
 *
 * \param rqparm - RQBalance Power Mode parameters struct
 */
static void __set_power_mode(struct rqbalance_params *rqparm)
{
    bool ret, cpus_error;
    short retry = 0;


    sysfs_write(SYS_UPCORE_THRESH, rqparm->up_thresholds);
    sysfs_write(SYS_DNCORE_THRESH, rqparm->down_thresholds);
    sysfs_write(SYS_BALANCE_LVL, rqparm->balance_level);

set_cpu:
    ret = sysfs_write(SYS_MAX_CPUS, rqparm->max_cpus);
    if (!ret)
        cpus_error = true;

    ret = sysfs_write(SYS_MIN_CPUS, rqparm->min_cpus);
    if (!ret)
        cpus_error = true;

    if (cpus_error) {
        cpus_error = false;
        retry++;

        if (retry < 2)
            goto set_cpu;
    }


    return;
}

/*
 * __set_special_power_mode - Writes special power configuration to the
 *                            RQBalance driver from extended control
 *                            and other external libraries
 *
 * \param max_cpus - Maximum number of plugged in CPUS/cores
 * \param min_cpus - Minimum number of plugged in CPUs/cores
 * \param up_thresholds - Upcore thresholds
 * \param down_thresholds - Downcore thresholds
 * \param balance_level - Frequency/cores balancement level
 */
void __set_special_power_mode(char* max_cpus, char* min_cpus,
                              char* up_thresholds, char* down_thresholds,
                              char* balance_level) {
    struct rqbalance_params *setparam;
    struct rqbalance_params *current = &rqb[cur_pwrmode];

    setparam = malloc(sizeof(struct rqbalance_params));

    if (max_cpus)
        memcpy(setparam->max_cpus, max_cpus,
               strlen(max_cpus));
    else
        memcpy(setparam->max_cpus, current->max_cpus,
               strlen(current->max_cpus));

    if (min_cpus)
        memcpy(setparam->min_cpus, min_cpus,
               strlen(min_cpus));
    else
        memcpy(setparam->min_cpus, current->min_cpus,
               strlen(current->min_cpus));

    if (up_thresholds)
        memcpy(setparam->up_thresholds, up_thresholds,
               strlen(up_thresholds));
    else
        memcpy(setparam->up_thresholds, current->up_thresholds,
               strlen(current->up_thresholds));

    if (down_thresholds)
        memcpy(setparam->down_thresholds, down_thresholds,
               strlen(down_thresholds));
    else
        memcpy(setparam->down_thresholds, current->down_thresholds,
               strlen(current->down_thresholds));

    if (balance_level)
        memcpy(setparam->balance_level, balance_level,
               strlen(balance_level));
    else
        memcpy(setparam->balance_level, current->balance_level,
               strlen(current->balance_level));

    __set_power_mode(setparam);

    free(setparam);

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

    cur_pwrmode = mode;
}

static void *powerserver_looper(void *unusedvar UNUSED)
{
    int ret;
    int32_t halext_reply = -EINVAL;
    uint8_t retry;
    socklen_t clientlen = sizeof(struct sockaddr_un);
    struct sockaddr_un client_addr;
    struct rqbalance_halext_params extparams;

reloop:
    ALOGI("PowerServer is waiting for connection...");
    if (clientsock)
        close(clientsock);
    retry = 0;
    while (((clientsock = accept(sock, (struct sockaddr*)&client_addr,
            &clientlen)) > 0) && (psthread_run == true))
    {
        ret = recv(clientsock, &extparams,
              sizeof(struct rqbalance_halext_params), 0);
        if (!ret) {
            ALOGE("Cannot receive data from client");
            goto reloop;
        }

        if (ret != sizeof(struct rqbalance_halext_params)) {
            ALOGE("Received data size mismatch!!");
            goto reloop;
        } else ret = 0;

        if (extparams.acquire)
             halext_reply = halext_perf_lock_acquire(&extparams);
        else
             halext_reply = halext_perf_lock_release(extparams.id);

retry_send:
	retry++;
        ret = send(clientsock, &halext_reply, sizeof(halext_reply), 0);
	if (ret == -1) {
		halext_reply = -EINVAL;
		if (retry < 50)
			goto retry_send;
		ALOGE("ERROR: Cannot send reply!!!");
		goto reloop;
	} else retry = 0;

        if (clientsock)
            close(clientsock);
    }

    ALOGI("PowerServer terminated.");
    return NULL;
}

static int manage_powerserver(bool start)
{
    int ret;
    struct stat st = {0};

    if (start == false) {
        psthread_run = false;
        if (clientsock) {
            shutdown(clientsock, SHUT_RDWR);
            close(clientsock);
        }
        if (sock) {
            shutdown(sock, SHUT_RDWR);
            close(sock);
        }

        return 0;
    }

    psthread_run = true;

    /* Create folder, if doesn't exist */
    if (stat(POWERSERVER_DIR, &st) == -1) {
        mkdir(POWERSERVER_DIR, 0773);
    }

    /* Get socket in the UNIX domain */
    sock = socket(PF_UNIX, SOCK_SEQPACKET, 0);
    if (sock < 0) {
        ALOGE("Could not create the socket");
        return -EPROTO;
    }

    /* Create address */
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, POWERSERVER_SOCKET);

    /* Free the existing socket file, if any */
    unlink(POWERSERVER_SOCKET);

    /* Bind the address to the socket */
    ret = bind(sock, (struct sockaddr*)&server_addr,
               sizeof(struct sockaddr_un));
    if (ret != 0) {
        ALOGE("Cannot bind socket");
        return -EINVAL;
    }

    /* Set socket permissions */
    chown(server_addr.sun_path, AID_ROOT, AID_SYSTEM);
    chmod(server_addr.sun_path, 0666);

    /* Listen on this socket */
    ret = listen(sock, POWERSERVER_MAXCONN);
    if (ret != 0) {
        ALOGE("Cannot listen on socket");
        return ret;
    }

    ret = pthread_create(&powerserver_thread, NULL, powerserver_looper, NULL);
    if (ret != 0) {
        ALOGE("Cannot create PowerServer thread");
        return -ENXIO;
    }

    return 0;
}

static bool init_all_rqb_params(void)
{
    int i, ret;

    rqb = malloc(sizeof(struct rqbalance_params) * POWER_MODE_MAX);
    assert(rqb != NULL);

    for (i = 0; i < POWER_MODE_MAX; i++)
    {
        ret = parse_xml_data(RQBHAL_CONF_FILE,
                rqb_param_string(i, false), &rqb[i]);
        if (ret < 0) {
            ALOGE("Cannot parse configuration for %s mode!!!",
                  rqb_param_string(i, false));
        }
    }

    return ret;
}

/*
 * power_init - Initializes the PowerHAL structs and configurations
 */
static void power_init(struct power_module *module UNUSED)
{
    int ret, dbg_lvl;
    int i;
    char ext_lib_path[127];
    char propval[2];
    struct rqbalance_params *rqbparm;

    ALOGI("Initializing PowerHAL...");

    ret = init_all_rqb_params();
    if (ret < 0)
        goto general_error;

    if (!param_perf_supported)
        ALOGW("No performance parameters. Going on.");

    hal_init_ok = true;

    property_get(PROP_DEBUGLVL, propval, "0");
    dbg_lvl = atoi(propval);

    if (dbg_lvl > 0) {
        ALOGW("WARNING: Starting in debug mode");
        for (i = 0; i < POWER_MODE_MAX; i++) {
	        print_parameters(i);
	}
    } else {
        ALOGI("Loading with debug off. To turn on, set %s", PROP_DEBUGLVL);
    }

    /* Init thermal_max_cpus and default profile */
    rqbparm = &rqb[POWER_MODE_BALANCED];
    sysfs_write(SYS_THERM_CPUS, rqbparm->max_cpus);
    set_power_mode(POWER_MODE_BALANCED);

    ALOGI("Initialized successfully.");

    ret = manage_powerserver(true);
    if (ret == 0)
        ALOGI("PowerHAL PowerServer started");
    else
        ALOGE("Could not start PowerHAL PowerServer");

    return;

general_error:
    ALOGE("PowerHAL initialization FAILED.");

    return;
}

void power_init_ext(void)
{
    if (init_all_rqb_params())
        hal_init_ok = true;
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

    return;
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

	/* Stop PowerServer: we don't need it while sleeping */
        manage_powerserver(false);

        set_power_mode(POWER_MODE_BATTERYSAVE);
    } else {
        ALOGI("Device is awake.");

	/* Restart PowerServer */
        if (!psthread_run)
            manage_powerserver(true);

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
