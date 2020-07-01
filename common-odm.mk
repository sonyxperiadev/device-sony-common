# Vendor version
TARGET_VENDOR_VERSION := v8

ifneq (,$(SONY_BUILD_ODM))

# Set this flag to true to build the ODM image
PRODUCT_BUILD_ODM_IMAGE := true
TARGET_COPY_OUT_ODM := odm
BOARD_ODMIMAGE_FILE_SYSTEM_TYPE := ext4

PRODUCT_ODM_PROPERTIES += \
    ro.odm.version=$(PLATFORM_VERSION)_$(SOMC_KERNEL_VERSION)_$(SOMC_PLATFORM)_$(TARGET_VENDOR_VERSION)

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
    qmuxd \
    qti \
    qrtr-ns

#EMBMS
PRODUCT_PACKAGES += \
    embms \
    embmslibrary

# IMS
PRODUCT_PACKAGES += \
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
    com.qualcomm.qti.uceservice@2.1 \
    com.qualcomm.qti.imscmservice@1.0 \
    com.qualcomm.qti.imscmservice@2.2 \
    vendor.qti.data.factory@1.1 \
    vendor.qti.data.slm@1.0 \
    vendor.qti.hardware.data.connection@1.0 \
    vendor.qti.hardware.data.dynamicdds@1.0 \
    vendor.qti.hardware.data.qmi@1.0 \
    vendor.qti.hardware.radio.ims@1.0 \
    vendor.qti.ims.callinfo@1.0 \
    vendor.qti.ims.rcsconfig@1.1 \
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

# Modem Switcher
PRODUCT_PACKAGES += \
    sony-modem-switcher \
    vendor.somc.hardware.modemswitcher@1.0-service \
    vendor.somc.hardware.modemswitcher@1.0

ifneq ($(TARGET_LEGACY_KEYMASTER),true)
ifneq ($(TARGET_KEYMASTER_V4),true)
# Keymaster
PRODUCT_PACKAGES += \
    android.hardware.keymaster@3.0-impl-qti
endif
# Gatekeeper
PRODUCT_PACKAGES += \
    android.hardware.gatekeeper@1.0-impl-qti
endif

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

# MMCamera Framework
ifneq ($(filter loire tone yoshino nile ganges seine,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libflash_pmic \
    libmmcamera_dbg \
    libmmcamera_chromaflash_lib \
    libmmcamera_dummyalgo \
    libmmcamera_eebinparse \
    libmmcamera_eeprom_util \
    libmmcamera_eztune_module \
    libmmcamera_facedetection_lib \
    libmmcamera_faceproc \
    libmmcamera_faceproc2 \
    libmmcamera_hdr_gb_lib \
    libmmcamera_hvx_add_constant \
    libmmcamera_hvx_grid_sum \
    libmmcamera_hvx_zzHDR.so \
    libmmcamera_imglib \
    libmmcamera_imglib_faceproc_adspstub \
    libmmcamera_imglib_faceproc_adspstub2 \
    libmmcamera_llvd \
    libmmcamera_optizoom_lib \
    libmmcamera_paaf_lib \
    libmmcamera_pdaf \
    libmmcamera_pdafcamif \
    libmmcamera_ppeiscore \
    libmmcamera_ppbase_module \
    libmmcamera_quadracfa \
    libmmcamera_stillmore_lib \
    libmmcamera_sw_tnr \
    libmmcamera_sw2d_lib \
    libmmcamera_thread_services \
    libmmcamera_tintless_algo \
    libmmcamera_tintless_bg_pca_algo \
    libmmcamera_trueportrait_lib \
    libmmcamera_tuning \
    libmmcamera_tuning_lookup

# Camera2 Framework
PRODUCT_PACKAGES += \
    libmmcamera2_c2d_module \
    libmmcamera2_cpp_module \
    libmmcamera2_frame_algorithm \
    libmmcamera2_iface_modules \
    libmmcamera2_imglib_modules \
    libmmcamera2_is \
    libmmcamera2_isp_modules \
    libmmcamera2_mct \
    libmmcamera2_mct_shimlayer \
    libmmcamera2_pp_buf_mgr \
    libmmcamera2_pproc_modules \
    libmmcamera2_q3a_core \
    libmmcamera2_q3a_release \
    libmmcamera2_sensor_modules \
    libmmcamera2_stats_algorithm \
    libmmcamera2_stats_lib \
    libmmcamera2_stats_modules

# Camera ISP
PRODUCT_PACKAGES += \
    libmmcamera_isp_abcc44 \
    libmmcamera_isp_abf40 \
    libmmcamera_isp_abf47 \
    libmmcamera_isp_abf48 \
    libmmcamera_isp_bcc40 \
    libmmcamera_isp_bpc40 \
    libmmcamera_isp_bpc47 \
    libmmcamera_isp_bpc48 \
    libmmcamera_isp_scaler_encoder44 \
    libmmcamera_isp_scaler_encoder46 \
    libmmcamera_isp_hdr46 \
    libmmcamera_isp_hdr48 \
    libmmcamera_isp_linearization40 \
    libmmcamera_isp_gic46 \
    libmmcamera_isp_gic48 \
    libmmcamera_isp_demosaic40 \
    libmmcamera_isp_demosaic47 \
    libmmcamera_isp_demosaic48 \
    libmmcamera_isp_gtm46 \
    libmmcamera_isp_clamp_video40 \
    libmmcamera_isp_aec_bg_stats47 \
    libmmcamera_isp_snr47 \
    libmmcamera_isp_color_correct40 \
    libmmcamera_isp_color_correct46 \
    libmmcamera_isp_color_xform_viewfinder40 \
    libmmcamera_isp_color_xform_viewfinder46 \
    libmmcamera_isp_bf_stats47 \
    libmmcamera_isp_be_stats44 \
    libmmcamera_isp_hdr_be_stats46 \
    libmmcamera_isp_mesh_rolloff40 \
    libmmcamera_isp_mesh_rolloff44 \
    libmmcamera_isp_template \
    libmmcamera_isp_color_xform_encoder40 \
    libmmcamera_isp_color_xform_video46 \
    libmmcamera_isp_color_xform_encoder46 \
    libmmcamera_isp_pdaf48 \
    libmmcamera_isp_hdr_bhist_stats44 \
    libmmcamera_isp_bg_stats44 \
    libmmcamera_isp_bg_stats46 \
    libmmcamera_isp_luma_adaptation40 \
    libmmcamera_isp_ihist_stats44 \
    libmmcamera_isp_ihist_stats46 \
    libmmcamera_isp_rs_stats44 \
    libmmcamera_isp_rs_stats46 \
    libmmcamera_isp_cac47 \
    libmmcamera_isp_chroma_enhan40 \
    libmmcamera_isp_clamp_encoder40 \
    libmmcamera_isp_sce40 \
    libmmcamera_isp_gamma40 \
    libmmcamera_isp_gamma44 \
    libmmcamera_isp_cs_stats44 \
    libmmcamera_isp_cs_stats46 \
    libmmcamera_isp_sub_module \
    libmmcamera_isp_ltm44 \
    libmmcamera_isp_ltm47 \
    libmmcamera_isp_fovcrop_viewfinder40 \
    libmmcamera_isp_fovcrop_viewfinder46 \
    libmmcamera_isp_fovcrop_encoder40 \
    libmmcamera_isp_fovcrop_encoder46 \
    libmmcamera_isp_scaler_video46 \
    libmmcamera_isp_black_level47 \
    libmmcamera_isp_black_level48 \
    libmmcamera_isp_chroma_suppress40 \
    libmmcamera_isp_pedestal_correct46 \
    libmmcamera_isp_fovcrop_video46 \
    libmmcamera_isp_clamp_viewfinder40 \
    libmmcamera_isp_demux40 \
    libmmcamera_isp_demux48 \
    libmmcamera_isp_mce40 \
    libmmcamera_isp_bhist_stats44 \
    libmmcamera_isp_scaler_viewfinder44 \
    libmmcamera_isp_scaler_viewfinder46 \
    libmmcamera_isp_wb40
endif

ifneq ($(filter loire tone,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libactuator_lc898217 \
    libactuator_lc898217xc \
    libchromatix_imx214_common \
    libchromatix_imx214_cpp_liveshot \
    libchromatix_imx214_cpp_preview \
    libchromatix_imx214_cpp_snapshot \
    libchromatix_imx214_cpp_video \
    libchromatix_imx214_liveshot \
    libchromatix_imx214_postproc \
    libchromatix_imx214_preview \
    libchromatix_imx214_snapshot \
    libchromatix_imx214_video \
    libchromatix_imx214_default_preview_lc898217 \
    libchromatix_imx214_default_video_lc898217 \
    libmmcamera_imx214
endif

ifneq ($(filter yoshino nile,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libchromatix_imx219_common \
    libchromatix_imx219_cpp_liveshot \
    libchromatix_imx219_cpp_preview \
    libchromatix_imx219_cpp_snapshot \
    libchromatix_imx219_cpp_video \
    libchromatix_imx219_postproc \
    libchromatix_imx219_liveshot \
    libchromatix_imx219_preview \
    libchromatix_imx219_snapshot \
    libchromatix_imx219_video \
    libchromatix_imx219_default_preview \
    libchromatix_imx219_default_video \
    libmmcamera_imx219
endif

ifneq ($(filter loire,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libchromatix_imx241_common \
    libchromatix_imx241_cpp_liveshot \
    libchromatix_imx241_cpp_preview \
    libchromatix_imx241_cpp_snapshot \
    libchromatix_imx241_cpp_video \
    libchromatix_imx241_postproc \
    libchromatix_imx241_liveshot \
    libchromatix_imx241_preview \
    libchromatix_imx241_snapshot \
    libchromatix_imx241_video \
    libchromatix_imx241_default_preview \
    libchromatix_imx241_default_video \
    libmmcamera_imx241
endif

ifneq ($(filter yoshino,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libactuator_lc898217xc \
    libchromatix_imx258_common \
    libchromatix_imx258_cpp_liveshot \
    libchromatix_imx258_cpp_preview \
    libchromatix_imx258_cpp_snapshot \
    libchromatix_imx258_cpp_video \
    libchromatix_imx258_video \
    libchromatix_imx258_liveshot \
    libchromatix_imx258_postproc \
    libchromatix_imx258_preview \
    libchromatix_imx258_snapshot \
    libchromatix_imx258_default_preview_lc898217 \
    libchromatix_imx258_default_video_lc898217 \
    libmmcamera_imx258
endif

ifneq ($(filter loire tone nile,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libactuator_bu64747BS4 \
    libchromatix_imx300_common \
    libchromatix_imx300_cpp_liveshot \
    libchromatix_imx300_cpp_preview \
    libchromatix_imx300_cpp_snapshot \
    libchromatix_imx300_cpp_video \
    libchromatix_imx300_postproc \
    libchromatix_imx300_liveshot \
    libchromatix_imx300_preview \
    libchromatix_imx300_snapshot \
    libchromatix_imx300_video \
    libchromatix_imx300_default_preview_bu64747 \
    libchromatix_imx300_default_video_bu64747 \
    libmmcamera_imx300
endif

ifneq ($(filter yoshino,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libactuator_bu64747BS2 \
    libchromatix_imx400_common \
    libchromatix_imx400_cpp_liveshot \
    libchromatix_imx400_cpp_preview \
    libchromatix_imx400_cpp_snapshot \
    libchromatix_imx400_cpp_video \
    libchromatix_imx400_postproc \
    libchromatix_imx400_liveshot \
    libchromatix_imx400_preview \
    libchromatix_imx400_snapshot \
    libchromatix_imx400_video \
    libchromatix_imx400_default_preview_bu64747 \
    libchromatix_imx400_default_video_bu64747 \
    libmmcamera_imx400
endif

ifneq ($(filter ganges seine,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libactuator_lc898219xi \
    libchromatix_imx486_common \
    libchromatix_imx486_cpp_liveshot \
    libchromatix_imx486_cpp_preview \
    libchromatix_imx486_cpp_snapshot \
    libchromatix_imx486_cpp_video \
    libchromatix_imx486_postproc \
    libchromatix_imx486_liveshot \
    libchromatix_imx486_preview \
    libchromatix_imx486_snapshot \
    libchromatix_imx486_video \
    libchromatix_imx486_zsl_preview \
    libchromatix_imx486_zsl_video \
    libmmcamera_imx486
endif

ifneq ($(filter ganges,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libactuator_lc898214xd \
    libchromatix_s5k3l6_common \
    libchromatix_s5k3l6_cpp_liveshot \
    libchromatix_s5k3l6_cpp_preview \
    libchromatix_s5k3l6_cpp_snapshot \
    libchromatix_s5k3l6_cpp_video \
    libchromatix_s5k3l6_postproc \
    libchromatix_s5k3l6_liveshot \
    libchromatix_s5k3l6_preview \
    libchromatix_s5k3l6_snapshot \
    libchromatix_s5k3l6_video \
    libchromatix_s5k3l6_zsl_preview \
    libchromatix_s5k3l6_zsl_video \
    libmmcamera_s5k3l6
endif

ifneq ($(filter ganges,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libactuator_lc898214xc \
    libchromatix_s5k4h8_common \
    libchromatix_s5k4h8_cpp_liveshot \
    libchromatix_s5k4h8_cpp_preview \
    libchromatix_s5k4h8_cpp_snapshot \
    libchromatix_s5k4h8_cpp_video \
    libchromatix_s5k4h8_postproc \
    libchromatix_s5k4h8_liveshot \
    libchromatix_s5k4h8_preview \
    libchromatix_s5k4h8_snapshot \
    libchromatix_s5k4h8_video \
    libchromatix_s5k4h8_zsl_preview \
    libchromatix_s5k4h8_zsl_video \
    libmmcamera_s5k4h8
endif

ifneq ($(filter ganges,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libchromatix_s5k4e8_common \
    libchromatix_s5k4e8_cpp_liveshot \
    libchromatix_s5k4e8_cpp_preview \
    libchromatix_s5k4e8_cpp_snapshot \
    libchromatix_s5k4e8_cpp_video \
    libchromatix_s5k4e8_postproc \
    libchromatix_s5k4e8_liveshot \
    libchromatix_s5k4e8_preview \
    libchromatix_s5k4e8_snapshot \
    libchromatix_s5k4e8_video \
    libchromatix_s5k4e8_zsl_preview \
    libchromatix_s5k4e8_zsl_video \
    libmmcamera_s5k4e8
endif


ifneq ($(filter ganges,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libchromatix_s5k4h7yx_common \
    libchromatix_s5k4h7yx_cpp_liveshot \
    libchromatix_s5k4h7yx_cpp_preview \
    libchromatix_s5k4h7yx_cpp_snapshot \
    libchromatix_s5k4h7yx_cpp_video \
    libchromatix_s5k4h7yx_postproc \
    libchromatix_s5k4h7yx_liveshot \
    libchromatix_s5k4h7yx_preview \
    libchromatix_s5k4h7yx_snapshot \
    libchromatix_s5k4h7yx_video \
    libchromatix_s5k4h7yx_zsl_preview \
    libchromatix_s5k4h7yx_zsl_video \
    libmmcamera_s5k4h7yx
endif

ifneq ($(filter seine,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libactuator_dw9768_tele \
    libchromatix_s5k4h7yx_front_common \
    libchromatix_s5k4h7yx_front_cpp_liveshot \
    libchromatix_s5k4h7yx_front_cpp_preview \
    libchromatix_s5k4h7yx_front_cpp_snapshot \
    libchromatix_s5k4h7yx_front_cpp_video \
    libchromatix_s5k4h7yx_front_postproc \
    libchromatix_s5k4h7yx_front_liveshot \
    libchromatix_s5k4h7yx_front_preview \
    libchromatix_s5k4h7yx_front_snapshot \
    libchromatix_s5k4h7yx_front_video \
    libchromatix_s5k4h7yx_front_zsl_preview \
    libchromatix_s5k4h7yx_front_zsl_video \
    libmmcamera_s5k4h7yx_front

PRODUCT_PACKAGES += \
    libactuator_dw9768_tele \
    libchromatix_s5k4h7yx_tele_common \
    libchromatix_s5k4h7yx_tele_cpp_liveshot \
    libchromatix_s5k4h7yx_tele_cpp_preview \
    libchromatix_s5k4h7yx_tele_cpp_snapshot \
    libchromatix_s5k4h7yx_tele_cpp_video \
    libchromatix_s5k4h7yx_tele_postproc \
    libchromatix_s5k4h7yx_tele_liveshot \
    libchromatix_s5k4h7yx_tele_preview \
    libchromatix_s5k4h7yx_tele_snapshot \
    libchromatix_s5k4h7yx_tele_video \
    libchromatix_s5k4h7yx_tele_zsl_preview \
    libchromatix_s5k4h7yx_tele_zsl_video \
    libmmcamera_s5k4h7yx_tele

PRODUCT_PACKAGES += \
    libactuator_dw9768_wide \
    libchromatix_s5k4h7yx_uwide_common \
    libchromatix_s5k4h7yx_uwide_cpp_liveshot \
    libchromatix_s5k4h7yx_uwide_cpp_preview \
    libchromatix_s5k4h7yx_uwide_cpp_snapshot \
    libchromatix_s5k4h7yx_uwide_cpp_video \
    libchromatix_s5k4h7yx_uwide_postproc \
    libchromatix_s5k4h7yx_uwide_liveshot \
    libchromatix_s5k4h7yx_uwide_preview \
    libchromatix_s5k4h7yx_uwide_snapshot \
    libchromatix_s5k4h7yx_uwide_video \
    libchromatix_s5k4h7yx_uwide_zsl_preview \
    libchromatix_s5k4h7yx_uwide_zsl_video \
    libmmcamera_s5k4h7yx_uwide
endif

# CAMX Libs
ifneq ($(filter kumano tama,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    com.qti.node.swregistration \
    com.qti.camx.chiiqutils \
    libcom.qti.chinodeutils \
    com.qti.chi.override \
    com.qti.node.eisv2 \
    com.qti.node.eisv3 \
    com.qti.node.swregistration \
    com.qti.camx.chiiqutils \
    com.qti.node.memcpy \
    com.qti.hvx.binning \
    com.qti.hvx.addconstant \
    com.qti.node.gpu \
    com.qti.node.remosaic \
    com.qti.node.stitch \
    com.qti.node.depth \
    com.qti.tuned.default \
    fdconfigpreview \
    fdconfigpreviewlite \
    fdconfigvideo \
    fdconfigvideolite \
    camera.sdm845 \
    camera.sm8150
endif

ifeq ($(SOMC_PLATFORM),tama)
PRODUCT_PACKAGES += \
    com.sony.sensormodule.blakiston_imx400 \
    com.sony.sensormodule.pansy_imx241 \
    com.sony.sensormodule.poodle_imx258 \
    com.sony.eeprom.blakiston \
    com.sony.eeprom.pansy \
    com.sony.eeprom.poodle \
    com.sony.sensor.imx400 \
    com.sony.sensor.imx241 \
    com.sony.sensor.imx258 \
    com.sony.tuned.imx400 \
    com.sony.tuned.imx241 \
    com.sony.tuned.imx258
endif

ifeq ($(SOMC_PLATFORM),kumano)
PRODUCT_PACKAGES += \
    com.sony.sensormodule.milvus_imx445 \
    com.sony.sensormodule.milvus_s5k3m3 \
    com.sony.sensormodule.vulture_imx563 \
    com.sony.sensormodule.rose_s5k4h7 \
    com.sony.sensormodule.goby_s5k3m3 \
    com.sony.eeprom.milvus_main \
    com.sony.eeprom.milvus_sub \
    com.sony.eeprom.rose_otp \
    com.sony.eeprom.goby_otp \
    com.sony.sensor.imx445 \
    com.sony.sensor.s5k3m3 \
    com.sony.sensor.s5k3m3sn \
    com.sony.sensor.imx563 \
    com.sony.sensor.s5k4h7 \
    com.sony.tuned.imx445 \
    com.sony.tuned.imx563 \
    com.sony.tuned.s5k3m3 \
    com.sony.tuned.s5k4h7
endif
endif
