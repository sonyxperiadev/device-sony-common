#!/vendor/bin/sh

echo 1 > /sys/kernel/boot_adsp/boot
echo 1 > /sys/kernel/boot_cdsp/boot
setprop vendor.qcom.adspup 1
