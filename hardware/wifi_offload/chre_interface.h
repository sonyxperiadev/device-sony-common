/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef WIFI_OFFLOAD_CHRE_INTERFACE_H_
#define WIFI_OFFLOAD_CHRE_INTERFACE_H_

#include "chre_constants.h"
#include "chre_host/host_protocol_host.h"
#include "chre_host/socket_client.h"
#include "chre_interface_callbacks.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

class ChreInterface;

class SocketCallbacks
    : public ::android::chre::SocketClient::ICallbacks,
      public ::android::chre::IChreMessageHandlers {
  public:
    SocketCallbacks(ChreInterface* parent);
    void onMessageReceived(const void* data, size_t length) override;
    void onConnected() override;
    void onConnectionAborted() override;
    void onDisconnected() override;
    void handleNanoappMessage(uint64_t appId, uint32_t messageType, uint16_t hostEndpoint,
                              const void* messageData, size_t messageDataLen) override;
    void handleHubInfoResponse(const char* name, const char* vendor, const char* toolchain,
                               uint32_t legacyPlatformVersion, uint32_t legacyToolchainVersion,
                               float peakMips, float stoppedPower, float sleepPower,
                               float peakPower, uint32_t maxMessageLen, uint64_t platformId,
                               uint32_t version) override;
    void handleNanoappListResponse(const ::chre::fbs::NanoappListResponseT& response) override;
    void handleLoadNanoappResponse(const ::chre::fbs::LoadNanoappResponseT& response) override;
    void handleUnloadNanoappResponse(const ::chre::fbs::UnloadNanoappResponseT& response) override;

  private:
    ChreInterface* mParent;
};

class ChreInterface {
  public:
    ChreInterface(ChreInterfaceCallbacks* callback);
    ~ChreInterface();
    /* Return the status of socket connection */
    bool isConnected();
    /* Send a message to the Nano app, returns true if send successful */
    bool sendCommandToApp(uint32_t messageType, const std::vector<uint8_t>& message);
    /* Connected or connection restart handling method */
    void reportConnectionEvent(ChreInterfaceCallbacks::ConnectionEvent /* event */);
    /* Requests Hub Information, returns true if Hub Info request was sent */
    bool getHubInfo();
    /* Request list of Nano apps, returns true if Nano app List request was sent */
    bool getNanoAppList();
    /* Invoked by the socket callbacks when a message is recieved from Nano app */
    void handleMessage(uint32_t messageType, const void* messageData, size_t messageDataLen);

  private:
    ::android::chre::SocketClient mClient;
    sp<SocketCallbacks> mSocketCallbacks;
    ChreInterfaceCallbacks* mServerCallbacks;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
#endif  // WIFI_OFFLOAD_CHRE_INTERFACE_H_
