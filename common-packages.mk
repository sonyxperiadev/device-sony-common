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

# Vendor service manager no longer included in API level 30+,
# however needed by odm blobs.
PRODUCT_PACKAGES += \
    vndservicemanager

# TEMPORARY: These libraries are deprecated, not referenced by any AOSP
# nor OSS HAL, but are still required by odm blobs (short term).
# We don't add a dependency on the vndk variants as those end up in
# /system but require these in /vendor instead:
PRODUCT_PACKAGES += \
    libhwbinder.vendor \
    libhidltransport.vendor

# Audio
PRODUCT_PACKAGES += \
    audio.bluetooth.default \
    audio.r_submix.default \
    audio.usb.default \
    audioadsprpcd \
    libaudio-resampler

# For audio.primary
PRODUCT_PACKAGES += \
    libtinyalsa \
    libaudioroute \
    tinymix

# Audio effects
PRODUCT_PACKAGES += \
    libqcomvisualizer \
    libqcomvoiceprocessing \
    libqcompostprocbundle

# Audio deps
PRODUCT_PACKAGES += \
    libfmq

# Audioreach audio
ifeq ($(TARGET_USES_AUDIOREACH),true)
# AGM
PRODUCT_PACKAGES += \
    libagm_compress_plugin \
    libagm_mixer_plugin \
    libagm_pcm_plugin

# PAL
PRODUCT_PACKAGES += \
    lib_bt_aptx \
    lib_bt_ble \
    lib_bt_bundle
endif

# GFX
PRODUCT_PACKAGES += \
    libhdmi \
    libqdutils \
    libqdMetaData

# OMX
PRODUCT_PACKAGES += \
    libc2dcolorconvert \
    libstagefrighthw \
    libOmxCore \
    libmm-omxcore \
    libOmxVdec \
    libOmxVdecHevc \
    libOmxVenc

# GPS
PRODUCT_PACKAGES += \
    batching.conf \
    gnss_antenna_info.conf \
    libbatching \
    libgeofencing \
    libgnss

# Location
PRODUCT_PACKAGES += \
    libloc_api_v02 \
    libloc_socket \
    liblocation_api_msg \
    liblocation_integration_api \
    liblocation_client_api \
    libgnsspps \
    libsynergy_loc_api

# IMS (OSS)
PRODUCT_PACKAGES += \
    qti-telephony-hidl-wrapper \
    qti_telephony_hidl_wrapper.xml \
    qti-telephony-utils \
    qti_telephony_utils.xml \
    telephony-ext \
    ims-ext-common \
    ims_ext_common.xml

# IPA
PRODUCT_PACKAGES += \
    libqti_vndfwk_detect.vendor \
    IPACM_cfg.xml \
    IPACM_Filter_cfg.xml \
    ipacm

# WLAN
PRODUCT_PACKAGES += \
    hostapd \
    libwpa_client \
    wpa_supplicant \
    wpa_supplicant.conf \
    libwpa_client

# NFC packages
PRODUCT_PACKAGES += \
    NfcNci \
    Tag

# CAMERA
PRODUCT_PACKAGES += \
    libmmcamera_interface \
    libmmjpeg_interface \
    libmmlib2d_interface \
    libmm-qcamera \
    libqomx_core

# Media
PRODUCT_PACKAGES += \
    libavservices_minijail.vendor

# OSS Time service
PRODUCT_PACKAGES += \
    timekeep \
    TimeKeep \

# OSS Power HAL
PRODUCT_PACKAGES += \
    librqbalance

# OSS WIFI and BT MAC tool
PRODUCT_PACKAGES += \
    macaddrsetup

# Carrier configuration
PRODUCT_PACKAGES += \
    CarrierConfig

# Telephony
PRODUCT_PACKAGES += \
    ModemConfig \
    QcRilAm \
    SimDetect

# QCOM Data
PRODUCT_PACKAGES += \
    librmnetctl

# RIL
PRODUCT_PACKAGES += \
    libqti_vndfwk_detect_vendor \
    libandroid_net \
    libprotobuf-cpp-full

# Prebuilt protobuf 3.9.1 for ODM HALs
PRODUCT_PACKAGES += \
    libprotobuf-cpp-full-3.9.1-vendorcompat \
    libprotobuf-cpp-lite-3.9.1-vendorcompat

# libRSDriver_adreno dependency
PRODUCT_PACKAGES += \
    libLLVM

# ExtendedSettings
PRODUCT_PACKAGES += \
    ExtendedSettings

# Charger
PRODUCT_PACKAGES += \
    charger_res_images_vendor

# AOSP Packages
PRODUCT_PACKAGES += \
    Launcher3QuickStep \
    libion \
    libjson \
    libxml2 \

# External exFat tools
PRODUCT_PACKAGES += \
    mkfs.exfat \
    fsck.exfat

# For config.fs
PRODUCT_PACKAGES += \
    fs_config_files \
    fs_config_dirs
