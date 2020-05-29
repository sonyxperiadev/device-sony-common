/*
 * Simulate a failing EMMC disk.
 * As of version 9.0 Pie 2019-07-12, Android only logs the storage health
 * statistics but does not act upon them, however third-party programs might
 * get access to the HAL data.
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

#ifdef HEALTH_DEBUG

#define LOG_TAG "HealthTestingMock"

#include <android-base/properties.h>
#include <android-base/logging.h>

#include "Mock.h"

namespace device {
namespace sony {
namespace health {

using android::hardware::health::V2_0::DiskStats;
using android::hardware::health::V2_0::StorageInfo;

HealthTestingMock::HealthTestingMock() {
    if (android::base::GetProperty("persist.vendor.health.fake_storage", "") == "1") {
        kHealthMockDebugPropSet = true;
        LOG(VERBOSE) << "Supplying fake storage and disk stats, don't panic...";
    }
}

void HealthTestingMock::FakeStorageInfo(StorageInfo *storage_info) {
    if (!kHealthMockDebugPropSet) {
        return;
    }
    storage_info->attr.isInternal = true;
    storage_info->attr.isBootDevice = true;
    storage_info->attr.name = "MMC0";

    storage_info->eol = 0x03; // "Urgent" -> Battery pretty much dead
    storage_info->version = "emmc 5.1";
    storage_info->lifetimeA = 0x0b; // "Exceeded Maximum Device Life Time"
    storage_info->lifetimeB = 0x0b; // "Exceeded Maximum Device Life Time"
}

void HealthTestingMock::FakeDiskStats(DiskStats *disk_stats) {
    if (!kHealthMockDebugPropSet) {
        return;
    }
    disk_stats->attr.isInternal = true;
    disk_stats->attr.isBootDevice = true;
    disk_stats->attr.name = "MMC0";
    // TODO: Implement stat mocking
}

}  // namespace health
}  // namespace sony
}  // namespace device

#endif  // HEALTH_DEBUG
