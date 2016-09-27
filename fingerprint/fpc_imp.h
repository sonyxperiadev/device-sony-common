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

#include <stdbool.h>

#define MAX_FINGERPRINTS 5
typedef struct
{
    uint32_t print_count;
    uint32_t prints[MAX_FINGERPRINTS];
} fpc_fingerprint_index_t;

uint64_t fpc_load_db_id(); //load db ID, used as authenticator ID in android
uint64_t fpc_load_auth_challenge(); //genertate and load an auth challenge for pre enroll
int fpc_set_auth_challenge(int64_t challenge); //set auth challenge during authenticate
int fpc_verify_auth_challenge(void* hat, uint32_t size); //verify auth challenge before enroll (ensure its still valid)
int fpc_get_hw_auth_obj(void * buffer, int length); //get HAT object (copied into buffer) on authenticate
// FIXME: This should probably only exist inside kitakami implementation?
// Make all functions return resolved index->id
// FIXME: Internal to kitakami:
uint32_t fpc_get_print_id(int id);
uint32_t fpc_get_print_count(); //get count of enrolled prints
uint32_t fpc_del_print_id(uint32_t id); //delete print at index
fpc_fingerprint_index_t fpc_get_print_ids(int count); //get list of print index's available
fpc_fingerprint_index_t fpc_get_print_index(int count); //get list of print index's available
int fpc_wait_for_finger(); //wait for event IRQ on print reader
int fpc_capture_image(); //capture image ready for enroll / auth
int fpc_enroll_step(uint32_t *remaining_touches); //step forward enroll & process image (only available if capture image returns OK)
// FIXME: index of next print should be retrieved using fpc_get-print_count internally in kitakami impl.
int fpc_enroll_start(int print_index); //start enrollment
int fpc_enroll_end(uint32_t *print_id); //end enrollment
int fpc_auth_start(); //start auth
uint32_t fpc_auth_step(uint32_t *print_id); //step forward auth & process image (only available if capture image returns OK)
int fpc_auth_end(); //end auth
// FIXME: This should be internal to kitakami implementation
uint32_t fpc_get_user_db_length(); //get size of working db
int fpc_set_gid(uint32_t gid);
int fpc_load_user_db(char* path); //load user DB into TZ app from storage
// FIXME: length should be fetched internally in kitakami implementation
int fpc_store_user_db(uint32_t length, char* path); //store running TZ db
int fpc_close(); //close this implementation
int fpc_init(); //init sensor

#endif
