# CAMX Libs
ifneq ($(filter kumano tama edo,$(SOMC_PLATFORM)),)
PRODUCT_PACKAGES += \
    libcamxexternalformatutils \
    com.qti.node.swregistration \
    com.qti.camx.chiiqutils \
    libcom.qti.chinodeutils \
    libcamera_nn_stub \
    com.qti.chi.override \
    com.qti.node.eisv2 \
    com.qti.node.eisv3 \
    com.qti.node.swregistration \
    com.qti.camx.chiiqutils \
    com.qti.node.memcpy \
    com.qti.hvx.binning \
    com.qti.hvx.addconstant \
    com.qti.node.fcv \
    com.qti.node.stitch \
    com.qti.node.gpu \
    com.qti.node.remosaic \
    com.qti.node.stich \
    com.qti.node.depth \
    com.qti.tuned.default \
    com.qti.stats.afd \
    fdconfigpreview \
    fdconfigpreviewlite \
    fdconfigvideo \
    fdconfigvideolite \
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
