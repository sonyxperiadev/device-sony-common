#!/vendor/bin/sh

echo 1 > /sys/kernel/boot_adsp/boot
echo 1 > /sys/kernel/boot_cdsp/boot
echo 1 > /sys/kernel/boot_slpi/boot
setprop vendor.qcom.devup 1
