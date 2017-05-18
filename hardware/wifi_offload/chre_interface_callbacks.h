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
#ifndef WIFI_OFFLOAD_CHRE_INTERFACE_CALLBACKS_H
#define WIFI_OFFLOAD_CHRE_INTERFACE_CALLBACKS_H

#include <cstdint>

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

class ChreInterfaceCallbacks {
  public:
    enum ConnectionEvent { CONNECTED, DISCONNECTED, CONNECTION_ABORT };
    virtual ~ChreInterfaceCallbacks() {}

    /* Invoked by the Chre interface when connection events are received */
    virtual void handleConnectionEvents(ConnectionEvent event) = 0;
    /* Invoked by the Chre interface when a message is recieved from Nano app */
    virtual void handleMessage(uint32_t messageType, const std::vector<uint8_t>& message) = 0;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
#endif  // WIFI_OFFLOAD_CHRE_INTERFACE_CALLBACKS_H
