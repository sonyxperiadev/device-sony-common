ifneq ($(filter yukon rhine shinano kanuti kitakami loire,$(PRODUCT_PLATFORM)),)

LOCAL_PATH := $(call my-dir)

include $(call all-makefiles-under,$(LOCAL_PATH))

endif

ifneq ($(filter shinano kitakami,$(PRODUCT_PLATFORM)),)
$(shell mkdir -p $(PRODUCT_OUT)/system/vendor/firmware)
$(shell pushd $(PRODUCT_OUT)/system/vendor/firmware > /dev/null && ln -s /vendor/etc/firmware/libpn547_fw.so libpn547_fw.so && popd > /dev/null)
endif

ifneq ($(filter loire tone yoshino,$(PRODUCT_PLATFORM)),)
# EGL libs
$(shell mkdir -p $(PRODUCT_OUT)/system/vendor/lib)
$(shell mkdir -p $(PRODUCT_OUT)/system/vendor/lib/hw)
$(shell mkdir -p $(PRODUCT_OUT)/system/vendor/lib64)
$(shell mkdir -p $(PRODUCT_OUT)/system/vendor/lib64/hw)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/egl egl && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libllvm-glnext.so libllvm-glnext.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libllvm-qgl.so libllvm-qgl.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libc2d30_bltlib.so libc2d30_bltlib.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libc2d30-a3xx.so libc2d30-a3xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libc2d30-a4xx.so libc2d30-a4xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libc2d30-a5xx.so libc2d30-a5xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libmm-qdcm.so libmm-qdcm.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/librs_adreno.so librs_adreno.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libadreno_utils.so libadreno_utils.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libCB.so libCB.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libgsl.so libgsl.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libC2D2.so libC2D2.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libRSDriver_adreno.so libRSDriver_adreno.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libOpenCL.so libOpenCL.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/libllvm-qcom.so libllvm-qcom.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib > /dev/null && ln -s /odm/lib/librs_adreno_sha1.so librs_adreno_sha1.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib/hw > /dev/null && ln -s /odm/lib/hw/vulkan.$(TARGET_BOARD_PLATFORM).so vulkan.$(TARGET_BOARD_PLATFORM).so && popd > /dev/null)

$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/egl egl && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libllvm-glnext.so libllvm-glnext.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libllvm-qgl.so libllvm-qgl.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libc2d30_bltlib.so libc2d30_bltlib.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libc2d30-a3xx.so libc2d30-a3xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libc2d30-a4xx.so libc2d30-a4xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libc2d30-a5xx.so libc2d30-a5xx.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libmm-qdcm.so libmm-qdcm.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/librs_adreno.so librs_adreno.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libadreno_utils.so libadreno_utils.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libCB.so libCB.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libgsl.so libgsl.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libC2D2.so libC2D2.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libRSDriver_adreno.so libRSDriver_adreno.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libOpenCL.so libOpenCL.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/libllvm-qcom.so libllvm-qcom.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64 > /dev/null && ln -s /odm/lib64/librs_adreno_sha1.so librs_adreno_sha1.so && popd > /dev/null)
$(shell pushd $(PRODUCT_OUT)/system/vendor/lib64/hw > /dev/null && ln -s /odm/lib64/hw/vulkan.$(TARGET_BOARD_PLATFORM).so vulkan.$(TARGET_BOARD_PLATFORM).so && popd > /dev/null)

# device firmware
$(shell mkdir -p $(PRODUCT_OUT)/odm/firmware)
$(shell pushd $(PRODUCT_OUT)/system/vendor > /dev/null && ln -s /odm/firmware firmware && popd > /dev/null)

$(shell pushd $(PRODUCT_OUT)/odm/firmware > /dev/null && ln -s /vendor/etc/firmware/libpn547_fw.so libpn547_fw.so && popd > /dev/null)

endif
