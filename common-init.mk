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
    atfwd.rc \
    cnd.rc \
    cnss-daemon.rc \
    dataqti.rc \
    dpmQmiMgr.rc \
    dpmd.rc \
    imsdaemon.rc \
    imsdatadaemon.rc \
    imsqmidaemon.rc \
    imsrcsd.rc \
    ims_rtp_daemon.rc \
    irsc_util.rc \
    mlog_qmi.rc \
    mdm_helper.rc \
    mdmmgr.rc \
    msm_irq.rc \
    mstatdaemon.rc \
    netmgrd.rc \
    pd_mapper.rc \
    per-proxy.rc \
    per-service.rc \
    port-bridge.rc \
    qrtr.rc \
    qmipriod.rc \
    qesdk-manager.rc \
    qseecom.rc \
    rmt_storage.rc \
    sct_service.rc \
    shsusrd.rc \
    sensors.rc \
    vendor.sensors.qesdk.rc \
    ta_qmi.rc \
    tftp_server.rc \
    vendor.qti.diag.rc

# RIL
PRODUCT_PACKAGES += \
    qcrild.rc \
    qcrild2.rc

# Common init scripts
PRODUCT_PACKAGES += \
    init.qcom.adspstart.sh \
    init.qcom.ipastart.sh \
    init.qcom.slpistart.sh

# modemswitcher
PRODUCT_PACKAGES += \
    vendor.somc.hardware.miscta@1.0-service.rc

# IMS/CnE bind-mount - Frameworks and permissions
PRODUCT_PACKAGES += \
    bindmount-misc.rc

# IMS/CnE bind-mount - APK apps/priv-apps
ifneq ($(TARGET_USES_ODM_APPS_BINDMOUNT),false)
PRODUCT_PACKAGES += \
    bindmount-apps.rc
endif

# QTI camera provider
ifeq ($(TARGET_USES_QTI_CAMERA),true)
PRODUCT_PACKAGES += \
    vendor.qti.camera.provider.rc
endif
