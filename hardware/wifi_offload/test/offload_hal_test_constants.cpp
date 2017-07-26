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
#include "offload_hal_test_constants.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {
namespace offload_hal_test_constants {

const uint8_t kSsid1[] = {'G', 'o', 'o', 'g', 'l', 'e'};
const size_t kSsid1_size = sizeof(kSsid1);
const uint8_t kSsid2[] = {'X', 'f', 'i', 'n', 'i', 't', 'y'};
const size_t kSsid2_size = sizeof(kSsid2);
const uint8_t kBssid[6] = {0x12, 0xef, 0xa1, 0x2c, 0x97, 0x8b};
const int16_t kRssi = -60;
const int16_t kRssiThreshold = -76;
const uint32_t kFrequency1 = 2412;
const uint32_t kFrequency2 = 2437;
const uint8_t kBssidSize = 6;
const uint64_t kTsf = 0;
const uint16_t kCapability = 0;
const uint8_t kNetworkFlags = 0;
const uint32_t kDisconnectedModeScanIntervalMs = 5000;
const uint64_t kSubscriptionDurationMs = 10000;
const uint64_t kScanDurationMs[2] = {2000, 500};
const uint32_t kNumChannelsScanned[] = {1, 2};
const uint8_t kChannelNotScanned = 0;
const uint32_t kDefaultNumScansRequestedByWifi = 2;
const uint32_t kDefaultNumScansServicedByWifi = 2;
const uint64_t kScanDurationTotalMs = 2000;
const uint32_t kNumChannelsTotalScanned = 3;
const uint32_t kNumChannelsInHistogram = 256;
const uint8_t kTestChannels[] = {1, 6, 11};
const uint32_t kNumTimesChannelScanned[] = {1, 1, 255};
const uint32_t kSystemStartTime = 32164;
const uint32_t kIncrementBetweenEvents = 10;
const size_t kRecordLength = 8;
const size_t kInvalidRecordLength = 4;

const wifi_offload::RpcLogRecord::RpcLogRecordType kChreRecordTypeList[] = {
    wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_INIT,
    wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_CONFIG_SCANS,
    wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_SUBSCRIBE_SCAN_RESULTS,
    wifi_offload::RpcLogRecord::RpcLogRecordType::REQ_SCAN,
    wifi_offload::RpcLogRecord::RpcLogRecordType::EVENT_RECVD_SCAN_RESULT,
    wifi_offload::RpcLogRecord::RpcLogRecordType::EVENT_SENT_SCAN_RESULT,
    wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_UNSUBSCRIBE_SCAN_RESULTS,
    wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_GET_SCAN_STATS,
};

const wifi_offload::RpcLogRecord::RpcLogRecordType kInvalidChreRecordTypeLog[] = {
    wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_LAST_ITEM,
    wifi_offload::RpcLogRecord::RpcLogRecordType::REQ_LAST_ITEM,
    wifi_offload::RpcLogRecord::RpcLogRecordType::EVENT_RECVD_LAST_ITEM,
    wifi_offload::RpcLogRecord::RpcLogRecordType::EVENT_SENT_LAST_ITEM,
};

const android::hardware::wifi::offload::V1_0::RecordName kHidlRecordNameList[] = {
    android::hardware::wifi::offload::V1_0::RecordName::CMD_INT,
    android::hardware::wifi::offload::V1_0::RecordName::CMD_CONFIG_SCANS,
    android::hardware::wifi::offload::V1_0::RecordName::CMD_SUBSCRIBE_SCAN_RESULTS,
    android::hardware::wifi::offload::V1_0::RecordName::REQ_SCAN,
    android::hardware::wifi::offload::V1_0::RecordName::EVENT_RECVD_SCAN_RESULT,
    android::hardware::wifi::offload::V1_0::RecordName::EVENT_SENT_SCAN_RESULT,
    android::hardware::wifi::offload::V1_0::RecordName::CMD_UNSUBSCRIBE_SCAN_RESULTS,
    android::hardware::wifi::offload::V1_0::RecordName::CMD_GET_SCAN_STATS,
};

}  // namespace offload_hal_test_constants
}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
