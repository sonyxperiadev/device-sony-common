/*
 * Copyright (C) 2016-2019 AngeloGioacchino Del Regno <kholk11@gmail.com>
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

#define LOG_TAG "RQBalance-PowerHAL"

#include <android/log.h>
#include <hidl/HidlTransportSupport.h>

#include "common.h"
#include "RQBalanceHALExt.h"
#include "Hints.h"
#include "Power.h"

#define UNUSED __attribute__((unused))

using android::sp;
using android::status_t;
using android::OK;

// libhwbinder:
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// Generated HIDL files
using android::hardware::power::V1_3::IPower;
using android::hardware::power::V1_3::implementation::Power;

int main(int argc UNUSED, char** argv UNUSED)
{
    ALOGI("Initializing RQBalance-PowerHAL...");

    android::sp<IPower> service = new Power();
    if (service == nullptr) {
        ALOGE("Can not create an instance of Power HAL Iface, exiting.");
        return 1;
    }
    configureRpcThreadpool(1, true /*callerWillJoin*/);

    status_t status = service->registerAsService();
    if (status != OK) {
        ALOGE("Could not register service for Power HAL Iface (%d), exiting.", status);
        return 1;
    }

    ALOGI("HIDL Service started.");

    joinRpcThreadpool();

    // In normal operation, we don't expect the thread pool to exit
    ALOGE("Power Service is shutting down");
    return 1;
}
