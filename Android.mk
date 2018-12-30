ifeq ($(PRODUCT_PLATFORM_SOD),true)

LOCAL_PATH := $(call my-dir)

include $(call all-subdir-makefiles)

endif
