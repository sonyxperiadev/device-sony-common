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

#ifndef DEVICE_SONY_HEALTH_STORAGESTATS_H
#define DEVICE_SONY_HEALTH_STORAGESTATS_H

#include <health2/Health.h>

namespace device {
namespace sony {
namespace health {

class StorageStats {
   public:
    StorageStats();

    void GetStorageVariant();
    void FillStoragePaths();
    void GetStorageInfo(std::vector<StorageInfo> &vec_storage_info);
    void GetDiskStats(std::vector<DiskStats> &vec_stats);

   private:
    bool ResolveEmmcPaths();

    enum storagevariants {
        STORAGE_TYPE_EMMC = 1,
        STORAGE_TYPE_UFS = 2
    };
    int kStorageVariant;
};

}  // namespace health
}  // namespace sony
}  // namespace device

#endif  // #ifndef DEVICE_SONY_HEALTH_STORAGESTATS_H
