#!/vendor/bin/sh

echo 1 > /sys/kernel/boot_cdsp/boot
setprop vendor.qcom.cdspup 1
