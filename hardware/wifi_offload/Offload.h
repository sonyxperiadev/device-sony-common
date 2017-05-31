#ifndef ANDROID_HARDWARE_WIFI_OFFLOAD_V1_0_OFFLOAD_H
#define ANDROID_HARDWARE_WIFI_OFFLOAD_V1_0_OFFLOAD_H

#include <android/hardware/wifi/offload/1.0/IOffload.h>

#include <android-base/macros.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

#include "offload_server.h"

namespace {
// Type of callback invoked by the death handler.
using on_death_cb_function = std::function<void(uint64_t)>;

// Private class used to keep track of death of callbacks
template<typename CallbackType>
class HidlDeathHandler : public android::hardware::hidl_death_recipient {
  public:
    HidlDeathHandler(const on_death_cb_function &user_cb_function)
        : cb_function_(user_cb_function) {
    }
    ~HidlDeathHandler() = default;

    // Death notification for callbacks.
    void serviceDied(uint64_t cookie,
                     const android::wp<android::hidl::base::V1_0::IBase> &who) override {
        cb_.clear();
        cb_function_(cookie);
    }

    void setCallback(android::wp<CallbackType> cb) {
        cb_ = cb;
    }

  private:
    android::wp<CallbackType> cb_;
    on_death_cb_function cb_function_;

    DISALLOW_COPY_AND_ASSIGN(HidlDeathHandler);
};
}  // namespace

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

/**
 * Interface object to communicate with Offload HAL
 */
class Offload : public IOffload {
   public:
    Offload();

    // Methods from ::android::hardware::wifi::offload::V1_0::IOffload follow.
    Return<void> configureScans(const ScanParam &param, const ScanFilter &filter,
                                configureScans_cb _hidl_cb) override;
    Return<void> getScanStats(getScanStats_cb _hidl_cb) override;
    Return<void> subscribeScanResults(uint32_t delayMs, subscribeScanResults_cb _hidl_cb) override;
    Return<void> unsubscribeScanResults() override;
    Return<void> setEventCallback(const sp<IOffloadCallback>& cb) override;
    // Methods from ::android::hidl::base::V1_0::IBase follow.

   private:
     OffloadStatus configureScansInternal(const ScanParam &param, const ScanFilter &filter);
     std::pair<OffloadStatus, ScanStats> getScanStatsInternal();
     OffloadStatus subscribeScanResultsInternal(uint32_t delayMs);
     void onObjectDeath(uint64_t cookie);

     std::unique_ptr<OffloadServer> mOffloadServer;
     uint64_t cookie_;
     sp<HidlDeathHandler<IOffloadCallback>> death_handler_;

     DISALLOW_COPY_AND_ASSIGN(Offload);
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_WIFI_OFFLOAD_V1_0_OFFLOAD_H
