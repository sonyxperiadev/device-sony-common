#include "Offload.h"

#include <android-base/logging.h>

#include "chre_interface_factory.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

Offload::Offload() : mOffloadServer(new OffloadServer(new ChreInterfaceFactory())) {
    LOG(android::base::INFO) << "Wifi Offload HAL impl";
}

// Methods from ::android::hardware::wifi::offload::V1_0::IOffload follow.
Return<void> Offload::configureScans(const ScanParam& param, const ScanFilter& filter) {
    if (!mOffloadServer->configureScans(param, filter)) {
        LOG(ERROR) << "Failure configuring scans";
    }
    return Void();
}

Return<void> Offload::getScanStats(getScanStats_cb offloadScanStatsCallback) {
    ScanStats stats;
    bool success;
    std::tie(stats, success) = mOffloadServer->getScanStats();
    if (!success) {
        LOG(ERROR) << "Invalid results reported";
    }
    offloadScanStatsCallback(stats);
    return Void();
}

Return<void> Offload::subscribeScanResults(uint32_t delayMs) {
    if (!mOffloadServer->subscribeScanResults(delayMs)) {
        LOG(ERROR) << "Unable to subscribe scans";
    }
    return Void();
}

Return<void> Offload::unsubscribeScanResults() {
    if (!mOffloadServer->unsubscribeScanResults()) {
        LOG(ERROR) << "Unable to unsubscribe";
    }
    return Void();
}

Return<void> Offload::setEventCallback(const sp<IOffloadCallback>& cb) {
    if (!mOffloadServer->setEventCallback(cb)) {
        LOG(ERROR) << "No callback set";
    }
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
