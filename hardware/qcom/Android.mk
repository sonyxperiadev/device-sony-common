# Board platforms lists to be used for
# TARGET_BOARD_PLATFORM specific featurization
QCOM_BOARD_PLATFORMS += msm8952 msm8996 msm8998 sdm660 sdm845

#List of targets that use video hw
MSM_VIDC_TARGET_LIST := msm8952 msm8996 msm8998 sdm660 sdm845

#List of targets that use master side content protection
MASTER_SIDE_CP_TARGET_LIST := msm8996 msm8998 sdm660 sdm845

ifeq ($(SOMC_KERNEL_VERSION),4.4)
audio-hal := hardware/qcom/audio
gps-hal := hardware/qcom/gps/sdm845
display-hal := hardware/qcom/display/msm8998
QCOM_MEDIA_ROOT := hardware/qcom/media/msm8998
OMX_VIDEO_PATH := mm-video-v4l2
media-hal := hardware/qcom/media/msm8998
endif

ifeq ($(SOMC_KERNEL_VERSION),4.9)
audio-hal := hardware/qcom/audio
gps-hal := hardware/qcom/gps/sdm845
display-hal := hardware/qcom/display/sde
QCOM_MEDIA_ROOT := hardware/qcom/media/sdm845
OMX_VIDEO_PATH := mm-video-v4l2
media-hal := hardware/qcom/media/sdm845
endif

SRC_CAMERA_HAL_DIR := vendor/qcom/opensource/camera
SRC_DISPLAY_HAL_DIR := $(display-hal)
SRC_MEDIA_HAL_DIR := $(QCOM_MEDIA_ROOT)

include device/sony/common/hardware/qcom/utils.mk

ifneq ($(TARGET_BOARD_PLATFORM),msm8998)
include hardware/qcom/msm8998/json-c/Android.mk
include hardware/qcom/msm8998/time-services/Android.mk
endif

include $(display-hal)/Android.mk
include $(call all-makefiles-under,$(audio-hal))
include $(call all-makefiles-under,$(gps-hal))
include $(call all-makefiles-under,$(media-hal))

ifeq ($(BOARD_HAVE_BLUETOOTH_QCOM),true)
ifeq ($(SOMC_KERNEL_VERSION),4.4)
include $(call all-makefiles-under,hardware/qcom/bt/msm8998)
endif

ifeq ($(SOMC_KERNEL_VERSION),4.9)
include $(call all-makefiles-under,hardware/qcom/bt/sdm845)
endif
endif
