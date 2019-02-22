/*
 * Copyright (C) 2016-2019 AngeloGioacchino Del Regno <kholk11@gmail.com>
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

#ifndef ANDROID_HARDWARE_POWER_V1_3_POWER_H
#define ANDROID_HARDWARE_POWER_V1_3_POWER_H

#include <atomic>
#include <thread>
#include <android/hardware/power/1.3/IPower.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

#include "common.h"

namespace android {
namespace hardware {
namespace power {
namespace V1_3 {
namespace implementation {

using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_3::IPower;
using ::android::hardware::Return;
using ::android::hardware::Void;
using PowerHint_1_0 = ::android::hardware::power::V1_0::PowerHint;
using PowerHint_1_2 = ::android::hardware::power::V1_2::PowerHint;
using PowerHint_1_3 = ::android::hardware::power::V1_3::PowerHint;

struct Power : public IPower
{
    // Methods from ::android::hardware::power::V1_0::IPower follow.
    Power();

    Return<void> setInteractive(bool /* interactive */) override;
    Return<void> powerHint(PowerHint_1_0 hint, int32_t data) override;
    Return<void> setFeature(Feature feature, bool activate) override;
    Return<void> getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb) override;

    // Methods from ::android::hardware::power::V1_1::IPower follow.
    Return<void> getSubsystemLowPowerStats(getSubsystemLowPowerStats_cb _hidl_cb) override;

    // Methods from ::android::hardware::power::V1_1::IPower follow.
    Return<void> powerHintAsync(PowerHint_1_0 hint, int32_t data) override;

    // Methods from ::android::hardware::power::V1_2::IPower follow.
    Return<void> powerHintAsync_1_2(PowerHint_1_2 hint, int32_t data) override;

    // Methods from ::android::hardware::power::V1_3::IPower follow.
    Return<void> powerHintAsync_1_3(PowerHint_1_3 hint, int32_t data) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    Return<void> debug(const hidl_handle& fd, const hidl_vec<hidl_string>& args) override;

 private:
    std::unique_ptr<RQBalanceHintsHandler> mRQBHandler;

    void getRPMLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb);
    void getRPMhLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb);

    std::atomic<bool> HalInitialized;
    bool legacy_platform;

    std::thread mInitThread;
};

}  // namespace implementation
}  // namespace V1_3
}  // namespace power
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_POWER_V1_3_POWER_H
