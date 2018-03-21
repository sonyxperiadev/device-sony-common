ifeq ($(WIFI_DRIVER_BUILT),qca_cld3)
$(shell mkdir -p $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR)/firmware/wlan/qca_cld)
ifneq ($(filter nile,$(PRODUCT_PLATFORM)),)
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /persist/wlan_mac.bin data/misc/wifi/wlan_mac.bin && popd > /dev/null)
else
$(shell pushd $(PRODUCT_OUT)/$(TARGET_COPY_OUT_VENDOR) > /dev/null && ln -s /data/misc/wifi/wlan_mac.bin firmware/wlan/qca_cld/wlan_mac.bin && popd > /dev/null)
endif
endif
