LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := odm_build_prop_version
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)

LOCAL_POST_INSTALL_CMD := echo \
  "ro.odm.version=$(PLATFORM_VERSION)_$(SOMC_KERNEL_VERSION)_$(SOMC_PLATFORM)_$(TARGET_VENDOR_VERSION)" \
  >$(TARGET_OUT_ODM)/build.prop

include $(BUILD_PHONY_PACKAGE)
