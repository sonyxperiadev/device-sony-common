# Composer
PRODUCT_PACKAGES += \
    vendor.qti.hardware.display.composer-service

# Linked by Adreno/EGL blobs for fallback if 3.0 doesn't exist
PRODUCT_PACKAGES += \
    vendor.qti.hardware.display.mapper@2.0.vendor

# Graphics allocator/mapper
PRODUCT_PACKAGES += \
    android.hardware.graphics.mapper@3.0-impl-qti-display \
    android.hardware.graphics.mapper@4.0-impl-qti-display

# android.hardware.graphics.allocator@3.0::IAllocator, and
# android.hardware.graphics.allocator@4.0::IAllocator if
# TARGET_USES_GRALLOC4 is not explicitly set to `false`:
PRODUCT_PACKAGES += \
    vendor.qti.hardware.display.allocator-service

# RIL
# Interface library needed by odm blobs:
PRODUCT_PACKAGES += \
    android.hardware.radio@1.6.vendor \
    android.hardware.radio.config@1.3.vendor \
    android.hardware.radio.deprecated@1.0.vendor \
    android.hardware.radio.config-V1-ndk.vendor \
    android.hardware.radio.messaging-V1-ndk.vendor \
    android.hardware.radio.modem-V1-ndk.vendor \
    android.hardware.radio.network-V1-ndk.vendor \
    android.hardware.radio.sim-V1-ndk.vendor \
    android.hardware.radio.voice-V1-ndk.vendor \
    android.hardware.radio-V1-ndk.vendor

# Secure Element
PRODUCT_PACKAGES += \
    android.hardware.secure_element-service.nxp \
    android.hardware.secure_element@1.2.vendor

# netmgrd
PRODUCT_PACKAGES += \
    android.system.net.netd@1.1.vendor

# Audio
PRODUCT_PACKAGES += \
    android.hardware.audio@7.1-impl \
    android.hardware.audio.service \
    android.hardware.audio.effect@7.0-impl \
    android.hardware.bluetooth.audio-impl \
    android.hardware.soundtrigger@2.3-impl

# Camera
ifneq ($(TARGET_USES_QTI_CAMERA),true)
ifeq ($(TARGET_USES_64BIT_CAMERA),true)
PRODUCT_PACKAGES += \
    android.hardware.camera.provider@2.5-impl:64 \
    android.hardware.camera.provider@2.5-service_64
else
PRODUCT_PACKAGES += \
    android.hardware.camera.provider@2.5-impl:32 \
    android.hardware.camera.provider@2.5-service
endif
endif

# External Camera
PRODUCT_PACKAGES += \
    android.hardware.camera.provider@2.5-external \
    android.hardware.camera.provider@2.5-external-service

# External Camera
PRODUCT_PACKAGES += \
    android.hardware.camera.metadata@3.6

# Media
PRODUCT_PACKAGES += \
    android.hardware.media.omx@1.0-service

# Sensorservice
PRODUCT_PACKAGES += \
    android.frameworks.sensorservice@1.0.vendor

# Wi-Fi
PRODUCT_PACKAGES += \
    android.hardware.wifi-service \
    android.hardware.wifi.hostapd@1.0.vendor

# NFC packages
PRODUCT_PACKAGES += \
    android.hardware.nfc-service.nxp

# GNSS
PRODUCT_PACKAGES += \
    android.hardware.gnss-aidl-service-qti

# Light
PRODUCT_PACKAGES += \
    android.hardware.light@2.0-service.sony

# Health
PRODUCT_PACKAGES += \
    android.hardware.health-service.sony

# Vibrator
PRODUCT_PACKAGES += \
    vendor.qti.hardware.vibrator.service

# Fingerprint
PRODUCT_PACKAGES += \
    android.hardware.biometrics.fingerprint@2.1-service.sony

# Gatekeeper passthrough service
PRODUCT_PACKAGES += \
    android.hardware.gatekeeper@1.0-service

# SPU
PRODUCT_PACKAGES += \
    android.hardware.authsecret@1.0.vendor

# DRM
PRODUCT_PACKAGES += \
    android.hardware.drm@1.0-impl \
    android.hardware.drm@1.0-service-lazy \
    android.hardware.drm@1.3-service-lazy.clearkey

# Usb HAL
PRODUCT_PACKAGES += \
    android.hardware.usb@1.2-service-qti

# Thermal HAL
PRODUCT_PACKAGES += \
    android.hardware.thermal@2.0.vendor

# Power
PRODUCT_PACKAGES += \
    android.hardware.power@1.3-service.sony

# Sensors
PRODUCT_PACKAGES += \
    android.hardware.sensors-service.multihal

# Only define bootctrl HAL availability on AB platforms:
ifeq ($(AB_OTA_UPDATER),true)
PRODUCT_PACKAGES += \
    android.hardware.boot-service.qti \
    android.hardware.boot-service.qti.recovery
endif
