# RenderScript HAL
PRODUCT_PACKAGES += \
    android.hardware.renderscript@1.0-impl

# Composer
# Everything prior to kernel 4.19 uses the sm8150 display HAL
ifeq ($(filter 4.14, $(SOMC_KERNEL_VERSION)),)
PRODUCT_PACKAGES += \
    vendor.qti.hardware.display.composer-service
else
PRODUCT_PACKAGES += \
    android.hardware.graphics.composer@2.3-service
endif

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

# Memtrack
PRODUCT_PACKAGES += \
    android.hardware.memtrack@1.0-impl \
    android.hardware.memtrack@1.0-service

# Configstore
PRODUCT_PACKAGES += \
    android.hardware.configstore@1.1-service

# RIL
# Interface library needed by odm blobs:
PRODUCT_PACKAGES += \
    android.hardware.radio@1.6.vendor \
    android.hardware.radio.config@1.3.vendor \
    android.hardware.radio.deprecated@1.0.vendor \
    android.hardware.secure_element@1.2.vendor

# netmgrd
PRODUCT_PACKAGES += \
    android.system.net.netd@1.1.vendor

# Audio
PRODUCT_PACKAGES += \
    android.hardware.audio@6.0-impl:32 \
    android.hardware.audio.service \
    android.hardware.audio.effect@6.0-impl:32 \
    android.hardware.bluetooth.audio@2.0-impl \
    android.hardware.soundtrigger@2.2-impl

# Camera
PRODUCT_PACKAGES += \
    android.frameworks.sensorservice@1.0.vendor
ifeq ($(TARGET_USES_64BIT_CAMERA),true)
PRODUCT_PACKAGES += \
    android.hardware.camera.provider@2.4-impl:64 \
    android.hardware.camera.provider@2.4-service_64
else
PRODUCT_PACKAGES += \
    android.hardware.camera.provider@2.4-impl:32 \
    android.hardware.camera.provider@2.4-service
endif

# Wi-Fi
PRODUCT_PACKAGES += \
    android.hardware.wifi@1.0-service

# NFC packages
PRODUCT_PACKAGES += \
    android.hardware.nfc@1.2-service

# GNSS
PRODUCT_PACKAGES += \
    android.hardware.gnss@2.1-impl-qti \
    android.hardware.gnss@2.1-service-qti

# Light
PRODUCT_PACKAGES += \
    android.hardware.light@2.0-service.sony

# Health
PRODUCT_PACKAGES += \
    android.hardware.health@2.0-service.sony

ifeq ($(TARGET_VIBRATOR_V1_2),true)
# QTI Haptics Vibrator
PRODUCT_PACKAGES += \
    vendor.qti.hardware.vibrator@1.2-service
else
# Vibrator
PRODUCT_PACKAGES += \
    android.hardware.vibrator@1.0-impl \
    android.hardware.vibrator@1.0-service
endif

# Fingerprint
PRODUCT_PACKAGES += \
    android.hardware.biometrics.fingerprint@2.1-service.sony

# Gatekeeper passthrough service
PRODUCT_PACKAGES += \
    android.hardware.gatekeeper@1.0-service

ifeq ($(TARGET_KEYMASTER_V4),true)
# Keymaster 4 passthrough service init file
# (executable is on odm)
PRODUCT_PACKAGES += \
    android.hardware.keymaster@4.0-service-qti.rc \
    android.hardware.keymaster@4.1.vendor
else
# Keymaster 3 passthrough service
PRODUCT_PACKAGES += \
    android.hardware.keymaster@3.0-service
endif

# SPU
PRODUCT_PACKAGES += \
    android.hardware.authsecret@1.0.vendor

# DRM
PRODUCT_PACKAGES += \
    android.hardware.drm@1.0-impl \
    android.hardware.drm@1.0-service-lazy \
    android.hardware.drm@1.3-service-lazy.clearkey

# Usb HAL
ifeq ($(filter 4.14, $(SOMC_KERNEL_VERSION)),)
PRODUCT_PACKAGES += \
    android.hardware.usb@1.2-service-qti
else
PRODUCT_PACKAGES += \
    android.hardware.usb@1.0-service
endif

# Thermal HAL
PRODUCT_PACKAGES += \
    android.hardware.thermal@1.0-impl \
    android.hardware.thermal@1.0-service

# Power
PRODUCT_PACKAGES += \
    android.hardware.power@1.3-service.sony

# Only define bootctrl HAL availability on AB platforms:
ifeq ($(AB_OTA_UPDATER),true)
PRODUCT_PACKAGES += \
    android.hardware.boot@1.1-impl-qti \
    android.hardware.boot@1.1-impl-qti.recovery \
    android.hardware.boot@1.1-service
endif
