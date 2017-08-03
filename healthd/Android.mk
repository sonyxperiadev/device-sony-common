LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := healthd_board_sony.cpp
LOCAL_MODULE := libhealthd.$(TARGET_DEVICE)
LOCAL_CFLAGS := -Werror
LOCAL_C_INCLUDES := \
    system/core/healthd/include \
    system/core/base/include \
    bootable/recovery/minui/include \
    bootable/recovery

include $(BUILD_STATIC_LIBRARY)
