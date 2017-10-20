ifneq ($(filter yukon rhine shinano kanuti kitakami loire tone yoshino,$(PRODUCT_PLATFORM)),)

LOCAL_PATH := $(call my-dir)

# vars for use by utils
empty :=
space := $(empty) $(empty)
colon := $(empty):$(empty)
underscore := $(empty)_$(empty)

# $(call match-word,w1,w2)
# checks if w1 == w2
# How it works
#   if (w1-w2 not empty or w2-w1 not empty) then not_match else match
#
# returns true or empty
#$(warning :$(1): :$(2): :$(subst $(1),,$(2)):) \
#$(warning :$(2): :$(1): :$(subst $(2),,$(1)):) \
#
define match-word
$(strip \
  $(if $(or $(subst $(1),$(empty),$(2)),$(subst $(2),$(empty),$(1))),,true) \
)
endef

# $(call find-word-in-list,w,wlist)
# finds an exact match of word w in word list wlist
#
# How it works
#   fill wlist spaces with colon
#   wrap w with colon
#   search word w in list wl, if found match m, return stripped word w
#
# returns stripped word or empty
define find-word-in-list
$(strip \
  $(eval wl:= $(colon)$(subst $(space),$(colon),$(strip $(2)))$(colon)) \
  $(eval w:= $(colon)$(strip $(1))$(colon)) \
  $(eval m:= $(findstring $(w),$(wl))) \
  $(if $(m),$(1),) \
)
endef

# $(call match-word-in-list,w,wlist)
# does an exact match of word w in word list wlist
# How it works
#   if the input word is not empty
#     return output of an exact match of word w in wordlist wlist
#   else
#     return empty
# returns true or empty
define match-word-in-list
$(strip \
  $(if $(strip $(1)), \
    $(call match-word,$(call find-word-in-list,$(1),$(2)),$(strip $(1))), \
  ) \
)
endef

# $(call is-board-platform-in-list,bpl)
# returns true or empty
define is-board-platform-in-list
$(call match-word-in-list,$(TARGET_BOARD_PLATFORM),$(1))
endef

# $(call is-platform-sdk-version-at-least,version)
# version is a numeric SDK_VERSION defined above
define is-platform-sdk-version-at-least
$(strip \
  $(if $(filter 1,$(shell echo "$$(( $(PLATFORM_SDK_VERSION) >= $(1) ))" )), \
    true, \
  ) \
)
endef

#List of targets that use video hw
MSM_VIDC_TARGET_LIST := msm8226 msm8916 msm8952 msm8974 msm8994 msm8996 msm8998

#List of targets that use master side content protection
MASTER_SIDE_CP_TARGET_LIST := msm8996 msm8998

include $(call all-makefiles-under,$(LOCAL_PATH))

audio-hal := hardware/qcom/audio
gps-hal := hardware/qcom/gps/msm8994

ifeq ($(TARGET_KERNEL_VERSION),3.10)
display-hal := hardware/qcom/display/msm8994
media-hal := hardware/qcom/media/msm8974
endif

ifeq ($(TARGET_KERNEL_VERSION),3.18)
display-hal := hardware/qcom/display/msm8996
media-hal := hardware/qcom/media/msm8996
endif

ifeq ($(TARGET_KERNEL_VERSION),4.4)
display-hal := hardware/qcom/display/msm8998
QCOM_MEDIA_ROOT := hardware/qcom/media/msm8998
OMX_VIDEO_PATH := mm-video-v4l2
media-hal := hardware/qcom/media/msm8998
endif

include $(display-hal)/Android.mk
include $(call all-makefiles-under,$(audio-hal))
include $(call all-makefiles-under,$(gps-hal))
include $(call all-makefiles-under,$(media-hal))

ifeq ($(BOARD_HAVE_BLUETOOTH_QCOM),true)
ifneq ($(filter rhine,$(PRODUCT_PLATFORM)),)
  include $(call all-makefiles-under,hardware/qcom/bt/msm8960)
else
  include $(call all-makefiles-under,hardware/qcom/bt/msm8998)
endif
endif

# EGL libs
$(shell mkdir -p $(PRODUCT_OUT)/system/vendor/lib)
$(shell mkdir -p $(PRODUCT_OUT)/system/vendor/lib/hw)
$(shell mkdir -p $(PRODUCT_OUT)/system/vendor/lib64)
$(shell mkdir -p $(PRODUCT_OUT)/system/vendor/lib64/hw)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/egl egl && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libllvm-glnext.so libllvm-glnext.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libllvm-qgl.so libllvm-qgl.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libc2d30_bltlib.so libc2d30_bltlib.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libc2d30-a3xx.so libc2d30-a3xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libc2d30-a4xx.so libc2d30-a4xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libc2d30-a5xx.so libc2d30-a5xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libmm-qdcm.so libmm-qdcm.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/librs_adreno.so librs_adreno.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libadreno_utils.so libadreno_utils.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libCB.so libCB.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libgsl.so libgsl.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libC2D2.so libC2D2.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libRSDriver_adreno.so libRSDriver_adreno.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libOpenCL.so libOpenCL.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libllvm-qcom.so libllvm-qcom.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/librs_adreno_sha1.so librs_adreno_sha1.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib/hw > /dev/null && ln -s /odm/lib/hw/vulkan.$(TARGET_BOARD_PLATFORM).so vulkan.$(TARGET_BOARD_PLATFORM).so && popd > /dev/null)

$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/egl egl && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libllvm-glnext.so libllvm-glnext.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libllvm-qgl.so libllvm-qgl.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libc2d30_bltlib.so libc2d30_bltlib.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libc2d30-a3xx.so libc2d30-a3xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libc2d30-a4xx.so libc2d30-a4xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libc2d30-a5xx.so libc2d30-a5xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libmm-qdcm.so libmm-qdcm.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/librs_adreno.so librs_adreno.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libadreno_utils.so libadreno_utils.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libCB.so libCB.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libgsl.so libgsl.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libC2D2.so libC2D2.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libRSDriver_adreno.so libRSDriver_adreno.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libOpenCL.so libOpenCL.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libllvm-qcom.so libllvm-qcom.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/librs_adreno_sha1.so librs_adreno_sha1.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64/hw > /dev/null && ln -s /odm/lib64/hw/vulkan.$(TARGET_BOARD_PLATFORM).so vulkan.$(TARGET_BOARD_PLATFORM).so && popd > /dev/null)

# device firmware
$(shell mkdir -p $(PRODUCT_OUT)/odm/firmware)
$(shell pushd $(PRODUCT_OUT)/system/vendor > /dev/null && ln -s /odm/firmware firmware && popd > /dev/null)


ifeq ($(NXP_CHIP_FW_TYPE), PN547C2)
$(shell pushd $(PRODUCT_OUT)/odm/firmware > /dev/null && ln -s /vendor/etc/firmware/libpn547_fw.so libpn547_fw.so && popd > /dev/null)
endif

ifeq ($(NXP_CHIP_FW_TYPE), PN553)
$(shell pushd $(PRODUCT_OUT)/odm/firmware > /dev/null && ln -s /vendor/etc/firmware/libpn553_fw.so libpn553_fw.so && popd > /dev/null)
endif

$(shell pushd $(PRODUCT_OUT)/odm/ > /dev/null && echo "ro.kernel.version=$(TARGET_KERNEL_VERSION)" >>odm_version.prop && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/odm/ > /dev/null && echo "ro.build.version=$(PLATFORM_VERSION)" >>odm_version.prop && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/odm/ > /dev/null && echo "ro.platform.version=$(TARGET_BOARD_PLATFORM)" >>odm_version.prop && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/odm/ > /dev/null && echo "ro.vendor.version=$(TARGET_VENDOR_VERSION)" >>odm_version.prop && popd > /dev/null)

endif
