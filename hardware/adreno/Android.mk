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
    hw/vulkan.adreno.so

# Create symlinks to 64-bit directory only:
SONY_SYMLINKS := $(foreach p,$(library_names), \
    /odm/lib64/$p:$(TARGET_COPY_OUT_VENDOR)/lib64/$p \
)

include $(SONY_BUILD_SYMLINKS)
