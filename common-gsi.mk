BUILDING_GSI := true

# Exclude all files under system/product and system/system_ext
PRODUCT_ARTIFACT_PATH_REQUIREMENT_ALLOWED_LIST += \
    system/product/% \
    system/system_ext/%

# GSI targets should install "unflattened" APEXes in /system
TARGET_FLATTEN_APEX := false

# GSI targets should install "flattened" APEXes in /system_ext as well
PRODUCT_INSTALL_EXTRA_FLATTENED_APEXES := true

# The flattened version of com.android.apex.cts.shim.v1 should be explicitly installed
# because the shim apex is prebuilt one and PRODUCT_INSTALL_EXTRA_FLATTENED_APEXES is not
# supported for prebuilt_apex modules yet.
PRODUCT_PACKAGES += com.android.apex.cts.shim.v1_with_prebuilts.flattened

# GSI specific tasks on boot
PRODUCT_PACKAGES += \
    init.gsi.rc \
    init.vndk-nodef.rc \

# Always build modules from source
MODULE_BUILD_FROM_SOURCE := true
