/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef OFFLOAD_HAL_TEST_UTILS_
#define OFFLOAD_HAL_TEST_UTILS_

#include "offload_hal_test_constants.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {
namespace offload_hal_test_utils {

void createChreScanResultsForTest(std::vector<wifi_offload::ScanResult>* chreScanResults);
void createChreScanStatsForTest(wifi_offload::ScanStats* chreScanStats);
void createHidlScanFilterForTest(ScanFilter* scanFilter);
void createHidlScanParamForTest(ScanParam* scanParam);
void createChreScanConfigForTest(wifi_offload::ScanConfig* scanConfig);
bool validateScanResult(const std::vector<ScanResult>& hidlScanResultsTest,
                        const std::vector<wifi_offload::ScanResult> chreScanResults);
bool validateScanStats(const ScanStats& hidlScanStats,
                       const wifi_offload::ScanStats& chreScanStats);
}  // namespace offload_hal_test_utils
}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android

#endif  // OFFLOAD_HAL_TEST_UTILS_
