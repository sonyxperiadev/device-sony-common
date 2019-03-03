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

-include device/sony/customization/Customization.mk

# Common path
COMMON_PATH := device/sony/common

# Do not build proprietary capability
TARGET_USES_AOSP := true

TARGET_BOARD_AUTO := true

TARGET_NO_RADIOIMAGE := true
TARGET_NO_BOOTLOADER := true
TARGET_NO_RECOVERY ?= false
TARGET_NO_KERNEL := false

# common cmdline parameters
ifneq ($(BOARD_USE_ENFORCING_SELINUX),true)
BOARD_KERNEL_CMDLINE += androidboot.selinux=permissive
endif
#BOARD_KERNEL_CMDLINE += console=ttyMSM0,115200,n8 androidboot.console=ttyMSM0
BOARD_KERNEL_CMDLINE += msm_rtb.filter=0x3F ehci-hcd.park=3
BOARD_KERNEL_CMDLINE += coherent_pool=8M
BOARD_KERNEL_CMDLINE += sched_enable_power_aware=1 user_debug=31
BOARD_KERNEL_CMDLINE += cgroup.memory=nokmem
BOARD_KERNEL_CMDLINE += printk.devkmsg=on

BOARD_MKBOOTIMG_ARGS := --ramdisk_offset $(BOARD_RAMDISK_OFFSET) --tags_offset $(BOARD_KERNEL_TAGS_OFFSET)

TARGET_KERNEL_ARCH := arm64
TARGET_KERNEL_HEADER_ARCH := arm64
TARGET_KERNEL_CROSS_COMPILE_PREFIX := aarch64-linux-android-
BOARD_KERNEL_IMAGE_NAME := Image.gz-dtb

# Use mke2fs to create ext4 images
TARGET_USES_MKE2FS := true
TARGET_USERIMAGES_USE_EXT4 := true

BOARD_ROOT_EXTRA_FOLDERS := odm
BOARD_ROOT_EXTRA_SYMLINKS += /$(TARGET_COPY_OUT_VENDOR)/dsp:/dsp
BOARD_ROOT_EXTRA_SYMLINKS += /$(TARGET_COPY_OUT_VENDOR)/firmware_mnt:/firmware
BOARD_ROOT_EXTRA_SYMLINKS += /$(TARGET_COPY_OUT_VENDOR)/bt_firmware:/bt_firmware
BOARD_ROOT_EXTRA_SYMLINKS += /mnt/vendor/persist:/persist

# Filesystem
TARGET_FS_CONFIG_GEN := $(COMMON_PATH)/config.fs

# GFX
USE_OPENGL_RENDERER := true
BOARD_USES_ADRENO := true
TARGET_USES_ION := true

MAX_EGL_CACHE_KEY_SIZE := 12*1024
MAX_EGL_CACHE_SIZE := 2048*1024

OVERRIDE_RS_DRIVER := libRSDriver_adreno.so
TARGET_FORCE_HWC_FOR_VIRTUAL_DISPLAYS := true

# Display
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
TARGET_USES_COLOR_METADATA := true
TARGET_USES_HWC2 := true

# Audio
BOARD_USES_ALSA_AUDIO := true
AUDIO_FEATURE_ENABLED_MULTI_VOICE_SESSIONS := true
USE_XML_AUDIO_POLICY_CONF := 1

# Camera
BOARD_QTI_CAMERA_32BIT_ONLY := true
BOARD_QTI_CAMERA_V2 := true
CAMERA_DAEMON_NOT_PRESENT := true

# GPS definitions for Qualcomm solution
BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE := $(TARGET_BOARD_PLATFORM)
BOARD_VENDOR_QCOM_LOC_PDK_FEATURE_SET := true
TARGET_NO_RPC := true

# Charger
BOARD_CHARGER_DISABLE_INIT_BLANK := true
BOARD_CHARGER_ENABLE_SUSPEND := true

# Check manifest and compatibility_matrix files at build time
PRODUCT_ENFORCE_VINTF_MANIFEST_OVERRIDE := true
VINTF_ENFORCE_NO_UNUSED_HALS := true

# Enable dex-preoptimization to speed up first boot sequence
ifeq ($(HOST_OS),linux)
  ifneq ($(TARGET_BUILD_VARIANT),eng)
    ifeq ($(WITH_DEXPREOPT),)
      WITH_DEXPREOPT := true
    endif
  endif
endif
WITH_DEXPREOPT_BOOT_IMG_AND_SYSTEM_SERVER_ONLY ?= true

BUILD_KERNEL := true
-include $(KERNEL_PATH)/common-headers/KernelHeaders.mk
-include $(KERNEL_PATH)/common-kernel/KernelConfig.mk

# Include build helpers for QCOM proprietary
-include vendor/qcom/proprietary/common/build/proprietary-build.mk

# SELinux
include device/sony/sepolicy/sepolicy.mk

DEVICE_MANIFEST_FILE := $(COMMON_PATH)/vintf/manifest.xml
DEVICE_MATRIX_FILE   := $(COMMON_PATH)/vintf/compatibility_matrix.xml

ifeq ($(PRODUCT_DEVICE_DS),true)
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hw.radio_ds.xml
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.hw.radio_ds.xml
else
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hw.radio_ss.xml
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.hw.radio_ss.xml
endif

ifeq ($(TARGET_KEYMASTER_V4),true)
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hw.keymaster_v4.xml
else
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hw.keymaster_v3.xml
endif
