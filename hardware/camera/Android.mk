ifneq ($(filter sdm845,$(TARGET_BOARD_PLATFORM)),)

$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_ODM)/lib)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_ODM)/lib/camera)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_ODM)/lib64)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_ODM)/lib64/camera)

endif
