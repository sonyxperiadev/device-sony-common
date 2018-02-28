# Board platforms lists to be used for
# TARGET_BOARD_PLATFORM specific featurization
QCOM_BOARD_PLATFORMS += msm8952 msm8996 msm8998 sdm660

#List of targets that use video hw
MSM_VIDC_TARGET_LIST := msm8226 msm8916 msm8952 msm8974 msm8994 msm8996 msm8998 sdm660

#List of targets that use master side content protection
MASTER_SIDE_CP_TARGET_LIST := msm8996 msm8998 sdm660

audio-hal := hardware/qcom/audio
gps-hal := hardware/qcom/gps/sdm845
display-hal := hardware/qcom/display/msm8998
QCOM_MEDIA_ROOT := hardware/qcom/media/msm8998
OMX_VIDEO_PATH := mm-video-v4l2
media-hal := hardware/qcom/media/msm8998

include device/sony/common/hardware/qcom/utils.mk

include $(display-hal)/Android.mk
include $(call all-makefiles-under,$(audio-hal))
include $(call all-makefiles-under,$(gps-hal))
include $(call all-makefiles-under,$(media-hal))

ifeq ($(BOARD_HAVE_BLUETOOTH_QCOM),true)
  include $(call all-makefiles-under,hardware/qcom/bt/msm8998)
endif
