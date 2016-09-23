LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#
# UIM Application
#

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/include

LOCAL_SRC_FILES:= \
    uim.c \
    upio.c \
    brcm_hci_dump.c \
    btsnoop.c \
    utils.c

LOCAL_CLANG := false
LOCAL_CFLAGS:= -c -W -Wall -O2 -D_POSIX_SOURCE -DUIM_DEBUG -DBLUEDROID_ENABLE_V4L2
LOCAL_SHARED_LIBRARIES:= libnetutils libcutils liblog

LOCAL_MODULE := brcm-uim-sysfs
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
