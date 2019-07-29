/*
 * Copyright (C) 2016-2019 AngeloGioacchino Del Regno <kholk11@gmail.com>
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


#define LOG_TAG "RQBalance-PowerHAL-Hints"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <mutex>
#include <pwd.h>
#include <utils/Log.h>
#include <utils/Trace.h>

#include "common.h"
#include "Hints.h"
#include "RQBalanceHALExt.h"
#include "Power.h"

#define UNUSED __attribute__((unused))

using android::hardware::hidl_vec;
using android::hardware::Return;
using android::hardware::Void;

/* XML Configuration support */
extern "C" {
int parse_xml_data(char* filepath,
            char* node, struct rqbalance_params *therqb);
}

/* PowerServer */
static int sock;
static struct sockaddr_un server_addr;
static pthread_t powerserver_thread;
static int clientsock;
static bool psthread_run;

static struct rqbalance_params *rqb;


RQBalanceHintsHandler::RQBalanceHintsHandler() :
        mHALExt(nullptr),
        cur_pwrmode(POWER_MODE_BALANCED) {

    mHALExt = std::make_unique<RQBalanceHALExt>(this);
}


RQBalanceHintsHandler::~RQBalanceHintsHandler() {
    ManagePowerServer(false);
}

/*
 * sysfs_write - Write string to sysfs path
 *
 * \param path - Path to the sysfs file
 * \param s    - String to write
 * \return Returns success (true) or failure (false)
 */
bool sysfs_write(const char *path, const char *s)
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
const char* rqb_param_string(rqb_pwr_mode_t pwrmode, bool compat)
{
    const char* type_string;
    const char* compat_string;

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
            break;
        case POWER_MODE_SUSTAINED:
            type_string = "sustained_perf";
            compat_string = "sustain";
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
static void print_parameters(rqb_pwr_mode_t pwrmode, int dbg_lvl)
{
    const char* mode_string = rqb_param_string(pwrmode, false);
    struct rqbalance_params *cur_params = &rqb[pwrmode];
    int i;

    ALOGI("Parameters for %s mode:", mode_string);
    ALOGI("Minimum cores:       %s", cur_params->min_cpus);
    ALOGI("Maximum cores:       %s", cur_params->max_cpus);
    ALOGI("Upcore thresholds:   %s", cur_params->up_thresholds);
    ALOGI("Downcore thresholds: %s", cur_params->down_thresholds);
    ALOGI("Balance level:       %s", cur_params->balance_level);

    if (dbg_lvl < 2)
        return;

    for (i = 0; i < CLUSTER_MAX; i++)
        ALOGI("Cluster %d MIN-MAX:   %s - %s", i,
               cur_params->freq_limit[i].min_freq,
               cur_params->freq_limit[i].max_freq);
}

void RQBalanceHintsHandler::print_params(rqb_pwr_mode_t pwrmode, int dbg_lvl)
{
    print_parameters(pwrmode, dbg_lvl);
    return;
}

/*
 * _SetPowerMode - Writes power configuration to the RQBalance driver
 *
 * \param rqparm - RQBalance Power Mode parameters struct
 */
void __SetPowerMode(struct rqbalance_params *rqparm)
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
void RQBalanceHintsHandler::__set_special_power_mode(char* max_cpus, char* min_cpus,
                              char* up_thresholds, char* down_thresholds,
                              char* balance_level)
{
    struct rqbalance_params *setparam;
    struct rqbalance_params *current = &rqb[cur_pwrmode];

    setparam = (struct rqbalance_params*)
                                 malloc(sizeof(struct rqbalance_params));

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

    __SetPowerMode(setparam);

    free(setparam);

    return;
}

void __set_cpufreq_mode(struct rqbalance_params *rqparm)
{
    int i;

    for (i = 0; i < CLUSTER_MAX; i++) {
        sysfs_write(SYS_CPU_HI_LIMIT, rqparm->freq_limit[i].max_freq);
        sysfs_write(SYS_CPU_LOW_LIMIT, rqparm->freq_limit[i].min_freq);
    }

    return;
}

/*
 * SetPowerMode - Writes power configuration to the RQBalance driver
 *
 * \param mode - RQBalance Power Mode (from enum rqb_pwr_mode_t)
 */
void RQBalanceHintsHandler::SetPowerMode(rqb_pwr_mode_t mode)
{
    const char* mode_string = rqb_param_string(mode, false);

    ALOGI("Setting %s mode", mode_string);

    __SetPowerMode(&rqb[mode]);
    __set_cpufreq_mode(&rqb[mode]);

    cur_pwrmode = mode;

    return;
}

int RQBalanceHintsHandler::PerfLockAcquire(struct rqbalance_halext_params *extparams)
{
    return mHALExt->PerfLockAcquire(extparams);
}

int RQBalanceHintsHandler::PerfLockRelease(uint32_t id)
{
    return mHALExt->PerfLockRelease(id);
}

static void *powerserver_looper(void *obj)
{
    int ret;
    int32_t halext_reply = -EINVAL;
    uint8_t retry;
    socklen_t clientlen = sizeof(struct sockaddr_un);
    struct sockaddr_un client_addr;
    struct rqbalance_halext_params extparams;
    RQBalanceHintsHandler *CPPLink = (RQBalanceHintsHandler *)obj;

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
             halext_reply = CPPLink->PerfLockAcquire(&extparams);
        else
             halext_reply = CPPLink->PerfLockRelease(extparams.id);

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

int RQBalanceHintsHandler::manage_powerserver(bool start)
{
    int ret;
    struct passwd *pwd;
    struct passwd *grp;
    uid_t uid;
    gid_t gid;

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

    // get system user and input group to call chown
    pwd = getpwnam("root");
    if (pwd == NULL) {
        ALOGD("failed to get uid for root");
        return -1;
    }
    uid = pwd->pw_uid;

    grp = getpwnam("system");
    if (grp == NULL) {
        ALOGD("failed to get gid for system");
        return -1;
    }
    gid = grp->pw_gid;


    psthread_run = true;

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
    chown(server_addr.sun_path, uid, gid);
    chmod(server_addr.sun_path, 0666);

    /* Listen on this socket */
    ret = listen(sock, POWERSERVER_MAXCONN);
    if (ret != 0) {
        ALOGE("Cannot listen on socket");
        return ret;
    }

    ret = pthread_create(&powerserver_thread, NULL, powerserver_looper, this);
    if (ret != 0) {
        ALOGE("Cannot create PowerServer thread");
        return -ENXIO;
    }

    return 0;
}

bool RQBalanceHintsHandler::init_all_rqb_params(void)
{
    int ret;
    int i;

    rqb = (struct rqbalance_params*) calloc(POWER_MODE_MAX,
                                            sizeof(struct rqbalance_params));
    assert(rqb != NULL);

    for (i = 0; i < POWER_MODE_MAX; i++)
    {
        ret = parse_xml_data((char*)RQBHAL_CONF_FILE,
                (char*)rqb_param_string((rqb_pwr_mode_t)i, false), &rqb[i]);
        if (ret < 0) {
            ALOGE("Cannot parse configuration for %s mode!!!",
                  rqb_param_string((rqb_pwr_mode_t)i, false));
        }
    }

    return ret;
}

int RQBalanceHintsHandler::InitializeHAL(void)
{
    int i, dbg_lvl, ret = -1;
    char propval[PROPERTY_VALUE_MAX];
    struct rqbalance_params *rqbparm;

    ret = init_all_rqb_params();
    if (ret < 0) {
        ALOGE("FAILURE: Cannot load params.");
        return -1;
    }

    property_get(PROP_DEBUGLVL, propval, "0");
    dbg_lvl = atoi(propval);

    if (dbg_lvl > 0) {
        ALOGW("WARNING: Starting in debug mode");
        for (i = 0; i < POWER_MODE_MAX; i++) {
            print_params((rqb_pwr_mode_t)i, dbg_lvl);
        }
    } else {
        ALOGI("Loading with debug off. To turn on, set %s", PROP_DEBUGLVL);
    }

    /* Init thermal_max_cpus and default profile */
    rqbparm = &rqb[POWER_MODE_BALANCED];
    sysfs_write(SYS_THERM_CPUS, rqbparm->max_cpus);
    SetPowerMode(POWER_MODE_BALANCED);

    ret = manage_powerserver(true);
    if (ret == 0)
        ALOGI("PowerHAL PowerServer started");
    else
        ALOGE("Could not start PowerHAL PowerServer");

    return ret;
}

int RQBalanceHintsHandler::ManagePowerServer(bool is_starting)
{
    return manage_powerserver(is_starting);
}

int RQBalanceHintsHandler::ManagePowerServerSafe(bool is_starting)
{
    if (is_starting && psthread_run)
        return -1;

    if (!is_starting && !psthread_run)
        return -1;

    return manage_powerserver(is_starting);
}

