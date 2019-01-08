/*
 * Shim lib that maps legacy healthd_board_* hooks to vendor lib functions.
 * Needed until the upstream health rework is complete.
 * Uses HealthBoardCommon.cpp functions internally.
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
#include <healthd/healthd.h>

#include <healthboardcommon/HealthBoardCommon.h>

/* healthd_board_init() is called from health@2.0:Health.cpp when
 * the IHealth object is initialized */
//void healthd_board_init(struct healthd_config *config) {
// Discard the pointer:
void healthd_board_init(struct healthd_config *) {
    ::device::sony::health::health_board_battery_init();
}

/* Called from libbatterymonitor/BatteryMonitor.cpp */
/* BatteryMonitor::update() { */
/*     logthis = !healthd_board_battery_update(&props); */
/* } */
int healthd_board_battery_update(struct android::BatteryProperties *props) {
    ::device::sony::health::health_board_battery_update(props);
    // return 0 to log periodic polled battery status to kernel log
    return 0;
}
