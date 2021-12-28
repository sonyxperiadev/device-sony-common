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

TARGET_NO_RADIOIMAGE := true
TARGET_NO_BOOTLOADER := true
TARGET_NO_RECOVERY ?= false
TARGET_NO_KERNEL := false

# common cmdline parameters
ifneq ($(BOARD_USE_ENFORCING_SELINUX),true)
BOARD_KERNEL_CMDLINE += androidboot.selinux=permissive
endif
#BOARD_KERNEL_CMDLINE += console=ttyMSM0,115200,n8 androidboot.console=ttyMSM0
BOARD_KERNEL_CMDLINE += androidboot.memcg=1
BOARD_KERNEL_CMDLINE += msm_rtb.filter=0x3F ehci-hcd.park=3
BOARD_KERNEL_CMDLINE += coherent_pool=8M
BOARD_KERNEL_CMDLINE += sched_enable_power_aware=1 user_debug=31
BOARD_KERNEL_CMDLINE += printk.devkmsg=on
BOARD_KERNEL_CMDLINE += loop.max_part=16
BOARD_KERNEL_CMDLINE += kpti=0

BOARD_MKBOOTIMG_ARGS += --ramdisk_offset $(BOARD_RAMDISK_OFFSET) --tags_offset $(BOARD_KERNEL_TAGS_OFFSET)

TARGET_KERNEL_ARCH := arm64
TARGET_KERNEL_HEADER_ARCH := arm64
BOARD_KERNEL_IMAGE_NAME := Image.gz-dtb

# CPU ARCH
TARGET_ARCH := arm64
TARGET_CPU_ABI2 :=
TARGET_CPU_VARIANT := generic

TARGET_2ND_ARCH := arm
TARGET_2ND_CPU_ABI2 := armeabi
TARGET_2ND_CPU_VARIANT := generic

# Use mke2fs to create ext4 images
TARGET_USES_MKE2FS := true
TARGET_USERIMAGES_USE_EXT4 := true

BOARD_ROOT_EXTRA_FOLDERS := odm
BOARD_ROOT_EXTRA_SYMLINKS += /mnt/vendor/persist:/persist

# Filesystem
TARGET_FS_CONFIG_GEN := $(COMMON_PATH)/config.fs

# GFX
USE_OPENGL_RENDERER := true
BOARD_USES_ADRENO := true
TARGET_USES_ION := true

MAX_EGL_CACHE_KEY_SIZE := 12*1024
MAX_EGL_CACHE_SIZE := 2048*1024

TARGET_FORCE_HWC_FOR_VIRTUAL_DISPLAYS := true

# Display
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
TARGET_USES_COLOR_METADATA := true
TARGET_USES_HWC2 := true
TARGET_USES_DISPLAY_RENDER_INTENTS := true

ifeq ($(TARGET_USES_GRALLOC1),true)
SOONG_CONFIG_NAMESPACES += gralloc
SOONG_CONFIG_gralloc := use_v1
SOONG_CONFIG_gralloc_use_v1 := true
else
SOONG_CONFIG_NAMESPACES += qtidisplay
SOONG_CONFIG_qtidisplay := drmpp gralloc4
SOONG_CONFIG_qtidisplay_drmpp := true
SOONG_CONFIG_qtidisplay_gralloc4 := true
endif

# Audio
USE_XML_AUDIO_POLICY_CONF := 1
BOARD_USES_ALSA_AUDIO := true
BOARD_SUPPORTS_OPENSOURCE_STHAL := true
AUDIO_FEATURE_ENABLED_SVA_MULTI_STAGE := true
AUDIO_FEATURE_ENABLED_MULTI_VOICE_SESSIONS := true

AUDIO_FEATURE_ENABLED_DLKM := false
AUDIO_USE_DEEP_AS_PRIMARY_OUTPUT := false
AUDIO_FEATURE_ENABLED_VBAT_MONITOR := true
AUDIO_FEATURE_ENABLED_ANC_HEADSET := true
AUDIO_FEATURE_ENABLED_FLUENCE := true
AUDIO_FEATURE_ENABLED_DS2_DOLBY_DAP ?= false
AUDIO_FEATURE_ENABLED_HFP := true
AUDIO_FEATURE_ENABLED_INCALL_MUSIC := true
AUDIO_FEATURE_ENABLED_MULTI_VOICE_SESSIONS := true
AUDIO_FEATURE_ENABLED_KPI_OPTIMIZE := true
AUDIO_FEATURE_ENABLED_SPKR_PROTECTION := true
AUDIO_FEATURE_ENABLED_DEV_ARBI := false
MM_AUDIO_ENABLED_FTM := true
TARGET_USES_QCOM_MM_AUDIO := true
AUDIO_FEATURE_ENABLED_SOURCE_TRACKING := true
AUDIO_FEATURE_ENABLED_GEF_SUPPORT := true
BOARD_SUPPORTS_QAHW ?= false
AUDIO_FEATURE_ENABLED_RAS := true
AUDIO_FEATURE_ENABLED_DYNAMIC_LOG := false
AUDIO_FEATURE_ENABLED_SND_MONITOR := true

# Camera
BOARD_QTI_CAMERA_32BIT_ONLY := true
BOARD_QTI_CAMERA_V2 := true
CAMERA_DAEMON_NOT_PRESENT := true

USE_SENSOR_MULTI_HAL := true

# GPS definitions for Qualcomm solution
BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE := $(TARGET_BOARD_PLATFORM)
BOARD_VENDOR_QCOM_LOC_PDK_FEATURE_SET := true
TARGET_NO_RPC := true

# Charger
BOARD_CHARGER_DISABLE_INIT_BLANK := true
BOARD_CHARGER_ENABLE_SUSPEND := true

# DRM
TARGET_ENABLE_MEDIADRM_64 := true

# Wi-Fi Concurrent STA/AP
WIFI_HIDL_FEATURE_DUAL_INTERFACE := true

# CNE
BOARD_USES_QCNE := true

# Enable dex-preoptimization to speed up first boot sequence
ifeq ($(HOST_OS),linux)
  ifneq ($(TARGET_BUILD_VARIANT),eng)
    ifeq ($(WITH_DEXPREOPT),)
      WITH_DEXPREOPT := true
    endif
  endif
endif
WITH_DEXPREOPT_BOOT_IMG_AND_SYSTEM_SERVER_ONLY ?= true

# Build kernel using kernel's Android.mk file.
# May be overriden by KernelConfig.mk if prebuilt kernel present.
# Can also be turned off in Customization.mk in case it is desired to use a
# custom ROM's kernel build system, e.g. LineageOS' or PE's.
BUILD_KERNEL ?= true

-include $(KERNEL_PATH)/common-kernel/KernelConfig.mk

ifeq ($(TARGET_NEEDS_DTBOIMAGE),true)
ifeq ($(BUILD_KERNEL),true)
BOARD_DTBO_IMAGE_NAME := dtbo-$(TARGET_DEVICE).img
BOARD_PREBUILT_DTBOIMAGE ?= $(PRODUCT_OUT)/$(BOARD_DTBO_IMAGE_NAME)
else
BOARD_PREBUILT_DTBOIMAGE ?= kernel/sony/msm-$(SOMC_KERNEL_VERSION)/common-kernel/dtbo-$(TARGET_DEVICE).img
endif
endif

# Include build helpers for QCOM proprietary
-include vendor/qcom/proprietary/common/build/proprietary-build.mk

# SELinux
include device/sony/sepolicy/sepolicy.mk

# Device manifest: What HALs the device provides
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/manifest.xml
# Framework compatibility matrix: What the device(=vendor) expects of the framework(=system)
DEVICE_MATRIX_FILE   += $(COMMON_PATH)/vintf/compatibility_matrix.xml

# Custom NXP NFC vendor interface
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.nxp.nxpnfc.xml

# SIM secure element, SIM1/SIM2
ifeq ($(PRODUCT_DEVICE_DS),true)
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hardware.secure_element_ds.xml
else
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hardware.secure_element_ss.xml
endif

# Dynamic Power Management
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.qualcomm.qti.dpm.xml

ifeq ($(PRODUCT_DEVICE_DS),true)
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hw.qcradio_ds.xml
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.hw.radio_ds.xml
else
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hw.qcradio_ss.xml
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.hw.radio_ss.xml
endif

DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hardware.radio.config.xml
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.hw.radio.internal.xml
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.hw.radio.uceservice.xml
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.hw.imsservices.xml
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.hw.dataservices.xml
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.somc.modem.xml

DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.hw.cneservices.xml

ifeq ($(TARGET_KEYMASTER_V4),true)
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hw.keymaster_v4.xml
else
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hw.keymaster_v3.xml
endif

ifeq ($(TARGET_VIBRATOR_V1_2),true)
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hardware.vibrator_v1.2.xml
else
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/android.hardware.vibrator_v1.0.xml
endif

# CamX manifests
ifneq ($(filter edo,$(SOMC_PLATFORM)),)
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/vintf/vendor.qti.hardware.camera.postproc.xml
endif

# New vendor security patch level: https://r.android.com/660840/
# Used by newer keymaster binaries
VENDOR_SECURITY_PATCH=$(PLATFORM_SECURITY_PATCH)

TARGET_USES_IMS:= true
