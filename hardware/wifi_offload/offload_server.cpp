#include "offload_server.h"

#include <android-base/logging.h>

#include <chrono>

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

bool OffloadServer::configureScans(const ScanParam& param, const ScanFilter& filter) {
    LOG(INFO) << "configureScans";
    return true;
}

std::pair<ScanStats, bool> OffloadServer::getScanStats() {
    LOG(INFO) << "getScanStats";
    return std::make_pair(mScanStats, true);
}

bool OffloadServer::subscribeScanResults(uint32_t delayMs) {
    LOG(INFO) << "subscribeScanResults with delay:" << delayMs;
    return true;
}

bool OffloadServer::unsubscribeScanResults() {
    LOG(INFO) << "unsubscribeScanResults";
    return true;
}

bool OffloadServer::setEventCallback(const sp<IOffloadCallback>& cb) {
    LOG(INFO) << "Set Event callback";
    bool result = false;
    if (cb != nullptr) {
        mEventCallback = cb;
        result = true;
    } else {
        LOG(WARNING) << "Invalid callback object";
    }
    return result;
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
