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
static void __set_power_mode(struct rqbalance_params *rqparm)
{
    sysfs_write(SYS_MAX_CPUS, rqparm->max_cpus);
    sysfs_write(SYS_MIN_CPUS, rqparm->min_cpus);
    sysfs_write(SYS_UPCORE_THRESH, rqparm->up_thresholds);
    sysfs_write(SYS_DNCORE_THRESH, rqparm->down_thresholds);
    sysfs_write(SYS_BALANCE_LVL, rqparm->balance_level);

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

#if 0
static int process_connection(void)
{
    register int clientsock;
    int ret, clientlen = sizeof(struct sockaddr_un);
    struct sockaddr_un client_addr;

    clientsock = accept(sock, (struct sockaddr*)&client_addr, &clientlen);
    if (!clientsock) {
        ALOGE("Cannot accept connection");
        ret = -EPROTO;
        goto end;
    }

    ret = recv(clientsock, &extparams,
           sizeof(struct rqbalance_halext_params), 0);
    if (!ret) {
        ALOGE("Cannot receive data from client");
        goto end;
    }

    /* Validate the received data */
    if (ret != sizeof(struct rqbalance_halext_params)) {
        ALOGE("Received data size mismatch!!");
        goto end;
    } else ret = 0; /* Function shall return 0 on success */

end:
    close(clientsock);

    return ret;
}
#endif

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
        mkdir(POWERSERVER_DIR, 0666);
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
    int i;
    bool ret;

    rqb = malloc(sizeof(struct rqbalance_params) * POWER_MODE_MAX);
    assert(rqb != NULL);

    for (i = 0; i < POWER_MODE_MAX; i++)
    {
        ret = parse_rqbalance_params(i);

        if (!ret){
            if (i == POWER_MODE_PERFORMANCE) {
                param_perf_supported = false;
                ret = true;
            } else {
                return ret;
            }
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
    char ext_lib_path[127];
    char propval[2];
    struct rqbalance_params *rqbparm;

    ALOGI("Initializing PowerHAL...");

    ret = init_all_rqb_params();
    if (!ret)
        goto general_error;

    if (!param_perf_supported)
        ALOGW("No performance parameters. Going on.");

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

    ret = manage_powerserver(true);
    if (ret == 0)
        ALOGI("PowerHAL PowerServer started");
    else
        ALOGE("Could not start PowerHAL PowerServer");

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

void power_init_ext(void)
{
    if (init_all_rqb_params())
        hal_init_ok = true;
}

/*
 * extended_power_hint - Process extended power hints
 *
 * \param hint - Power hint (from extended_hint_t)
 * \param data - Any kind of supplementary variable relative to the hint
 */
static void extended_power_hint(extended_hint_t hint, void *data)
{
    //struct rqbalance_halext_params *extparams;
    int tmp;

    switch (hint) {
/*
        case POWER_HINT_EXT_LOCK_ACQUIRE:
            extparams = (struct rqbalance_halext_params*)data;
            halext_perf_lock_acquire(extparams);
            break;
        case POWER_HINT_EXT_LOCK_RELEASE:
            tmp = (int32_t)data;
            halext_perf_lock_release(tmp);
            break;
*/
        case POWER_HINT_EXT_CLIENT_CONN:
            ALOGI("Received ext client conn");
 //           if (sock)
   //             process_connection();
            break;
        default:
            break;
    }

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
    bool hint_processed = true;

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
            hint_processed = false;
            break;
    }

    if (hint_processed)
        return;

    extended_power_hint((extended_hint_t)hint, data);
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
