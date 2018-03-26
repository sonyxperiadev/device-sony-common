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

# librqbalance enablement
PRODUCT_PROPERTY_OVERRIDES += \
    ro.vendor.extension_library=/vendor/lib/librqbalance.so

# Limit dex2oat threads to improve thermals
PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.dex2oat-threads=2 \
    dalvik.vm.image-dex2oat-threads=4

# Platform specific default properties
PRODUCT_PROPERTY_OVERRIDES += \
    persist.data.qmi.adb_logmask=0

# configure adb over wifi only on the eng build
ifneq (,$(filter eng, $(TARGET_BUILD_VARIANT)))
PRODUCT_PROPERTY_OVERRIDES += \
    service.adb.tcp.port=5555
endif

# Default to LTE/GSM/WCDMA.
PRODUCT_PROPERTY_OVERRIDES += \
    ro.telephony.default_network=9

# System props for the data modules
PRODUCT_PROPERTY_OVERRIDES += \
    ro.use_data_netmgrd=true \
    persist.data.netmgrd.qos.enable=true \
    ro.data.large_tcp_window_size=true

# Enable Power save functionality for modem
PRODUCT_PROPERTY_OVERRIDES += \
    persist.radio.add_power_save=1 \
    persist.radio.apm_sim_not_pwdn=1 \
    persist.radio.oem_socket=false

# Ringer
PRODUCT_PROPERTY_OVERRIDES += \
    ro.telephony.call_ring.multiple=false

# System props for telephony System prop to turn on CdmaLTEPhone always
PRODUCT_PROPERTY_OVERRIDES += \
    telephony.lteOnCdmaDevice=0

# debug.sf.latch_unsignaled
# - This causes SurfaceFlinger to latch
#   buffers even if their fences haven't signaled
PRODUCT_PROPERTY_OVERRIDES += \
    debug.sf.latch_unsignaled=1

# Hardware User Interface parameters
PRODUCT_PROPERTY_OVERRIDES += \
    ro.hwui.texture_cache_size=48 \
    ro.hwui.layer_cache_size=32 \
    ro.hwui.r_buffer_cache_size=4 \
    ro.hwui.path_cache_size=24 \
    ro.hwui.gradient_cache_size=1 \
    ro.hwui.drop_shadow_cache_size=5 \
    ro.hwui.texture_cache_flushrate=0.5 \
    ro.hwui.text_small_cache_width=1024 \
    ro.hwui.text_small_cache_height=1024 \
    ro.hwui.text_large_cache_width=2048 \
    ro.hwui.text_large_cache_height=1024

# DRM service
PRODUCT_PROPERTY_OVERRIDES += \
    drm.service.enabled=true

# VIDC: debug_levels 1:ERROR 2:HIGH 4:LOW 0:NOLOGS 7:AllLOGS
PRODUCT_PROPERTY_OVERRIDES += \
    vidc.debug.level=1

# Audio
PRODUCT_PROPERTY_OVERRIDES += \
    persist.audio.fluence.voicecall=true \
    persist.audio.fluence.voicecomm=true \
    persist.audio.fluence.voicerec=false \
    persist.audio.fluence.speaker=true \
    media.aac_51_output_enabled=true \
    audio.deep_buffer.media=1 \
    fmas.hdph_sgain=0

# Property to enable user to access Google WFD settings.
PRODUCT_PROPERTY_OVERRIDES += \
    persist.debug.wfd.enable=1

# Property to choose between virtual/external wfd display
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.wfd.virtual=0

# Wi-Fi interface name
PRODUCT_PROPERTY_OVERRIDES += \
    wifi.interface=wlan0

# BT address
PRODUCT_PROPERTY_OVERRIDES += \
    ro.bt.bdaddr_path=/data/misc/bluetooth/bluetooth_bdaddr

# System prop for NFC DT
PRODUCT_PROPERTY_OVERRIDES += \
    ro.nfc.port=I2C

# Camera Debug
PRODUCT_PROPERTY_OVERRIDES += \
    persist.camera.hal.debug=0 \
    persist.camera.kpi.debug=0 \
    persist.camera.global.debug=0

# Disable Dual Camera
PRODUCT_PROPERTY_OVERRIDES += \
    persist.camera.dual.camera.mpo=0 \
    persist.camera.dc.frame.sync=0 \
    persist.camera.dcrf.enable=0

# Camera
PRODUCT_PROPERTY_OVERRIDES += \
    persist.camera.gyro.disable=1 \
    persist.camera.feature.cac=0 \
    persist.camera.ois.disable=0 \
    persist.camera.eis.enable=0 \
    persist.camera.zsl.mode=1 \
    persist.camera.exif.rotation=off \
    persist.camera.lib2d.rotation=on

# Sensors debug
PRODUCT_PROPERTY_OVERRIDES += \
    persist.debug.sensors.hal=0 \
    debug.qualcomm.sns.daemon=0 \
    debug.qualcomm.sns.hal=0 \
    debug.qualcomm.sns.libsensor1=0

# RILD
PRODUCT_PROPERTY_OVERRIDES += \
    rild.libpath=/odm/lib64/libril-qc-qmi-1.so \
    ril.subscription.types=NV,RUIM

# OpenGLES version
PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=196610

# Vulkan
# b/68017541
PRODUCT_PROPERTY_OVERRIDES += \
    ro.qcom.adreno.qgl.ShaderStorageImageExtendedFormats=0

# Vendor version
PRODUCT_PROPERTY_OVERRIDES += \
    ro.vendor.version=$(TARGET_VENDOR_VERSION)
