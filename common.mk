# Copyright (C) 2014 The Android Open Source Project
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

$(call inherit-product-if-exists, device/sony/customization/customization.mk)

# Vendor version
TARGET_VENDOR_VERSION := v4

# Specify a "dev-keys" configuration.  Keys from
# vendor/oss/release-keys will be used instead of the keys under
# build/make/target/product/security, with the exception of the verity
# key.  For verity, the build system will still use
# build/make/target/product/security/verity.pk8, but this can be
# changed using a post-build re-signing operation (or by making a
# change inside the build project).
PRODUCT_DEFAULT_DEV_CERTIFICATE := vendor/oss/release-keys/testkey

# Common path
COMMON_PATH := device/sony/common

# Build scripts
SONY_CLEAR_VARS := $(COMMON_PATH)/sony_clear_vars.mk
SONY_BUILD_SYMLINKS := $(COMMON_PATH)/sony_build_symlinks.mk

DEVICE_PACKAGE_OVERLAYS += $(COMMON_PATH)/overlay

PRODUCT_ENFORCE_RRO_TARGETS := \
    framework-res

# Codecs Configuration
PRODUCT_COPY_FILES += \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:$(TARGET_COPY_OUT_VENDOR)/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:$(TARGET_COPY_OUT_VENDOR)/etc/media_codecs_google_telephony.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video.xml:$(TARGET_COPY_OUT_VENDOR)/etc/media_codecs_google_video.xml

# Common etc
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/rootdir/system/etc/nfcee_access.xml:system/etc/nfcee_access.xml

# GPS Configuration
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/rootdir/vendor/etc/gps.conf:$(TARGET_COPY_OUT_VENDOR)/etc/gps.conf \
    $(COMMON_PATH)/rootdir/system/etc/gps_debug.conf:system/etc/gps_debug.conf

# Sensors common
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/rootdir/vendor/etc/sensors/sensors_settings:$(TARGET_COPY_OUT_VENDOR)/etc/sensors/sensors_settings

# QMI
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/rootdir/vendor/etc/data/dsi_config.xml:$(TARGET_COPY_OUT_VENDOR)/etc/data/dsi_config.xml \
    $(COMMON_PATH)/rootdir/vendor/etc/data/netmgr_config.xml:$(TARGET_COPY_OUT_VENDOR)/etc/data/netmgr_config.xml \
    $(COMMON_PATH)/rootdir/vendor/etc/data/qmi_config.xml:$(TARGET_COPY_OUT_VENDOR)/etc/data/qmi_config.xml

# Sec Configuration
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/rootdir/vendor/etc/sec_config:$(TARGET_COPY_OUT_VENDOR)/etc/sec_config

# Seccomp policy
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/rootdir/vendor/etc/seccomp_policy/mediaextractor.policy:$(TARGET_COPY_OUT_VENDOR)/etc/seccomp_policy/mediaextractor.policy \
    $(COMMON_PATH)/rootdir/vendor/etc/seccomp_policy/mediacodec.policy:$(TARGET_COPY_OUT_VENDOR)/etc/seccomp_policy/mediacodec.policy

# Audio Configuration
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/rootdir/vendor/etc/audio_effects.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio_effects.xml \
    frameworks/av/services/audiopolicy/config/a2dp_audio_policy_configuration.xml:$(TARGET_COPY_OUT_VENDOR)/etc/a2dp_audio_policy_configuration.xml \
    frameworks/av/services/audiopolicy/config/audio_policy_volumes.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio_policy_volumes.xml \
    frameworks/av/services/audiopolicy/config/default_volume_tables.xml:$(TARGET_COPY_OUT_VENDOR)/etc/default_volume_tables.xml \
    frameworks/av/services/audiopolicy/config/r_submix_audio_policy_configuration.xml:$(TARGET_COPY_OUT_VENDOR)/etc/r_submix_audio_policy_configuration.xml \
    frameworks/av/services/audiopolicy/config/usb_audio_policy_configuration.xml:$(TARGET_COPY_OUT_VENDOR)/etc/usb_audio_policy_configuration.xml

# Public Libraries
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/rootdir/vendor/etc/public.libraries.txt:$(TARGET_COPY_OUT_VENDOR)/etc/public.libraries.txt

# librqbalance
PRODUCT_PACKAGES += \
    librqbalance

# PRODUCT_PLATFORM isn't set yet, thus we check the available path
ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
# ramdump cleaner
PRODUCT_PACKAGES += \
    rdclean.sh
endif

# Qcom scripts
PRODUCT_PACKAGES += \
    init.qcom.devstart.sh \
    init.qcom.ipastart.sh

# Depend on generation of adreno symlinks (from /vendor to /odm)
PRODUCT_PACKAGES += \
    adreno_symlinks

# APN list
PRODUCT_COPY_FILES += \
    device/sample/etc/old-apns-conf.xml:system/etc/old-apns-conf.xml \
    device/sample/etc/apns-full-conf.xml:system/etc/apns-conf.xml

PRODUCT_CUSTOM_IMAGE_MAKEFILES := $(COMMON_PATH)/odm.mk

$(call inherit-product, device/sony/common/common-init.mk)
$(call inherit-product, device/sony/common/common-odm.mk)
$(call inherit-product, device/sony/common/common-packages.mk)
$(call inherit-product, device/sony/common/common-perm.mk)
$(call inherit-product, device/sony/common/common-prop.mk)
$(call inherit-product, device/sony/common/common-treble.mk)
