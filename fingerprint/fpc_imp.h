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

#ifndef __FPCIMPL_H_
#define __FPCIMPL_H_

#include "QSEEComFunc.h"
#include <stdbool.h>
#include "tz_api.h"

struct QSEECom_handle * mHandle;
struct QSEECom_handle * mHdl;

uint64_t fpc_load_db_id(); //load db ID, used as authenticator ID in android
uint64_t fpc_load_auth_challange(); //genertate and load an auth challange for pre enroll
uint32_t fpc_set_auth_challange(); //set auth challange during authenticate
int fpc_verify_auth_challange(void* hat, uint32_t size); //verify auth challage before enroll (ensure its still valid)
int fpc_get_hw_auth_obj(void * buffer, int length); //get HAT object (copied into buffer) on authenticate
uint32_t fpc_get_remaining_touches(); //get remaining prints needed during enroll
uint32_t fpc_get_print_id(int id); //gets uid of print from index provided
uint32_t fpc_get_print_count(); //get count of enrolled prints
uint32_t fpc_del_print_id(uint32_t id); //delete print at index
fpc_get_pint_index_cmd_t fpc_get_print_index(int count); //get list of print index's available
int fpc_wait_for_finger(); //wait for event IRQ on print reader
int fpc_capture_image(); //capture image ready for enroll / auth
int fpc_enroll_step(); //step forward enroll & process image (only available if capture image returns OK)
int fpc_enroll_start(int print_index); //start enrollment
int fpc_enroll_end(); //end enrollment
int fpc_auth_start(); //start auth
uint32_t fpc_auth_step(); //step forward auth & process image (only available if capture image returns OK)
int fpc_auth_end(); //end auth
uint32_t fpc_get_user_db_length(); //get size of working db
uint32_t fpc_load_user_db(char* path); //load user DB into TZ app from storage
uint32_t fpc_store_user_db(uint32_t length, char* path); //store running TZ db
int fpc_close(); //close this implementation
int fpc_init(); //init sensor

#endif
