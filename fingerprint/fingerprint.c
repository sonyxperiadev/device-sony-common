/*
 * Copyright (C) 2016 Shane Francis / Jens Andersen
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

#define LOG_TAG "AOSP FPC HAL"

#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <cutils/log.h>
#include <hardware/hardware.h>
#include <hardware/fingerprint.h>
#include <pthread.h>
#include "fpc_imp.h"


uint64_t challenge = 0;
bool auth_thread_running = false;

pthread_t thread;
pthread_mutex_t lock;

fingerprint_notify_t callback;
char db_path[255];

void *enroll_thread_loop()
{
    ALOGI("%s", __func__);

    uint32_t print_count = fpc_get_print_count();
    ALOGD("%s : print count is : %u", __func__, print_count);

    fpc_enroll_start(print_count);

    int status = 1;

    while((status = fpc_capture_image()) >= 0) {
        ALOGD("%s : Got Input", __func__);

        if (status <= FINGERPRINT_ACQUIRED_TOO_FAST) {
            fingerprint_msg_t msg;
            msg.type = FINGERPRINT_ACQUIRED;
            msg.data.acquired.acquired_info = status;
            callback(&msg);
        }

        //image captured
        if (status == FINGERPRINT_ACQUIRED_GOOD) {
            ALOGI("%s : Enroll Step", __func__);
            if (fpc_enroll_step() < 100) {
                int remaining_touches = fpc_get_remaining_touches();
                ALOGI("%s : Touches Remaining : %d", __func__, remaining_touches);
                if (remaining_touches > 0) {
                    fingerprint_msg_t msg;
                    msg.type = FINGERPRINT_TEMPLATE_ENROLLING;
                    msg.data.enroll.finger.fid = 0;
                    msg.data.enroll.finger.gid = 0;
                    msg.data.enroll.samples_remaining = remaining_touches;
                    msg.data.enroll.msg = 0;
                    callback(&msg);
                }
            } else {
                int print_index = fpc_enroll_end();

                if (print_index < 0){
                    ALOGE("%s : Error getting new print index : %d", __func__,print_index);
                    fingerprint_msg_t msg;
                    msg.type = FINGERPRINT_ERROR;
                    msg.data.error = FINGERPRINT_ERROR_UNABLE_TO_PROCESS;
                    callback(&msg);
                    break;
                }

                ALOGI("%s : Got print index : %d", __func__,print_index);

                uint32_t db_length = fpc_get_user_db_length();
                ALOGI("%s : User Database Length Is : %lu", __func__,(unsigned long) db_length);
                fpc_store_user_db(db_length, db_path);

                uint32_t print_id = fpc_get_print_id(print_index);
                ALOGI("%s : Got print id : %lu", __func__,(unsigned long) print_id);

                fingerprint_msg_t msg;
                msg.type = FINGERPRINT_TEMPLATE_ENROLLING;
                msg.data.enroll.finger.fid = print_id;
                msg.data.enroll.finger.gid = 0;
                msg.data.enroll.samples_remaining = 0;
                msg.data.enroll.msg = 0;
                callback(&msg);
                break;
            }
        }

        pthread_mutex_lock(&lock);
        if (!auth_thread_running) {
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);
    }

    fpc_enroll_end();
    ALOGI("%s : finishing",__func__);

    pthread_mutex_lock(&lock);
    auth_thread_running = false;
    pthread_mutex_unlock(&lock);
    return NULL;
}


void *auth_thread_loop()
{
    ALOGI("%s", __func__);
    fpc_auth_start();

    int status = 1;

    while((status = fpc_capture_image()) >= 0) {
        ALOGD("%s : Got Input", __func__);

        if (status <= FINGERPRINT_ACQUIRED_TOO_FAST) {
            fingerprint_msg_t msg;
            msg.type = FINGERPRINT_ACQUIRED;
            msg.data.acquired.acquired_info = status;
            callback(&msg);
        }

        if (status == FINGERPRINT_ACQUIRED_GOOD) {

            int verify_state = fpc_auth_step();
            ALOGI("%s : Auth step = %d", __func__, verify_state);

            if (verify_state >= 0) {

                uint32_t print_id = fpc_get_print_id(verify_state);
                ALOGI("%s : Got print id : %lu", __func__, (unsigned long) print_id);

                hw_auth_token_t hat;
                fpc_get_hw_auth_obj(&hat, sizeof(hw_auth_token_t));

                ALOGI("%s : hat->challange %lu",__func__,(unsigned long) hat.challenge);
                ALOGI("%s : hat->user_id %lu",__func__,(unsigned long) hat.user_id);
                ALOGI("%s : hat->authenticator_id %lu",__func__,(unsigned long) hat.authenticator_id);
                ALOGI("%s : hat->authenticator_type %d",__func__, hat.authenticator_type);
                ALOGI("%s : hat->timestamp %lu",__func__,(unsigned long) hat.timestamp);
                ALOGI("%s : hat size %lu",__func__,(unsigned long) sizeof(hw_auth_token_t));

                fingerprint_msg_t msg;
                msg.type = FINGERPRINT_AUTHENTICATED;
                msg.data.authenticated.finger.gid = 0;
                msg.data.authenticated.finger.fid = print_id;

                msg.data.authenticated.hat = hat;

                callback(&msg);
                break;
            }
        }

        pthread_mutex_lock(&lock);
        if (!auth_thread_running) {
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);
    }

    fpc_auth_end();
    ALOGI("%s : finishing",__func__);

    pthread_mutex_lock(&lock);
    auth_thread_running = false;
    pthread_mutex_unlock(&lock);
    return NULL;
}

static int fingerprint_close(hw_device_t *dev)
{
    fpc_close();
    if (dev) {
        free(dev);
        return 0;
    } else {
        return -1;
    }
}

static uint64_t fingerprint_pre_enroll(struct fingerprint_device __unused *dev)
{
    challenge = fpc_load_auth_challange();
    ALOGI("%s : Challange is : %jd",__func__,challenge);
    return challenge;
}

static int fingerprint_enroll(struct fingerprint_device __unused *dev,
                              const hw_auth_token_t __unused *hat,
                              uint32_t __unused gid,
                              uint32_t __unused timeout_sec)
{


    pthread_mutex_lock(&lock);
    bool thread_running = auth_thread_running;
    pthread_mutex_unlock(&lock);

    if (thread_running) {
        ALOGE("%s : Error, thread already running\n", __func__);
        return -1;
    }


    ALOGI("%s : hat->challange %lu",__func__,(unsigned long) hat->challenge);
    ALOGI("%s : hat->user_id %lu",__func__,(unsigned long) hat->user_id);
    ALOGI("%s : hat->authenticator_id %lu",__func__,(unsigned long) hat->authenticator_id);
    ALOGI("%s : hat->authenticator_type %d",__func__,hat->authenticator_type);
    ALOGI("%s : hat->timestamp %lu",__func__,(unsigned long) hat->timestamp);
    ALOGI("%s : hat size %lu",__func__,(unsigned long) sizeof(hw_auth_token_t));

    fpc_verify_auth_challange((void*) hat, sizeof(hw_auth_token_t));

    pthread_mutex_lock(&lock);
    auth_thread_running = true;
    pthread_mutex_unlock(&lock);

    if(pthread_create(&thread, NULL, enroll_thread_loop, NULL)) {
        ALOGE("%s : Error creating thread\n", __func__);
        auth_thread_running = false;
        return FINGERPRINT_ERROR;
    }

    return 0;

}

static uint64_t fingerprint_get_auth_id(struct fingerprint_device __unused *dev)
{

    uint64_t id = fpc_load_db_id();
    ALOGI("%s : ID : %jd",__func__,id );
    return id;

}

static int fingerprint_cancel(struct fingerprint_device __unused *dev)
{
    ALOGI("%s : +",__func__);

    pthread_mutex_lock(&lock);
    bool thread_running = auth_thread_running;
    pthread_mutex_unlock(&lock);

    ALOGI("%s : check thread running",__func__);
    if (!auth_thread_running) {
        ALOGI("%s : - (thread not running)",__func__);
        return 0;
    }

    pthread_mutex_lock(&lock);
    auth_thread_running = false;
    pthread_mutex_unlock(&lock);

    ALOGI("%s : join running thread",__func__);
    pthread_join(thread, NULL);

    ALOGI("%s : -",__func__);
    return 0;
}

static int fingerprint_remove(struct fingerprint_device __unused *dev,
                              uint32_t __unused gid, uint32_t __unused fid)
{

    //Maximum prints per gid is 5
    uint32_t prints[5];
    uint32_t print_count = fpc_get_print_count();
    ALOGD("%s : print count is : %u", __func__, print_count);

    fpc_get_pint_index_cmd_t print_indexs = fpc_get_print_index(print_count);

    //populate print array with index
    prints[0] = print_indexs.p1;
    prints[1] = print_indexs.p2;
    prints[2] = print_indexs.p3;
    prints[3] = print_indexs.p4;
    prints[4] = print_indexs.p5;

    ALOGI("%s : delete print : %lu", __func__,(unsigned long) fid);

    for (uint32_t i = 0; i < print_count; i++){
        uint32_t print_id = fpc_get_print_id(prints[i]);

        ALOGD("%s : found print : %lu at index %d", __func__,(unsigned long) print_id, prints[i]);

        if (print_id == fid){
            ALOGD("%s : Print index found at : %d", __func__, i);

            int ret = fpc_del_print_id(prints[i]);

            ALOGD("%s : fpc_del_print_id returns : %d", __func__, ret);

            if (ret == 0){

                uint32_t db_length = fpc_get_user_db_length();
                ALOGD("%s : User Database Length Is : %lu", __func__,(unsigned long) db_length);
                fpc_store_user_db(db_length, db_path);

                fingerprint_msg_t msg;
                msg.type = FINGERPRINT_TEMPLATE_REMOVED;
                msg.data.removed.finger.fid = print_id;
                msg.data.removed.finger.gid = 0;
                callback(&msg);

                return 0;
            }
        }
    }

    fingerprint_msg_t msg;
    msg.type = FINGERPRINT_ERROR;
    msg.data.error = FINGERPRINT_ERROR_UNABLE_TO_REMOVE;
    callback(&msg);

    return FINGERPRINT_ERROR;
}

static int fingerprint_set_active_group(struct fingerprint_device __unused *dev,
                                        uint32_t __unused gid, const char __unused *store_path)
{

    sprintf(db_path,"%s/data_%d.db",store_path,gid);
    ALOGI("%s : storage path set to : %s",__func__, db_path);
    fpc_load_user_db(db_path);
    return 0;

}

static int fingerprint_enumerate(struct fingerprint_device *dev,
                                 fingerprint_finger_id_t *results,
                                 uint32_t *max_size)
{

    uint32_t print_count = fpc_get_print_count();
    ALOGD("%s : print count is : %u", __func__, print_count);
    fpc_get_pint_index_cmd_t print_indexs = fpc_get_print_index(print_count);
    uint32_t prints[5];

    //populate print array with index
    prints[0] = print_indexs.p1;
    prints[1] = print_indexs.p2;
    prints[2] = print_indexs.p3;
    prints[3] = print_indexs.p4;
    prints[4] = print_indexs.p5;


    if (*max_size == 0) {
        *max_size = print_count;
    } else {
        for (size_t i = 0; i < *max_size && i < print_count; i++) {

            uint32_t print_id = fpc_get_print_id(prints[i]);
            ALOGD("%s : found print : %lu at index %d", __func__,(unsigned long) print_id, prints[i]);

            results[i].fid = print_id;
            results[i].gid = 0;
        }
    }

    return print_count;
}

static int fingerprint_authenticate(struct fingerprint_device __unused *dev,
                                    uint64_t __unused operation_id, __unused uint32_t gid)
{

    if (auth_thread_running) {
        ALOGE("%s : Error, thread already running\n", __func__);
        return -1;
    }

    pthread_mutex_lock(&lock);
    auth_thread_running = true;
    pthread_mutex_unlock(&lock);

    if(pthread_create(&thread, NULL, auth_thread_loop, NULL)) {
        ALOGE("%s : Error creating thread\n", __func__);
        auth_thread_running = false;
        return FINGERPRINT_ERROR;
    }

    return 0;
}

static int set_notify_callback(struct fingerprint_device *dev,
                               fingerprint_notify_t notify)
{
    /* Decorate with locks */
    dev->notify = notify;
    callback = notify;
    return 0;
}

static int fingerprint_open(const hw_module_t* module, const char __unused *id,
                            hw_device_t** device)
{

    ALOGI("%s",__func__);

    if (device == NULL) {
        ALOGE("NULL device on open");
        return -EINVAL;
    }


    if (fpc_init() < 0) {
        ALOGE("Could not init FPC device");
        return -EINVAL;
    }

    fingerprint_device_t *dev = malloc(sizeof(fingerprint_device_t));
    memset(dev, 0, sizeof(fingerprint_device_t));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = FINGERPRINT_MODULE_API_VERSION_2_0;
    dev->common.module = (struct hw_module_t*) module;
    dev->common.close = fingerprint_close;

    dev->pre_enroll = fingerprint_pre_enroll;
    dev->enroll = fingerprint_enroll;
    dev->get_authenticator_id = fingerprint_get_auth_id;
    dev->cancel = fingerprint_cancel;
    dev->remove = fingerprint_remove;
    dev->set_active_group = fingerprint_set_active_group;
    dev->enumerate = fingerprint_enumerate;
    dev->authenticate = fingerprint_authenticate;
    dev->set_notify = set_notify_callback;
    dev->notify = NULL;

    *device = (hw_device_t*) dev;
    return 0;
}

static struct hw_module_methods_t fingerprint_module_methods = {
    .open = fingerprint_open,
};

fingerprint_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag                = HARDWARE_MODULE_TAG,
        .module_api_version = FINGERPRINT_MODULE_API_VERSION_2_0,
        .hal_api_version    = HARDWARE_HAL_API_VERSION,
        .id                 = FINGERPRINT_HARDWARE_MODULE_ID,
        .name               = "Kitakami Fingerprint HAL",
        .author             = "Shane Francis / Jens Andersen",
        .methods            = &fingerprint_module_methods,
    },
};
