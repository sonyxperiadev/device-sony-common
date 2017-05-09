ifneq ($(filter yukon rhine shinano kanuti kitakami loire tone,$(PRODUCT_PLATFORM)),)

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
MSM_VIDC_TARGET_LIST := msm8226 msm8916 msm8952 msm8974 msm8994 msm8996

#List of targets that use master side content protection
MASTER_SIDE_CP_TARGET_LIST := msm8996

include $(call all-makefiles-under,$(LOCAL_PATH))

# Platform common HALs
audio-hal := hardware/qcom/audio

# Platform specific HALs
ifeq ($(TARGET_BOARD_PLATFORM),msm8974)
  display-hal := hardware/qcom/display/msm8994
  gps-hal := hardware/qcom/gps/msm8994
  media-hal := hardware/qcom/media/msm8994
else
  display-hal := hardware/qcom/display/msm8996
  gps-hal := hardware/qcom/gps/msm8994
  media-hal := hardware/qcom/media/msm8996
endif

include $(display-hal)/Android.mk
include $(call all-makefiles-under,$(audio-hal))
include $(call all-makefiles-under,$(gps-hal))
include $(call all-makefiles-under,$(media-hal))

ifeq ($(BOARD_HAVE_BLUETOOTH_QCOM),true)
ifneq ($(filter rhine,$(PRODUCT_PLATFORM)),)
  include $(call all-makefiles-under,hardware/qcom/bt/msm8960)
else
  include $(call all-makefiles-under,hardware/qcom/bt/msm8992)
endif
endif
ifeq ($(BOARD_WLAN_DEVICE),qcwcn)
  include $(call all-makefiles-under,hardware/qcom/wlan/qcwcn)
endif

endif
