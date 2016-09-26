ifneq ($(filter yukon rhine shinano kanuti kitakami loire tone,$(PRODUCT_PLATFORM)),)

LOCAL_PATH := $(call my-dir)

include $(call all-makefiles-under,$(LOCAL_PATH))

endif
