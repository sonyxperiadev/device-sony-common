/*
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

#ifndef DEVICE_SONY_HEALTH_CYCLECOUNTBACKUPRESTORE_H
#define DEVICE_SONY_HEALTH_CYCLECOUNTBACKUPRESTORE_H

namespace device {
namespace sony {
namespace health {

static const std::string kSysCycleFile = "/sys/class/power_supply/bms/cycle_count";
static const std::string kPersistCycleFile = "/mnt/vendor/persist/battery/battery_cycle_count";

class CycleCountBackupRestore {
   public:
    CycleCountBackupRestore();
    void Restore();
    void Backup(int battery_level);

   private:
    int persist_cycles;
    int sysfs_cycles;
    int saved_soc_;
    int soc_inc_;

    void Read(const std::string &path, int &cycles);
    void Write(int cycles, const std::string &path);
    void UpdateAndSave();
};

}  // namespace health
}  // namespace sony
}  // namespace device

#endif  // #ifndef DEVICE_SONY_HEALTH_CYCLECOUNTBACKUPRESTORE_H
