# Create symlinks for camera libraries. Currently only applicable
# to sdm845 boards.

LOCAL_PATH := $(call my-dir)

ifneq ($(filter sdm845,$(TARGET_BOARD_PLATFORM)),)

include $(SONY_CLEAR_VARS)
LOCAL_MODULE := camera_symlinks
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)

library_names := \
    camera \
    hw/camera.qcom.so \
    hw/com.qti.chi.override.so \
    libcamxfdalgov7.so \
    libcamxfdengine.so \
    libcamxstatscore.so \
    libcamxtintlessalgo.so \
    libcom.qti.chinodeutils.so

# Create symlinks to 32-bit camera libraries:
SONY_SYMLINKS := $(foreach p,$(library_names), \
    /odm/lib/$p:$(TARGET_COPY_OUT_VENDOR)/lib/$p \
)

# Special exception for camera.qcom.so that is also linked to as camera.sdm845.so:
SONY_SYMLINKS += /odm/lib/hw/camera.qcom.so:$(TARGET_COPY_OUT_VENDOR)/lib/hw/camera.sdm845.so

include $(SONY_BUILD_SYMLINKS)

endif
