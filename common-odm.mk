# Vendor version
TARGET_VENDOR_VERSION := v1

ifneq (,$(SONY_BUILD_ODM))

PRODUCT_VENDOR_MOVE_ENABLED := true

# Set this flag to true to build the ODM image
PRODUCT_BUILD_ODM_IMAGE := true
TARGET_COPY_OUT_ODM := odm
BOARD_ODMIMAGE_FILE_SYSTEM_TYPE := ext4

PRODUCT_ODM_PROPERTIES += \
    ro.odm.version=$(PLATFORM_VERSION)_$(SOMC_KERNEL_VERSION)_$(SOMC_PLATFORM)_$(TARGET_VENDOR_VERSION)

# Clear device-specific info from platform-level odm image
PRODUCT_PACKAGES += \
    odm_prop_cleaner

# SDE DRM
PRODUCT_PACKAGES += \
    libsdedrm

# Secure MSM
PRODUCT_PACKAGES += \
    libkeymasterutils \
    qseecomd

# IRQ Balance
PRODUCT_PACKAGES += \
    msm_irqbalance

# Peripheral Manager
PRODUCT_PACKAGES += \
    pm-service \
    pm-proxy

# BT
PRODUCT_PACKAGES += \
    libbthost_if

# WiFi
PRODUCT_PACKAGES += \
    cnss-daemon

# Util
PRODUCT_PACKAGES += \
    libsystem_health_mon \
    libxml \
    mlog_qmi_service \
    sct_service

# Remotefs
PRODUCT_PACKAGES += \
    rmt_storage

# GPS
PRODUCT_PACKAGES += \
    liblocdiagiface \
    libdataitems

# Miscta
PRODUCT_PACKAGES += \
    vendor.somc.hardware.miscta@1.0-service \
    vendor.somc.hardware.miscta@1.0 \
    libMiscTaWrapper \
    libmiscta \
    libta \
    ta_qmi_service \
    tad_static

# Audio
PRODUCT_PACKAGES += \
    vendor.qti.hardware.dsp@1.0 \
    liblistenhardware \
    libacdbloader \
    libacdbmapper \
    libacdbrtac \
    libadiertac \
    libadsprpc \
    libaudcal \
    libaudioalsa

# Graphite
PRODUCT_PACKAGES += \
    libgcs-calwrapper \
    libgcs-ipc \
    libgcs-osal \
    libgcs

# Sensors
PRODUCT_PACKAGES += \
    sscrpcd \
    libsnsapi \
    libsensor_reg  \
    libsensor1 \
    sensors.ssc \
    sensors.qcom

# Data
PRODUCT_PACKAGES += \
    libqcmaputils \
    libconfigdb \
    libdiag \
    libdiag_system \
    libdsi_netctrl \
    libdsutils \
    libnetmgr \
    liblqe \
    libqdi \
    libqdp \
    adpl\
    qdi \
    netmgrd

PRODUCT_PACKAGES += \
    pd-mapper \
    libpdmapper \
    libpdnotifier

# QMI
PRODUCT_PACKAGES += \
    irsc_util \
    libidl \
    libsmemlog \
    libqcci_legacy \
    libqmi \
    libqmi_cci \
    libqmi_client_helper \
    libqmi_client_qmux \
    libqmi_common_so \
    libqmi_csi \
    libqmi_encdec \
    libqmiservices \
    libqmiservices_ext \
    libqrtr \
    libqsocket \
    qti \
    qrtr-ns

#EMBMS
PRODUCT_PACKAGES += \
    embms \
    embmslibrary

# IMS
PRODUCT_PACKAGES += \
    qcrilhook \
    IWlanService \
    uceShimService \
    datastatusnotification \
    imsdatadaemon \
    imsqmidaemon \
    imsrcsd \
    ims_rtp_daemon \
    libcneapiclient \
    libcne \
    lib-dplmedia \
    lib-imscmservice \
    lib-imsdpl \
    lib-imsqimf \
    lib-imsrcsbaseimpl \
    lib-imsrcs-v2 \
    lib-imsvtextutils \
    lib-imsvt \
    lib-imsvtutils \
    lib-imsxml \
    lib-rtpcommon \
    lib-rtpcore \
    lib-rtpdaemoninterface \
    lib-rtpsl \
    lib-siputility \
    lib-uceservice \
    com.qualcomm.qti.uceservice@2.0 \
    com.qualcomm.qti.uceservice@2.1 \
    com.qualcomm.qti.uceservice@2.2 \
    com.qualcomm.qti.imscmservice@1.0 \
    com.qualcomm.qti.imscmservice@2.2 \
    vendor.qti.data.mwqem@1.0 \
    vendor.qti.hardware.mwqemadapter@1.0 \
    vendor.qti.data.factory@1.1 \
    vendor.qti.data.slm@1.0 \
    vendor.qti.hardware.data.connection@1.0 \
    vendor.qti.hardware.data.latency@1.0 \
    vendor.qti.hardware.data.dynamicdds@1.0 \
    vendor.qti.hardware.data.qmi@1.0 \
    vendor.qti.hardware.radio.ims@1.0 \
    vendor.qti.ims.callcapability@1.0 \
    vendor.qti.ims.callinfo@1.0 \
    vendor.qti.ims.rcsconfig@1.1 \
    vendor.qti.ims.rcsconfig@2.1 \
    vendor.qti.imsrtpservice@1.0-service-Impl \
    vendor.qti.imsrtpservice@1.0 \
    vendor.qti.latency@2.0 \
    QtiTelephonyService \
    QtiTelephonyServicelibrary \
    uimgbalibrary \
    uimgbamanagerlibrary \
    uimlpalibrary \
    uimremoteclient \
    uimremoteclientlibrary \
    uimremoteserver \
    uimremoteserverlibrary \
    com.qualcomm.qti.imscmservice-V2.0-java \
    com.qualcomm.qti.imscmservice-V2.1-java \
    com.qualcomm.qti.imscmservice-V2.2-java \
    vendor.qti.data.factory-V1.0-java \
    vendor.qti.data.factory-V2.0-java \
    vendor.qti.ims.callinfo-V1.0-java \
    vendor.qti.ims.rcsconfig-V1.0-java \
    vendor.qti.hardware.data.dynamicdds-V1.0-java \
    vendor.qti.hardware.data.connection-V1.1-java \
    vendor.qti.hardware.data.connection-V1.0-java

# DPM
PRODUCT_PACKAGES += \
    dpmapi \
    com.qti.dpmframework

# MDM
PRODUCT_PACKAGES += \
    vendor.qti.hardware.radio.atcmdfwd@1.0 \
    ks \
    mdm_helper \
    ATFWD-daemon

# Modem Switcher
PRODUCT_PACKAGES += \
    sony-modem-switcher \
    vendor.somc.hardware.modemswitcher@1.0-service \
    vendor.somc.hardware.modemswitcher@1.0

ifneq ($(TARGET_KEYMASTER_V4),true)
# Keymaster
PRODUCT_PACKAGES += \
    android.hardware.keymaster@3.0-impl-qti
endif

# Gatekeeper
PRODUCT_PACKAGES += \
    android.hardware.gatekeeper@1.0-impl-qti

# Ril
PRODUCT_PACKAGES += \
    qcrild \
    libril-qc-hal-qmi \
    libril-qc-ltedirectdisc \
    libril-qc-radioconfig \
    libril-qcril-hook-oem \
    librilqmiservices \
    libsettings

# JPEG
PRODUCT_PACKAGES += \
    libfastcvopt \
    libjpegdhw \
    libjpegdmahw \
    libjpegehw \
    libmmjpeg \
    libmmqjpeg_codec \
    libmmqjpegdma \
    libqomx_jpegdec \
    libqomx_jpegenc_pipe \
    libqomx_jpegenc \

endif
