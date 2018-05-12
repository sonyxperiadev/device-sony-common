$(shell pushd $(PRODUCT_OUT)/odm/ > /dev/null && echo "ro.odm.version=$(PLATFORM_VERSION)_$(SOMC_KERNEL_VERSION)_$(SOMC_PLATFORM)_$(TARGET_VENDOR_VERSION)" >build.prop && popd > /dev/null)
