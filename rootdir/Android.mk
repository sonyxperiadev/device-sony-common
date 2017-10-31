LOCAL_PATH := $(call my-dir)

ifneq (,$(filter loire tone yoshino,$(PRODUCT_PLATFORM)))
ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
include $(CLEAR_VARS)
LOCAL_MODULE             := rdclean.sh
LOCAL_MODULE_CLASS       := EXECUTABLES
LOCAL_SRC_FILES_arm64    := vendor/bin/rdclean.sh
LOCAL_INIT_RC_64         := vendor/etc/init/rdclean.rc
LOCAL_MODULE_TARGET_ARCH := arm64
LOCAL_VENDOR_MODULE      := true
include $(BUILD_PREBUILT)
endif
endif
