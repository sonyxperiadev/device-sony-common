/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <gtest/gtest.h>

#include <android-base/logging.h>
#include "offload_hal_test_utils.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

using namespace offload_hal_test_constants;

class OffloadUtilsTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    void TearDown() override {
    }
};

TEST_F(OffloadUtilsTest, ConvertScanResultToHidlTest) {
    std::vector<ScanResult> hidlScanResultsTest;
    std::vector<wifi_offload::ScanResult> chreScanResultsTest;
    offload_hal_test_utils::createChreScanResultsForTest(&chreScanResultsTest);
    EXPECT_TRUE(offload_utils::ToHidlScanResults(chreScanResultsTest, &hidlScanResultsTest));
    EXPECT_TRUE(
        offload_hal_test_utils::validateScanResult(hidlScanResultsTest, chreScanResultsTest));
}

TEST_F(OffloadUtilsTest, ConvertScanStatsToHidlTest) {
    ScanStats hidlScanStatsTest;
    wifi_offload::ScanStats chreScanStatsTest;
    offload_hal_test_utils::createChreScanStatsForTest(&chreScanStatsTest);
    EXPECT_TRUE(offload_utils::ToHidlScanStats(chreScanStatsTest, &hidlScanStatsTest));
    EXPECT_TRUE(offload_hal_test_utils::validateScanStats(hidlScanStatsTest, chreScanStatsTest));
}

TEST_F(OffloadUtilsTest, ConvertScanConfigToChreTest) {
    wifi_offload::ScanConfig scanConfigTest;
    wifi_offload::ScanConfig scanConfig;
    ScanParam scanParamTest;
    ScanFilter scanFilterTest;
    offload_hal_test_utils::createHidlScanParamForTest(&scanParamTest);
    offload_hal_test_utils::createHidlScanFilterForTest(&scanFilterTest);
    offload_hal_test_utils::createChreScanConfigForTest(&scanConfig);
    EXPECT_TRUE(offload_utils::ToChreScanConfig(scanParamTest, scanFilterTest, &scanConfigTest));
    EXPECT_TRUE(scanConfig == scanConfigTest);
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
