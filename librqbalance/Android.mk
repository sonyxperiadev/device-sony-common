# Copyright (C) 2012 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)


ifneq ($(TARGET_DEVICE),)
    LOCAL_CFLAGS += -DSOCNAME=\"$(TARGET_DEVICE)\"
endif

ifeq ($(TARGET_USES_64_BIT_BINDER),true)
    LOCAL_CFLAGS += -DIS_64BIT_SYSTEM=1
endif

LOCAL_SRC_FILES := rqbalance_ctl.c
LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    libhardware \
    libutils \
    libbinder \
    libpowermanager

LOCAL_MODULE := librqbalance
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
