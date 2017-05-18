#ifndef WIFI_OFFLOAD_SERVER_H_
#define WIFI_OFFLOAD_SERVER_H_

#include <android/hardware/wifi/offload/1.0/IOffload.h>

#include "chre_interface_factory.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

class OffloadServer;

class ChreInterfaceCallbacksImpl : public ChreInterfaceCallbacks {
  public:
    ChreInterfaceCallbacksImpl(OffloadServer* server);
    ~ChreInterfaceCallbacksImpl() override;

    void handleConnectionEvents(ChreInterfaceCallbacks::ConnectionEvent event);
    void handleMessage(uint32_t messageType, const std::vector<uint8_t>& message);

  private:
    OffloadServer* mServer;
};

/**
 * Interface object to communicate with Offload HAL
 */
class OffloadServer {
  public:
    OffloadServer(ChreInterfaceFactory* factory);

    OffloadStatus configureScans(const ScanParam& param, const ScanFilter& filter);
    std::pair<OffloadStatus, ScanStats> getScanStats();
    OffloadStatus subscribeScanResults(uint32_t delayMs);
    bool unsubscribeScanResults();
    bool setEventCallback(const sp<IOffloadCallback>& cb);
    void clearEventCallback();

  private:
    void invokeErrorCallback(const android::hardware::wifi::offload::V1_0::OffloadStatus& status);
    void handleScanResult(const std::vector<uint8_t>& message);
    void handleScanStats(const std::vector<uint8_t>& message);

    ScanStats mScanStats;
    std::mutex mScanStatsLock;
    std::condition_variable mScanStatsCond;
    std::mutex mOffloadLock;
    OffloadStatus mScanStatsStatus;

    std::unique_ptr<ChreInterfaceCallbacksImpl> mChreInterfaceCallbacks;
    std::unique_ptr<ChreInterface> mChreInterface;
    sp<IOffloadCallback> mEventCallback;

    friend class ChreInterfaceCallbacksImpl;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android

#endif  // WIFI_OFFLOAD_SERVER_H_
