# RenderScript HAL
PRODUCT_PACKAGES += \
    android.hardware.renderscript@1.0-impl

# Composer
ifneq ($(filter sm8250,$(TARGET_BOARD_PLATFORM)),)
PRODUCT_PACKAGES += \
    vendor.qti.hardware.display.composer-service
else
PRODUCT_PACKAGES += \
    android.hardware.graphics.composer@2.3-impl:64 \
    android.hardware.graphics.composer@2.3-service
endif

# Graphics allocator/mapper
ifeq ($(TARGET_HARDWARE_GRAPHICS_V3),true)

PRODUCT_PACKAGES += \
    android.hardware.graphics.mapper@3.0-impl-qti-display \
    android.hardware.graphics.mapper@4.0-impl-qti-display

# android.hardware.graphics.allocator@3.0::IAllocator, and
# android.hardware.graphics.allocator@4.0::IAllocator if
# TARGET_USES_GRALLOC4 is not explicitly set to `false`:
PRODUCT_PACKAGES += \
    vendor.qti.hardware.display.allocator-service

else

PRODUCT_PACKAGES += \
    android.hardware.graphics.allocator@2.0-impl:64 \
    android.hardware.graphics.allocator@2.0-service \
    android.hardware.graphics.mapper@2.0-impl

endif

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
    android.hardware.radio@1.4 \
    android.hardware.radio.config@1.2

# Audio
PRODUCT_PACKAGES += \
    android.hardware.audio@6.0-impl:32 \
    android.hardware.audio.service \
    android.hardware.audio.effect@6.0-impl:32 \
    android.hardware.bluetooth.audio@2.0-impl \
    android.hardware.soundtrigger@2.2-impl

# Camera
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

# Sensors
# hardware.ssc.so links against display mappers, of which
# the interface libraries are explicitly included here:
PRODUCT_PACKAGES += \
    android.hardware.sensors@2.1-service.multihal \
    vendor.qti.hardware.display.mapper@1.1.vendor \
    vendor.qti.hardware.display.mapper@3.0.vendor

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
    android.hardware.keymaster@4.0-service-qti.rc
else
# Keymaster 3 passthrough service
PRODUCT_PACKAGES += \
    android.hardware.keymaster@3.0-service
endif

# DRM
PRODUCT_PACKAGES += \
    android.hardware.drm@1.0-impl \
    android.hardware.drm@1.0-service-lazy \
    android.hardware.drm@1.3-service-lazy.clearkey

ifneq ($(BOARD_USE_LEGACY_USB),true)
# Usb HAL
PRODUCT_PACKAGES += \
    android.hardware.usb@1.0-service
else
# Simple Usb HAL
PRODUCT_PACKAGES += \
    android.hardware.usb@1.0-service.basic
endif

# Thermal HAL
PRODUCT_PACKAGES += \
    android.hardware.thermal@1.0-impl \
    android.hardware.thermal@1.0-service

# Power
PRODUCT_PACKAGES += \
    android.hardware.power@1.3-service.sony

ifeq ($(AB_OTA_UPDATER),true)
# Boot control
PRODUCT_PACKAGES += \
    android.hardware.boot@1.0-impl \
    android.hardware.boot@1.0-impl.recovery \
    android.hardware.boot@1.0-service
endif
