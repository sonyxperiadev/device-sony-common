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

# Flag: FOTA number set
fota_num_set := false

# Init configuration for init_sony
BOARD_SONY_INIT_FLAGS :=
BOARD_SONY_INIT_INCLUDE := $(COMMON_PATH)/include

# Board: yukon
ifneq ($(filter yukon,$(PRODUCT_PLATFORM)),)
ifneq ($(filter eagle,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="22"
fota_num_set := true
endif
ifneq ($(filter flamingo,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="18"
fota_num_set := true
endif
ifneq ($(filter seagull,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="21"
fota_num_set := true
endif
ifneq ($(filter tianchi,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="16"
fota_num_set := true
endif
endif

# Board: rhine
ifneq ($(filter rhine,$(PRODUCT_PLATFORM)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="16"
fota_num_set := true
endif

# Board: shinano
ifneq ($(filter shinano,$(PRODUCT_PLATFORM)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="16"
fota_num_set := true
endif

# Board: kanuti
ifneq ($(filter kanuti,$(PRODUCT_PLATFORM)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="21"
fota_num_set := true
endif

# Board: kitakami
ifneq ($(filter kitakami,$(PRODUCT_PLATFORM)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="32"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MAJOR="259"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MINOR="0"
fota_num_set := true
endif

# Board: loire
ifneq ($(filter loire,$(PRODUCT_PLATFORM)),)
ifneq ($(filter kugo,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="46"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MAJOR="259"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MINOR="14"
fota_num_set := true
else
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="42"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MAJOR="259"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MINOR="10"
fota_num_set := true
endif
endif

# Abort if the device is not handled
ifneq ($(fota_num_set),true)
$(error device-sony-common-init: DEV_BLOCK_FOTA_NUM missing for "$(TARGET_DEVICE)", platform "$(PRODUCT_PLATFORM)")
endif

# Debug current init_sony settings
$(info device-sony-common-init: init_sony for "$(TARGET_DEVICE)", platform "$(PRODUCT_PLATFORM)", with $(BOARD_SONY_INIT_FLAGS))

# FOTA check is broken on all devices
BOARD_SONY_INIT_FLAGS += -DFOTA_RAMDISK_CHECK="0"

# Ensure keycheck is used on all devices on AOSP
BOARD_SONY_INIT_FLAGS += -DKEYCHECK_ENABLED="1"

# Only use Volume+ on all devices on AOSP
BOARD_SONY_INIT_FLAGS += -DKEYCHECK_NO_DOWN="1"
