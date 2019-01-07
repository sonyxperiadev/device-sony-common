LOCAL_PATH := $(call my-dir)

ifeq ($(WIFI_DRIVER_BUILT),qca_cld3)

include $(SONY_CLEAR_VARS)
LOCAL_MODULE := qca_cld3_symlinks
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)

SONY_SYMLINKS := /data/vendor/wifi/wlan_mac.bin:$(TARGET_COPY_OUT_VENDOR)/firmware/wlan/qca_cld/wlan_mac.bin

include $(SONY_BUILD_SYMLINKS)

endif
