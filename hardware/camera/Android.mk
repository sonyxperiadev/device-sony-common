ifneq ($(filter sdm845,$(TARGET_BOARD_PLATFORM)),)

$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_ODM)/lib)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_ODM)/lib/camera)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_ODM)/lib64)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_ODM)/lib64/camera)

$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/lib > /dev/null && ln -sf /odm/lib/hw/camera.qcom.so hw/camera.sdm845.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/lib > /dev/null && ln -sf /odm/lib/hw/camera.qcom.so hw/camera.qcom.so && popd > /dev/null)

$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/lib > /dev/null && ln -sf /odm/lib/camera camera && popd > /dev/null)

$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/lib > /dev/null && ln -sf /odm/lib/libcom.qti.chinodeutils.so libcom.qti.chinodeutils.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/lib > /dev/null && ln -sf /odm/lib/libcamxfdalgov7.so libcamxfdalgov7.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/lib > /dev/null && ln -sf /odm/lib/libcamxfdengine.so libcamxfdengine.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/lib > /dev/null && ln -sf /odm/lib/libcamxstatscore.so libcamxstatscore.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/lib > /dev/null && ln -sf /odm/lib/libcamxtintlessalgo.so libcamxtintlessalgo.so && popd > /dev/null)

endif
