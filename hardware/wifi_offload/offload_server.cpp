#include "offload_server.h"

#include <android-base/logging.h>
#include <chre/apps/wifi_offload/flatbuffers_serialization.h>
#include <chre/apps/wifi_offload/host_message_types.h>

#include "offload_status_util.h"
#include "offload_utils.h"

using namespace android::hardware::wifi::offload::V1_0::implementation::chre_constants;

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

class OffloadServer;

OffloadServer::OffloadServer(ChreInterfaceFactory* factory)
    : mChreInterfaceCallbacks(new ChreInterfaceCallbacksImpl(this)),
      mChreInterface(factory->getChreInterface(mChreInterfaceCallbacks.get())) {
    LOG(VERBOSE) << "Wifi Offload HAL impl";
}

OffloadStatus OffloadServer::configureScans(const ScanParam& param, const ScanFilter& filter) {
    LOG(INFO) << "configureScans";
    if (!mChreInterface->isConnected()) {
        return createOffloadStatus(OffloadStatusCode::ERROR,
                                   "Not connected to hardware implementation");
    }
    wifi_offload::ScanConfig scanConfig;
    if (!offload_utils::ToChreScanConfig(param, filter, &scanConfig)) {
        return createOffloadStatus(OffloadStatusCode::ERROR,
                                   "Unable to convert scan configuration");
    }
    uint8_t buffer[kMaxMessageLen];
    size_t result_size = wifi_offload::fbs::Serialize(scanConfig, buffer, kMaxMessageLen);
    if (result_size <= 0) {
        return createOffloadStatus(OffloadStatusCode::ERROR, "Scan config serialization failed");
    }
    std::vector<uint8_t> message(buffer, buffer + result_size);
    if (!mChreInterface->sendCommandToApp(wifi_offload::HostMessageType::HOST_CMD_CONFIG_SCANS,
                                          message)) {
        return createOffloadStatus(OffloadStatusCode::ERROR, "Unable to send config message");
    }
    return createOffloadStatus(OffloadStatusCode::OK);
}

std::pair<OffloadStatus, ScanStats> OffloadServer::getScanStats() {
    LOG(INFO) << "getScanStats";
    OffloadStatus status = createOffloadStatus(OffloadStatusCode::OK);
    return std::make_pair(status, mScanStats);
}

OffloadStatus OffloadServer::subscribeScanResults(uint32_t delayMs) {
    LOG(INFO) << "subscribeScanResults with delay:" << delayMs;
    if (!mChreInterface->isConnected()) {
        return createOffloadStatus(OffloadStatusCode::ERROR, "Not connected to hardware");
    }
    uint32_t* buffer = &delayMs;
    std::vector<uint8_t> message(reinterpret_cast<uint8_t*>(buffer),
                                 reinterpret_cast<uint8_t*>(buffer) + kSubscriptionDelayMsBufLen);
    if (!mChreInterface->sendCommandToApp(
            wifi_offload::HostMessageType::HOST_CMD_SUBSCRIBE_SCAN_RESULTS, message)) {
        return createOffloadStatus(OffloadStatusCode::ERROR, "Unable to request scans");
    }
    return createOffloadStatus(OffloadStatusCode::OK);
}

bool OffloadServer::unsubscribeScanResults() {
    bool result = false;
    LOG(INFO) << "unsubscribeScanResults";
    if (!mChreInterface->isConnected()) {
        LOG(WARNING) << "Failed to send unsubscribe scan results message";
        return false;
    }
    if (!mChreInterface->sendCommandToApp(
            wifi_offload::HostMessageType::HOST_CMD_UNSUBSCRIBE_SCAN_RESULTS, {})) {
        LOG(WARNING) << "Failed to send unsubscribe scan results message";
        return false;
    }
    return true;
}

bool OffloadServer::setEventCallback(const sp<IOffloadCallback>& cb) {
    LOG(INFO) << "Set Event callback";
    bool result = false;
    if (cb != nullptr) {
        mEventCallback = cb;
        result = true;
    }
    return result;
}

void OffloadServer::clearEventCallback() {
    if (mEventCallback != nullptr) {
        mEventCallback.clear();
    }
    LOG(INFO) << "Event callback cleared";
}

ChreInterfaceCallbacksImpl::ChreInterfaceCallbacksImpl(OffloadServer* server) : mServer(server) {
}

ChreInterfaceCallbacksImpl::~ChreInterfaceCallbacksImpl() {
}

void ChreInterfaceCallbacksImpl::handleConnectionEvents(
    ChreInterfaceCallbacks::ConnectionEvent event) {
    LOG(VERBOSE) << "Connection event received " << (int)event;
}

void ChreInterfaceCallbacksImpl::handleMessage(uint32_t messageType,
                                               const std::vector<uint8_t>& message) {
    LOG(VERBOSE) << "Message from Nano app " << messageType;
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
