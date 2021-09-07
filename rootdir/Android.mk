LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := init.recovery.$(TARGET_DEVICE)
LOCAL_SRC_FILES := init.recovery.common.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := init.recovery.$(TARGET_DEVICE)
LOCAL_MODULE_SUFFIX := .rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := init.$(TARGET_DEVICE)
LOCAL_SRC_FILES := vendor/etc/init/hw/init.common.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := init.$(TARGET_DEVICE)
LOCAL_MODULE_SUFFIX := .rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init/hw
include $(BUILD_PREBUILT)

ifneq ($(filter sdm845 sm6125 sm8150,$(TARGET_BOARD_PLATFORM)),)
include $(CLEAR_VARS)
LOCAL_MODULE := init.qcom.cdspstart.sh
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_SRC_FILES_arm64 := vendor/bin/init.qcom.cdspstart.sh
LOCAL_INIT_RC_64  := vendor/etc/init/cdspstart.rc
LOCAL_MODULE_TARGET_ARCH := arm64
LOCAL_VENDOR_MODULE := true
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := cdsprpcd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/cdsprpcd.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif # TARGET_BOARD_PLATFORM in (sdm845 sm6125 sm8150)

ifneq ($(filter sdm660 msm8998 sdm845 sm8150,$(TARGET_BOARD_PLATFORM)),)
include $(CLEAR_VARS)
LOCAL_MODULE := sscrpcd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/sscrpcd.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif # TARGET_BOARD_PLATFORM in (sdm660 msm8998 sdm845 sm8150)

ifneq ($(filter sm6125 ,$(TARGET_BOARD_PLATFORM)),)
include $(CLEAR_VARS)
LOCAL_MODULE := hvdcp_opti.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/hvdcp_opti.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif # TARGET_BOARD_PLATFORM in (sm6125)

ifeq ($(WIFI_DRIVER_BUILT),qca_cld3)
include $(CLEAR_VARS)
LOCAL_MODULE := cnss-daemon.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/cnss-daemon.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif
