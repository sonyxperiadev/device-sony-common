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
    cap_inc_ = 0;
}

void LearnedCapacityBackupRestore::Restore() {
    ReadFromPersistStorage();
    ReadFromSRAM();
    UpdateAndSave();
}

void LearnedCapacityBackupRestore::Backup() {
    if (++cap_inc_ >= kLCBackupTrigger) {
        LOG(VERBOSE) << "LC: Triggered ReadFromSRAM() and UpdateAndSave() !";
        ReadFromSRAM();
        UpdateAndSave();
        cap_inc_ = 0;
    }
}

void LearnedCapacityBackupRestore::ReadFromPersistStorage() {
    std::string buffer;

    if (!android::base::ReadFileToString(std::string(kPersistChargeFullFile), &buffer)) {
        LOG(ERROR) << "Cannot read battery capacity persist file from " << kPersistChargeFullFile << ": " << strerror(errno);
        return;
    }

    if (sscanf(buffer.c_str(), "%d", &sw_cap_) < 1) {
        LOG(ERROR) << "Data format is wrong in the battery capacity persist file: " << buffer;
        return;
    }
    LOG(VERBOSE) << " Read max battery capacity of " << sw_cap_ / kCapConversionFactor << " mAh from persist storage";
}

void LearnedCapacityBackupRestore::SaveToPersistStorage() {
    char strData[kBuffSize];

    snprintf(strData, kBuffSize, "%d", sw_cap_);

    if (!android::base::WriteStringToFile(strData, std::string(kPersistChargeFullFile))) {
        LOG(ERROR) << "Write battery capacity persist file error: " << strerror(errno);
        return;
    }
    LOG(INFO) << "Saved learned max battery capacity of " << sw_cap_ / kCapConversionFactor << " mAh to persist storage";
}

void LearnedCapacityBackupRestore::ReadFromSRAM() {
    std::string buffer;

    if (!android::base::ReadFileToString(std::string(kSysChargeFullFile), &buffer)) {
        LOG(ERROR) << "Read max battery capacity from sysfs error: " << strerror(errno);
        return;
    }

    buffer = android::base::Trim(buffer);

    if (sscanf(buffer.c_str(), "%d", &hw_cap_) < 1) {
        LOG(ERROR) << "Failed to parse sysfs battery capacity data: " << buffer;
        return;
    }
    LOG(VERBOSE) << "Read learned max battery capaticy from sysfs: " << hw_cap_ / kCapConversionFactor << " mAh";
}

void LearnedCapacityBackupRestore::SaveToSRAM() {
    char strData[kBuffSize];

    snprintf(strData, kBuffSize, "%d", hw_cap_);

    if (!android::base::WriteStringToFile(strData, std::string(kSysChargeFullFile))) {
        LOG(ERROR) << "Write max battery capacity to sysfs error: " << strerror(errno);
        return;
    }
    LOG(INFO) << "Successfully restored max battery capacity of " << hw_cap_ / kCapConversionFactor << " mAh";
}

void LearnedCapacityBackupRestore::UpdateAndSave() {
    if (hw_cap_) {
        if ((hw_cap_ < sw_cap_) || (sw_cap_ == 0)) {
            sw_cap_ = hw_cap_;
            SaveToPersistStorage();
        } else if (hw_cap_ > sw_cap_) {
            hw_cap_ = sw_cap_;
            SaveToSRAM();
        }
    }
}

}  // namespace health
}  // namespace sony
}  // namespace device
