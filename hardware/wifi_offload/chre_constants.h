#ifndef WIFI_OFFLOAD_CHRE_CONSTANTS_H_
#define WIFI_OFFLOAD_CHRE_CONSTANTS_H_

#include <cstdint>

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {
namespace chre_constants {

extern const char kSocketName[];
extern const uint64_t kWifiOffloadNanoAppId;
extern const uint32_t kMaxMessageLen;
extern const uint32_t kHubInfoRequestBufLen;
extern const uint32_t kNanoAppListRequestBufLen;
extern const uint32_t kSubscriptionDelayMsBufLen;
extern const uint32_t kNanosecondsPerMillisecond;
extern const size_t kHistogramChannels;
extern const size_t kBssidLen;

}  // namespace chre_constants
}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android

#endif  // WIFI_OFFLOAD_CHRE_CONSTANTS_H_
