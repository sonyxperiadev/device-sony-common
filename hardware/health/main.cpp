/*
 * Entrypoint for the main HAL service.
 *
 * Copyright (C) 2018 The Android Open Source Project
 * Copyright (C) 2019 Felix Elsner
 * Copyright (C) 2025 Pavel Dubrova
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

#define LOG_TAG "android.hardware.health-service.sony"

#include <android-base/logging.h>
#include <android/binder_interface_utils.h>
#include <health-impl/Health.h>
#include <health/utils.h>

#ifndef CHARGER_FORCE_NO_UI
#include <health-impl/ChargerUtils.h>
#endif

#include "CycleCountBackupRestore.h"
#include "LearnedCapacityBackupRestore.h"

using aidl::android::hardware::health::HalHealthLoop;
using aidl::android::hardware::health::HealthInfo;
using android::hardware::health::InitHealthdConfig;

#ifndef CHARGER_FORCE_NO_UI
using aidl::android::hardware::health::charger::ChargerCallback;
using aidl::android::hardware::health::charger::ChargerModeMain;

namespace aidl::android::hardware::health {
class ChargerCallbackImpl : public ChargerCallback {
    bool ChargerEnableSuspend() override { return true; }

   public:
    ChargerCallbackImpl(const std::shared_ptr<aidl::android::hardware::health::Health>& service)
        : ChargerCallback(service) {}
};
}  // namespace aidl::android::hardware::health
#endif

namespace aidl::android::hardware::health::implementation {
using device::sony::health::CycleCountBackupRestore;
using device::sony::health::LearnedCapacityBackupRestore;

class HealthImpl : public Health {
    CycleCountBackupRestore ccBackupRestore;
    LearnedCapacityBackupRestore lcBackupRestore;

    void UpdateHealthInfo(HealthInfo* health_info) override {
        ccBackupRestore.Backup(health_info->batteryLevel);
        lcBackupRestore.Backup();
    }

   public:
    HealthImpl(std::string_view instance_name, std::unique_ptr<healthd_config>&& config)
        : Health(std::move(instance_name), std::move(config)) {
        ccBackupRestore.Restore();
        lcBackupRestore.Restore();
    }
};
}  // namespace aidl::android::hardware::health::implementation

static void wait_for_device() {
    std::vector<std::string> paths = {
        "/sys/class/power_supply/bms/capacity",
        "/sys/class/power_supply/battery/capacity"
    };
    for (int retry = 0; retry < 30; retry++) {
        for (const auto& path : paths) {
            if (access(path.c_str(), F_OK) == 0) {
                return;
            }
        }
        sleep(1);
    }
}

int main(int argc, char** argv) {
    using aidl::android::hardware::health::implementation::HealthImpl;
#ifdef __ANDROID_RECOVERY__
    android::base::InitLogging(argv, android::base::KernelLogger);
#endif

    wait_for_device();

    auto config = std::make_unique<healthd_config>();
    InitHealthdConfig(config.get());

    auto binder = ndk::SharedRefBase::make<HealthImpl>("default", std::move(config));

    if (argc >= 2 && argv[1] == std::string_view("--charger")) {
#ifndef CHARGER_FORCE_NO_UI
        return ChargerModeMain(binder, std::make_shared<aidl::android::hardware::health::ChargerCallbackImpl>(binder));
#endif
        LOG(INFO) << "Starting charger mode without UI.";
    } else {
        LOG(INFO) << "Starting health HAL.";
    }

    auto hal_health_loop = std::make_shared<HalHealthLoop>(binder, binder);
    return hal_health_loop->StartLoop();
}
