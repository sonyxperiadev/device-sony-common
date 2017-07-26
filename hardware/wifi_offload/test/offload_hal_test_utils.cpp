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
#include "offload_hal_test_utils.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {
namespace offload_hal_test_utils {

using namespace offload_hal_test_constants;

void createChreScanResultsForTest(std::vector<wifi_offload::ScanResult>* chreScanResults) {
    wifi_offload::ScanResult chreScanResult;
    chreScanResult.security_modes_ = kNetworkFlags;
    chreScanResult.capability_ = kCapability;
    chreScanResult.frequency_scanned_mhz_ = kFrequency1;
    chreScanResult.rssi_dbm_ = kRssi;
    chreScanResult.tsf_ = kTsf;
    chreScanResult.ssid_.SetData(&kSsid1[0], kSsid1_size);
    memcpy(&chreScanResult.bssid_[0], &kBssid[0], kBssidSize);
    chreScanResults->push_back(chreScanResult);
}

void createChreScanStatsForTest(wifi_offload::ScanStats* chreScanStats) {
    chreScanStats->num_scans_requested_by_nanoapp_ = kDefaultNumScansRequestedByWifi;
    chreScanStats->num_scans_serviced_by_hardware_ = kDefaultNumScansServicedByWifi;
    chreScanStats->num_scans_serviced_by_cache_ =
        (kDefaultNumScansRequestedByWifi - kDefaultNumScansServicedByWifi);
    for (size_t j = 0; j < kNumChannelsScanned[0] + kNumChannelsScanned[1]; j++) {
        chreScanStats->channel_histogram_.IncrementScanCountForChannelForTest(
            kTestChannels[j], static_cast<uint32_t>(kNumTimesChannelScanned[j]));
    }
    chreScanStats->last_subscription_duration_ms_ = kSubscriptionDurationMs;
    wifi_offload::ScanRecord scanRecord1;
    scanRecord1.time_spent_scanning_ms_ = kScanDurationMs[0];
    scanRecord1.num_channels_scanned_ = kNumChannelsScanned[0];
    scanRecord1.num_entries_aggregated_ = 1;
    chreScanStats->scan_records_.push_back(scanRecord1);
    wifi_offload::ScanRecord scanRecord2;
    scanRecord2.time_spent_scanning_ms_ = kScanDurationMs[1];
    scanRecord2.num_channels_scanned_ = kNumChannelsScanned[1];
    scanRecord2.num_entries_aggregated_ = 1;
    chreScanStats->scan_records_.push_back(scanRecord2);

    for (size_t i = 0; i < kRecordLength; i++) {
        wifi_offload::RpcLogRecord logRecord;
        logRecord.record_type_ = kChreRecordTypeList[i];
        logRecord.timestamp_chre_ms_ = kSystemStartTime + i * kIncrementBetweenEvents;
        chreScanStats->rpc_log_records_.push_back(logRecord);
    }
}

void createHidlScanFilterForTest(ScanFilter* scanFilter) {
    std::vector<NetworkInfo> nw_info_list;
    NetworkInfo nwInfo1;
    std::vector<uint8_t> ssid1(kSsid1, kSsid1 + kSsid1_size);
    nwInfo1.ssid = ssid1;
    nwInfo1.flags = kNetworkFlags;
    NetworkInfo nwInfo2;
    std::vector<uint8_t> ssid2(kSsid2, kSsid2 + kSsid2_size);
    nwInfo2.ssid = ssid2;
    nwInfo2.flags = kNetworkFlags;
    nw_info_list.push_back(nwInfo1);
    nw_info_list.push_back(nwInfo2);
    scanFilter->preferredNetworkInfoList = nw_info_list;
    scanFilter->rssiThreshold = kRssiThreshold;
}

void createHidlScanParamForTest(ScanParam* scanParam) {
    scanParam->disconnectedModeScanIntervalMs = kDisconnectedModeScanIntervalMs;
    std::vector<uint32_t> frequencyList{kFrequency1, kFrequency2};
    scanParam->frequencyList = frequencyList;
    std::vector<hidl_vec<uint8_t>> ssidList;
    std::vector<uint8_t> ssid1(kSsid1, kSsid1 + kSsid1_size);
    std::vector<uint8_t> ssid2(kSsid2, kSsid2 + kSsid2_size);
    ssidList.push_back(ssid1);
    ssidList.push_back(ssid2);
    scanParam->ssidList = ssidList;
}

void createChreScanConfigForTest(wifi_offload::ScanConfig* scanConfig) {
    scanConfig->scan_params_.disconnected_mode_scan_interval_ms_ = kDisconnectedModeScanIntervalMs;
    wifi_offload::Ssid chreSsid1;
    chreSsid1.SetData(&kSsid1[0], kSsid1_size);
    wifi_offload::Ssid chreSsid2;
    chreSsid2.SetData(&kSsid2[0], kSsid2_size);
    scanConfig->scan_params_.ssids_to_scan_.push_back(chreSsid1);
    scanConfig->scan_params_.ssids_to_scan_.push_back(chreSsid2);
    scanConfig->scan_params_.frequencies_to_scan_mhz_.push_back(kFrequency1);
    scanConfig->scan_params_.frequencies_to_scan_mhz_.push_back(kFrequency2);
    scanConfig->scan_filter_.min_rssi_threshold_dbm_ = kRssiThreshold;
    wifi_offload::PreferredNetwork chreNwInfo1;
    chreNwInfo1.security_modes_ = kNetworkFlags;
    chreNwInfo1.ssid_.SetData(&kSsid1[0], kSsid1_size);
    scanConfig->scan_filter_.networks_to_match_.push_back(std::move(chreNwInfo1));
    wifi_offload::PreferredNetwork chreNwInfo2;
    chreNwInfo2.security_modes_ = kNetworkFlags;
    chreNwInfo2.ssid_.SetData(&kSsid2[0], kSsid2_size);
    scanConfig->scan_filter_.networks_to_match_.push_back(std::move(chreNwInfo2));
}

bool validateScanResult(const std::vector<ScanResult>& hidlScanResultsTest,
                        const std::vector<wifi_offload::ScanResult> chreScanResults) {
    if (hidlScanResultsTest.size() != chreScanResults.size())
        return false;
    uint32_t i = 0;
    for (const auto& hidlScanResult : hidlScanResultsTest) {
        if (hidlScanResult.tsf != chreScanResults[i].tsf_)
            return false;
        if (hidlScanResult.rssi != chreScanResults[i].rssi_dbm_)
            return false;
        if (hidlScanResult.capability != chreScanResults[i].capability_)
            return false;
        if (hidlScanResult.frequency != chreScanResults[i].frequency_scanned_mhz_)
            return false;
        for (int j = 0; j < kBssidSize; j++) {
            if (hidlScanResult.bssid[j] != chreScanResults[i].bssid_[j])
                return false;
        }
        chreWifiSsidListItem chreWifiSsid;
        chreScanResults[i].ssid_.ToChreWifiSsidListItem(&chreWifiSsid);
        for (size_t k = 0; k < chreWifiSsid.ssidLen; k++) {
            if (hidlScanResult.networkInfo.ssid[k] != chreWifiSsid.ssid[k])
                return false;
        }
        if (hidlScanResult.networkInfo.flags != kNetworkFlags) {
            return false;
        }
        i++;
    }
    return true;
}

bool validateScanStats(const ScanStats& hidlScanStats,
                       const wifi_offload::ScanStats& chreScanStats) {
    if (hidlScanStats.subscriptionDurationMs != chreScanStats.last_subscription_duration_ms_)
        return false;
    if (hidlScanStats.numScansRequestedByWifi != chreScanStats.num_scans_requested_by_nanoapp_)
        return false;
    if (hidlScanStats.numScansServicedByWifi != chreScanStats.num_scans_serviced_by_hardware_)
        return false;
    if (hidlScanStats.numScansServicedbyCache != chreScanStats.num_scans_serviced_by_cache_)
        return false;
    for (uint32_t i = 0; i < kNumChannelsInHistogram; i++) {
        if (hidlScanStats.histogramChannelsScanned[i] !=
            chreScanStats.channel_histogram_.GetChannelScanCount(i)) {
            return false;
        }
    }
    if (hidlScanStats.scanRecord.size() != chreScanStats.scan_records_.size())
        return false;
    uint32_t i = 0;
    for (const auto& scanRecord : hidlScanStats.scanRecord) {
        if (scanRecord.durationMs != chreScanStats.scan_records_[i].time_spent_scanning_ms_)
            return false;
        if (scanRecord.numChannelsScanned != chreScanStats.scan_records_[i].num_channels_scanned_)
            return false;
        if (scanRecord.numEntriesAggregated !=
            chreScanStats.scan_records_[i].num_entries_aggregated_)
            return false;
        i++;
    }
    if (hidlScanStats.logRecord.size() != chreScanStats.rpc_log_records_.size())
        return false;
    i = 0;

    for (const auto& logRecord : hidlScanStats.logRecord) {
        if (logRecord.recordName != kHidlRecordNameList[i]) {
            return false;
        }
        if (logRecord.logTimeMs != chreScanStats.rpc_log_records_[i].timestamp_chre_ms_)
            return false;
        i++;
    }
    return true;
}

}  // namespace offload_hal_test_utils
}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
