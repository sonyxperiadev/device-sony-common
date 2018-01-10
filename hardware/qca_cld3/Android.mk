ifeq ($(WIFI_DRIVER_BUILT),qca_cld3)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/firmware/wlan/qca_cld)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /odm/firmware/wlan/qca_cld/WCNSS_qcom_cfg.ini firmware/wlan/qca_cld/WCNSS_qcom_cfg.ini && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /odm/firmware/wlan/qca_cld/bdwlan.bin firmware/wlan/qca_cld/bdwlan.bin && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /data/misc/wifi/wlan_mac.bin firmware/wlan/qca_cld/wlan_mac.bin && popd > /dev/null)
endif