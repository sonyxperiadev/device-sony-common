#!/vendor/bin/sh

echo 1 > /sys/kernel/boot_cdsp/boot

MAX_RETRIES=100
SLEEP_TIME=0.1
attempt=0

# Check for the 'remoteproc' entry with 'cdsp' in its name
remoteproc_cdsp=""

# Check each remoteproc entry to see if the 'name' contains 'cdsp'
for proc in /sys/class/remoteproc/remoteproc*; do
    if [[ -f "$proc/name" && $(cat "$proc/name") == *"cdsp"* ]]; then
        remoteproc_cdsp=$(basename "$proc")
        break
    fi
done

while (( attempt < MAX_RETRIES )); do
    # Check remoteproc state
    remoteproc_cdsp_state=$(cat /sys/class/remoteproc/$remoteproc_cdsp/state 2>/dev/null || echo "UNKNOWN")

    if [[ "$remoteproc_cdsp_state" == "running" ]]; then
        setprop vendor.qcom.cdspup 1
        log -t "$(basename $0)" "CDSP is ready!"
        exit 0
    fi

    log -t "$(basename $0)" "CDSP is not ready yet. Retrying..."
    ((attempt++))
    sleep $SLEEP_TIME
done

log -t "$(basename $0)" "CDSP is not booted after $((MAX_RETRIES * SLEEP_TIME)) seconds."
exit 1
