#!/vendor/bin/sh

/vendor/bin/log -p w -t "vendor.restorecon_persist" "Resetting RESTORECON_LAST xattr for /mnt/vendor/persist/"
/vendor/bin/toybox_vendor setfattr -x security.restorecon_last /mnt/vendor/persist/
