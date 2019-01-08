/*
 * Vendor-specific functions.
 * Persist battery cycle count and capacity across device resets.
 *
 *
 * Copyright (C) 2018 The Android Open Source Project
 * Copyright (C) 2019 Felix Elsner
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

#include <batteryservice/BatteryService.h>

#include "CycleCountBackupRestore.h"
#include "LearnedCapacityBackupRestore.h"

/* Anonymous namespace, don't pollute global: */
namespace {
using ::device::sony::health::CycleCountBackupRestore;
using ::device::sony::health::LearnedCapacityBackupRestore;
static CycleCountBackupRestore ccBackupRestore;
static LearnedCapacityBackupRestore lcBackupRestore;
}

namespace device {
namespace sony {
namespace health {

void health_board_battery_init() {
    ccBackupRestore.Restore();
    lcBackupRestore.Restore();
}

void health_board_battery_update(struct android::BatteryProperties *props) {
    ccBackupRestore.Backup(props->batteryLevel);
    lcBackupRestore.Backup();
}

}  // namespace health
}  // namespace sony
}  // namespace device
