# Creates symlinks relative to the PRODUCT_OUT folder.
# This is different from BOARD_ROOT_EXTRA_SYMLINKS in that it allows symlinks on other partitions.
#
# Links can be specified in TARGET:DIRECTORY or TARGET:LINK_NAME notation. This creates a link named LINK_NAME or DIRECTORY/$(basename TARGET) pointing to TARGET.
#
# This script is based on conversion from BOARD_ROOT_EXTRA_SYMLINKS to LOCAL_POST_INSTALL_CMD from system/core/rootdir/Android.mk.

ifeq ($(SONY_SYMLINKS),)
    $(warning "No symlinks set!")
else
LOCAL_POST_INSTALL_CMD := $(foreach s,$(SONY_SYMLINKS), \
    $(eval p := $(subst :,$(space),$(s))) \
    mkdir -p $(dir $(PRODUCT_OUT)/$(word 2,$(p))); \
    ln -sf $(word 1,$(p)) $(PRODUCT_OUT)/$(word 2,$(p)); \
)
endif

include $(BUILD_PHONY_PACKAGE)
