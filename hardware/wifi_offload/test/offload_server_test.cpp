/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <gtest/gtest.h>

#include <android-base/logging.h>

#include "mock_chre_interface.h"
#include "mock_chre_interface_factory.h"

#include "chre_interface_callbacks.h"
#include "offload_server.h"

namespace {

using android::hardware::wifi::offload::V1_0::implementation::ChreInterfaceCallbacks;
using android::hardware::wifi::offload::V1_0::implementation::MockChreInterface;
using android::hardware::wifi::offload::V1_0::implementation::ChreInterface;

ChreInterface*
    CaptureCallback(ChreInterfaceCallbacks* callback,
            std::unique_ptr<ChreInterfaceCallbacks>* chre_interface_callback,
            std::unique_ptr<testing::NiceMock<MockChreInterface>>* chre_interface) {
    chre_interface->reset(new testing::NiceMock<MockChreInterface>(callback));
    chre_interface_callback->reset(callback);
    return chre_interface->get();
}

}  // namespace

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

class OffloadServerTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
        ON_CALL(*chre_interface_factory_, getChreInterface(testing::_))
            .WillByDefault(testing::Invoke(std::bind(CaptureCallback, std::placeholders::_1,
                                                     &chre_interface_callback_, &chre_interface_)));
    }

    void TearDown() override {
    }

    std::unique_ptr<testing::NiceMock<MockChreInterfaceFactory>> chre_interface_factory_{
        new testing::NiceMock<MockChreInterfaceFactory>()};
    std::unique_ptr<testing::NiceMock<MockChreInterface>> chre_interface_;
    std::unique_ptr<ChreInterfaceCallbacks> chre_interface_callback_;
};

TEST_F(OffloadServerTest, createOffloadServerTest) {
    EXPECT_CALL(*chre_interface_factory_, getChreInterface(testing::_));
    OffloadServer* server = new OffloadServer(chre_interface_factory_.get());
    EXPECT_FALSE(chre_interface_callback_.get() == nullptr);
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
