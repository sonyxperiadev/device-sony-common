$(shell pushd $(PRODUCT_OUT)/odm/ > /dev/null && echo "ro.kernel.version=$(TARGET_KERNEL_VERSION)" >odm_version.prop && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/odm/ > /dev/null && echo "ro.build.version=$(PLATFORM_VERSION)" >>odm_version.prop && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/odm/ > /dev/null && echo "ro.platform.version=$(TARGET_BOARD_PLATFORM)" >>odm_version.prop && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/odm/ > /dev/null && echo "ro.vendor.version=$(TARGET_VENDOR_VERSION)" >>odm_version.prop && popd > /dev/null)
