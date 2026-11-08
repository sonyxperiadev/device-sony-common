#!/vendor/bin/sh
#
# Selects the media target variant for the current SoC.
#
# Sets:
#   vendor.media.target_variant

TAG="${0##*/}"

PLATFORM="$(getprop ro.board.platform)"

# Base variant name and vidc node address for the platform.
# Define VIDC_NODE only for platforms that have several SKU variants.
case "$PLATFORM" in
    sm6450|sm6475)
        BASE="_parrot"
        VIDC_NODE="aa00000.qcom,vidc"
        ;;
    sm8450)
        BASE="_taro"
        ;;
    sm8550)
        BASE="_kalama"
        ;;
    sm8650)
        BASE="_pineapple"
        ;;
    *)
        # Not a supported platform, leave the property unset.
        log -t "$TAG" "Unsupported platform '$PLATFORM', nothing to do"
        exit 0
        ;;
esac

# Read the SKU version from the vidc node (only if the platform has SKUs).
# Stays empty if the node is missing or unreadable.
SKU_VERSION=""
if [ -n "$VIDC_NODE" ]; then
    SKU_NODE="/sys/devices/platform/soc/$VIDC_NODE/sku_version"
    [ -r "$SKU_NODE" ] && read -r SKU_VERSION < "$SKU_NODE"
fi

# Append the SKU suffix (_v0, _v1, ...) if the SKU is a valid number.
# No SKU support, unknown SKU or no SKU node: use the base variant as is.
case "$SKU_VERSION" in
    ""|*[!0-9]*) VARIANT="$BASE" ;;
    *)           VARIANT="${BASE}_v${SKU_VERSION}" ;;
esac

setprop vendor.media.target_variant "$VARIANT"

log -t "$TAG" "platform=$PLATFORM sku=$SKU_VERSION variant=$VARIANT"
