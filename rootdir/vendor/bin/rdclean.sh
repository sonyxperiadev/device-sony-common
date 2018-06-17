#!/system/bin/sh

slot_suffix="$(getprop ro.boot.slot_suffix)"
dev="/dev/block/bootdevice/by-name/rdimage$slot_suffix"

tag=`dd if=$dev bs=1 count=8 2>/dev/null`
[ "$tag" != "ANDROID!" ] && {
  tag=`echo $tag | sed -e 's,^.\(...\).*$,\1,'`
  [ "$tag" != "ELF" ] && exit 0
}

log -p w -t rdclean clearing ramdump partition
cat /dev/zero > $dev
