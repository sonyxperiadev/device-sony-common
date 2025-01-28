#!/vendor/bin/sh

echo 1 > /sys/kernel/boot_slpi/boot

MAX_RETRIES=100
SLEEP_TIME=0.1
attempt=0

# Check for the 'remoteproc' entry with 'slpi' in its name
remoteproc_slpi=""

# Check each remoteproc entry to see if the 'name' contains 'slpi'
for proc in /sys/class/remoteproc/remoteproc*; do
    if [[ -f "$proc/name" && $(cat "$proc/name") == *"slpi"* ]]; then
        remoteproc_slpi=$(basename "$proc")
        break
    fi
done

while (( attempt < MAX_RETRIES )); do
    # For kernels >= 5.10, check remoteproc state; for earlier kernels, check subsys state
    remoteproc_slpi_state=$(cat /sys/class/remoteproc/$remoteproc_slpi/state 2>/dev/null || echo "UNKNOWN")
    subsys_slpi_state=$(cat /sys/class/subsys/subsys_slpi/device/subsys*/state 2>/dev/null || echo "UNKNOWN")

    if [[ "$subsys_slpi_state" == "ONLINE" || "$remoteproc_slpi_state" == "running" ]]; then
        setprop vendor.qcom.slpiup 1
        log -t "$(basename $0)" "SLPI is ready!"
        exit 0
    fi

    log -t "$(basename $0)" "SLPI is not ready yet. Retrying..."
    ((attempt++))
    sleep $SLEEP_TIME
done

log -t "$(basename $0)" "SLPI is not booted after $((MAX_RETRIES * SLEEP_TIME)) seconds."
exit 1
