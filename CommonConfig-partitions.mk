# Copyright (C) 2026 The Android Open Source Project
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

# Filesystem Config Generator
TARGET_FS_CONFIG_GEN := $(COMMON_PATH)/config.fs

# Extra symlinks for the root filesystem
BOARD_ROOT_EXTRA_SYMLINKS += /mnt/vendor/persist:/persist

# Set the block size for flash (multiple of 64 kernel pages)
BOARD_FLASH_BLOCK_SIZE := $(shell echo $$(($(BOARD_KERNEL_PAGESIZE) * 64)))

# Build super image
BOARD_BUILD_SUPER_IMAGE_BY_DEFAULT := true

# Define dynamic partition groups
BOARD_SUPER_PARTITION_GROUPS := sod_dynamic_partitions

# Define the list of dynamic partitions for SOD group
BOARD_SOD_DYNAMIC_PARTITIONS_PARTITION_LIST := \
    system \
    system_ext \
    product \
    vendor

ifeq ($(PRODUCT_BUILD_ODM_IMAGE),true)
BOARD_SOD_DYNAMIC_PARTITIONS_PARTITION_LIST += \
    odm
endif

# Definitions for boot partition
BOARD_MKBOOTIMG_ARGS += --ramdisk_offset $(BOARD_RAMDISK_OFFSET) --tags_offset $(BOARD_KERNEL_TAGS_OFFSET)
BOARD_MKBOOTIMG_ARGS += --header_version $(BOARD_BOOT_HEADER_VERSION)

# Definitions for init_boot partition
BOARD_MKBOOTIMG_INIT_ARGS += --header_version $(BOARD_INIT_BOOT_HEADER_VERSION)

# Definitions for vendor_boot partition
# https://source.android.com/devices/bootloader/partitions/vendor-boot-partitions#build-support
BOARD_INCLUDE_DTB_IN_BOOTIMG := true
BOARD_RAMDISK_USE_LZ4 := true

# Definitions for dtbo partition
BOARD_DTBO_IMAGE_NAME := dtbo-$(TARGET_DEVICE).img
ifeq ($(BUILD_KERNEL),true)
BOARD_PREBUILT_DTBOIMAGE ?= $(PRODUCT_OUT)/$(BOARD_DTBO_IMAGE_NAME)
else
BOARD_PREBUILT_DTBOIMAGE ?= kernel/sony/msm-$(SOMC_KERNEL_VERSION)/common-kernel/$(SOMC_PLATFORM)/$(BOARD_DTBO_IMAGE_NAME)
endif

# Definitions for recovery partition
# https://source.android.com/devices/bootloader/partitions/generic-boot#combinations
ifeq ($(PRODUCT_BUILD_RECOVERY_IMAGE),true)
# Device with recovery partition
BOARD_EXCLUDE_KERNEL_FROM_RECOVERY_IMAGE := true
BOARD_USES_FULL_RECOVERY_IMAGE := true
else
# Device without recovery partition
BOARD_MOVE_RECOVERY_RESOURCES_TO_VENDOR_BOOT := true
endif

# Definitions for system partition
BOARD_SYSTEMIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_SYSTEMIMAGE_PARTITION_RESERVED_SIZE := 52428800

# Definitions for system_ext partition
TARGET_COPY_OUT_SYSTEM_EXT := system_ext
BOARD_SYSTEM_EXTIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_SYSTEM_EXTIMAGE_PARTITION_RESERVED_SIZE := 52428800

# Definitions for vendor partition
TARGET_COPY_OUT_VENDOR := vendor
BOARD_VENDORIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_VENDORIMAGE_PARTITION_RESERVED_SIZE := 52428800

# Definitions for product partition
TARGET_COPY_OUT_PRODUCT := product
BOARD_PRODUCTIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_PRODUCTIMAGE_PARTITION_RESERVED_SIZE := 52428800

# Definitions for odm partition
TARGET_COPY_OUT_ODM := odm
BOARD_ODMIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_ODMTIMAGE_PARTITION_RESERVED_SIZE := 52428800

# Definitions for userdata partition
TARGET_USERIMAGES_USE_F2FS := true
BOARD_USERDATAIMAGE_FILE_SYSTEM_TYPE := f2fs

# Definitions for metadata partition
BOARD_USES_METADATA_PARTITION := true

# Define the list of A/B partitions
# which are common across all platforms
AB_OTA_PARTITIONS += \
    boot \
    dtbo \
    system \
    system_ext \
    product \
    vbmeta \
    vbmeta_system \
    vendor \
    vendor_boot

ifeq ($(PRODUCT_BUILD_INIT_BOOT_IMAGE),true)
AB_OTA_PARTITIONS += \
    init_boot
endif

ifeq ($(PRODUCT_BUILD_PVMFW_IMAGE),true)
AB_OTA_PARTITIONS += \
    pvmfw
endif

ifeq ($(PRODUCT_BUILD_RECOVERY_IMAGE),true)
AB_OTA_PARTITIONS += \
    recovery
endif

ifeq ($(PRODUCT_BUILD_ODM_IMAGE),true)
AB_OTA_PARTITIONS += \
    odm
endif

# Enable AVB (Android Verified Boot) version 2.0
BOARD_AVB_ENABLE := true

# Enable chained vbmeta for system images
# https://source.android.com/docs/core/architecture/partitions/generic-boot#chain-vbmeta
BOARD_AVB_VBMETA_SYSTEM := system system_ext product
ifeq ($(PRODUCT_BUILD_PVMFW_IMAGE),true)
BOARD_AVB_VBMETA_SYSTEM += pvmfw
endif
BOARD_AVB_VBMETA_SYSTEM_KEY_PATH := external/avb/test/data/testkey_rsa4096.pem
BOARD_AVB_VBMETA_SYSTEM_ALGORITHM := SHA256_RSA4096
BOARD_AVB_VBMETA_SYSTEM_ROLLBACK_INDEX := $(PLATFORM_SECURITY_PATCH_TIMESTAMP)
BOARD_AVB_VBMETA_SYSTEM_ROLLBACK_INDEX_LOCATION := 1

# Enable chained vbmeta for boot image
BOARD_AVB_BOOT_KEY_PATH := external/avb/test/data/testkey_rsa4096.pem
BOARD_AVB_BOOT_ALGORITHM := SHA256_RSA4096
BOARD_AVB_BOOT_ROLLBACK_INDEX := $(PLATFORM_SECURITY_PATCH_TIMESTAMP)
BOARD_AVB_BOOT_ROLLBACK_INDEX_LOCATION := 2

# Enable chained vbmeta for init_boot image
BOARD_AVB_INIT_BOOT_KEY_PATH := external/avb/test/data/testkey_rsa4096.pem
BOARD_AVB_INIT_BOOT_ALGORITHM := SHA256_RSA4096
BOARD_AVB_INIT_BOOT_ROLLBACK_INDEX := $(PLATFORM_SECURITY_PATCH_TIMESTAMP)
BOARD_AVB_INIT_BOOT_ROLLBACK_INDEX_LOCATION := 3

# Define hash algorithm for partitions
BOARD_AVB_SYSTEM_ADD_HASHTREE_FOOTER_ARGS += --hash_algorithm sha256
BOARD_AVB_SYSTEM_EXT_ADD_HASHTREE_FOOTER_ARGS += --hash_algorithm sha256
BOARD_AVB_PRODUCT_ADD_HASHTREE_FOOTER_ARGS += --hash_algorithm sha256
BOARD_AVB_VENDOR_ADD_HASHTREE_FOOTER_ARGS += --hash_algorithm sha256
BOARD_AVB_ODM_ADD_HASHTREE_FOOTER_ARGS += --hash_algorithm sha256
