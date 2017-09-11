# Copyright 2014 The Android Open Source Project
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

# Init configuration for init_sony
BOARD_SONY_INIT_FLAGS :=
BOARD_SONY_INIT_INCLUDE := $(COMMON_PATH)/include

# FOTA check is broken on all devices
BOARD_SONY_INIT_FLAGS += -DFOTA_RAMDISK_CHECK="0"

# Ensure keycheck is used on all devices on AOSP
BOARD_SONY_INIT_FLAGS += -DKEYCHECK_ENABLED="1"

# Only use Volume+ on all devices on AOSP
BOARD_SONY_INIT_FLAGS += -DKEYCHECK_NO_DOWN="1"
