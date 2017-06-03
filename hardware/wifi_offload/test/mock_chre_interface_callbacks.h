/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef WIFI_OFFLOAD_MOCK_CHRE_INTERFACE_CALLBACKS_H_
#define WIFI_OFFLOAD_MOCK_CHRE_INTERFACE_CALLBACKS_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "chre_interface_callbacks.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

class MockChreInterfaceCallbacks : public ChreInterfaceCallbacks {
  public:
    MockChreInterfaceCallbacks();
    ~MockChreInterfaceCallbacks() override = default;

    MOCK_METHOD1(handleConnectionEvents, void(ChreInterfaceCallbacks::ConnectionEvent event));
    MOCK_METHOD2(handleMessage, void(uint32_t messageType, const std::vector<uint8_t>& message));
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
#endif  // WIFI_OFFLOAD_MOCK_CHRE_INTERFACE_CALLBACKS_H_
