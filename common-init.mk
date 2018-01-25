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

# Common init scripts
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/rootdir/init.common.rc:root/init.common.rc \
    $(COMMON_PATH)/rootdir/init.common.usb.rc:root/init.common.usb.rc

# Common init services
PRODUCT_PACKAGES += \
    adb_tcp.rc \
    adsprpcd.rc \
    cnss-daemon.rc \
    irsc_util.rc \
    mlog_qmi.rc \
    msm_irq.rc \
    netmgrd.rc \
    pd_mapper.rc \
    per-proxy.rc \
    per-service.rc \
    qmuxd.rc \
    qseecom.rc \
    rild2.rc \
    rmt_storage.rc \
    sct_service.rc \
    sensors.rc \
    tad.rc \
    ta_qmi.rc \
    tftp_server.rc \
    wpa_supplicant.rc \
    xtra.rc
