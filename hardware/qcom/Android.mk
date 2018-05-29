# Board platforms lists to be used for
# TARGET_BOARD_PLATFORM specific featurization
QCOM_BOARD_PLATFORMS += msm8916 msm8952 msm8996 msm8998

#List of targets that use video hw
MSM_VIDC_TARGET_LIST := msm8916 msm8952 msm8996 msm8998

#List of targets that use master side content protection
MASTER_SIDE_CP_TARGET_LIST := msm8996 msm8998

audio-hal := hardware/qcom/audio
gps-hal := hardware/qcom/gps/msm8994

ifeq ($(TARGET_KERNEL_VERSION),3.18)
display-hal := hardware/qcom/display/msm8996
media-hal := hardware/qcom/media/msm8996
endif

ifeq ($(TARGET_KERNEL_VERSION),4.4)
QCOM_MEDIA_ROOT := hardware/qcom/media/msm8998
display-hal := hardware/qcom/display/msm8998
media-hal := hardware/qcom/media/msm8998
endif

include device/sony/common/hardware/qcom/utils.mk

include $(display-hal)/Android.mk
include $(call all-makefiles-under,$(audio-hal))
include $(call all-makefiles-under,$(gps-hal))
include $(call all-makefiles-under,$(media-hal))

ifeq ($(BOARD_HAVE_BLUETOOTH_QCOM),true)
  include $(call all-makefiles-under,hardware/qcom/bt/msm8998)
endif

ifeq ($(BOARD_WLAN_DEVICE),qcwcn)
  include $(call all-makefiles-under,hardware/qcom/wlan/qcwcn)
endif
