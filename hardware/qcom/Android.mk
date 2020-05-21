ifneq ($(wildcard device/sony/common/hardware/qcom/custom.mk),)
    include device/sony/common/hardware/qcom/custom.mk
else
# Board platforms lists to be used for
# TARGET_BOARD_PLATFORM specific featurization
QCOM_BOARD_PLATFORMS += msm8952 msm8996 msm8998 sdm660 sdm845 sm8150

# Some supported platforms need a different media hal
# This list selects platforms that should use the latest media hal
# All other platforms automatically fallback to the legacy hal
QCOM_NEW_MEDIA_PLATFORM := sdm845 sm8150

#List of targets that use video hw
MSM_VIDC_TARGET_LIST := msm8952 msm8996 msm8998 sdm660 sdm845 sm8150

#List of targets that use master side content protection
MASTER_SIDE_CP_TARGET_LIST := msm8996 msm8998 sdm660 sdm845 sm8150

audio-hal := hardware/qcom/audio
gps-hal := hardware/qcom/gps/sdm845
ipa-hal := vendor/qcom/opensource/data/ipacfg-mgr/sdm845

display-hal := vendor/qcom/opensource/display

ifneq ($(filter $(QCOM_NEW_MEDIA_PLATFORM), $(TARGET_BOARD_PLATFORM)),)
QCOM_MEDIA_ROOT := vendor/qcom/opensource/media/sm8150
else
QCOM_MEDIA_ROOT := vendor/qcom/opensource/media/sdm660-libion
endif

OMX_VIDEO_PATH := mm-video-v4l2
media-hal := $(QCOM_MEDIA_ROOT)

SRC_CAMERA_HAL_DIR ?= vendor/qcom/opensource/camera
SRC_DISPLAY_HAL_DIR := $(display-hal)
SRC_MEDIA_HAL_DIR := $(QCOM_MEDIA_ROOT)
TARGET_KERNEL_VERSION := $(SOMC_KERNEL_VERSION)

include device/sony/common/hardware/qcom/utils.mk

include $(call all-makefiles-under,$(audio-hal))
include $(call all-makefiles-under,$(gps-hal))
endif
