ifneq ($(filter yukon rhine shinano kanuti kitakami loire tone,$(PRODUCT_PLATFORM)),)

LOCAL_PATH := $(call my-dir)

#List of targets that use video hw
MSM_VIDC_TARGET_LIST := msm8226 msm8916 msm8952 msm8974 msm8994 msm8996

#List of targets that use master side content protection
MASTER_SIDE_CP_TARGET_LIST := msm8996

include $(call all-makefiles-under,$(LOCAL_PATH))

endif
