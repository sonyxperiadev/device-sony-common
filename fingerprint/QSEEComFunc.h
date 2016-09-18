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

#ifndef __QSEECOMFUNC_H_
#define __QSEECOMFUNC_H_

#include <dlfcn.h>
#include <stdio.h>
#include <linux/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h> // open function
#include <unistd.h> // close function
#include <linux/msm_ion.h>
#include "QSEEComAPI.h"

typedef int (*start_app_fn)(struct QSEECom_handle **clnt_handle, const char *path, const char *fname, uint32_t sb_size);
typedef int (*shutdown_app_fn)(struct QSEECom_handle **clnt_handle);
typedef int (*load_external_elf)(struct QSEECom_handle **clnt_handle, const char *path,  const char *fname);
typedef int (*unload_external_elf)(struct QSEECom_handle **handle);
typedef int (*register_listener)(struct QSEECom_handle **handle, uint32_t lstnr_id, uint32_t sb_length, uint32_t flags);
typedef int (*unregister_listener)(struct QSEECom_handle *handle);
typedef int (*send_cmd)(struct QSEECom_handle *handle, void *send_buf, uint32_t sbuf_len, void *rcv_buf, uint32_t rbuf_len);
typedef int (*send_modified_cmd)(struct QSEECom_handle *handle, void *send_buf, uint32_t sbuf_len, void *resp_buf, uint32_t rbuf_len, struct QSEECom_ion_fd_info  *ifd_data);
typedef int (*receive_req)(struct QSEECom_handle *handle, void *buf, uint32_t len);
typedef int (*send_resp)(struct QSEECom_handle *handle, void *send_buf, uint32_t len);
typedef int (*set_bandwidth)(struct QSEECom_handle *handle, bool high);
typedef int (*app_load_query)(struct QSEECom_handle *handle, char *app_name);

// DLSYM function pointers
start_app_fn mStartApp;
shutdown_app_fn mStopApp;
load_external_elf load_external_elf_fn;
unload_external_elf unload_external_elf_fn;
register_listener register_listener_fn;
unregister_listener unregister_listener_fn;
send_cmd send_cmd_fn;
send_modified_cmd send_modified_cmd_fn;
receive_req receive_req_fn;
send_resp send_resp_fn;
set_bandwidth set_bandwidth_fn;
app_load_query app_load_query_fn;

struct qcom_km_ion_info_t {
    int32_t ion_fd;
    int32_t ifd_data_fd;
    struct ion_handle_data ion_alloc_handle;
    unsigned char * ion_sbuffer;
    uint32_t sbuf_len;
};

static void *mLibHandle = NULL;

int open_handle();

int32_t qcom_km_ion_dealloc(struct qcom_km_ion_info_t *handle);
int32_t qcom_km_ION_memalloc(struct qcom_km_ion_info_t *handle, uint32_t size);

char* qsee_error_strings(int err);

#endif
