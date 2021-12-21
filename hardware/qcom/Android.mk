ifneq ($(wildcard device/sony/common/hardware/qcom/custom.mk),)
    include device/sony/common/hardware/qcom/custom.mk
else
# Board platforms lists to be used for
# TARGET_BOARD_PLATFORM specific featurization
QCOM_BOARD_PLATFORMS += sdm660 sdm845 sm6125 sm6350 sm8150 sm8250 sm8350

# Some supported platforms need a different media hal
# This list selects platforms that should use the latest media hal
# All other platforms automatically fallback to the legacy hal
QCOM_NEW_MEDIA_PLATFORM := sdm845 sm6125 sm6350 sm8150 sm8250 sm8350

#List of targets that use video hw
MSM_VIDC_TARGET_LIST := sdm660 sdm845 sm6125 sm6350 sm8150 sm8250 sm8350

#List of targets that use master side content protection
MASTER_SIDE_CP_TARGET_LIST := sdm660 sdm845 sm6125 sm6350 sm8150 sm8250 sm8350

ifneq ($(filter $(QCOM_NEW_MEDIA_PLATFORM), $(TARGET_BOARD_PLATFORM)),)
QCOM_MEDIA_ROOT := vendor/qcom/opensource/media/sm8250
else
QCOM_MEDIA_ROOT := vendor/qcom/opensource/media/sdm660-libion
endif

OMX_VIDEO_PATH := mm-video-v4l2

SRC_CAMERA_HAL_DIR ?= vendor/qcom/opensource/camera
SRC_DISPLAY_HAL_DIR := vendor/qcom/opensource/display/sm8250
SRC_MEDIA_HAL_DIR := $(QCOM_MEDIA_ROOT)
TARGET_KERNEL_VERSION := $(SOMC_KERNEL_VERSION)

include device/sony/common/hardware/qcom/utils.mk

ifeq ($(TARGET_USES_AOSP_AUDIO_HAL),true)
audio-hal := hardware/qcom/audio
include $(call all-makefiles-under,$(audio-hal))
endif

endif
