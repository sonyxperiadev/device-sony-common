LOCAL_PATH := $(call my-dir)

ifneq ($(filter sdm660 msm8998 sdm845,$(TARGET_BOARD_PLATFORM)),)

include $(SONY_CLEAR_VARS)
LOCAL_MODULE := tftp_symlinks
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)

target_combinations := \
    /persist/rfs/shared:shared \
    /persist/hlos_rfs/shared:hlos \
    /firmware:readonly/firmware \
    /odm/firmware:readonly/vendor/firmware

target_prefixes := \
    mpss \
    adsp \
    slpi \
    cdsp

# Prepend vendor and prefix directory to all link names:
SONY_SYMLINKS := $(foreach prefix,$(target_prefixes), \
    $(foreach s,$(target_combinations), \
        $(eval p := $(subst :,$(space),$(s))) \
        $(word 1,$(p)):$(TARGET_COPY_OUT_VENDOR)/rfs/msm/$(prefix)/$(word 2,$(p)) \
    ) \
)

# Edgecase for readwrite folders that all point to their own persist folder:
SONY_SYMLINKS += $(foreach prefix,$(target_prefixes), \
    /persist/rfs/msm/$(prefix):$(TARGET_COPY_OUT_VENDOR)/rfs/msm/$(prefix)/readwrite \
)

# Edgecase for tombstone folders that do not follow the above pattern:
SONY_SYMLINKS += \
    /data/vendor/tombstones/modem:$(TARGET_COPY_OUT_VENDOR)/rfs/msm/mpss/ramdumps \
    /data/vendor/tombstones/lpass:$(TARGET_COPY_OUT_VENDOR)/rfs/msm/adsp/ramdumps \
    /data/vendor/tombstones/rfs/cdsp:$(TARGET_COPY_OUT_VENDOR)/rfs/msm/cdsp/ramdumps \
    /data/vendor/tombstones/rfs/slpi:$(TARGET_COPY_OUT_VENDOR)/rfs/msm/slpi/ramdumps

include $(SONY_BUILD_SYMLINKS)

endif
