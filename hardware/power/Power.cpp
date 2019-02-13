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

#define LOG_TAG "RQBalance-PowerHAL"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>
#include <android-base/stringprintf.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <mutex>
#include <pwd.h>
#include <utils/Log.h>
#include <utils/Trace.h>

#include "common.h"
#include "RQBalanceHALExt.h"
#include "Hints.h"
#include "Power.h"

#define UNUSED __attribute__((unused))

namespace android {
namespace hardware {
namespace power {
namespace V1_3 {
namespace implementation {

using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_0::PowerStatePlatformSleepState;
using ::android::hardware::power::V1_0::Status;
using ::android::hardware::power::V1_1::PowerStateSubsystem;
using ::android::hardware::power::V1_1::PowerStateSubsystemSleepState;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

Power::Power() :
        mRQBHandler(nullptr),
        HalInitialized(false) {
    mInitThread = std::thread([this]()
        {
            mRQBHandler = std::make_unique<RQBalanceHintsHandler>();
            int rc = mRQBHandler->InitializeHAL();
            if (rc)
                return;

            // Now start to take powerhint
            HalInitialized.store(true);
        });
    mInitThread.detach();
}

Return<void> Power::powerHint(PowerHint_1_0 hint, int32_t data)
{
    if (!HalInitialized) {
        return Void();
    }

    switch(hint) {
        case PowerHint_1_0::VSYNC:
        case PowerHint_1_0::INTERACTION:
            break;

        case PowerHint_1_0::LOW_POWER:
            if (data) {
                mRQBHandler->SetPowerMode(POWER_MODE_BATTERYSAVE);
            } else {
                mRQBHandler->SetPowerMode(POWER_MODE_BALANCED);
            }
            break;

        case PowerHint_1_0::SUSTAINED_PERFORMANCE:
            if (data) {
                mRQBHandler->SetPowerMode(POWER_MODE_SUSTAINED);
            } else {
                mRQBHandler->SetPowerMode(POWER_MODE_BALANCED);
            }
            break;

        case PowerHint_1_0::VR_MODE:
            if (data) {
                mRQBHandler->SetPowerMode(POWER_MODE_SUSTAINED);
            } else {
                mRQBHandler->SetPowerMode(POWER_MODE_BALANCED);
            }
            break;

        case PowerHint_1_0::LAUNCH:
            if (data) {
                mRQBHandler->SetPowerMode(POWER_MODE_PERFORMANCE);
            } else {
                mRQBHandler->SetPowerMode(POWER_MODE_BALANCED);
            }
            break;


        default:
            break;
    }
    return Void();
}


Return<void> Power::powerHintAsync(PowerHint_1_0 hint, int32_t data)
{
    // just call the normal power hint in this oneway function
    return powerHint(hint, data);
}

// Methods from ::android::hardware::power::V1_2::IPower follow.
Return<void> Power::powerHintAsync_1_2(PowerHint_1_2 hint, int32_t data)
{
    if (!HalInitialized) {
        return Void();
    }

    /* Stub: Not yet supported in RQBalance-PowerHAL, but support planned */
    switch(hint) {
        case PowerHint_1_2::AUDIO_LOW_LATENCY:
            break;
        case PowerHint_1_2::AUDIO_STREAMING:
            break;
        case PowerHint_1_2::CAMERA_LAUNCH:
            break;
        case PowerHint_1_2::CAMERA_STREAMING:
            break;
        case PowerHint_1_2::CAMERA_SHOT:
            break;
        default:
            return powerHint(static_cast<PowerHint_1_0>(hint), data);
    }
    return Void();
}

// Methods from ::android::hardware::power::V1_3::IPower follow.
Return<void> Power::powerHintAsync_1_3(PowerHint_1_3 hint, int32_t data)
{
    if (!HalInitialized) {
        return Void();
    }

    /* Stub: Not yet supported in RQBalance-PowerHAL, but support planned */
    if (hint == PowerHint_1_3::EXPENSIVE_RENDERING) {
        return Void();
    } else {
        return powerHintAsync_1_2(static_cast<PowerHint_1_2>(hint), data);
    }
    return Void();
}

/*
 * set_interactive - Performs power management actions for awake/sleep
 *
 * \param module - This HAL's info sym struct
 * \param on - 1: System awake 0: System asleep
 */
Return<void> Power::setInteractive(bool interactive)
{
    if (!HalInitialized)
        return Void();

    if (!interactive) {
        ALOGI("Device is asleep.");

        /* Stop PowerServer: we don't need it while sleeping */
        mRQBHandler->ManagePowerServer(false);

        mRQBHandler->SetPowerMode(POWER_MODE_BATTERYSAVE);
    } else {
        ALOGI("Device is awake.");

        /* Restart PowerServer */
        mRQBHandler->ManagePowerServerSafe(true);

        mRQBHandler->SetPowerMode(POWER_MODE_BALANCED);
    }

    return Void();
}

Return<void> Power::getPlatformLowPowerStats(
                          getPlatformLowPowerStats_cb _hidl_cb UNUSED)
{
    return Void();
}

Return<void> Power::getSubsystemLowPowerStats(
                          getSubsystemLowPowerStats_cb _hidl_cb UNUSED)
{
    return Void();
}

Return<void> Power::debug(const hidl_handle& handle UNUSED,
                          const hidl_vec<hidl_string>&)
{
    return Void();
}

Return<void> Power::setFeature(Feature feature, bool activate)
{
    if (feature == Feature::POWER_FEATURE_DOUBLE_TAP_TO_WAKE) {
#ifdef TAP_TO_WAKE_NODE
            ALOGI("Double tap to wake is %s.",
                   activate ? "enabled" : "disabled");
            sysfs_write(TAP_TO_WAKE_NODE, activate ? "1" : "0");
#else
            ALOGD("Tried to set DT2W %s, but no node declared in HAL!",
                   activate ? "enabled" : "disabled");
#endif
    }

    return Void();
}

}  // namespace implementation
}  // namespace V1_3
}  // namespace power
}  // namespace hardware
}  // namespace android
