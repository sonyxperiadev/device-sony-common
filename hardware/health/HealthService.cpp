/*
 * Entrypoint for the main HAL service.
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

#define LOG_TAG "android.hardware.health@2.0-service.sony"

/* For health_service_main() */
#include <health2/service.h>

#include <batteryservice/BatteryService.h>
#include <health2/Health.h>
#include <healthd/healthd.h>

#include "CycleCountBackupRestore.h"
#include "LearnedCapacityBackupRestore.h"

namespace {
using ::device::sony::health::CycleCountBackupRestore;
using ::device::sony::health::LearnedCapacityBackupRestore;
static CycleCountBackupRestore ccBackupRestore;
static LearnedCapacityBackupRestore lcBackupRestore;
}  // namespace

/* healthd_board_init() is called from health@2.0:Health.cpp when
 * the IHealth object is initialized */
void healthd_board_init(struct healthd_config *) {
    ccBackupRestore.Restore();
    lcBackupRestore.Restore();
}

/* Called from libbatterymonitor/BatteryMonitor.cpp */
/* BatteryMonitor::update() { */
/*     logthis = !healthd_board_battery_update(&props); */
/* } */
int healthd_board_battery_update(struct android::BatteryProperties *props) {
    ccBackupRestore.Backup(props->batteryLevel);
    lcBackupRestore.Backup();
    // return 0 to log periodic polled battery status to kernel log
    return 0;
}

int main() {

    /* Setting the instance name explicitly is better */
    return health_service_main("default");

    /* health_service_main() from libhealthservice:HealthServiceCommon.cpp */
    /* sets: healthd_mode_ops = &healthd_mode_service_2_0_ops, */
    /* returns healthd_main() from health@2.0/healthd_common.cpp */
    /* healthd_main() runs healthd_init(): */
    /*  -> healthd_mode_ops->init(&healthd_config), */
    /*     ( + wakealarm_init(), uevent_init() ) */
    /* healthd_mode_ops->init is HealthServiceCommon.cpp:healthd_mode_service_2_0_init() */
    /* healthd_mode_service_2_0_init() then starts up the IHealth instance via */
    /* IHealth service = Health::initInstance(config); */
    /* After healthd_init() is done, healthd_common.cpp starts healthd_mainloop() */
    /* The mainloop then sets up some polling and runs forever, reacting to */
    /* "SUBSYSTEM=power_supply" uevents and setting timers */
}
