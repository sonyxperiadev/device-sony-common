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

# Common init services
PRODUCT_PACKAGES += \
    init.usb.rc \
    adb_tcp.rc \
    adpl.rc \
    adsprpcd.rc \
    audiopd.rc \
    cdsprpcd.rc \
    cnss-daemon.rc \
    ipacm.rc \
    dataqti.rc \
    dpmQmiMgr.rc \
    dpmd.rc \
    imsdatadaemon.rc \
    imsqmidaemon.rc \
    imsrcsd.rc \
    ims_rtp_daemon.rc \
    irsc_util.rc \
    mlog_qmi.rc \
    msm_irq.rc \
    netmgrd.rc \
    pd_mapper.rc \
    per-proxy.rc \
    per-service.rc \
    qmuxd.rc \
    qrtr.rc \
    qseecom.rc \
    rmt_storage.rc \
    sct_service.rc \
    adsp-sensorspdr.rc \
    sdsp-sensorspdr.rc \
    sensors.rc \
    sscrpcd.rc \
    tad.rc \
    ta_qmi.rc \
    tftp_server.rc \
    wpa_supplicant.rc

# RIL
PRODUCT_PACKAGES += \
    qcrild.rc \
    qcrild2.rc

# Common init scripts
PRODUCT_PACKAGES += \
    init.qcom.modem.sh \
    init.qcom.adspstart.sh \
    init.qcom.cdspstart.sh \
    init.qcom.ipastart.sh \
    init.qcom.slpistart.sh

# modemswitcher
PRODUCT_PACKAGES += \
    vendor.somc.hardware.miscta@1.0-service.rc \
    vendor.somc.hardware.modemswitcher@1.0-service.rc \
    init.sony-modem-switcher.rc
