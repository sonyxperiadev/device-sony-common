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

#include <hidl/HidlTransportSupport.h>
#include <hwbinder/IPCThreadState.h>

#include <android-base/logging.h>
#include <utils/StrongPointer.h>

#include "Offload.h"

// libhwbinder
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using android::hardware::wifi::offload::V1_0::IOffload;
using android::hardware::wifi::offload::V1_0::implementation::Offload;

int main(int /*argc*/, char **argv) {
    android::base::InitLogging(
        argv, android::base::LogdLogger(android::base::SYSTEM));
    LOG(android::base::INFO) << "Wifi Offload HAL service start ";

    // Setup hwbinder
    configureRpcThreadpool(1, true /* callerWillJoin */);

    // Register service
    android::sp<IOffload> service = new Offload();
    CHECK_EQ(service->registerAsService(), android::NO_ERROR)
        << "Failed to register Wifi Offload HAL";

    joinRpcThreadpool();

    LOG(android::base::INFO) << "Wifi Offload HAL service exit";
    return 0;
}
