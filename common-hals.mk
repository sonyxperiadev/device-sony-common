# Display
PRODUCT_PACKAGES += \
    mapper.qti \
    vendor.qti.hardware.display.allocator-service \
    vendor.qti.hardware.display.composer-service \
    vendor.qti.hardware.display.demura-service \
    vendor.qti.hardware.display.snapalloc-impl

# Linked by Adreno/EGL blobs for fallback if 3.0 doesn't exist
PRODUCT_PACKAGES += \
    vendor.qti.hardware.display.mapper@2.0.vendor

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

# Audioreach
PRODUCT_PACKAGES += \
    audiohalservice.qti

# External Camera
# TODO: Enable this once external_camera_config.xml is provided.
# The config contains the IDs of internal video devices to be ignored,
# so it will most likely need to be included in each device tree,
# since the IDs differ between devices.
#PRODUCT_PACKAGES += \
#    android.hardware.camera.provider-V1-external-service

# QTI Camera
PRODUCT_PACKAGES += \
    android.hardware.camera.device@3.7.vendor \
    android.hardware.camera.metadata@3.6.vendor \
    android.hardware.camera.provider@2.7.vendor

# Media
PRODUCT_PACKAGES += \
    android.hardware.media.omx@1.0-service \
    libcodec2_hidl@1.0.vendor

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

# Health
PRODUCT_PACKAGES += \
    android.hardware.health-service.sony

# Vibrator
PRODUCT_PACKAGES += \
    vendor.qti.hardware.vibrator.service

# Fingerprint
ifneq ($(TARGET_DEVICE_NO_FPC), true)
PRODUCT_PACKAGES += \
    android.hardware.biometrics.fingerprint@2.1-service.sony
endif

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
    android.hardware.usb-service.qti \
    android.hardware.usb.gadget-service.qti

# Thermal HAL
PRODUCT_PACKAGES += \
    android.hardware.thermal@2.0.vendor

# Power
PRODUCT_PACKAGES += \
    android.hardware.power-service

# Sensors
PRODUCT_PACKAGES += \
    android.hardware.sensors-service.multihal

# Boot control
PRODUCT_PACKAGES += \
    android.hardware.boot-service.qti \
    android.hardware.boot-service.qti.recovery
