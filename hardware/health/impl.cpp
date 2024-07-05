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

#define LOG_TAG "android.hardware.health@2.1-impl-sony"

#include <batteryservice/BatteryService.h>
#include <health/utils.h>
#include <health2impl/Health.h>

#include <string_view>

#include "CycleCountBackupRestore.h"
#include "LearnedCapacityBackupRestore.h"

using ::android::hardware::health::InitHealthdConfig;
using ::android::hardware::health::V2_1::IHealth;

using namespace std::literals;

namespace device {
namespace sony {
namespace health {
using ::android::hardware::health::V2_1::HealthInfo;
using ::android::hardware::health::V2_1::implementation::Health;
using ::device::sony::health::CycleCountBackupRestore;
using ::device::sony::health::LearnedCapacityBackupRestore;

class SonyHealth : public Health {
    CycleCountBackupRestore ccBackupRestore;
    LearnedCapacityBackupRestore lcBackupRestore;

    virtual void UpdateHealthInfo(HealthInfo *health_info) {
        ccBackupRestore.Backup(health_info->legacy.legacy.batteryLevel);
        lcBackupRestore.Backup();
    }

   public:
    SonyHealth(std::unique_ptr<struct healthd_config> &&config) : Health(std::move(config)) {
        ccBackupRestore.Restore();
        lcBackupRestore.Restore();
    }
};
}  // namespace health
}  // namespace sony
}  // namespace device

extern "C" IHealth *HIDL_FETCH_IHealth(const char *instance) {
    if (instance != "default"sv) {
        return nullptr;
    }
    auto config = std::make_unique<healthd_config>();
    InitHealthdConfig(config.get());

    return new device::sony::health::SonyHealth(std::move(config));
}
