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

#include "QSEEComFunc.h"

#define LOG_TAG "QSEE_WRAPPER"
#define LOG_NDEBUG 0

//#define USE_QSEE_WRAPPER 1

#include <cutils/log.h>

int open_handle()
{

#ifdef USE_QSEE_WRAPPER
    printf("Open Target Lib : libDSEEComAPI\n");
    mLibHandle = dlopen("libDSEEComAPI.so", RTLD_NOW);
#else
    printf("Open Target Lib : libQSEEComAPI\n");
    mLibHandle = dlopen("libQSEEComAPI.so", RTLD_NOW);
#endif

    if (!mLibHandle) {
        printf("Failed to initialize QSEECom API library\n");
        return -1;
    } else {
        printf("Initialized QSEECom API library\n");

        bool hasError = false;

        mStartApp = (start_app_fn) dlsym(mLibHandle, "QSEECom_start_app");

        if (!mStartApp) {
            printf("Failed to initialize QSEECom_start_app API function\n");
            hasError = true;
        }

        mStopApp = (shutdown_app_fn) dlsym(mLibHandle, "QSEECom_shutdown_app");

        if (!mStopApp) {
            printf("Failed to initialize QSEECom_shutdown_app API function");
            hasError = true;
        }

        load_external_elf_fn = (load_external_elf) dlsym(mLibHandle, "QSEECom_load_external_elf");

        if (!load_external_elf_fn) {
            printf("Failed to initialize QSEECom_load_external_elf API function\n");
            hasError = true;
        }

        unload_external_elf_fn = (unload_external_elf) dlsym(mLibHandle, "QSEECom_unload_external_elf");

        if (!unload_external_elf_fn) {
            printf("Failed to initialize QSEECom_unload_external_elf API function\n");
            hasError = true;
        }

        register_listener_fn = (register_listener) dlsym(mLibHandle, "QSEECom_register_listener");

        if (!register_listener_fn) {
            printf("Failed to initialize QSEECom_register_listener API function\n");
            hasError = true;
        }

        unregister_listener_fn = (unregister_listener) dlsym(mLibHandle, "QSEECom_unregister_listener");

        if (!unregister_listener_fn) {
            printf("Failed to initialize QSEECom_unregister_listener API function\n");
            hasError = true;
        }

        send_cmd_fn = (send_cmd) dlsym(mLibHandle, "QSEECom_send_cmd");

        if (!send_cmd_fn) {
            printf("Failed to initialize QSEECom_send_cmd API function\n");
            hasError = true;
        }

        send_modified_cmd_fn = (send_modified_cmd) dlsym(mLibHandle, "QSEECom_send_modified_cmd");

        if (!send_modified_cmd_fn) {
            printf("Failed to initialize QSEECom_send_modified_cmd API function\n");
            hasError = true;
        }

        receive_req_fn = (receive_req) dlsym(mLibHandle, "QSEECom_receive_req");

        if (!receive_req_fn) {
            printf("Failed to initialize QSEECom_receive_req API function\n");
            hasError = true;
        }

        send_resp_fn = (send_resp) dlsym(mLibHandle, "QSEECom_send_resp");

        if (!send_resp_fn) {
            printf("Failed to initialize QSEECom_send_resp API function\n");
            hasError = true;
        }

        set_bandwidth_fn = (set_bandwidth) dlsym(mLibHandle, "QSEECom_set_bandwidth");

        if (!set_bandwidth_fn) {
            printf("Failed to initialize QSEECom_set_bandwidth API function\n");
            hasError = true;
        }

        app_load_query_fn = (app_load_query) dlsym(mLibHandle, "QSEECom_app_load_query");

        if (!app_load_query_fn) {
            printf("Failed to initialize QSEECom_app_load_query API function\n");
            hasError = true;
        }


        if (hasError) {
            printf("Close Target Lib : libQSEEComAPI\n");
            dlclose(mLibHandle);
            mLibHandle = NULL;
            return -1;
        }

    }

    return 1;
}


int32_t qcom_km_ION_memalloc(struct qcom_km_ion_info_t *handle,
                             uint32_t size)
{
    int32_t ret = 0;
    int32_t iret = 0;
    int32_t fd = 0;
    unsigned char *v_addr;
    struct ion_allocation_data ion_alloc_data;
    int32_t ion_fd;
    int32_t rc;
    struct ion_fd_data ifd_data;
    struct ion_handle_data handle_data;
    /* open ION device for memory management
     * O_DSYNC -> uncached memory
    */
    if(handle == NULL) {
        ALOGE("Error:: null handle received");
        return -1;
    }
    ion_fd  = open("/dev/ion", O_RDONLY | O_DSYNC);
    if (ion_fd < 0) {
        ALOGE("Error::Cannot open ION device");
        return -1;
    }
    handle->ion_sbuffer = NULL;
    handle->ifd_data_fd = 0;
    /* Size of allocation */
    ion_alloc_data.len = (size + 4095) & (~4095);
    /* 4K aligned */
    ion_alloc_data.align = 4096;
    /* memory is allocated from EBI heap */
    ion_alloc_data.heap_id_mask = ION_HEAP(ION_QSECOM_HEAP_ID);
    /* Set the memory to be uncached */
    ion_alloc_data.flags = 0;
    /* IOCTL call to ION for memory request */
    rc = ioctl(ion_fd, ION_IOC_ALLOC, &ion_alloc_data);
    if (rc) {
        ret = -1;
        goto alloc_fail;
    }

    ifd_data.handle = ion_alloc_data.handle;

    /* Call MAP ioctl to retrieve the ifd_data.fd file descriptor */
    rc = ioctl(ion_fd, ION_IOC_MAP, &ifd_data);
    if (rc) {
        ret = -1;
        goto ioctl_fail;
    }
    /* Make the ion mmap call */
    v_addr = (unsigned char *)mmap(NULL, ion_alloc_data.len,
                                   PROT_READ | PROT_WRITE,
                                   MAP_SHARED, ifd_data.fd, 0);
    if (v_addr == MAP_FAILED) {
        ALOGE("Error::ION MMAP failed");
        ret = -1;
        goto map_fail;
    }
    handle->ion_fd = ion_fd;
    handle->ifd_data_fd = ifd_data.fd;
    handle->ion_sbuffer = v_addr;
    handle->ion_alloc_handle.handle = ion_alloc_data.handle;
    handle->sbuf_len = size;
    return ret;
map_fail:
    if (handle->ion_sbuffer != NULL) {
        iret = munmap(handle->ion_sbuffer, ion_alloc_data.len);
        if (iret)
            ALOGE("Error::Failed to unmap memory for load image. ret = %d", ret);
    }
ioctl_fail:
    handle_data.handle = ion_alloc_data.handle;
    if (handle->ifd_data_fd)
        close(handle->ifd_data_fd);
    iret = ioctl(ion_fd, ION_IOC_FREE, &handle_data);
    if (iret) {
        ALOGE("Error::ION FREE ioctl returned error = %d",iret);
    }
alloc_fail:
    if (ion_fd > 0)
        close(ion_fd);
    return ret;
}


int32_t qcom_km_ion_dealloc(struct qcom_km_ion_info_t *handle)
{
    struct ion_handle_data handle_data;
    int32_t ret = 0;
    /* Deallocate the memory for the listener */
    ret = munmap(handle->ion_sbuffer, (handle->sbuf_len + 4095) & (~4095));
    if (ret) {
        ALOGE("Error::Unmapping ION Buffer failed with ret = %d", ret);
    }
    handle_data.handle = handle->ion_alloc_handle.handle;
    close(handle->ifd_data_fd);
    ret = ioctl(handle->ion_fd, ION_IOC_FREE, &handle_data);
    if (ret) {
        ALOGE("Error::ION Memory FREE ioctl failed with ret = %d", ret);
    }
    close(handle->ion_fd);
    return ret;
}


char* qsee_error_strings(int err)
{
    switch (err)
    {
        case QSEECOM_LISTENER_REGISTER_FAIL:
            return "QSEECom: Failed to register listener\n";
        case QSEECOM_LISTENER_ALREADY_REGISTERED:
            return "QSEECom: Listener already registered\n";
        case QSEECOM_LISTENER_UNREGISTERED:
            return "QSEECom: Listener unregistered\n";
        case QSEECOM_APP_ALREADY_LOADED:
            return "QSEECom: Trustlet already loaded\n";
        case QSEECOM_APP_NOT_LOADED:
            return "QSEECom: Trustlet not loaded\n";
        case QSEECOM_APP_QUERY_FAILED:
            return "QSEECom: Failed to query trustlet\n";
        default:
            return "QSEECom: Unknown error\n";
    }
}

