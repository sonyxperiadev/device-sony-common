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

ifneq (,$(filter 4.4,$(TARGET_KERNEL_VERSION)))
include $(CLEAR_VARS)
LOCAL_MODULE := init.recovery.$(TARGET_DEVICE)
LOCAL_SRC_FILES := init.recovery.common.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := init.recovery.$(TARGET_DEVICE)
LOCAL_MODULE_SUFFIX := .rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)
endif
