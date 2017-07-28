#include "offload_server.h"

#include <android-base/logging.h>
#include <chre/apps/wifi_offload/flatbuffers_serialization.h>
#include <chre/apps/wifi_offload/host_message_types.h>

#include "offload_status_util.h"
#include "offload_utils.h"

using namespace android::hardware::wifi::offload::V1_0::implementation::chre_constants;
using android::hardware::wifi::offload::V1_0::OffloadStatus;

namespace {
constexpr auto kScanStatsTimeout = std::chrono::milliseconds(500);
}

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
        return createOffloadStatus(OffloadStatusCode::NO_CONNECTION,
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
    mScanStatsStatus = createOffloadStatus(OffloadStatusCode::OK);
    if (!mChreInterface->isConnected()) {
        return {createOffloadStatus(OffloadStatusCode::NO_CONNECTION, "Unable to send scan stats"),
                {}};
    }
    if (!mChreInterface->sendCommandToApp(wifi_offload::HostMessageType::HOST_CMD_GET_SCAN_STATS,
                                          {})) {
        return {createOffloadStatus(OffloadStatusCode::ERROR, "Unable to send scan stats command"),
                {}};
    }
    LOG(VERBOSE) << "Sent getScanStats command";
    {
        std::unique_lock<std::mutex> lock(mScanStatsLock);
        auto timeout_status = mScanStatsCond.wait_for(lock, kScanStatsTimeout);
        if (timeout_status == std::cv_status::timeout) {
            std::lock_guard<std::mutex> lock(mOffloadLock);
            LOG(WARNING) << "Timeout waiting for scan stats";
            return {createOffloadStatus(OffloadStatusCode::TIMEOUT, "Scan stats not received"), {}};
        }
    }
    return std::make_pair(mScanStatsStatus, mScanStats);
}

OffloadStatus OffloadServer::subscribeScanResults(uint32_t delayMs) {
    LOG(INFO) << "subscribeScanResults with delay:" << delayMs;
    if (!mChreInterface->isConnected()) {
        return createOffloadStatus(OffloadStatusCode::NO_CONNECTION, "Not connected to hardware");
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

void OffloadServer::resetNanoApp() {
    LOG(INFO) << "resetting Nano app";
    if (!mChreInterface->isConnected()) {
        LOG(WARNING) << "Unable to reset nano app, not connected";
        return;
    }
    if (!mChreInterface->sendCommandToApp(wifi_offload::HostMessageType::HOST_CMD_RESET, {})) {
        LOG(ERROR) << "Unable to send Reset command to Nano app";
    }
}

bool OffloadServer::unsubscribeScanResults() {
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
    if (cb == nullptr) {
        return false;
    }
    std::lock_guard<std::mutex> lock(mOffloadLock);
    mEventCallback = cb;
    return true;
}

void OffloadServer::clearEventCallback() {
    std::lock_guard<std::mutex> lock(mOffloadLock);
    if (mEventCallback != nullptr) {
        mEventCallback.clear();
    }
    LOG(INFO) << "Event callback cleared";
}

void OffloadServer::invokeErrorCallbackAndResetIfNeeded(const OffloadStatus& status) {
    if (status.code != OffloadStatusCode::OK) {
        resetNanoApp();
    }
    std::lock_guard<std::mutex> lock(mOffloadLock);
    if (mEventCallback != nullptr) {
        mEventCallback->onError(status);
    }
}

ChreInterfaceCallbacksImpl::ChreInterfaceCallbacksImpl(OffloadServer* server) : mServer(server) {
}

ChreInterfaceCallbacksImpl::~ChreInterfaceCallbacksImpl() {
}

void ChreInterfaceCallbacksImpl::handleConnectionEvents(
    ChreInterfaceCallbacks::ConnectionEvent event) {
    switch (event) {
        case ChreInterfaceCallbacks::ConnectionEvent::DISCONNECTED:
        case ChreInterfaceCallbacks::ConnectionEvent::CONNECTION_ABORT: {
            LOG(ERROR) << "Connection to socket lost";
            mServer->invokeErrorCallbackAndResetIfNeeded(
                createOffloadStatus(OffloadStatusCode::NO_CONNECTION, "Connection to socket lost"));
        } break;
        case ChreInterfaceCallbacks::ConnectionEvent::CONNECTED: {
            LOG(INFO) << "Connected to socket";
            mServer->invokeErrorCallbackAndResetIfNeeded(
                createOffloadStatus(OffloadStatusCode::OK));
        } break;
        default:
            LOG(WARNING) << "Invalid connection event received " << (int)event;
            break;
    }
}

void OffloadServer::handleScanResult(const std::vector<uint8_t>& message) {
    std::vector<wifi_offload::ScanResult> scanResults;
    std::vector<ScanResult> hidlScanResults;
    std::string errorMessage;
    if (!wifi_offload::fbs::Deserialize((uint8_t*)message.data(), message.size(), &scanResults)) {
        invokeErrorCallbackAndResetIfNeeded(
            createOffloadStatus(OffloadStatusCode::ERROR, "Cannot deserialize scan results"));
        return;
    }
    if (!offload_utils::ToHidlScanResults(scanResults, &hidlScanResults)) {
        invokeErrorCallbackAndResetIfNeeded(createOffloadStatus(
            OffloadStatusCode::ERROR, "Cannot convert scan results to HIDL format"));
        return;
    }
    {
        std::lock_guard<std::mutex> lock(mOffloadLock);
        if (mEventCallback != nullptr) {
            mEventCallback->onScanResult(hidlScanResults);
        }
    }
}

void OffloadServer::handleScanStats(const std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(mScanStatsLock);
    wifi_offload::ScanStats stats;
    OffloadStatus status;
    // Deserialize scan stats
    status = createOffloadStatus(OffloadStatusCode::OK);
    LOG(VERBOSE) << "Received scan stats";
    if (!wifi_offload::fbs::Deserialize((uint8_t*)message.data(), message.size(), &stats)) {
        status = createOffloadStatus(OffloadStatusCode::ERROR, "Cannot deserailize scan stats");
    } else if (!offload_utils::ToHidlScanStats(stats, &mScanStats)) {
        status = createOffloadStatus(OffloadStatusCode::ERROR,
                                     "Cannot convert Scan stats to HIDL format");
    }
    mScanStatsStatus = status;
    mScanStatsCond.notify_all();
}

void ChreInterfaceCallbacksImpl::handleMessage(uint32_t messageType,
                                               const std::vector<uint8_t>& message) {
    LOG(VERBOSE) << "Message from Nano app " << messageType;
    switch (messageType) {
        case wifi_offload::HostMessageType::HOST_MSG_SCAN_RESULTS: {
            LOG(INFO) << "Received scan results";
            mServer->handleScanResult(message);
        } break;
        case wifi_offload::HostMessageType::HOST_MSG_SCAN_STATS:
            LOG(VERBOSE) << "Received scan stats from Nano app";
            mServer->handleScanStats(message);
            break;
        case wifi_offload::HostMessageType::HOST_MSG_ERROR:
            LOG(VERBOSE) << "Received error message from Nano app";
            {
                std::string errorMessage;
                if (offload_utils::ToHidlErrorMessage(message[0], &errorMessage)) {
                    mServer->invokeErrorCallbackAndResetIfNeeded(
                        createOffloadStatus(OffloadStatusCode::ERROR, errorMessage));
                }
            }
            break;
        default:
            LOG(WARNING) << "Unknown message received" << messageType;
            break;
    }
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
