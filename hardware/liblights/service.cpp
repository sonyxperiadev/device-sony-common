/*
 * Copyright 2016 The Android Open Source Project
 * Copyright 2018 Shane Francis
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

#define LOG_TAG "android.hardware.light@2.0-service.sony"

#include "Light.h"
#include <android-base/logging.h>
#include <android/hardware/light/2.0/ILight.h>
#include <hidl/HidlSupport.h>
#include <hidl/HidlTransportSupport.h>

using android::NO_ERROR;
using android::sp;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::hardware::light::V2_0::ILight;
using android::hardware::light::V2_0::implementation::Light;

int main()
{
    LOG(INFO) << __func__ << " : Start HAL";
    android::sp<ILight> light = new Light();

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (light != nullptr) {
        auto rc = light->registerAsService();
        if (rc != NO_ERROR) {
            LOG(ERROR) << "Cannot start Light service: " << rc;
            return rc;
        }
    } else {
        LOG(ERROR) << "Can't create instance of Light, nullptr";
    }

    joinRpcThreadpool();

    return 0; // should never get here
}
