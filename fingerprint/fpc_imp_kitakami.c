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

#include "QSEEComAPI.h"
#include "QSEEComFunc.h"
#include "fpc_imp.h"
#include "tz_api_kitakami.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>

#define LOG_TAG "FPC IMP"
#define LOG_NDEBUG 0

#include <cutils/log.h>

#define SPI_CLK_FILE "/sys/bus/spi/devices/spi0.1/clk_enable"
#define SPI_PREP_FILE "/sys/bus/spi/devices/spi0.1/spi_prepare"
#define SPI_WAKE_FILE "/sys/bus/spi/devices/spi0.1/wakeup_enable"
#define SPI_IRQ_FILE "/sys/bus/spi/devices/spi0.1/irq"

static struct QSEECom_handle * mHandle;
static struct QSEECom_handle * mHdl;

static int qsee_load_trustlet(struct QSEECom_handle **clnt_handle,
                       const char *path, const char *fname,
                       uint32_t __attribute__((unused))sb_size)
{
    int ret = 0;
    char* errstr;

    ALOGE("Starting app %s\n", fname);
    ret = mStartApp(clnt_handle, path, fname, 1024);
    if (ret < 0) {
        errstr = qsee_error_strings(ret);
        ALOGE("Could not load app %s. Error: %s (%d)\n",
              fname, errstr, ret);
    } else
        ALOGE("TZ App loaded : %s\n", fname);

    return ret;
}

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

int sys_fs_irq_poll(char *path)
{

    char buf[80];
    int ret = 0;
    int result;
    struct pollfd pollfds[2];
    pollfds[0].fd = open(path, O_RDONLY | O_NONBLOCK);

    if (pollfds[0].fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return -1 ;
    }

    char dummybuf;
    read(pollfds[0].fd, &dummybuf, 1);
    pollfds[0].events = POLLPRI;

    result = poll(pollfds, 1, 1000);

    switch (result) {
    case 0:
        ALOGD ("timeout\n");
        close(pollfds[0].fd);
        return -1;
    case -1:
        ALOGE ("poll error \n");
        close(pollfds[0].fd);
        return -1;
    default:
        ALOGD ("IRQ GOT \n");
        close(pollfds[0].fd);
        break;
    }

    close(pollfds[0].fd);

    return ret;
}

int device_enable()
{
    if (sysfs_write(SPI_PREP_FILE,"enable")< 0) {
        return -1;
    }

    if (sysfs_write(SPI_CLK_FILE,"1")< 0) {
        return -1;
    }
    return 1;
}

int device_disable()
{
    if (sysfs_write(SPI_CLK_FILE,"0")< 0) {
        return -1;
    }

    if (sysfs_write(SPI_PREP_FILE,"disable")< 0) {
        return -1;
    }
    return 1;
}

int send_modified_command_to_tz(uint32_t cmd, struct QSEECom_handle * handle, void * buffer, uint32_t len)
{

    fpc_send_mod_cmd_t* send_cmd = (fpc_send_mod_cmd_t*) handle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) handle->ion_sbuffer + 64;

    struct QSEECom_ion_fd_info  ion_fd_info;
    struct qcom_km_ion_info_t ihandle;

    ihandle.ion_fd = 0;

    if (qcom_km_ION_memalloc(&ihandle, len) <0) {
        ALOGE("ION allocation  failed");
        return -1;
    }

    memset(&ion_fd_info, 0, sizeof(struct QSEECom_ion_fd_info));
    ion_fd_info.data[0].fd = ihandle.ifd_data_fd;
    ion_fd_info.data[0].cmd_buf_offset = 4;

    send_cmd->cmd_id = cmd;
    send_cmd->v_addr = (intptr_t) ihandle.ion_sbuffer;
    send_cmd->length = len;
    send_cmd->extra = 0x00;

    memcpy((unsigned char *)ihandle.ion_sbuffer, buffer, len);

    int ret = send_modified_cmd_fn(handle,send_cmd,64,rec_cmd,64,&ion_fd_info);

    if(ret < 0) {
        qcom_km_ion_dealloc(&ihandle);
        return -1;
    }

    if (send_cmd->v_addr != 0) {
        ALOGE("Error on TZ\n");
        qcom_km_ion_dealloc(&ihandle);
        return -1;
    }

    qcom_km_ion_dealloc(&ihandle);
    return 0;
}

int send_normal_command(uint32_t cmd, uint32_t param, struct QSEECom_handle * handle)
{

    fpc_send_std_cmd_t* send_cmd = (fpc_send_std_cmd_t*) handle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) handle->ion_sbuffer + 64;

    send_cmd->cmd_id = cmd;
    send_cmd->ret_val = param;

    int ret = send_cmd_fn(handle,send_cmd,64,rec_cmd,64);

    if(ret < 0) {
        return -1;
    }

    return rec_cmd->ret_val;
}

uint64_t get_int64_command(uint32_t cmd, uint32_t param, struct QSEECom_handle * handle)
{

    fpc_send_int64_cmd_t* send_cmd = (fpc_send_int64_cmd_t*) handle->ion_sbuffer;
    fpc_send_int64_cmd_t* rec_cmd = (fpc_send_int64_cmd_t*) handle->ion_sbuffer + 64;

    send_cmd->cmd_id = cmd;
    send_cmd->ret_val = param;

    int ret = send_cmd_fn(handle,send_cmd,64,rec_cmd,64);

    if(ret < 0) {
        return -1;
    }

    return send_cmd->ret_val;

}

int fpc_set_auth_challenge(int64_t __unused challenge)
{
    return send_normal_command(FPC_SET_AUTH_CHALLENGE,0,mHandle);
}

uint64_t fpc_load_auth_challenge()
{
    return get_int64_command(FPC_GET_AUTH_CHALLENGE,0,mHandle);
}

uint64_t fpc_load_db_id()
{
    return get_int64_command(FPC_GET_DB_ID,0,mHandle);
}

int fpc_get_hw_auth_obj(void * buffer, int length)
{

    fpc_send_mod_cmd_t* send_cmd = (fpc_send_mod_cmd_t*) mHandle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer + 64;

    struct QSEECom_ion_fd_info  ion_fd_info;
    struct qcom_km_ion_info_t ihandle;

    ihandle.ion_fd = 0;

    if (qcom_km_ION_memalloc(&ihandle, length) <0) {
        ALOGE("ION allocation  failed");
        return -1;
    }

    memset(&ion_fd_info, 0, sizeof(struct QSEECom_ion_fd_info));
    ion_fd_info.data[0].fd = ihandle.ifd_data_fd;
    ion_fd_info.data[0].cmd_buf_offset = 4;

    send_cmd->cmd_id = FPC_GET_AUTH_HAT;
    send_cmd->v_addr = (intptr_t) ihandle.ion_sbuffer;
    send_cmd->length = length;
    send_cmd->extra = 0x00;

    memset((unsigned char *)ihandle.ion_sbuffer, 0, length);

    int ret = send_modified_cmd_fn(mHandle,send_cmd,64,rec_cmd,64,&ion_fd_info);

    memcpy(buffer, (unsigned char *)ihandle.ion_sbuffer, length);

    if(ret < 0) {
        qcom_km_ion_dealloc(&ihandle);
        return -1;
    }

    if (send_cmd->v_addr != 0) {
        ALOGE("Error on TZ\n");
        qcom_km_ion_dealloc(&ihandle);
        return -1;
    }

    qcom_km_ion_dealloc(&ihandle);
    return 0;

}

int fpc_verify_auth_challenge(void* hat, uint32_t size)
{
    return send_modified_command_to_tz(FPC_VERIFY_AUTH_CHALLENGE,mHandle,hat,size);
}

static uint32_t fpc_get_remaining_touches()
{
    return send_normal_command(FPC_GET_REMAINING_TOUCHES,0,mHandle);
}

uint32_t fpc_del_print_id(uint32_t id)
{

    uint32_t print_count = fpc_get_print_count();
    ALOGD("%s : print count is : %u", __func__, print_count);
    fpc_fingerprint_index_t print_indexs = fpc_get_print_ids(print_count);
    ALOGI("%s : delete print : %lu", __func__,(unsigned long) id);

    for (uint32_t i = 0; i < print_indexs.print_count; i++){

        uint32_t print_id = fpc_get_print_id(print_indexs.prints[i]);

        if (print_id == id){
                ALOGD("%s : Print index found at : %d", __func__, i);
                return send_normal_command(FPC_GET_DEL_PRINT,print_indexs.prints[i],mHandle);
        }
    }

    return -1;
}

// Returns -1 on error, 1 on check again and 0 on ready to capture
int fpc_wait_for_finger()
{

    int finger_state  = send_normal_command(FPC_CHK_FP_LOST,FPC_CHK_FP_LOST,mHandle);

    if (finger_state == 4) {
        ALOGD("%s : WAIT FOR FINGER UP\n", __func__);
    } else if (finger_state == 8) {
        ALOGD("%s : WAIT FOR FINGER DOWN\n", __func__);
    } else if (finger_state == 2) {
        ALOGD("%s : WAIT FOR FINGER NOT NEEDED\n", __func__);
        return 1;
    } else {
        return -1;
    }

    sysfs_write(SPI_WAKE_FILE,"1");
    if (send_normal_command(FPC_SET_WAKE,0,mHandle) != 0) {
        ALOGE("Error sending FPC_SET_WAKE to tz\n");
        return -1;
    }
    sysfs_write(SPI_CLK_FILE,"0");

    ALOGD("Attempting to poll device IRQ\n");

    if (sys_fs_irq_poll(SPI_IRQ_FILE) < 0) {
        sysfs_write(SPI_CLK_FILE,"1");
        sysfs_write(SPI_WAKE_FILE,"0");
        return 1;
    }

    sysfs_write(SPI_CLK_FILE,"1");
    sysfs_write(SPI_WAKE_FILE,"0");

    int wake_type = send_normal_command(FPC_GET_WAKE_TYPE,0,mHandle);

    if (wake_type == 3) {
        ALOGD("%s : READY TO CAPTURE\n", __func__);
        return 0;
    } else {
        ALOGD("%s : NOT READY TRY AGAIN\n", __func__);
        return 1;
    }

    return 1;
}

// Attempt to capture image
int fpc_capture_image()
{

    if (device_enable() < 0) {
        ALOGE("Error starting device\n");
        return -1;
    }


    int ret = fpc_wait_for_finger();

    if (ret == 0) {
        //If wait reported 0 we can try and capture the image
        ret = send_normal_command(FPC_CAPTURE_IMAGE,0,mHandle);
    } else {
        //return a high value as to not trigger a user notification
        ret = 1000; //same as FINGERPRINT_ERROR_VENDOR_BASE
    }

    if (device_disable() < 0) {
        ALOGE("Error stopping device\n");
        return -1;
    }

    return ret;
}

int fpc_enroll_step(uint32_t *remaining_touches)
{

    fpc_send_std_cmd_t* send_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer + 64;

    send_cmd->cmd_id = FPC_ENROLL_STEP;
    send_cmd->ret_val = 0x24;

    int ret = send_cmd_fn(mHandle,send_cmd,64,rec_cmd,64);

    if(ret < 0) {
        return -1;
    }
    if(rec_cmd->ret_val < 0)
        return rec_cmd->ret_val;

    int touches = fpc_get_remaining_touches();
    if(touches < 0)
        return touches;

    *remaining_touches = touches;
    return rec_cmd->ret_val;
}

int fpc_enroll_start(int print_index)
{
    fpc_send_enroll_start_cmd_t* send_cmd = (fpc_send_enroll_start_cmd_t*) mHandle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer + 64;

    send_cmd->cmd_id = FPC_ENROLL_START;
    send_cmd->ret_val = 0x00;
    send_cmd->na1 = 0x45;
    send_cmd->print_index = print_index;

    int ret = send_cmd_fn(mHandle,send_cmd,64,rec_cmd,64);

    if(ret < 0) {
        return -1;
    }

    return rec_cmd->ret_val;
}

int fpc_enroll_end(uint32_t *print_id)
{

    int index = send_normal_command(FPC_ENROLL_END,0x0,mHandle);

    if (index < 0 || index > 4) {
        ALOGE("Error sending FPC_ENROLL_END to tz\n");
        return -1;
    }

    *print_id = fpc_get_print_id(index);

    return 0;
}

fpc_fingerprint_index_t fpc_get_print_ids(int count)
{

    fpc_fingerprint_index_t data;

    fpc_send_std_cmd_t* send_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer;
    fpc_get_pint_index_cmd_t* rec_cmd = (fpc_get_pint_index_cmd_t*) mHandle->ion_sbuffer + 64;

    send_cmd->cmd_id = FPC_GET_ID_LIST;
    send_cmd->ret_val = count;
    send_cmd->length = count;

    int ret = send_cmd_fn(mHandle,send_cmd,64,rec_cmd,64);

    data.prints[0] = rec_cmd->p1;
    data.prints[1] = rec_cmd->p2;
    data.prints[2] = rec_cmd->p3;
    data.prints[3] = rec_cmd->p4;
    data.prints[4] = rec_cmd->p5;
    data.print_count = rec_cmd->print_count;

    return data;
}

int fpc_auth_start()
{

    int print_count = fpc_get_print_count();
    fpc_fingerprint_index_t prints;
    ALOGI("%s : Number Of Prints Available : %d",__func__,print_count);

    prints = fpc_get_print_ids(print_count);

    fpc_get_pint_index_cmd_t* send_cmd = (fpc_get_pint_index_cmd_t*) mHandle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer + 64;


    send_cmd->cmd_id = FPC_AUTH_START;
    send_cmd->p1 = prints.prints[0];
    send_cmd->p2 = prints.prints[1];
    send_cmd->p3 = prints.prints[2];
    send_cmd->p4 = prints.prints[3];
    send_cmd->p5 = prints.prints[4];
    send_cmd->print_count = prints.print_count;

    int ret = send_cmd_fn(mHandle,send_cmd,64,rec_cmd,64);

    if(ret < 0) {
        ALOGE("Error sending FPC_AUTH_START to tz\n");
        return -1;
    }

    return rec_cmd->ret_val;
}

uint32_t fpc_auth_step(uint32_t *print_id)
{

    fpc_send_std_cmd_t* send_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer;
    fpc_send_auth_cmd_t* rec_cmd = (fpc_send_auth_cmd_t*) mHandle->ion_sbuffer + 64;

    send_cmd->cmd_id = FPC_AUTH_STEP;
    send_cmd->ret_val = 0x00;

    int ret = send_cmd_fn(mHandle,send_cmd,64,rec_cmd,64);

    if(ret < 0) {
        return -1;
    }

    //if the print didnt capture properly return error and continue
    if (rec_cmd->ret_val < 2) {
        return -1;
    }



    *print_id = fpc_get_print_id(rec_cmd->id);
    return 0;
}

int fpc_auth_end()
{

    uint32_t ret = send_normal_command(FPC_AUTH_END,0x0,mHandle);

    if (ret != 0) {
        ALOGE("Error sending FPC_AUTH_END to tz\n");
        return -1;
    }
    return ret;
}

uint32_t fpc_get_print_id(int id)
{

    fpc_send_std_cmd_t* send_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer + 64;

    send_cmd->cmd_id = FPC_GET_PRINT_ID;
    send_cmd->ret_val = id;

    int ret = send_cmd_fn(mHandle,send_cmd,64,rec_cmd,64);

    if(ret < 0) {
        return -1;
    }

    return send_cmd->ret_val;
}


uint32_t fpc_get_print_count()
{

    fpc_send_std_cmd_t* send_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer + 64;

    send_cmd->cmd_id = FPC_GET_ID_COUNT;
    send_cmd->ret_val = 0x00;

    int ret = send_cmd_fn(mHandle,send_cmd,64,rec_cmd,64);

    if(ret < 0) {
        return -1;
    }

    return send_cmd->ret_val;
}


fpc_fingerprint_index_t fpc_get_print_index(int count)
{

    fpc_fingerprint_index_t data;

    fpc_send_std_cmd_t* send_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer;
    fpc_get_pint_index_cmd_t* rec_cmd = (fpc_get_pint_index_cmd_t*) mHandle->ion_sbuffer + 64;

    send_cmd->cmd_id = FPC_GET_ID_LIST;
    send_cmd->ret_val = count;
    send_cmd->length = count;

    int ret = send_cmd_fn(mHandle,send_cmd,64,rec_cmd,64);

    data.prints[0] = fpc_get_print_id(rec_cmd->p1);
    data.prints[1] = fpc_get_print_id(rec_cmd->p2);
    data.prints[2] = fpc_get_print_id(rec_cmd->p3);
    data.prints[3] = fpc_get_print_id(rec_cmd->p4);
    data.prints[4] = fpc_get_print_id(rec_cmd->p5);
    data.print_count = rec_cmd->print_count;

    return data;
}


uint32_t fpc_get_user_db_length()
{

    fpc_send_std_cmd_t* send_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer + 64;

    send_cmd->cmd_id = FPC_GET_DB_LENGTH;
    send_cmd->ret_val = 0x00;

    int ret = send_cmd_fn(mHandle,send_cmd,64,rec_cmd,64);

    if(ret < 0) {
        return -1;
    }

    return send_cmd->ret_val;
}


int fpc_load_user_db(char* path)
{

    FILE *f = fopen(path, "r");

    if (f == NULL) {
        ALOGE("Error opening file : %s", path);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    ALOGI("Loading DB of size : %ld", fsize);

    struct qcom_km_ion_info_t ihandle;
    struct QSEECom_ion_fd_info  ion_fd_info;

    if (qcom_km_ION_memalloc(&ihandle, fsize) <0) {
        ALOGE("ION allocation  failed");
        return -1;
    }

    fread(ihandle.ion_sbuffer, fsize, 1, f);

    fclose(f);

    fpc_send_mod_cmd_t* send_cmd = (fpc_send_mod_cmd_t*) mHandle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer + 64;

    memset(&ion_fd_info, 0, sizeof(struct QSEECom_ion_fd_info));
    ion_fd_info.data[0].fd = ihandle.ifd_data_fd;
    ion_fd_info.data[0].cmd_buf_offset = 4;

    send_cmd->cmd_id = FPC_SET_DB_DATA;
    send_cmd->v_addr = (intptr_t) ihandle.ion_sbuffer;
    send_cmd->length = fsize;
    send_cmd->extra = 0x00;

    int ret = send_modified_cmd_fn(mHandle,send_cmd,64,rec_cmd,64,&ion_fd_info);

    if(ret < 0) {
        qcom_km_ion_dealloc(&ihandle);
        return -1;
    }

    if (send_cmd->v_addr != 0) {
        ALOGE("Error on TZ\n");
        qcom_km_ion_dealloc(&ihandle);
        return -1;
    }

    qcom_km_ion_dealloc(&ihandle);
    return 0;

}

int fpc_store_user_db(uint32_t length, char* path)
{

    fpc_send_mod_cmd_t* send_cmd = (fpc_send_mod_cmd_t*) mHandle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) mHandle->ion_sbuffer + 64;

    struct QSEECom_ion_fd_info  ion_fd_info;
    struct qcom_km_ion_info_t ihandle;

    ihandle.ion_fd = 0;

    if (qcom_km_ION_memalloc(&ihandle, length) <0) {
        ALOGE("ION allocation  failed");
        return -1;
    }

    memset(&ion_fd_info, 0, sizeof(struct QSEECom_ion_fd_info));
    ion_fd_info.data[0].fd = ihandle.ifd_data_fd;
    ion_fd_info.data[0].cmd_buf_offset = 4;

    send_cmd->cmd_id = FPC_GET_DB_DATA;
    send_cmd->v_addr = (intptr_t) ihandle.ion_sbuffer;
    send_cmd->length = length;
    send_cmd->extra = 0x00;

    memset((unsigned char *)ihandle.ion_sbuffer, 0, length);

    int ret = send_modified_cmd_fn(mHandle,send_cmd,64,rec_cmd,64,&ion_fd_info);

    if(ret < 0) {
        qcom_km_ion_dealloc(&ihandle);
        return -1;
    }

    if (send_cmd->v_addr != 0) {
        ALOGE("Error on TZ\n");
        qcom_km_ion_dealloc(&ihandle);
        return -1;
    }


    FILE *f = fopen(path, "w");

    if (f == NULL) {
        ALOGE("Error opening file : %s", path);
        return -1;
    }

    fwrite(ihandle.ion_sbuffer, length, 1, f);

    fclose(f);

    qcom_km_ion_dealloc(&ihandle);
    return 0;
}

int fpc_set_gid(uint32_t __unused gid)
{
    // Not used on kitakami
    return 0;
};

int fpc_close()
{
    if (device_disable() < 0) {
        ALOGE("Error stopping device\n");
        return -1;
    }
    return 1;
}

int fpc_init()
{
    int ret = 0;

    ALOGE("INIT FPC TZ APP\n");

    open_handle();

    if (open_handle() < 1) {
        ALOGE("Qseecom Lib Not Open !\n");
        return -1;
    }

    if (device_enable() < 0) {
        ALOGE("Error starting device\n");
        return -1;
    }

    if (qsee_load_trustlet(&mHandle, FP_TZAPP_PATH,
                             FP_TZAPP_NAME, 1024) < 0)
        return -1;

    if (qsee_load_trustlet(&mHdl, KM_TZAPP_PATH,
                             KM_TZAPP_NAME, 1024) < 0)
        return -1;

    // Start creating one off command to get cert from keymaster
    fpc_send_std_cmd_t *req = (fpc_send_std_cmd_t *) mHdl->ion_sbuffer;
    req->cmd_id = 0x205;
    req->ret_val = 0x02;

    void * send_buf = mHdl->ion_sbuffer;
    void * rec_buf = mHdl->ion_sbuffer + 64;

    if (send_cmd_fn(mHdl, send_buf, 64, rec_buf, 1024-64) < 0) {
        return -1;
    }

    //Send command to keymaster
    fpc_send_std_cmd_t* ret_data = (fpc_send_std_cmd_t*) rec_buf;

    ALOGE("Keymaster Response Code : %u\n", ret_data->ret_val);
    ALOGE("Keymaster Response Length : %u\n", ret_data->length);

    void * data_buff = &ret_data->length + 1;

    if (send_modified_command_to_tz(FPC_SET_INIT_DATA,mHandle,data_buff,ret_data->length) < 0) {
        ALOGE("Error sending data to tz\n");
        return -1;
    }

    if (send_normal_command(FPC_INIT,0,mHandle) != 0) {
        ALOGE("Error sending FPC_INIT to tz\n");
        return -1;
    }

    if (send_normal_command(FPC_GET_INIT_STATE,0,mHandle) != 0) {
        ALOGE("Error sending FPC_GET_INIT_STATE to tz\n");
        return -1;
    }

    if (send_normal_command(FPC_INIT_UNK_1,0,mHandle) != 12) {
        ALOGE("Error sending FPC_INIT_UNK_1 to tz\n");
        return -1;
    }

    if (device_enable() < 0) {
        ALOGE("Error starting device\n");
        return -1;
    }

    if (send_normal_command(FPC_INIT_UNK_2,0,mHandle) != 0) {
        ALOGE("Error sending FPC_INIT_UNK_2 to tz\n");
        return -1;
    }

    int fpc_info = send_normal_command(FPC_INIT_UNK_0,0,mHandle);

    ALOGI("Got device data : %d \n", fpc_info);

    if (device_disable() < 0) {
        ALOGE("Error stopping device\n");
        return -1;
    }

    set_bandwidth_fn(mHandle,true);

    if (send_normal_command(FPC_INIT_NEW_DB,0,mHandle) != 0) {
        ALOGE("Error sending FPC_INIT_NEW_DB to tz\n");
        return -1;
    }

    if (send_normal_command(FPC_SET_FP_STORE,0,mHandle) != 0) {
        ALOGE("Error sending FPC_SET_FP_STORE to tz\n");
        return -1;
    }

    set_bandwidth_fn(mHandle,false);

    return 1;

}
