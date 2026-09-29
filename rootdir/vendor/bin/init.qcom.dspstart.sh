#!/vendor/bin/sh
#
# Universal start script for remoteproc DSPs.
#
# Usage:
#   init.qcom.dspstart.sh <adsp|cdsp|slpi>

TAG="${0##*/}"
DSP="$1"

# Validate the argument and derive the upper-case name used in log messages.
case "$DSP" in
    adsp) DSP_UPPER="ADSP" ;;
    cdsp) DSP_UPPER="CDSP" ;;
    slpi) DSP_UPPER="SLPI" ;;
    *)
        log -t "$TAG" "Usage: $TAG <adsp|cdsp|slpi>"
        exit 1
        ;;
esac

# Total wait time for each stage = MAX_RETRIES * SLEEP_TIME (10 s by default).
MAX_RETRIES=100
SLEEP_TIME=0.1

BOOT_NODE="/sys/kernel/boot_${DSP}/boot"

# Wait for the boot node to appear.
# The node is created by the kernel driver, which may probe slightly later
# than this script is started.
attempt=0
while [ ! -e "$BOOT_NODE" ]; do
    attempt=$((attempt + 1))
    if [ "$attempt" -ge "$MAX_RETRIES" ]; then
        log -t "$TAG" "$DSP_UPPER boot node $BOOT_NODE not found"
        exit 1
    fi
    sleep "$SLEEP_TIME"
done

# Ask the kernel to boot the DSP.
if ! echo 1 > "$BOOT_NODE"; then
    log -t "$TAG" "Failed to write to $BOOT_NODE"
    exit 1
fi

# Find the remoteproc device whose "name" contains the DSP name.
# Sets REMOTEPROC_DIR on success, leaves it empty otherwise.
find_remoteproc() {
    REMOTEPROC_DIR=""
    for proc in /sys/class/remoteproc/remoteproc*; do
        [ -r "$proc/name" ] || continue
        name=""
        read -r name < "$proc/name"
        case "$name" in
            *"$DSP"*)
                REMOTEPROC_DIR="$proc"
                return 0
                ;;
        esac
    done
    return 1
}

# Wait until the remoteproc reports "running".
# Any other state is treated as "not running"; if "running" is not
# reached within the timeout, the script fails.
# The remoteproc lookup is done inside the loop, because the entry
# may not be registered yet when the script starts.
REMOTEPROC_DIR=""
state=""
attempt=0
while [ "$attempt" -lt "$MAX_RETRIES" ]; do
    [ -n "$REMOTEPROC_DIR" ] || find_remoteproc

    if [ -n "$REMOTEPROC_DIR" ]; then
        read -r state 2>/dev/null < "$REMOTEPROC_DIR/state"
    fi

    if [ "$state" = "running" ]; then
        setprop "vendor.qcom.${DSP}up" 1
        log -t "$TAG" "$DSP_UPPER is ready!"
        exit 0
    fi

    attempt=$((attempt + 1))
    sleep "$SLEEP_TIME"
done

log -t "$TAG" "$DSP_UPPER is not booted after $MAX_RETRIES retries"
exit 1
