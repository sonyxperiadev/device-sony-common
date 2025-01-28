#!/vendor/bin/sh

echo 1 > /sys/kernel/boot_adsp/boot

MAX_RETRIES=100
SLEEP_TIME=0.1
attempt=0

# Check for the 'remoteproc' entry with 'adsp' in its name
remoteproc_adsp=""

# Check each remoteproc entry to see if the 'name' contains 'adsp'
for proc in /sys/class/remoteproc/remoteproc*; do
    if [[ -f "$proc/name" && $(cat "$proc/name") == *"adsp"* ]]; then
        remoteproc_adsp=$(basename "$proc")
        break
    fi
done

while (( attempt < MAX_RETRIES )); do
    # For kernels >= 5.10, check remoteproc state; for earlier kernels, check subsys state
    remoteproc_adsp_state=$(cat /sys/class/remoteproc/$remoteproc_adsp/state 2>/dev/null || echo "UNKNOWN")
    subsys_adsp_state=$(cat /sys/class/subsys/subsys_adsp/device/subsys*/state 2>/dev/null || echo "UNKNOWN")

    if [[ "$subsys_adsp_state" == "ONLINE" || "$remoteproc_adsp_state" == "running" ]]; then
        setprop vendor.qcom.adspup 1
        log -t "$(basename $0)" "ADSP is ready!"
        exit 0
    fi

    log -t "$(basename $0)" "ADSP is not ready yet. Retrying..."
    ((attempt++))
    sleep $SLEEP_TIME
done

log -t "$(basename $0)" "ADSP is not booted after $((MAX_RETRIES * SLEEP_TIME)) seconds."
exit 1
