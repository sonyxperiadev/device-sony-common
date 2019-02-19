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
#include "power-helper.h"

#define UNUSED __attribute__((unused))

/* RPM runs at 19.2Mhz. Divide by 19200 for msec */
#define RPM_CLK 19200
extern struct stats_section master_sections[];

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

            /* Check if we're RPM or RPMh */
            legacy_platform = power_helper_is_legacy();

            /* Now start to take powerhints */
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

void Power::getRPMLowPowerStats(
                          getPlatformLowPowerStats_cb _hidl_cb)
{
    hidl_vec<PowerStatePlatformSleepState> states;
    uint64_t stats[RPM_SYSTEM_SLEEP_STATE_COUNT * SYSTEM_STATE_STATS_COUNT] = {0};
    uint64_t *state_stats;
    struct PowerStatePlatformSleepState *state;

    states.resize(RPM_SYSTEM_SLEEP_STATE_COUNT);

    if (extract_rpm_system_stats(stats, ARRAY_SIZE(stats)) != 0) {
        states.resize(0);
        goto done;
    }

    /* Update statistics for XO_shutdown */
    state = &states[SYSTEM_STATE_VLOW];
    state->name = "XO_shutdown";
    state_stats = &stats[SYSTEM_STATE_VLOW * RPM_SYSTEM_SLEEP_STATE_COUNT];
    state->residencyInMsecSinceBoot = state_stats[ACCUMULATED_TIME_MS];
    state->totalTransitions = state_stats[TOTAL_COUNT];
    state->supportedOnlyInSuspend = false;
    state->voters.resize(0);

    /* Update statistics for VMIN */
    state = &states[SYSTEM_STATE_VMIN];
    state->name = "VMIN";
    state_stats = &stats[SYSTEM_STATE_VMIN * RPM_SYSTEM_SLEEP_STATE_COUNT];
    state->residencyInMsecSinceBoot = state_stats[ACCUMULATED_TIME_MS];
    state->totalTransitions = state_stats[TOTAL_COUNT];
    state->supportedOnlyInSuspend = false;
    state->voters.resize(0);

done:
    _hidl_cb(states, Status::SUCCESS);
    return;
}

void Power::getRPMhLowPowerStats(
                          getPlatformLowPowerStats_cb _hidl_cb)
{
    hidl_vec<PowerStatePlatformSleepState> states;
    uint64_t stats[SYSTEM_SLEEP_STATE_COUNT * SYSTEM_STATE_STATS_COUNT] = {0};
    uint64_t *state_stats;
    struct PowerStatePlatformSleepState *state;

    states.resize(SYSTEM_SLEEP_STATE_COUNT);

    if (extract_rpmh_system_stats(stats, ARRAY_SIZE(stats)) != 0) {
        states.resize(0);
        goto done;
    }

    /* Update statistics for AOSD */
    state = &states[SYSTEM_STATE_AOSD];
    state->name = "AOSD";
    state_stats = &stats[SYSTEM_STATE_AOSD * SYSTEM_STATE_STATS_COUNT];
    state->residencyInMsecSinceBoot = state_stats[ACCUMULATED_TIME_MS];
    state->totalTransitions = state_stats[TOTAL_COUNT];
    state->supportedOnlyInSuspend = false;
    state->voters.resize(0);

    /* Update statistics for CXSD */
    state = &states[SYSTEM_STATE_CXSD];
    state->name = "CXSD";
    state_stats = &stats[SYSTEM_STATE_CXSD * SYSTEM_STATE_STATS_COUNT];
    state->residencyInMsecSinceBoot = state_stats[ACCUMULATED_TIME_MS];
    state->totalTransitions = state_stats[TOTAL_COUNT];
    state->supportedOnlyInSuspend = false;
    state->voters.resize(0);

done:
    _hidl_cb(states, Status::SUCCESS);
    return;
}

#ifdef REMOVE_POWER_STATISTICS
Return<void> Power::getPlatformLowPowerStats(
                          getPlatformLowPowerStats_cb _hidl_cb)
{
    hidl_vec<PowerStatePlatformSleepState> states;

    states.resize(0);

    _hidl_cb(states, Status::SUCCESS);
    return Void();
}

Return<void> Power::getSubsystemLowPowerStats(
                          getSubsystemLowPowerStats_cb _hidl_cb)
{
    hidl_vec<PowerStateSubsystem> subsystems;

    subsystems.resize(0);

    _hidl_cb(subsystems, Status::SUCCESS);
    return Void();
}

#else

Return<void> Power::getPlatformLowPowerStats(
                          getPlatformLowPowerStats_cb _hidl_cb)
{
    if (legacy_platform)
        getRPMLowPowerStats(_hidl_cb);
    else
        getRPMhLowPowerStats(_hidl_cb);

    return Void();
}

static int get_master_low_power_stats(
                          hidl_vec<PowerStateSubsystem> *subsystems,
                          bool is_legacy_pltfm)
{
    uint64_t *all_stats;
    uint64_t *section_stats;
    uint8_t  stats_count = MASTER_COUNT;
    uint8_t  master_stats_count = MASTER_STATS_COUNT;
    uint8_t  stats_sz;
    struct PowerStateSubsystem *subsystem;
    struct PowerStateSubsystemSleepState *state;
    int rc = 0;

    if (is_legacy_pltfm) {
        stats_count = RPM_MAX_VOTER_STATS;
        master_stats_count = RPM_MASTER_STATS_COUNT;
    }

    stats_sz = stats_count * master_stats_count;

    all_stats = (uint64_t*) calloc(stats_sz, sizeof(uint64_t));
    if (all_stats == NULL)
        return -1;

    if (is_legacy_pltfm)
        rc = extract_rpm_master_stats(all_stats, stats_sz);
    else
        rc = extract_rpmh_master_stats(all_stats, stats_sz);

    if (rc != 0) {
        for (size_t i = 0; i < stats_count; i++) {
            (*subsystems)[i].name = master_sections[i].label;
            (*subsystems)[i].states.resize(0);
        }

        free(all_stats);
        return -1;
    }

    for (size_t i = 0; i < stats_count; i++) {
        subsystem = &(*subsystems)[i];
        subsystem->name = master_sections[i].label;
        subsystem->states.resize(MASTER_SLEEP_STATE_COUNT);

        state = &(subsystem->states[MASTER_SLEEP]);
        section_stats = &all_stats[i * master_stats_count];

        state->name = "Sleep";
        state->totalTransitions = section_stats[SLEEP_ENTER_COUNT];
        state->residencyInMsecSinceBoot = section_stats[SLEEP_CUMULATIVE_DURATION_MS] / RPM_CLK;
        state->lastEntryTimestampMs = section_stats[SLEEP_LAST_ENTER_TSTAMP_MS] / RPM_CLK;
        state->supportedOnlyInSuspend = false;
    }

    free(all_stats);
    return 0;
}

static int get_wlan_low_power_stats(struct PowerStateSubsystem *subsystem)
{
    uint64_t stats[WLAN_STATS_COUNT] = {0};
    struct PowerStateSubsystemSleepState *state;

    subsystem->name = "wlan";

    if (extract_wlan_stats(stats, ARRAY_SIZE(stats)) != 0) {
        subsystem->states.resize(0);
        return -1;
    }

    subsystem->states.resize(WLAN_SLEEP_STATE_COUNT);

    /* Update statistics for Active State */
    state = &subsystem->states[WLAN_STATE_ACTIVE];
    state->name = "Active";
    state->residencyInMsecSinceBoot = stats[CUMULATIVE_TOTAL_ON_TIME_MS];
    state->totalTransitions = stats[DEEP_SLEEP_ENTER_COUNTER];
    state->lastEntryTimestampMs = 0; //FIXME need a new value from Qcom
    state->supportedOnlyInSuspend = false;

    /* Update statistics for Deep-Sleep state */
    state = &subsystem->states[WLAN_STATE_DEEP_SLEEP];
    state->name = "Deep-Sleep";
    state->residencyInMsecSinceBoot = stats[CUMULATIVE_SLEEP_TIME_MS];
    state->totalTransitions = stats[DEEP_SLEEP_ENTER_COUNTER];
    state->lastEntryTimestampMs = stats[LAST_DEEP_SLEEP_ENTER_TSTAMP_MS];
    state->supportedOnlyInSuspend = false;

    return 0;
}

Return<void> Power::getSubsystemLowPowerStats(
                          getSubsystemLowPowerStats_cb _hidl_cb)
{
    hidl_vec<PowerStateSubsystem> subsystems;
    int stats_cnt = STATS_SOURCE_COUNT;
    int wlan_ss = SUBSYSTEM_WLAN;

    if (legacy_platform) {
        stats_cnt = RPM_MAX_VOTER_STATS + MAX_WLAN_STATS;
        wlan_ss = RPM_VOTER_SLPI + 1;
    }

    subsystems.resize(stats_cnt);

    // Get low power stats for all of the system masters.
    if (get_master_low_power_stats(&subsystems, legacy_platform) != 0) {
        ALOGE("%s: failed to process master stats", __func__);
    }

    // Get WLAN subsystem low power stats.
    if (get_wlan_low_power_stats(&subsystems[wlan_ss]) != 0) {
        ALOGE("%s: failed to process wlan stats", __func__);
    }

    _hidl_cb(subsystems, Status::SUCCESS);
    return Void();
}
#endif /* REMOVE_POWER_STATISTICS */

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
