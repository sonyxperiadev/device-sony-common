# Mount points

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := firmware_folders
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)

LOCAL_POST_INSTALL_CMD := mkdir -p \
    $(TARGET_OUT_VENDOR)/dsp \
    $(TARGET_OUT_VENDOR)/firmware_mnt \
    $(TARGET_OUT_VENDOR)/bt_firmware

include $(BUILD_PHONY_PACKAGE)
