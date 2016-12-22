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

# Board: yukon
ifneq ($(filter yukon,$(PRODUCT_PLATFORM)),)
ifneq ($(filter eagle,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="22"
endif
ifneq ($(filter flamingo,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="18"
endif
ifneq ($(filter seagull,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="21"
endif
ifneq ($(filter tianchi,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="16"
endif
endif

# Board: rhine
ifneq ($(filter rhine,$(PRODUCT_PLATFORM)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="16"
endif

# Board: shinano
ifneq ($(filter shinano,$(PRODUCT_PLATFORM)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="16"
endif

# Board: kanuti
ifneq ($(filter kanuti,$(PRODUCT_PLATFORM)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="21"
endif

# Board: kitakami
ifneq ($(filter kitakami,$(PRODUCT_PLATFORM)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="32"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MAJOR="259"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MINOR="0"
endif

# Board: loire
ifneq ($(filter loire,$(PRODUCT_PLATFORM)),)
ifneq ($(filter kugo,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="46"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MAJOR="259"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MINOR="14"
else
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="42"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MAJOR="259"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MINOR="10"
endif
endif

# Board: tone
ifneq ($(filter tone,$(PRODUCT_PLATFORM)),)
ifneq ($(filter kagura,$(TARGET_DEVICE)),)
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="48"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MAJOR="259"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MINOR="16"
else
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_NUM="45"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MAJOR="259"
BOARD_SONY_INIT_FLAGS += -DDEV_BLOCK_FOTA_MINOR="13"
endif
endif

# FOTA check is broken on all devices
BOARD_SONY_INIT_FLAGS += -DFOTA_RAMDISK_CHECK="0"

# Ensure keycheck is used on all devices on AOSP
BOARD_SONY_INIT_FLAGS += -DKEYCHECK_ENABLED="1"

# Only use Volume+ on all devices on AOSP
BOARD_SONY_INIT_FLAGS += -DKEYCHECK_NO_DOWN="1"
