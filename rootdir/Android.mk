LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_KEYMASTER_V4), true)
include $(CLEAR_VARS)
LOCAL_MODULE := android.hardware.keymaster@4.0-service-qti.rc
LOCAL_SRC_FILES := vendor/etc/init/android.hardware.keymaster@4.0-service-qti.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif # $(TARGET_KEYMASTER_V4) == true

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

ifeq ($(TARGET_NEEDS_AUDIOPD), true)
include $(CLEAR_VARS)
LOCAL_MODULE := audiopd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/audiopd.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif

# Note: Sensors PDR can be on ADSP **or** on SDSP
ifeq ($(TARGET_NEEDS_ADSP_SENSORS_PDR), true)
 # Let's error out if the developer makes an impossible choice
 ifeq ($(TARGET_NEEDS_SDSP_SENSORS_PDR), true)
  $(error Activating both ADSP and SDSP Sensors PDR is wrong!!!)
 endif

include $(CLEAR_VARS)
LOCAL_MODULE := adsp-sensorspdr.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/adsp-sensorspdr.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

else ifeq ($(TARGET_NEEDS_SDSP_SENSORS_PDR), true)
include $(CLEAR_VARS)
LOCAL_MODULE := sdsp-sensorspdr.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/sdsp-sensorspdr.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif

ifneq ($(filter msm8952,$(TARGET_BOARD_PLATFORM)),)
include $(CLEAR_VARS)
LOCAL_MODULE := qmuxd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/qmuxd.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif

ifeq ($(TARGET_USES_TAD_V2), true)
include $(CLEAR_VARS)
LOCAL_MODULE := tad.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/tad2.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
else
include $(CLEAR_VARS)
LOCAL_MODULE := tad.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/tad.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif

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

ifneq ($(filter sdm660 msm8998 sdm845 sm6125 sm8150,$(TARGET_BOARD_PLATFORM)),)
include $(CLEAR_VARS)
LOCAL_MODULE := pd_mapper.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/pd_mapper.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := tftp_server.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/tftp_server.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif # TARGET_BOARD_PLATFORM in (sdm660 msm8998 sdm845 sm6125 sm8150)

ifeq ($(WIFI_DRIVER_BUILT),qca_cld3)
include $(CLEAR_VARS)
LOCAL_MODULE := cnss-daemon.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/cnss-daemon.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif
