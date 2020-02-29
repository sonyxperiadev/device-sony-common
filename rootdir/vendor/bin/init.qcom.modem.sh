#!/vendor/bin/sh

#
# Make modem config folder and copy firmware config to that folder for RIL
#

# Fill the current and previous version temporary env variables
cur_version_info=`cat /vendor/firmware_mnt/verinfo/ver_info.txt`

if [ -f /data/vendor/modem_config/ver_info.txt ]; then
    prev_version_info=`cat /data/vendor/modem_config/ver_info.txt`
else
    # Userdata is blank or modem_config is bad: this will trigger a
    # cur-prev ver mismatch and will copy modem configuration in data
    prev_version_info=""
fi

if [ ! -f /vendor/firmware_mnt/verinfo/ver_info.txt -o "$prev_version_info" != "$cur_version_info" ]; then
    # Version is different: delete everything and copy the new stuff over.
    rm -rf /data/vendor/modem_config

    mkdir /data/vendor/modem_config
    chmod 770 /data/vendor/modem_config

    # Copy the modem operator configurations: mbn_sw
    cp -r /vendor/firmware_mnt/image/modem_pr/mcfg/configs/* /data/vendor/modem_config/
    chown -hR radio.root /data/vendor/modem_config

    # For checking the current vs new version of the modem operator configurations
    cp /vendor/firmware_mnt/verinfo/ver_info.txt /data/vendor/modem_config/
    chown radio.root /data/vendor/modem_config/ver_info.txt

    # This file exists only on some versions of the modem_pr. Try copying it anyway.
    cp /vendor/firmware_mnt/image/modem_pr/mbn_ota.txt /data/vendor/modem_config
    chown radio.root /data/vendor/modem_config/mbn_ota.txt

    # To support both libril-qc-hal-qmi and libril-qc-qmi-1 paths
    rm /data/vendor/radio/modem_config
    ln -s /data/vendor/modem_config /data/vendor/radio/modem_config
fi

# This file is recreated at every boot (for reasons) by init.rc: set it right.
echo 1 > /data/vendor/radio/copy_complete

