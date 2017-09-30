ifneq ($(filter yukon rhine shinano kanuti kitakami loire,$(PRODUCT_PLATFORM)),)

LOCAL_PATH := $(call my-dir)

include $(call all-makefiles-under,$(LOCAL_PATH))

$(shell mkdir -p $(PRODUCT_OUT)/system/vendor/firmware)
$(shell pushd $(PRODUCT_OUT)/system/vendor/firmware > /dev/null && ln -s /vendor/etc/firmware/libpn547_fw.so libpn547_fw.so && popd > /dev/null)

endif
