# Remove device-specific props from odm

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := odm_prop_cleaner
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)

LOCAL_POST_INSTALL_CMD := sed -i '/^ro.odm.version/!d' $(TARGET_OUT_ODM)/etc/build.prop

include $(BUILD_PHONY_PACKAGE)
