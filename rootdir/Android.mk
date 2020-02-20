LOCAL_PATH := $(call my-dir)

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

ifeq ($(TARGET_KEYMASTER_V4), true)
include $(CLEAR_VARS)
LOCAL_MODULE := android.hardware.keymaster@4.0-service-qti
LOCAL_SRC_FILES := vendor/etc/init/android.hardware.keymaster@4.0-service-qti.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := android.hardware.keymaster@4.0-service-qti
LOCAL_MODULE_SUFFIX := .rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif # $(TARGET_KEYMASTER_V4) == true

include $(CLEAR_VARS)
LOCAL_MODULE := init.qcom.slpistart.sh
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_SRC_FILES_arm64 := vendor/bin/init.qcom.slpistart.sh
LOCAL_INIT_RC_64  := vendor/etc/init/slpistart.rc
LOCAL_MODULE_TARGET_ARCH := arm64
LOCAL_VENDOR_MODULE := true
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := init.qcom.adspstart.sh
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_SRC_FILES_arm64 := vendor/bin/init.qcom.adspstart.sh
LOCAL_INIT_RC_64  := vendor/etc/init/adspstart.rc
LOCAL_MODULE_TARGET_ARCH := arm64
LOCAL_VENDOR_MODULE := true
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := init.qcom.ipastart.sh
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_SRC_FILES_arm64 := vendor/bin/init.qcom.ipastart.sh
LOCAL_INIT_RC_64 := vendor/etc/init/ipastart.rc
LOCAL_MODULE_TARGET_ARCH := arm64
LOCAL_VENDOR_MODULE := true
include $(BUILD_PREBUILT)

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

include $(CLEAR_VARS)
LOCAL_MODULE := init.usb.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/init.usb.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := adb_tcp.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/adb_tcp.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := adsprpcd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/adsprpcd.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)


include $(CLEAR_VARS)
LOCAL_MODULE := ipacm.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/ipacm.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := irsc_util.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/irsc_util.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := imsdatadaemon.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/imsdatadaemon.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := imsqmidaemon.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/imsqmidaemon.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := imsrcsd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/imsrcsd.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := ims_rtp_daemon.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/ims_rtp_daemon.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := mlog_qmi.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/mlog_qmi.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := msm_irq.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/msm_irq.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := netmgrd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/netmgrd.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := per-proxy.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/per-proxy.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := per-service.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/per-service.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

ifneq ($(filter msm8916 msm8952,$(TARGET_BOARD_PLATFORM)),)
include $(CLEAR_VARS)
LOCAL_MODULE := qmuxd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/qmuxd.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif

include $(CLEAR_VARS)
LOCAL_MODULE := qrtr.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/qrtr.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := adpl.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/adpl.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := qti.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/qti.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := dpmQmiMgr.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/dpmQmiMgr.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := dpmd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/dpmd.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := qseecom.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/qseecom.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := rild2.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/rild2.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := rmt_storage.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/rmt_storage.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := sct_service.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/sct_service.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := sensors.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/sensors.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

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

include $(CLEAR_VARS)
LOCAL_MODULE := ta_qmi.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/ta_qmi.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := wpa_supplicant.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/wpa_supplicant.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

ifneq ($(filter sdm660 msm8998 sdm845 sm8150,$(TARGET_BOARD_PLATFORM)),)
ifneq ($(filter sdm845 sm8150,$(TARGET_BOARD_PLATFORM)),)
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
endif

include $(CLEAR_VARS)
LOCAL_MODULE := pd_mapper.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/pd_mapper.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := sscrpcd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/sscrpcd.rc
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
endif

ifeq ($(WIFI_DRIVER_BUILT),qca_cld3)
include $(CLEAR_VARS)
LOCAL_MODULE := cnss-daemon.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := vendor/etc/init/cnss-daemon.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/init
include $(BUILD_PREBUILT)
endif
