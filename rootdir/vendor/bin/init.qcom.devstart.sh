#!/vendor/bin/sh

echo 1 > /sys/kernel/boot_adsp/boot
echo 1 > /sys/kernel/boot_slpi/boot
setprop sys.qcom.devup 1
