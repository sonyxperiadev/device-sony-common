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
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define LOG_TAG "QSEE_WRAPPER"
//#define LOG_NDEBUG 0

//#define USE_QSEE_WRAPPER 1
#ifdef USE_QSEE_WRAPPER
#define QSEE_LIBRARY "libDSEEComAPI.so"
#else
#define QSEE_LIBRARY "libQSEEComAPI.so"
#endif

#include <cutils/log.h>

//Forward declarations

static int qsee_load_trustlet(struct qsee_handle_t* qsee_handle,
                              struct QSEECom_handle **clnt_handle,
                              const char *path, const char *fname,
                              uint32_t sb_size);
char* qsee_error_strings(int err);
int32_t qcom_km_ion_dealloc(struct qcom_km_ion_info_t *handle);
int32_t qcom_km_ion_memalloc(struct qcom_km_ion_info_t *handle,
                             uint32_t size);
typedef struct {
    void *libHandle;
} _priv_data_t;



int32_t qsee_open_handle(struct qsee_handle_t** ret_handle)
{
    struct qsee_handle_t *handle = NULL;
    _priv_data_t *data = NULL;
    int32_t ret = -1;

    ALOGD("Using Target Lib : %s\n", QSEE_LIBRARY);
    data = (_priv_data_t*)malloc(sizeof(_priv_data_t));
    if(data == NULL)
    {
        ALOGE("Error allocating memory: %s\n", strerror(errno));
        goto exit;
    }
    data->libHandle = dlopen(QSEE_LIBRARY, RTLD_NOW);
    if(data->libHandle == NULL)
    {
        ALOGE("Failed to load QSEECom API library: %s\n", strerror(errno));
        goto exit_err_data;
    }
    ALOGD("Loaded QSEECom API library at %p\n", data->libHandle);

    handle =(struct qsee_handle_t*)malloc(sizeof(struct qsee_handle_t));
    if(handle == NULL)
    {
        ALOGE("Error allocating memory: %s\n", strerror(errno));
        goto exit_err_dlhandle;
    }

    handle->_data = data;

    ALOGD("Loaded QSEECom API library...\n");
    // Setup internal functions
    handle->ion_alloc = qcom_km_ion_memalloc;
    handle->ion_free = qcom_km_ion_dealloc;
    handle->load_trustlet = qsee_load_trustlet;

    // Setup QSEECom Functions
    // NOTE: dlsym doesn't allocate anything, so there's nothing extra to free from these calls!

    handle->start_app = (start_app_def) dlsym(data->libHandle, "QSEECom_start_app");
    if(handle->start_app == NULL)
    {
        ALOGE("Error loading QSEECom_start_app: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    handle->shutdown_app = (shutdown_app_def) dlsym(data->libHandle, "QSEECom_shutdown_app");
    if(handle->shutdown_app == NULL)
    {
        ALOGE("Error loading QSEECom_shutdown_app: %s\n", strerror(errno));
        goto exit_err_handle;

    }

    handle->load_external_elf  = (load_external_elf_def) dlsym(data->libHandle, "QSEECom_load_external_elf");
    if(handle->load_external_elf == NULL)
    {
        ALOGE("Error loading QSEECom_load_external_elf: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    handle->unload_external_elf = (unload_external_elf_def) dlsym(data->libHandle, "QSEECom_unload_external_elf");
    if(handle->unload_external_elf == NULL)
    {
        ALOGE("Error loading QSEECom_unload_external_elf: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    handle->register_listener = (register_listener_def) dlsym(data->libHandle, "QSEECom_register_listener");
    if(handle->register_listener == NULL)
    {
        ALOGE("Error loading QSEECom_register_listener: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    handle->unregister_listener = (unregister_listener_def) dlsym(data->libHandle, "QSEECom_unregister_listener");
    if(handle->unregister_listener == NULL)
    {
        ALOGE("Error loading QSEECom_unregister_listener: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    handle->send_cmd = (send_cmd_def) dlsym(data->libHandle, "QSEECom_send_cmd");
    if(handle->send_cmd == NULL)
    {
        ALOGE("Error loading QSEECom_send_cmd: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    handle->send_modified_cmd = (send_modified_cmd_def) dlsym(data->libHandle, "QSEECom_send_modified_cmd");
    if(handle->send_modified_cmd == NULL)
    {
        ALOGE("Error loading QSEECom_send_modified_cmd: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    handle->receive_req = (receive_req_def) dlsym(data->libHandle, "QSEECom_receive_req");
    if(handle->receive_req == NULL)
    {
        ALOGE("Error loading QSEECom_receive_req: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    handle->send_resp = (send_resp_def) dlsym(data->libHandle, "QSEECom_send_resp");
    if(handle->send_resp == NULL)
    {
        ALOGE("Error loading QSEECom_send_resp: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    handle->set_bandwidth = (set_bandwidth_def) dlsym(data->libHandle, "QSEECom_set_bandwidth");
    if(handle->set_bandwidth == NULL)
    {
        ALOGE("Error loading QSEECom_set_bandwidth: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    handle->app_load_query = (app_load_query_def) dlsym(data->libHandle, "QSEECom_app_load_query");
    if(handle->app_load_query == NULL)
    {
        ALOGE("Error loading QSEECom_app_load_query: %s\n", strerror(errno));
        goto exit_err_handle;
    }

    *ret_handle = handle;
    return 0;

exit_err_handle:
    if(handle != NULL) {
        free(handle);
    }
exit_err_dlhandle:
    if(data->libHandle != NULL) {
        dlclose(data->libHandle);
        data->libHandle = NULL;
    }
exit_err_data:
    if(data != NULL)
        free(data);
exit:
    return ret;
}

int qsee_free_handle(struct qsee_handle_t** handle_ptr)
{
    _priv_data_t *data = NULL;
    struct qsee_handle_t *handle;
    handle = *handle_ptr;
    data = (_priv_data_t*)handle->_data;

    dlclose(data->libHandle);
    free(data);
    free(handle);
    *handle_ptr = NULL;
    return 0;
}


int32_t qcom_km_ion_memalloc(struct qcom_km_ion_info_t *handle,
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

int qsee_load_trustlet(struct qsee_handle_t* qsee_handle,
                              struct QSEECom_handle **clnt_handle,
                              const char *path, const char *fname,
                              uint32_t sb_size)
{
    int ret = 0;
    char* errstr;
    int sz = sb_size;
    // Too small size causes failures, so force a reasonable size
    if(sz < 1024) {
        ALOGD("Warning: sb_size too small, increasing to avoid breakage");
        sz = 1024;
    }

    ALOGE("Starting app %s\n", fname);
    ret = qsee_handle->start_app(clnt_handle, path, fname, sz);
    if (ret < 0) {
        errstr = qsee_error_strings(ret);
        ALOGE("Could not load app %s. Error: %s (%d)\n",
              fname, errstr, ret);
    } else
        ALOGE("TZ App loaded : %s\n", fname);

    return ret;
}

