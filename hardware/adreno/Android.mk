# EGL libs

LOCAL_PATH := $(call my-dir)

include $(SONY_CLEAR_VARS)
LOCAL_MODULE := adreno_symlinks
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)

library_names := \
    egl \
    libC2D2.so \
    libCB.so \
    libOpenCL.so \
    libRSDriver_adreno.so \
    libadreno_utils.so \
    libc2d30_bltlib.so \
    libgsl.so \
    libllvm-glnext.so \
    libllvm-qcom.so \
    librs_adreno.so \
    librs_adreno_sha1.so \
    hw/vulkan.$(TARGET_BOARD_PLATFORM).so

# Create symlinks to 32- and 64-bit directories:
SONY_SYMLINKS := $(foreach lib_dir,lib lib64, \
    $(foreach p,$(library_names), \
        /odm/$(lib_dir)/$p:$(TARGET_COPY_OUT_VENDOR)/$(lib_dir)/$p \
    ) \
)

include $(SONY_BUILD_SYMLINKS)
