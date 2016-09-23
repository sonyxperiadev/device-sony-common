ifeq ($(BOARD_HAVE_BCM_FM), true)

LOCAL_PATH := $(call my-dir)

include $(call all-makefiles-under,$(LOCAL_PATH))

endif
