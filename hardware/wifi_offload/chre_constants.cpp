#include "chre_constants.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {
namespace chre_constants {

const char kSocketName[] = "chre";
const uint64_t kWifiOffloadNanoAppId = 5147455389092024332;
const uint32_t kMaxMessageLen = 2048;
const uint32_t kHubInfoRequestBufLen = 64;
const uint32_t kNanoAppListRequestBufLen = 64;
const uint32_t kSubscriptionDelayMsBufLen = 4;
const uint32_t kNanosecondsPerMillisecond = 1e6;
const size_t kBssidLen = 6;
const size_t kHistogramChannels = 256;

}  // namespace chre_constants
}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
