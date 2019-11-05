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

#ifndef DEVICE_SONY_HEALTH_LEARNEDCAPACITYBACKUPRESTORE_H
#define DEVICE_SONY_HEALTH_LEARNEDCAPACITYBACKUPRESTORE_H

namespace device {
namespace sony {
namespace health {

static const std::string kSysChargeFullFile = "/sys/class/power_supply/bms/charge_full";
static const std::string kPersistChargeFullFile = "/mnt/vendor/persist/battery/battery_charge_full";

class LearnedCapacityBackupRestore {
   public:
    LearnedCapacityBackupRestore();
    void Restore();
    void Backup();

   private:
    int persist_capacity;
    int sysfs_capacity;
    int cap_counter;

    void ReadFromPersistStorage();
    void SaveToPersistStorage();
    void ReadFromSRAM();
    void SaveToSRAM();
    void UpdateAndSave();
};

}  // namespace health
}  // namespace sony
}  // namespace device

#endif  // #ifndef DEVICE_SONY_HEALTH_LEARNEDCAPACITYBACKUPRESTORE_H
