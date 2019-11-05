/*
 * Read maximum battery capacity from sysfs, then compare with
 * value stored in /mnt/vendor/persist/battery/battery_charge_full.
 * If one is lower than the other, restore the lower value.
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

#include <stdexcept>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include "LearnedCapacityBackupRestore.h"

namespace device {
namespace sony {
namespace health {

static constexpr int kLCBackupTrigger = 8;

static constexpr int kBuffSize = 256;
/* Battery capacity is given in microampere hours */
/* Divide by 1000 to get milliampere hours("mAh") */
static constexpr int kCapConversionFactor = 1000;

LearnedCapacityBackupRestore::LearnedCapacityBackupRestore() {
    cap_counter = 0;
}

void LearnedCapacityBackupRestore::Restore() {
    ReadFromPersistStorage();
    ReadFromSRAM();
    UpdateAndSave();
}

void LearnedCapacityBackupRestore::Backup() {
    if (++cap_counter >= kLCBackupTrigger) {
        ReadFromSRAM();
        UpdateAndSave();
        cap_counter = 0;
    }
}

void LearnedCapacityBackupRestore::ReadFromPersistStorage() {
    std::string buffer;

    if (!android::base::ReadFileToString(kPersistChargeFullFile, &buffer)) {
        LOG(WARNING) << "Could not read battery capacity persist file from "
                     << kPersistChargeFullFile << ": " << strerror(errno);
        return;
    }

    buffer = ::android::base::Trim(buffer);
    try {
        persist_capacity = std::stoi(buffer);
    } catch (std::out_of_range &e) {
        LOG(WARNING) << "Battery capacity in persist storage file is out of bounds: " << buffer;
        return;
    } catch (std::invalid_argument &e) {
        LOG(WARNING) << "Data format is wrong in the battery capacity persist file: " << buffer;
        return;
    }
    LOG(VERBOSE) << " Read max battery capacity of "
                 << persist_capacity / kCapConversionFactor << " mAh from persist storage";
}

void LearnedCapacityBackupRestore::SaveToPersistStorage() {
    char strData[kBuffSize];

    snprintf(strData, kBuffSize, "%d", persist_capacity);

    if (!android::base::WriteStringToFile(strData, kPersistChargeFullFile)) {
        LOG(WARNING) << "Write battery capacity persist file error: " << strerror(errno);
        return;
    }
    LOG(INFO) << "Saved learned max battery capacity of "
              << persist_capacity / kCapConversionFactor << " mAh to persist storage";
}

void LearnedCapacityBackupRestore::ReadFromSRAM() {
    std::string buffer;

    if (!android::base::ReadFileToString(kSysChargeFullFile, &buffer)) {
        LOG(WARNING) << "Read max battery capacity from sysfs error: " << strerror(errno);
        return;
    }

    buffer = android::base::Trim(buffer);

    if (sscanf(buffer.c_str(), "%d", &sysfs_capacity) < 1) {
        LOG(WARNING) << "Failed to parse sysfs battery capacity data: " << buffer;
        return;
    }
    LOG(VERBOSE) << "Read learned max battery capaticy from sysfs: "
                 << sysfs_capacity / kCapConversionFactor << " mAh";
}

void LearnedCapacityBackupRestore::SaveToSRAM() {
    char strData[kBuffSize];

    snprintf(strData, kBuffSize, "%d", sysfs_capacity);

    if (!android::base::WriteStringToFile(strData, kSysChargeFullFile)) {
        LOG(WARNING) << "Write max battery capacity to sysfs error: " << strerror(errno);
        return;
    }
    LOG(INFO) << "Successfully restored max battery capacity of "
              << sysfs_capacity / kCapConversionFactor << " mAh";
}

void LearnedCapacityBackupRestore::UpdateAndSave() {
    if (sysfs_capacity) {
        if ((sysfs_capacity < persist_capacity) || (persist_capacity == 0)) {
            persist_capacity = sysfs_capacity;
            SaveToPersistStorage();
        } else if (sysfs_capacity > persist_capacity) {
            sysfs_capacity = persist_capacity;
            SaveToSRAM();
        }
    }
}

}  // namespace health
}  // namespace sony
}  // namespace device
