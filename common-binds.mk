# Copyright (C) 2014 The Android Open Source Project
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

# xxxx WARNING xxxx
# This makefile copies an empty file in place of various files: this is
# done because we have our nice jars in the ODM partition, but we cannot
# let Android take them from here, because the IMS implementation wants
# us to have the IMS APKs in /product or /system and it is impossible to
# retrieve the dependencies elsewhere because Android disallows namespace
# crossing between the system namespace and the vendor namespace.
#
# We have to do it like this on Android 10, but we plan to bind mount the
# entire ODM system_ext folder to /system_ext on Android 11, which seems
# to be made exactly for this - or a very very similar - purpose (vendor
# component extensions for system namespace).
#
# This, then, is a temporary workaround that will be applied only to
# Android 10 based builds.

# Empty framework jars to bindmount from ODM
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/QtiTelephonyServicelibrary.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/com.qti.dpmframework.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/com.qualcomm.qti.imscmservice-V2.0-java.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/com.qualcomm.qti.imscmservice-V2.1-java.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/com.qualcomm.qti.imscmservice-V2.2-java.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/dpmapi.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/embmslibrary.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/qcrilhook.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/uimgbalibrary.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/uimgbamanagerlibrary.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/uimlpalibrary.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/uimremoteclientlibrary.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/uimremoteserverlibrary.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/uimservicelibrary.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/vendor.qti.data.factory-V1.0-java.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/vendor.qti.data.factory-V2.0-java.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/vendor.qti.hardware.data.connection-V1.0-java.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/vendor.qti.hardware.data.connection-V1.1-java.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/vendor.qti.hardware.data.dynamicdds-V1.0-java.jar \
    $(COMMON_PATH)/misc/emptyfile.zip:$(TARGET_COPY_OUT_PRODUCT)/framework/vendor.qti.ims.rcsconfig-V1.0-java.jar

ifneq ($(TARGET_USES_ODM_APPS_BINDMOUNT),false)
# Create folders for priv-apps
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/misc/emptyfile:$(TARGET_COPY_OUT_PRODUCT)/priv-app/dpmserviceapp/.placeholder \
    $(COMMON_PATH)/misc/emptyfile:$(TARGET_COPY_OUT_PRODUCT)/priv-app/ims/.placeholder

# Create folders for apps
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/misc/emptyfile:$(TARGET_COPY_OUT_PRODUCT)/app/datastatusnotification/.placeholder \
    $(COMMON_PATH)/misc/emptyfile:$(TARGET_COPY_OUT_PRODUCT)/app/embms/.placeholder \
    $(COMMON_PATH)/misc/emptyfile:$(TARGET_COPY_OUT_PRODUCT)/app/uceShimService/.placeholder \
    $(COMMON_PATH)/misc/emptyfile:$(TARGET_COPY_OUT_PRODUCT)/app/uimgbaservice/.placeholder \
    $(COMMON_PATH)/misc/emptyfile:$(TARGET_COPY_OUT_PRODUCT)/app/uimlpaservice/.placeholder \
    $(COMMON_PATH)/misc/emptyfile:$(TARGET_COPY_OUT_PRODUCT)/app/uimremoteclient/.placeholder \
    $(COMMON_PATH)/misc/emptyfile:$(TARGET_COPY_OUT_PRODUCT)/app/uimremoteserver/.placeholder \
    $(COMMON_PATH)/misc/emptyfile:$(TARGET_COPY_OUT_PRODUCT)/app/QtiTelephonyService/.placeholder
endif # TARGET_USES_ODM_APPS_BINDMOUNT true
