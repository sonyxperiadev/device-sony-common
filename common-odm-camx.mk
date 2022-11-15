# CAMX Libs
ifeq ($(filter seine,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libcamera_nn_stub \
    libcamerapostproc \
    libcamerapostproc.bitra \
    libcamxexternalformatutils \
    libcom.qti.chinodeutils \
    com.qti.camx.chiiqutils \
    com.qti.chi.override \
    com.qti.chi.override.bitra \
    com.qti.chiusecaseselector \
    com.qti.feature2.anchorsync \
    com.qti.feature2.demux \
    com.qti.feature2.frameselect \
    com.qti.feature2.fusion \
    com.qti.feature2.generic \
    com.qti.feature2.gs \
    com.qti.feature2.gs.sm8350 \
    com.qti.feature2.gs.sm8450 \
    com.qti.feature2.hdr \
    com.qti.feature2.memcpy \
    com.qti.feature2.mfsr \
    com.qti.feature2.qcfa \
    com.qti.feature2.rawhdr \
    com.qti.feature2.rt \
    com.qti.feature2.serializer \
    com.qti.feature2.stub \
    com.qti.feature2.swmf \
    com.qti.feature2.anchorsync.bitra \
    com.qti.feature2.demux.bitra \
    com.qti.feature2.frameselect.bitra \
    com.qti.feature2.fusion.bitra \
    com.qti.feature2.rt.bitra \
    com.qti.feature2.memcpy.bitra \
    com.qti.feature2.mfsr.bitra \
    com.qti.feature2.qcfa.bitra \
    com.qti.feature2.hdr.bitra \
    com.qti.feature2.generic.bitra \
    com.qti.feature2.rawhdr.bitra \
    com.qti.feature2.serializer.bitra \
    com.qti.feature2.stub.bitra \
    com.qti.feature2.swmf.bitra \
    com.qti.feature2.gs.bitra \
    com.qti.node.eisv2 \
    com.qti.node.eisv3 \
    com.qti.node.swregistration \
    com.qti.node.memcpy \
    com.qti.hvx.binning \
    com.qti.hvx.addconstant \
    com.qti.node.dummyrtb \
    com.qti.node.dummysat \
    com.qti.node.depth \
    com.qti.node.dewarp \
    com.qti.node.formatconversion \
    com.qti.node.fcv \
    com.qti.node.gpu \
    com.qti.node.ml \
    com.qti.node.remosaic \
    com.qti.node.stich \
    com.qti.node.swaidenoiser \
    com.qti.node.swbestats \
    com.qti.node.swcac \
    com.qti.node.swlsc \
    com.qti.node.swmctf \
    com.qti.node.swmfnr \
    com.qti.node.swpdpc \
    com.qti.node.swpreprocess \
    com.qti.settings.sm6350 \
    com.qti.settings.sm8150 \
    com.qti.settings.sm8250 \
    com.qti.settings.sm8350 \
    com.qti.settings.sm8450 \
    com.qti.stats.common \
    com.qti.stats.aecwrapper \
    com.qti.stats.afd \
    com.qti.stats.afwrapper \
    com.qti.stats.awbwrapper \
    com.qti.stats.hafoverride \
    com.qti.tuned.default \
    com.qtistatic.stats.aec \
    com.qtistatic.stats.af \
    com.qtistatic.stats.awb \
    com.qtistatic.stats.pdlib \
    libdepthmapwrapper \
    fdconfigpreview \
    fdconfigpreviewlite \
    fdconfigvideo \
    fdconfigvideolite \
    vendor.qti.hardware.camera.aon@1.0-service-impl \
    vendor.qti.hardware.camera.aom@1.0 \
    vendor.qti.hardware.camera.postproc@1.0 \
    camera.qcom
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
