# Create symlinks for rfsa libraries.

LOCAL_PATH := $(call my-dir)

include $(SONY_CLEAR_VARS)
LOCAL_MODULE := rfsa_symlinks
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)

library_names := \
    rfsa

# Create symlinks to 64-bit rfsa libraries:
# Default DSP search path is: /vendor/lib/rfsa/adsp
# TODO: Newer platforms will likely require a /vendor/lib64/rfs/dsp symlink
SONY_SYMLINKS := $(foreach p,$(library_names), \
    /odm/lib/$p:$(TARGET_COPY_OUT_VENDOR)/lib/$p \
)

include $(SONY_BUILD_SYMLINKS)
