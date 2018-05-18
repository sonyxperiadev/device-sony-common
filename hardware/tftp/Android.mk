ifneq ($(filter sdm660 msm8998 sdm845,$(TARGET_BOARD_PLATFORM)),)

$(shell rm -r $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/rfs/)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/rfs/msm/mpss/readonly/vendor)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/rfs/msm/adsp/readonly/vendor)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/rfs/msm/slpi/readonly/vendor)

$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /data/tombstones/modem rfs/msm/mpss/ramdumps && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /persist/rfs/msm/mpss rfs/msm/mpss/readwrite && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /persist/rfs/shared rfs/msm/mpss/shared && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /persist/hlos_rfs/shared rfs/msm/mpss/hlos && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /firmware rfs/msm/mpss/readonly/firmware && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /odm/firmware rfs/msm/mpss/readonly/vendor/firmware && popd > /dev/null)

$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /data/tombstones/lpass rfs/msm/adsp/ramdumps && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /persist/rfs/msm/adsp rfs/msm/adsp/readwrite && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /persist/rfs/shared rfs/msm/adsp/shared && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /persist/hlos_rfs/shared rfs/msm/adsp/hlos && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /firmware rfs/msm/adsp/readonly/firmware && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /odm/firmware rfs/msm/adsp/readonly/vendor/firmware && popd > /dev/null)

$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /data/tombstones/lpass rfs/msm/slpi/ramdumps && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /persist/rfs/msm/slpi rfs/msm/slpi/readwrite && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /persist/rfs/shared rfs/msm/slpi/shared && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /persist/hlos_rfs/shared rfs/msm/slpi/hlos && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /firmware rfs/msm/slpi/readonly/firmware && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /odm/firmware rfs/msm/slpi/readonly/vendor/firmware && popd > /dev/null)

endif
