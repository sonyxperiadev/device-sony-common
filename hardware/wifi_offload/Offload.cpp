#include "Offload.h"

#include <android-base/logging.h>

#include "chre_interface_factory.h"
#include "hidl_return_util.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

using hidl_return_util::validateAndCall;

Offload::Offload()
    : mOffloadServer(new OffloadServer(new ChreInterfaceFactory())), cookie_(0),
      death_handler_(new HidlDeathHandler<IOffloadCallback>(
          std::bind(&Offload::onObjectDeath, this, std::placeholders::_1))) {
    LOG(android::base::INFO) << "Wifi Offload HAL impl";
}

// Methods from ::android::hardware::wifi::offload::V1_0::IOffload follow.
Return<void> Offload::configureScans(const ScanParam &param, const ScanFilter &filter,
                                     configureScans_cb _hidl_cb) {
    return validateAndCall(this, &Offload::configureScansInternal, _hidl_cb, param, filter);
}

Return<void> Offload::getScanStats(getScanStats_cb _hidl_cb) {
    return validateAndCall(this, &Offload::getScanStatsInternal, _hidl_cb);
}

Return<void> Offload::subscribeScanResults(uint32_t delayMs, subscribeScanResults_cb _hidl_cb) {
    return validateAndCall(this, &Offload::subscribeScanResultsInternal, _hidl_cb, delayMs);
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
        return Void();
    }
    cookie_ = reinterpret_cast<uint64_t>(cb.get());
    death_handler_->setCallback(cb);
    cb->linkToDeath(death_handler_, cookie_);
    return Void();
}

OffloadStatus Offload::configureScansInternal(const ScanParam &param, const ScanFilter &filter) {
    return mOffloadServer->configureScans(param, filter);
}

std::pair<OffloadStatus, ScanStats> Offload::getScanStatsInternal() {
    return mOffloadServer->getScanStats();
}

OffloadStatus Offload::subscribeScanResultsInternal(uint32_t delayMs) {
    return mOffloadServer->subscribeScanResults(delayMs);
}

void Offload::onObjectDeath(uint64_t cookie) {
    if (cookie == cookie_) {
        LOG(DEBUG) << "OffloadCallback death notification received";
        mOffloadServer->clearEventCallback();
        cookie_ = 0;
    }
}
// Methods from ::android::hidl::base::V1_0::IBase follow.

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
