# Kernel headers intermediary module

Selects `qti_kernel_headers` repo based on kernel version so that multiple
versions can live inside on AOSP tree.

Needs following changes:

```make
SOONG_CONFIG_NAMESPACES += qti_kernel_headers
SOONG_CONFIG_qti_kernel_headers := version
SOONG_CONFIG_qti_kernel_headers_version := 4.14
```

`common-headers` needs to rename the module name from `qti_kernel_headers` to
`qti_kernel_headers_4.14` (based on version).
