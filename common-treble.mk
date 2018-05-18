PRODUCT_FULL_TREBLE := true

# RenderScript HAL
PRODUCT_PACKAGES += \
    android.hardware.renderscript@1.0-impl

# Graphics
PRODUCT_PACKAGES += \
    android.hardware.graphics.allocator@2.0-impl \
    android.hardware.graphics.allocator@2.0-service \
    android.hardware.graphics.composer@2.1-impl \
    android.hardware.graphics.composer@2.1-service \
    android.hardware.graphics.mapper@2.0-impl

# Memtrack
PRODUCT_PACKAGES += \
    android.hardware.memtrack@1.0-impl \
    android.hardware.memtrack@1.0-service

# Configstore
PRODUCT_PACKAGES += \
    android.hardware.configstore@1.0-service

# Audio
PRODUCT_PACKAGES += \
    android.hardware.audio@2.0-impl \
    android.hardware.audio@2.0-service \
    android.hardware.audio.effect@2.0-impl \
    android.hardware.soundtrigger@2.0-impl

# Camera
PRODUCT_PACKAGES += \
    android.hardware.camera.provider@2.4-impl \
    android.hardware.camera.provider@2.4-service

# Netutils
PRODUCT_PACKAGES += \
    netutils-wrapper-1.0

# Wi-Fi
PRODUCT_PACKAGES += \
    android.hardware.wifi@1.0-service

# Bluetooth
PRODUCT_PACKAGES += \
    android.hardware.bluetooth@1.0-impl-qti \
    android.hardware.bluetooth@1.0-impl \
    android.hardware.bluetooth@1.0-service

# NFC packages
PRODUCT_PACKAGES += \
    android.hardware.nfc@1.0-impl \
    android.hardware.nfc@1.0-service

# GNSS
PRODUCT_PACKAGES += \
    android.hardware.gnss@1.0-impl-qti \
    android.hardware.gnss@1.0-service-qti

# Light
PRODUCT_PACKAGES += \
    android.hardware.light@2.0-service.sony

# Sensors
PRODUCT_PACKAGES += \
    android.hardware.sensors@1.0-impl \
    android.hardware.sensors@1.0-service

# Vibrator
PRODUCT_PACKAGES += \
    android.hardware.vibrator@1.0-impl \
    android.hardware.vibrator@1.0-service

# Fingerprint
PRODUCT_PACKAGES += \
    android.hardware.biometrics.fingerprint@2.1-service.sony

ifneq ($(TARGET_LEGACY_KEYMASTER),true)
# Keymaster
PRODUCT_PACKAGES += \
    android.hardware.keymaster@3.0-impl-qti \
    android.hardware.keymaster@3.0-service-qti

# Gatekeeper
PRODUCT_PACKAGES += \
    android.hardware.gatekeeper@1.0-impl-qti \
    android.hardware.gatekeeper@1.0-service-qti
else
# Keymaster
PRODUCT_PACKAGES += \
    android.hardware.keymaster@3.0-impl \
    android.hardware.keymaster@3.0-service

# Gatekeeper
PRODUCT_PACKAGES += \
    android.hardware.gatekeeper@1.0-impl \
    android.hardware.gatekeeper@1.0-service
endif

# DRM
PRODUCT_PACKAGES += \
    android.hardware.drm@1.0-impl \
    android.hardware.drm@1.0-service

# Usb HAL
PRODUCT_PACKAGES += \
    android.hardware.usb@1.0-service

# Thermal HAL
PRODUCT_PACKAGES += \
    android.hardware.thermal@1.0-impl \
    android.hardware.thermal@1.0-service

# Power
PRODUCT_PACKAGES += \
    android.hardware.power@1.0-impl \
    android.hardware.power@1.0-service
