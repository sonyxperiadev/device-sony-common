# Copyright 2014 The Android Open Source Project
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

CUSTOM_IMAGE_MOUNT_POINT := odm
CUSTOM_IMAGE_PARTITION_SIZE := 419430400
CUSTOM_IMAGE_FILE_SYSTEM_TYPE := ext4
CUSTOM_IMAGE_DICT_FILE := device/sony/common/odm_info.txt
CUSTOM_IMAGE_SELINUX := true

CUSTOM_IMAGE_COPY_FILES := \
			$(foreach p,$(TARGET_OUT_ODM)/odm_version.prop,$(p):) \
			$(foreach p,$(TARGET_OUT_ODM)/bin,$(p):) \
			$(foreach p,$(TARGET_OUT_ODM)/firmware,$(p):) \
			$(foreach p,$(TARGET_OUT_ODM)/lib,$(p):) \
			$(foreach p,$(TARGET_OUT_ODM)/lib64,$(p):)
