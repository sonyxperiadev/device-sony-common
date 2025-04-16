# CAMX Libs
ifeq ($(filter seine,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    camx.common@1.0-helper \
    camera.qcom \
    camx.device@3.2-impl \
    camx.device@3.3-impl \
    camx.device@3.4-ext-impl \
    camx.device@3.4-impl \
    camx.device@3.5-ext-impl \
    camx.device@3.5-impl \
    camx.device@3.6-ext-impl \
    camx.device@3.6-impl \
    camx.device@3.7-impl \
    camx.provider@2.4-external \
    camx.provider@2.4-impl \
    camx.provider@2.4-legacy \
    camx.provider@2.5-external \
    camx.provider@2.5-legacy \
    camx.provider@2.6-legacy \
    camx.provider@2.7-legacy \
    com.qti.camx.chiiqutils \
    com.qti.chi.override \
    com.qti.chi.override.bitra \
    com.qti.chiusecaseselector \
    com.qti.feature2.afbrckt \
    com.qti.feature2.anchorsync \
    com.qti.feature2.anchorsync.bitra \
    com.qti.feature2.demux \
    com.qti.feature2.demux.bitra \
    com.qti.feature2.derivedoffline \
    com.qti.feature2.frameselect \
    com.qti.feature2.frameselect.bitra \
    com.qti.feature2.fusion \
    com.qti.feature2.fusion.bitra \
    com.qti.feature2.generic \
    com.qti.feature2.generic.bitra \
    com.qti.feature2.gs \
    com.qti.feature2.gs.bitra \
    com.qti.feature2.gs.netrani \
    com.qti.feature2.gs.sm6375 \
    com.qti.feature2.gs.sm6450 \
    com.qti.feature2.gs.sm8350 \
    com.qti.feature2.gs.sm8450 \
    com.qti.feature2.gs.sm8550 \
    com.qti.feature2.hdr \
    com.qti.feature2.hdr.bitra \
    com.qti.feature2.mcreprocrt \
    com.qti.feature2.memcpy \
    com.qti.feature2.memcpy.bitra \
    com.qti.feature2.mfsr \
    com.qti.feature2.mfsr.bitra \
    com.qti.feature2.mfsr.netrani \
    com.qti.feature2.mfsr.sm8450 \
    com.qti.feature2.ml \
    com.qti.feature2.ml.fillmore \
    com.qti.feature2.mux \
    com.qti.feature2.qcfa \
    com.qti.feature2.qcfa.bitra \
    com.qti.feature2.rawhdr \
    com.qti.feature2.rawhdr.bitra \
    com.qti.feature2.realtimeserializer \
    com.qti.feature2.rt \
    com.qti.feature2.rt.bitra \
    com.qti.feature2.rtmcx \
    com.qti.feature2.serializer \
    com.qti.feature2.serializer.bitra \
    com.qti.feature2.statsregeneration \
    com.qti.feature2.stub \
    com.qti.feature2.stub.bitra \
    com.qti.feature2.swmf \
    com.qti.feature2.swmf.bitra \
    com.qti.hvx.addconstant \
    com.qti.hvx.binning \
    com.qti.node.afbfusion \
    com.qti.node.customhwnode \
    com.qti.node.depth \
    com.qti.node.dewarp \
    com.qti.node.dummydepth \
    com.qti.node.dummyrtb \
    com.qti.node.dummysat \
    com.qti.node.eisv2 \
    com.qti.node.eisv3 \
    com.qti.node.fcv \
    com.qti.node.formatconversion \
    com.qti.node.gme \
    com.qti.node.gpu \
    com.qti.node.gyrornn \
    com.qti.node.hdr10pgen \
    com.qti.node.hdr10phist \
    com.qti.node.memcpy \
    com.qti.node.ml \
    com.qti.node.mlinference \
    com.qti.node.muxer \
    com.qti.node.remosaic \
    com.qti.node.stich \
    com.qti.node.sw2dlut \
    com.qti.node.swaidenoiser \
    com.qti.node.swbestats \
    com.qti.node.swcac \
    com.qti.node.swec \
    com.qti.node.swfusion \
    com.qti.node.swhme \
    com.qti.node.swlsc \
    com.qti.node.swmctf \
    com.qti.node.swpdpc \
    com.qti.node.swpreprocess \
    com.qti.node.swregistration \
    com.qti.node.swvrt \
    com.qti.settings.clarence \
    com.qti.settings.fillmore \
    com.qti.settings.netrani \
    com.qti.settings.sm6350 \
    com.qti.settings.sm6375 \
    com.qti.settings.sm6450 \
    com.qti.settings.sm8150 \
    com.qti.settings.sm8250 \
    com.qti.settings.sm8350 \
    com.qti.settings.sm8450 \
    com.qti.settings.sm8550 \
    com.qti.stats.aec \
    com.qti.stats.aecwrapper \
    com.qti.stats.af \
    com.qti.stats.afd \
    com.qti.stats.afwrapper \
    com.qti.stats.awbwrapper \
    com.qti.stats.common \
    com.qti.stats.hafoverride \
    com.qti.tuned.default \
    com.qtistatic.stats.aec \
    com.qtistatic.stats.af \
    com.qtistatic.stats.awb \
    com.qtistatic.stats.pdlib \
    com.qualcomm.mcx.distortionmapper \
    com.qualcomm.mcx.linearmapper \
    com.qualcomm.mcx.nonlinearmapper \
    com.qualcomm.mcx.policy.mfl \
    com.qualcomm.mcx.policy.xr \
    com.qualcomm.qti.mcx.usecase.extension \
    fdconfigpreview \
    fdconfigpreviewlite \
    fdconfigvideo \
    fdconfigvideolite \
    libcamera_nn_stub \
    libcamerapostproc \
    libcamerapostproc.bitra \
    libcamxcommonutils \
    libcamxcore \
    libcamxexternalformatutils \
    libcamxhwlcvp \
    libcamxhwlfd \
    libcamxhwlqsat \
    libcamxhwltitan17x \
    libcamximageformatutils \
    libcamxsettingsmanager \
    libcamxswispiqmodule \
    libchilog \
    libcom.qti.chinodeutils \
    libdepthcomputation \
    libdepthmapwrapper \
    libdepthmapwrapper_secure \
    libeepromcutter \
    libopencv \
    libqshcamera\
    libspectre \
    vendor.qti.camera.provider \
    vendor.qti.camera.provider-service_64 \
    vendor.qti.hardware.camera.aon@1.0-service-impl \
    vendor.qti.hardware.camera.aon@1.3 \
    vendor.qti.hardware.camera.postproc@1.0 \
    vendor.qti.hardware.camera.postproc@1.0-service-impl
endif

ifeq ($(SOMC_PLATFORM),edo)
PRODUCT_PACKAGES += \
    com.sony.sensor.imx316 \
    com.sony.sensor.imx557 \
    com.sony.sensor.s5k3t2 \
    com.sony.sensor.s5k4h7 \
    com.sony.sensor.imx363 \
    com.sony.eeprom.eiger_main \
    com.sony.eeprom.eiger_tele \
    com.sony.eeprom.icarus \
    com.sony.eeprom.eiger_uw \
    com.sony.eeprom.rose_otp
endif

ifeq ($(SOMC_PLATFORM),lena)
PRODUCT_PACKAGES += \
    com.sony.sensor.imx486 \
    com.sony.sensor.s5k4h7 \
    com.sony.sensor.ov8856 \
    com.sony.eeprom.lena_front \
    com.sony.eeprom.lena_tele \
    com.sony.eeprom.lena_uwide \
    com.sony.eeprom.lena_wide
endif

ifeq ($(SOMC_PLATFORM),sagami)
PRODUCT_PACKAGES += \
    com.sony.sensor.s5k3t2 \
    com.sony.sensor.s5k4h7 \
    com.sony.sensor.imx557 \
    com.sony.sensor.imx663 \
    com.sony.eeprom.hohgant_wide \
    com.sony.eeprom.hohgant_uwide \
    com.sony.eeprom.lohner \
    com.sony.eeprom.rose_otp
endif

ifeq ($(SOMC_PLATFORM),nagara)
PRODUCT_PACKAGES += \
    com.sony.sensor.imx557 \
    com.sony.sensor.imx563 \
    com.sony.sensor.imx650 \
    com.sony.sensor.imx663 \
    com.sony.eeprom.castore_wide \
    com.sony.eeprom.castore_uwide \
    com.sony.eeprom.crocus \
    com.sony.eeprom.camino_tele
endif

ifeq ($(SOMC_PLATFORM),yodo)
PRODUCT_PACKAGES += \
    com.sony.sensor.imx888 \
    com.sony.sensor.imx563 \
    com.sony.sensor.imx650 \
    com.sony.sensor.imx663 \
    com.sony.eeprom.yodo_wide \
    com.sony.eeprom.yodo_uwide \
    com.sony.eeprom.yodo_tele \
    com.sony.eeprom.yodo_front
endif

ifeq ($(SOMC_PLATFORM),murray)
PRODUCT_PACKAGES += \
    com.sony.eeprom.murray_front \
    com.sony.eeprom.murray_tele \
    com.sony.eeprom.murray_uwide \
    com.sony.eeprom.murray_wide \
    com.sony.sensor.hi846 \
    com.sony.sensor.hi847 \
    com.sony.sensor.imx486
endif

ifeq ($(SOMC_PLATFORM),zambezi)
PRODUCT_PACKAGES += \
    com.sony.eeprom.zambezi_front \
    com.sony.eeprom.zambezi_tele \
    com.sony.eeprom.zambezi_uwide \
    com.sony.eeprom.zambezi_wide \
    com.sony.sensor.hi846 \
    com.sony.sensor.hi847 \
    com.sony.sensor.imx582
endif

ifeq ($(SOMC_PLATFORM),columbia)
PRODUCT_PACKAGES += \
    com.sony.eeprom.columbia_front \
    com.sony.eeprom.columbia_tele \
    com.sony.eeprom.columbia_uwide \
    com.sony.eeprom.columbia_wide \
    com.sony.sensor.hi846 \
    com.sony.sensor.hi847 \
    com.sony.sensor.imx582
endif
