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

#include "mock_chre_interface_callbacks.h"

#include "chre_interface.h"

namespace {

const size_t kBufSize = 256;
const uint8_t kDefaultValue = 0xaa;
const uint32_t kDefaultMessageType = 0;

}  // namespace

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

class ChreInterfaceTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
        chre_interface_.reset(new ChreInterface(chre_interface_callback_.get()));
    }

    void TearDown() override {
    }

    std::unique_ptr<testing::NiceMock<MockChreInterfaceCallbacks>> chre_interface_callback_{
        new testing::NiceMock<MockChreInterfaceCallbacks>()};
    std::unique_ptr<ChreInterface> chre_interface_;
};

TEST_F(ChreInterfaceTest, ChreInterfaceConnectionEventTest) {
    EXPECT_CALL(*chre_interface_callback_, handleConnectionEvents(testing::_));
    chre_interface_->reportConnectionEvent(ChreInterfaceCallbacks::CONNECTED);
    EXPECT_TRUE(chre_interface_->isConnected());
}

TEST_F(ChreInterfaceTest, ChreInterfaceHandleMessageTest) {
    EXPECT_CALL(*chre_interface_callback_, handleMessage(testing::_, testing::_));
    uint32_t messageType;
    std::vector<uint8_t> buffer_recvd;
    ON_CALL(*chre_interface_callback_, handleMessage(testing::_, testing::_))
        .WillByDefault(
            DoAll(testing::SaveArg<0>(&messageType), testing::SaveArg<1>(&buffer_recvd)));
    uint8_t buffer_sent[kBufSize];
    for (size_t j = 0; j < kBufSize; j++) {
        buffer_sent[j] = kDefaultValue;
    }
    chre_interface_->handleMessage(kDefaultMessageType, (void*)&buffer_sent[0], kBufSize);
    EXPECT_EQ(messageType, kDefaultMessageType);
    EXPECT_EQ(buffer_recvd.size(), kBufSize);
    for (size_t i = 0; i < buffer_recvd.size(); i++) {
        EXPECT_EQ(buffer_recvd[i], buffer_sent[i]);
    }
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
