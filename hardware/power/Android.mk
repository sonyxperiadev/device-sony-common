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


ifneq ($(TARGET_TAP_TO_WAKE_NODE),)
    LOCAL_CFLAGS += -DTAP_TO_WAKE_NODE=\"$(TARGET_TAP_TO_WAKE_NODE)\"
endif
LOCAL_HEADER_LIBRARIES := libhardware_headers

LOCAL_C_INCLUDES := external/expat/lib

LOCAL_MODULE := android.hardware.power@1.3-service.sony
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true
LOCAL_INIT_RC := android.hardware.power@1.3-service.sony.rc

# PowerHAL and HALExtension
LOCAL_SRC_FILES := \
    main.cpp \
    Power.cpp \
    Hints.cpp \
    RQBalanceHALExt.cpp \
    expatparser.c \
    power-helper.c

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    libexpat \
    libdl \
    libbase \
    libutils \
    libhardware \
    libhidlbase \
    android.hardware.power@1.3

include $(BUILD_EXECUTABLE)

