/*
 * Read battery cycle count from sysfs, then compare with value stored in
 * /mnt/vendor/persist/battery/battery_cycle_count.
 * If one is higher than the other, restore the higher value.
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

#include "CycleCountBackupRestore.h"

namespace device {
namespace sony {
namespace health {

static constexpr int kCCBackupTrigger = 20;

CycleCountBackupRestore::CycleCountBackupRestore() {
    sw_cycles_ = 0;
    hw_cycles_ = 0;
    saved_soc_ = -1;
    soc_inc_ = 0;
}

void CycleCountBackupRestore::Restore() {
    /* Our battery driver doesn't seem to report a battery
     * serial number, so we have to instruct users to wipe
     * /mnt/vendor/persist/battery/ when they swap batteries */
    Read(kPersistCycleFile, sw_cycles_);
    Read(kSysCycleFile, hw_cycles_);
    UpdateAndSave();
}

void CycleCountBackupRestore::Backup(int battery_level) {
    if (saved_soc_ == -1) {
        saved_soc_ = battery_level;
        return;
    }
    // Cycle counts only increases on increasing level
    if (battery_level > saved_soc_) {
        soc_inc_ += battery_level - saved_soc_;
    }
    saved_soc_ = battery_level;
    // To avoid writting file too often just rate limit it
    if (soc_inc_ >= kCCBackupTrigger) {
        LOG(VERBOSE) << "CC: Triggered Read(kSysCycleFile) and UpdateAndSave() !";
        Read(kSysCycleFile, hw_cycles_);
        UpdateAndSave();
        soc_inc_ = 0;
    }
}

void CycleCountBackupRestore::Read(const std::string &path, int &cycles) {
    std::string buffer;

    if (!android::base::ReadFileToString(path, &buffer)) {
        LOG(ERROR) << "Failed to read battery cycles from " << path;
        return;
    }

    buffer = ::android::base::Trim(buffer);
    try {
        cycles = std::stoi(buffer);
    } catch (std::out_of_range &e) {
        LOG(ERROR) << "Battery cycle count in persist storage file is out of bounds: " << path;
        return;
    } catch (std::invalid_argument &e) {
        LOG(ERROR) << "Data format is wrong in persist storage file: " << path;
        return;
    }
    LOG(VERBOSE) << "Read " << cycles << " battery cycles from " << path;
}

void CycleCountBackupRestore::Write(int cycles, const std::string &path) {
    std::string str_data = std::to_string(cycles);

    if (!android::base::WriteStringToFile(str_data, path)) {
        LOG(ERROR) << "Write battery cycles to " << path << " error: " << strerror(errno);
        return;
    }
    LOG(INFO) << "Wrote " << str_data << " battery cycles to " << path;
}

void CycleCountBackupRestore::UpdateAndSave() {
    if (hw_cycles_ < sw_cycles_) {
        hw_cycles_ = sw_cycles_;
        Write(hw_cycles_, kSysCycleFile);
    } else if (hw_cycles_ > sw_cycles_) {
        sw_cycles_ = hw_cycles_;
        Write(sw_cycles_, kPersistCycleFile);
    }
}

}  // namespace health
}  // namespace sony
}  // namespace device
