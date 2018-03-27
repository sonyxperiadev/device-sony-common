LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true
ifeq ($(TARGET_COMPILE_WITH_MSM_KERNEL),true)
LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
LOCAL_ADDITIONAL_DEPENDENCIES := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr
endif
LOCAL_MODULE := android.hardware.light@2.0-service.sony
LOCAL_INIT_RC := android.hardware.light@2.0-service.sony.rc
LOCAL_SRC_FILES := \
    service.cpp \
    Light.cpp

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libdl \
    libbase \
    libutils \
    libhardware \
    libhidlbase \
    libhidltransport \
    android.hardware.light@2.0

include $(BUILD_EXECUTABLE)
