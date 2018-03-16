ifeq ($(PRODUCT_PLATFORM_SOD),true)

LOCAL_PATH := $(call my-dir)

include $(call all-makefiles-under,$(LOCAL_PATH))

ifneq ($(TARGET_BOARD_PLATFORM),msm8998)
  include hardware/qcom/msm8998/Android.mk
endif

endif
