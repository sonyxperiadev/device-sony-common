# TODO: Sort alphabetically
# TODO: Sort out which libs only need to be present as 32/64bit
# TODO: Sort out which HIDL definitions can go

# RenderScript HAL
PRODUCT_PACKAGES += \
    android.hardware.renderscript@1.0-impl

# Graphics
PRODUCT_PACKAGES += \
    android.hardware.graphics.allocator@2.0-impl:64 \
    android.hardware.graphics.allocator@2.0-service \
    android.hardware.graphics.composer@2.1-impl:64 \
    android.hardware.graphics.composer@2.1-service \
    android.hardware.graphics.mapper@2.0-impl

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
    android.hardware.radio.config@1.0

# Audio
PRODUCT_PACKAGES += \
    android.hardware.audio@4.0-impl:32 \
    android.hardware.audio@2.0-service \
    android.hardware.audio.effect@4.0-impl:32 \
    android.hardware.soundtrigger@2.1-impl:32

# Camera
PRODUCT_PACKAGES += \
    android.hardware.camera.provider@2.4-impl:32 \
    android.hardware.camera.provider@2.4-service \
    camera.device@3.4-impl \
    camera.device@3.4-external-impl

# Wi-Fi
PRODUCT_PACKAGES += \
    android.hardware.wifi@1.0-service

# HIDL definitions only for now
PRODUCT_PACKAGES += \
    android.hardware.wifi.hostapd@1.0 \
    android.hardware.wifi.supplicant@1.1 \
    android.hardware.wifi.offload@1.0

# C2 Codecs
PRODUCT_PACKAGES += \
    hardware.google.media.c2@1.0 \
    hardware.google.media.c2@1.0-service \
    libmedia_codecserviceregistrant

# NFC packages
# The 1.1 -impl and -service are in hardware/nxp
PRODUCT_PACKAGES += \
    android.hardware.nfc@1.1-impl \
    android.hardware.nfc@1.1-service

# GNSS
PRODUCT_PACKAGES += \
    android.hardware.gnss@1.1-impl-qti \
    android.hardware.gnss@1.1-service-qti

# Light
PRODUCT_PACKAGES += \
    android.hardware.light@2.0-service.sony

# Health
PRODUCT_PACKAGES += \
    android.hardware.health@2.0-service.sony
# Remove deprecated backup healthd; this can be omitted
# once healthd is no longer included by default.
DEVICE_FRAMEWORK_MANIFEST_FILE += \
    system/libhidl/vintfdata/manifest_healthd_exclude.xml
# health.filesystem will come in Q
#    android.hardware.health.filesystem

# Sensors
PRODUCT_PACKAGES += \
    android.hardware.sensors@1.0-impl:64 \
    android.hardware.sensors@1.0-service

# Vibrator
PRODUCT_PACKAGES += \
    android.hardware.vibrator@1.0-impl \
    android.hardware.vibrator@1.0-service

# Fingerprint
PRODUCT_PACKAGES += \
    android.hardware.biometrics.fingerprint@2.1-service.sony

ifeq ($(TARGET_LEGACY_KEYMASTER),true)
# Keymaster
PRODUCT_PACKAGES += \
    android.hardware.keymaster@3.0-impl \
    android.hardware.keymaster@3.0-service

# Gatekeeper
PRODUCT_PACKAGES += \
    android.hardware.gatekeeper@1.0-impl \
    android.hardware.gatekeeper@1.0-service
else ifeq ($(TARGET_KEYMASTER_V4),true)
# Keymaster 4
PRODUCT_PACKAGES += \
    android.hardware.keymaster@4.0-service-qti

# Gatekeeper
PRODUCT_PACKAGES += \
    android.hardware.gatekeeper@1.0-impl-qti \
    android.hardware.gatekeeper@1.0-service-qti
else
# Keymaster
PRODUCT_PACKAGES += \
    android.hardware.keymaster@3.0-impl-qti \
    android.hardware.keymaster@3.0-service-qti

# Gatekeeper
PRODUCT_PACKAGES += \
    android.hardware.gatekeeper@1.0-impl-qti \
    android.hardware.gatekeeper@1.0-service-qti
endif

# Crypto
PRODUCT_PACKAGES += \
    android.hardware.authsecret@1.0-service

# User warnings
PRODUCT_PACKAGES += \
    android.hardware.confirmationui@1.0-service

# Low-power processing domain
PRODUCT_PACKAGES += \
   android.hardware.contexthub@1.0-impl \
   android.hardware.contexthub@1.0-service

# Bluetooth A2DP offloading
# HIDL definitions only here, -impl is handled in platforms
PRODUCT_PACKAGES += \
    android.hardware.bluetooth.a2dp@1.0

# Dump device-specific state
PRODUCT_PACKAGES += \
    android.hardware.dumpstate@1.0-service

# DRM
PRODUCT_PACKAGES += \
    android.hardware.drm@1.0-impl:32 \
    android.hardware.drm@1.0-service \
    android.hardware.drm@1.1-service.clearkey \
    android.hardware.cas@1.0-service

ifneq ($(BOARD_USE_LEGACY_USB),true)
# Usb HAL
PRODUCT_PACKAGES += \
    android.hardware.usb@1.0-service
else
# Simple Usb HAL
PRODUCT_PACKAGES += \
    android.hardware.usb@1.0-service.basic
endif

# HIDL definitions, HALs are implemented in ipacm blob
PRODUCT_PACKAGES += \
    android.hardware.tetheroffload.config@1.0 \
    android.hardware.tetheroffload.control@1.0

# Thermal HAL
PRODUCT_PACKAGES += \
    android.hardware.thermal@1.0-impl \
    android.hardware.thermal@1.0-service

# Power
PRODUCT_PACKAGES += \
    android.hardware.power@1.3-service.sony

# Power stats will come in Q
#android.hardware.power.stats

ifeq ($(AB_OTA_UPDATER),true)
# Boot control
PRODUCT_PACKAGES += \
    android.hardware.boot@1.0-impl:64 \
    android.hardware.boot@1.0-service
endif
