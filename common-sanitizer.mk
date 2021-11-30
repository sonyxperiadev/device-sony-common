ifneq ($(filter lena edo sagami,$(SOMC_PLATFORM)),)
PRODUCT_CFI_INCLUDE_PATHS += \
    hardware/interfaces/camera\
    vendor/qcom/proprietary/camx \
    vendor/qcom/proprietary/chi-cdk

ifneq ($(filter sagami,$(SOMC_PLATFORM)),)
PRODUCT_INTEGER_OVERFLOW_INCLUDE_PATHS += \
    hardware/interfaces\
    hardware/qcom \
    vendor/qcom/proprietary \
    vendor/qcom/opensource

PRODUCT_BOUNDS_INCLUDE_PATHS += \
    hardware/interfaces\
    hardware/qcom \
    vendor/qcom/proprietary \
    vendor/qcom/opensource
endif
endif
