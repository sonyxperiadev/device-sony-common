#ifndef WIFI_OFFLOAD_SERVER_UTILS_H_
#define WIFI_OFFLOAD_SERVER_UTILS_H_

#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include <chre/apps/wifi_offload/flatbuffers_serialization.h>

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {
namespace offload_utils {

bool ToHidlScanResults(const std::vector<wifi_offload::ScanResult>& chreScanResults,
                       std::vector<ScanResult>* hidlScanResults);
bool ToHidlScanStats(const wifi_offload::ScanStats& chreScanStats, ScanStats* hildScanStats);
bool ToChreScanConfig(const ScanParam& param, const ScanFilter& filter,
                      wifi_offload::ScanConfig* scanConfig);
bool ToHidlErrorMessage(uint32_t errorCode, std::string* errorMessage);

}  //  namespace offload_utils
}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android

#endif  // WIFI_OFFLOAD_SERVER_UTILS_H_
