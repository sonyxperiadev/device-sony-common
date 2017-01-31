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

#ifndef __TZAPI_H_
#define __TZAPI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FP_TZAPP_PATH "/etc/firmware/"
#define FP_TZAPP_NAME "tzfingerprint"

#define KM_TZAPP_PATH "/firmware/image/"
#define KM_TZAPP_NAME "kmota"

#define BUFFER_SIZE 64

//enumerate tz app command ID's
enum fingerprint_cmd_t {
    FPC_INIT = 0x00,
    FPC_ENROLL_START = 0x03,
    FPC_ENROLL_STEP = 0x04,
    FPC_ENROLL_END = 0x05,
    FPC_AUTH_START = 0x07,
    FPC_AUTH_STEP = 0x08,
    FPC_AUTH_END = 0x09,
    FPC_CHK_FP_LOST = 0x0A,
    FPC_SET_WAKE = 0x0B,
    FPC_GET_WAKE_TYPE = 0x0C,
    FPC_INIT_NEW_DB = 0x0F,
    FPC_SET_FP_STORE = 0x1A,
    FPC_GET_PRINT_ID = 0x1F,
    FPC_SET_DB_DATA = 0x10,
    FPC_GET_DB_LENGTH = 0x11,
    FPC_GET_DB_DATA = 0x12,
    FPC_GET_ID_LIST = 0x13,
    FPC_GET_ID_COUNT = 0x14,
    FPC_GET_DEL_PRINT = 0x15,
    FPC_CAPTURE_IMAGE = 0x17,
    FPC_GET_INIT_STATE = 0x19,
    FPC_GET_AUTH_HAT = 0x21,
    FPC_GET_DB_ID = 0x20,
    FPC_VERIFY_AUTH_CHALLENGE = 0x22,
    FPC_GET_AUTH_CHALLENGE = 0x23,
    FPC_SET_AUTH_CHALLENGE = 0x24,
    FPC_SET_INIT_DATA = 0x25,
    FPC_INIT_UNK_0 = 0x2A,
    FPC_INIT_UNK_1 = 0x2B,
    FPC_GET_REMAINING_TOUCHES = 0x2C,
    FPC_INIT_UNK_2 = 0x26,
};

typedef struct {
    uint32_t cmd_id;
    uint32_t ret_val; //Some cases this is used for return value of the command
    uint32_t length; //Some length of data supplied by previous modified command
    uint32_t extra; //Some length of data supplied by previous modified command
} fpc_send_std_cmd_t;



typedef struct {
    uint32_t cmd_id;
    uint32_t ret_val; //Some cases this is used for return value of the command
    uint32_t length; //Some length of data supplied by previous modified command
    uint32_t id; //Some length of data supplied by previous modified command
} fpc_send_auth_cmd_t;


typedef struct {
    uint32_t cmd_id;
    uint32_t ret_val; //Some cases this is used for return value of the command
    uint32_t na1; // always 0x45 ?
    uint32_t na2; //???
    uint32_t na3; //???
    uint32_t na4; //???
    uint32_t print_index;
} fpc_send_enroll_start_cmd_t;


typedef struct {
    uint32_t cmd_id;
    uint32_t p1; //Some cases this is used for return value of the command
    uint32_t p2; // always 0x45 ?
    uint32_t p3; //???
    uint32_t p4; //???
    uint32_t p5; //???
    uint32_t print_count;
} fpc_get_pint_index_cmd_t;

typedef struct {
    uint32_t cmd_id;
    uint32_t v_addr; //Virtual address of ion mmap buffer
    uint32_t length; //Length of data on ion buffer
    uint32_t extra; //???
} fpc_send_mod_cmd_t;


typedef struct {
    uint32_t cmd_id;
    uint32_t v_addr;
    uint64_t ret_val; //64bit int
} fpc_send_int64_cmd_t;

#ifdef __cplusplus
}
#endif
#endif
