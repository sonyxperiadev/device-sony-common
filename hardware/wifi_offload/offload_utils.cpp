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

#include "offload_utils.h"

#include <android-base/logging.h>
#include <chre/apps/wifi_offload/error_codes.h>

namespace {

bool ToHidlRecordName(const wifi_offload::RpcLogRecord::RpcLogRecordType& chreRecordType,
                      android::hardware::wifi::offload::V1_0::RecordName* hidlRecordName) {
    bool result = true;
    switch (chreRecordType) {
        case wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_INIT:
            *hidlRecordName = android::hardware::wifi::offload::V1_0::RecordName::CMD_INT;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_CONFIG_SCANS:
            *hidlRecordName = android::hardware::wifi::offload::V1_0::RecordName::CMD_CONFIG_SCANS;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_SUBSCRIBE_SCAN_RESULTS:
            *hidlRecordName =
                android::hardware::wifi::offload::V1_0::RecordName::CMD_SUBSCRIBE_SCAN_RESULTS;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_UNSUBSCRIBE_SCAN_RESULTS:
            *hidlRecordName =
                android::hardware::wifi::offload::V1_0::RecordName::CMD_UNSUBSCRIBE_SCAN_RESULTS;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_GET_SCAN_STATS:
            *hidlRecordName =
                android::hardware::wifi::offload::V1_0::RecordName::CMD_GET_SCAN_STATS;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::CMD_RESET:
            *hidlRecordName = android::hardware::wifi::offload::V1_0::RecordName::CMD_RESET;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::EVENT_RECVD_SCAN_RESULT_ASYNC:
            *hidlRecordName =
                android::hardware::wifi::offload::V1_0::RecordName::EVENT_RECVD_SCAN_RESULT_ASYNC;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::EVENT_RECVD_SCAN_RESULT:
            *hidlRecordName =
                android::hardware::wifi::offload::V1_0::RecordName::EVENT_RECVD_SCAN_RESULT;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::EVENT_SENT_SCAN_RESULT:
            *hidlRecordName =
                android::hardware::wifi::offload::V1_0::RecordName::EVENT_SENT_SCAN_RESULT;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::EVENT_SENT_ABORT:
            *hidlRecordName = android::hardware::wifi::offload::V1_0::RecordName::EVENT_SENT_ABORT;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::EVENT_SENT_ERROR:
            *hidlRecordName = android::hardware::wifi::offload::V1_0::RecordName::EVENT_SENT_ERROR;
            break;
        case wifi_offload::RpcLogRecord::RpcLogRecordType::REQ_SCAN:
            *hidlRecordName = android::hardware::wifi::offload::V1_0::RecordName::REQ_SCAN;
            break;
        default:
            result = false;
            break;
    }
    return result;
}

uint8_t ToChreSecurityMode(uint8_t hidlSecurityMode) {
    uint8_t chreSecurityMode = 0;
    if (hidlSecurityMode & android::hardware::wifi::offload::V1_0::SecurityMode::OPEN) {
        chreSecurityMode |= wifi_offload::SecurityMode::OPEN;
    }
    if (hidlSecurityMode & android::hardware::wifi::offload::V1_0::SecurityMode::WEP) {
        chreSecurityMode |= wifi_offload::SecurityMode::WEP;
    }
    if (hidlSecurityMode & android::hardware::wifi::offload::V1_0::SecurityMode::PSK) {
        chreSecurityMode |= wifi_offload::SecurityMode::PSK;
    }
    if (hidlSecurityMode & android::hardware::wifi::offload::V1_0::SecurityMode::EAP) {
        chreSecurityMode |= wifi_offload::SecurityMode::EAP;
    }
    return chreSecurityMode;
}

uint8_t ToHidlSecurityMode(uint8_t chreSecurityMode) {
    uint8_t hidlSecurityMode = 0;
    if (chreSecurityMode & wifi_offload::SecurityMode::OPEN) {
        hidlSecurityMode |= android::hardware::wifi::offload::V1_0::SecurityMode::OPEN;
    }
    if (chreSecurityMode & wifi_offload::SecurityMode::WEP) {
        hidlSecurityMode |= android::hardware::wifi::offload::V1_0::SecurityMode::WEP;
    }
    if (chreSecurityMode & wifi_offload::SecurityMode::PSK) {
        hidlSecurityMode |= android::hardware::wifi::offload::V1_0::SecurityMode::PSK;
    }
    if (chreSecurityMode & wifi_offload::SecurityMode::EAP) {
        hidlSecurityMode |= android::hardware::wifi::offload::V1_0::SecurityMode::EAP;
    }
    return hidlSecurityMode;
}

}  // namespace

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {
namespace offload_utils {

bool ToHidlScanResult(const wifi_offload::ScanResult& chreScanResult, ScanResult* hidlScanResult) {
    if (hidlScanResult == nullptr) {
        return false;
    }
    hidlScanResult->tsf = chreScanResult.tsf_;
    hidlScanResult->capability = chreScanResult.capability_;
    hidlScanResult->rssi = chreScanResult.rssi_dbm_;
    hidlScanResult->frequency = chreScanResult.frequency_scanned_mhz_;
    memcpy(&hidlScanResult->bssid[0], &chreScanResult.bssid_[0],
           wifi_offload::ScanResult::kBssidSize);
    chreWifiSsidListItem chreWifiSsid;
    chreScanResult.ssid_.ToChreWifiSsidListItem(&chreWifiSsid);
    std::vector<uint8_t> ssid;
    for (size_t i = 0; i < chreWifiSsid.ssidLen; i++) {
        ssid.push_back(chreWifiSsid.ssid[i]);
    }
    hidlScanResult->networkInfo.ssid = ssid;
    hidlScanResult->networkInfo.flags = ToHidlSecurityMode(chreScanResult.security_modes_);
    return true;
}

bool ToHidlScanResults(const std::vector<wifi_offload::ScanResult>& chreScanResults,
                       std::vector<ScanResult>* hidlScanResults) {
    LOG(VERBOSE) << "ScanResults from CHRE, size=" << chreScanResults.size();
    for (const auto& scan_result_from_nano_app : chreScanResults) {
        ScanResult hidl_scan_result;
        if (!ToHidlScanResult(scan_result_from_nano_app, &hidl_scan_result)) {
            return false;
        }
        hidlScanResults->push_back(hidl_scan_result);
    }
    return true;
}

bool ToHidlScanStats(const wifi_offload::ScanStats& chreScanStats, ScanStats* hidlScanStats) {
    hidlScanStats->subscriptionDurationMs = chreScanStats.last_subscription_duration_ms_;
    hidlScanStats->numScansRequestedByWifi = chreScanStats.num_scans_requested_by_nanoapp_;
    hidlScanStats->numScansServicedByWifi = chreScanStats.num_scans_serviced_by_hardware_;
    hidlScanStats->numScansServicedbyCache = chreScanStats.num_scans_serviced_by_cache_;
    std::vector<ScanRecord> hidlScanRecords;
    for (const auto& chreScanRecord : chreScanStats.scan_records_) {
        ScanRecord hidlScanRecord;
        hidlScanRecord.durationMs = chreScanRecord.time_spent_scanning_ms_;
        hidlScanRecord.numChannelsScanned = chreScanRecord.num_channels_scanned_;
        hidlScanRecord.numEntriesAggregated = chreScanRecord.num_entries_aggregated_;
        hidlScanRecords.push_back(hidlScanRecord);
    }
    hidlScanStats->scanRecord = hidlScanRecords;
    std::vector<LogRecord> logRecords;
    for (const auto& chreLogRecord : chreScanStats.rpc_log_records_) {
        LogRecord logRecord;
        if (!ToHidlRecordName(chreLogRecord.record_type_, &logRecord.recordName)) {
            return false;
        }
        logRecord.logTimeMs = chreLogRecord.timestamp_chre_ms_;
        logRecords.push_back(logRecord);
    }
    hidlScanStats->logRecord = logRecords;
    for (size_t i = 0; i < hidlScanStats->histogramChannelsScanned.elementCount(); i++) {
        hidlScanStats->histogramChannelsScanned[i] =
            chreScanStats.channel_histogram_.GetChannelScanCount(i);
    }
    return true;
}

bool ToChreScanConfig(const ScanParam& param, const ScanFilter& filter,
                      wifi_offload::ScanConfig* scanConfig) {
    scanConfig->scan_params_.disconnected_mode_scan_interval_ms_ =
        param.disconnectedModeScanIntervalMs;
    for (const auto& ssid : param.ssidList) {
        wifi_offload::Ssid chreSsid;
        chreSsid.SetData(ssid.data(), ssid.size());
        scanConfig->scan_params_.ssids_to_scan_.push_back(chreSsid);
    }
    for (const auto& freq : param.frequencyList) {
        scanConfig->scan_params_.frequencies_to_scan_mhz_.push_back(freq);
    }
    scanConfig->scan_filter_.min_rssi_threshold_dbm_ = filter.rssiThreshold;
    for (const auto& nwInfo : filter.preferredNetworkInfoList) {
        wifi_offload::PreferredNetwork chreNwInfo;
        chreNwInfo.security_modes_ = ToChreSecurityMode(nwInfo.flags);
        chreNwInfo.ssid_.SetData(nwInfo.ssid.data(), nwInfo.ssid.size());
        scanConfig->scan_filter_.networks_to_match_.push_back(std::move(chreNwInfo));
    }
    return true;
}

bool ToHidlErrorMessage(uint32_t errorCode, std::string* errorMessage) {
    bool reportError = true;
    switch (errorCode) {
        case wifi_offload::ErrorCode::FAILED_TO_ALLOCATE_MESSAGE_BUFFER:
            *errorMessage = "Failed to allocate message buffer";
            break;
        case wifi_offload::ErrorCode::FAILED_TO_SERIALIZE_MESSAGE:
            *errorMessage = "Failed to serialize message";
            break;
        case wifi_offload::ErrorCode::FAILED_TO_SEND_MESSAGE:
            *errorMessage = "Failed to send message";
            break;
        case wifi_offload::ErrorCode::FAILED_TO_DESERIALIZE_SCAN_CONFIG:
            *errorMessage = "Failed to deserialize scan config";
            break;
        case wifi_offload::ErrorCode::INVALID_SUBSCRIBE_MESSAGE_SIZE:
            *errorMessage = "Invalid subscribe message size";
            break;
        case wifi_offload::ErrorCode::SCAN_CONFIG_NOT_INITIALIZED:
            *errorMessage = "Scan config not initialized";
            break;
        case wifi_offload::ErrorCode::UNSPECIFIED_HOST_ENDPOINT:
            *errorMessage = "Unspecified host end point";
            break;
        case wifi_offload::ErrorCode::FAILED_TO_SEND_SCAN_RESULTS:
            *errorMessage = "Failed to send scan results";
            break;
        case wifi_offload::ErrorCode::FAILED_TO_SEND_SCAN_STATS:
            *errorMessage = "Failed to send scan stats";
            break;
        case wifi_offload::ErrorCode::ONDEMAND_SCAN_NOT_SUPPORTED:
            *errorMessage = "On demand scans not supported";
            break;
        case wifi_offload::ErrorCode::FAILED_TO_SEND_ONDEMAND_SCAN_REQUEST:
            *errorMessage = "Failed to send on demand scan request";
            break;
        case wifi_offload::ErrorCode::FAILED_TO_SEND_ONDEMAND_SCAN_REQUEST_ASYNC:
            *errorMessage = "Failed to send on demand scan request async";
            break;
        case wifi_offload::ErrorCode::OUT_OF_ORDER_SCAN_RESULTS:
            *errorMessage = "Out of order scan results";
            break;
        case wifi_offload::ErrorCode::INCOMPLETE_SCAN_RESULTS_BEFORE_SCAN_REQUEST:
            *errorMessage = "Incomplete scan results before scan request";
            break;
        case wifi_offload::ErrorCode::FAILED_TO_SET_SCAN_TIMER:
            *errorMessage = "Failed to set scan timer";
            break;
        case wifi_offload::ErrorCode::SCAN_MONITORING_NOT_SUPPORTED:
            *errorMessage = "Scan Monitoring not supported";
            break;
        case wifi_offload::ErrorCode::FAILED_TO_START_SCAN_MONITORING:
            *errorMessage = "Failed to start scan monitoring";
            reportError = false;
            break;
        case wifi_offload::ErrorCode::FAILED_TO_STOP_SCAN_MONITORING:
            *errorMessage = "Failed to stop scan monitoring";
            reportError = false;
            break;
        case wifi_offload::ErrorCode::FAILED_TO_CONFIGURE_SCAN_MONITORING_ASYNC:
            *errorMessage = "Failed to configure scan monitoring async";
            reportError = false;
            break;
        default:
            *errorMessage = "Invalid error code";
            reportError = false;
            break;
    }
    return reportError;
}

}  // namespace offload_utils
}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
