#!/usr/bin/env bash

# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if [ -z $ANDROID_BUILD_TOP ]; then
  echo "You need to source and lunch before you can use this script"
  exit 1
fi

echo "Running tests"
set -e # fail early

# NOTE We can't actually run these commands, since they rely on functions added by
#      build/envsetup.sh to the bash shell environment.
echo "+ mmma -j32 $ANDROID_BUILD_TOP/device/google/wahoo"
make -j32 -C $ANDROID_BUILD_TOP -f build/core/main.mk \
    MODULES-IN-device-google-wahoo-wifi_offload

set -x # print commands

adb root
adb wait-for-device
adb remount
adb sync

adb shell /data/nativetest/vendor/wifi-offload-service-unit-tests/wifi-offload-service-unit-tests
adb shell /data/nativetest64/vendor/wifi-offload-service-unit-tests/wifi-offload-service-unit-tests
