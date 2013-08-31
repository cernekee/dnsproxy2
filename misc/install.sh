#!/bin/bash

set -ex

adb push libs/armeabi-v7a/dnsproxy2 /data/local/tmp
adb push res/raw/20dnsproxy2 /data/local/tmp
adb shell "su -c 'mount -o rw,remount /system'"
adb shell "su -c 'cp -f /data/local/tmp/dnsproxy2 /system/xbin/'"
adb shell "su -c 'cp -f /data/local/tmp/20dnsproxy2 /system/etc/init.d/'"
adb shell "su -c 'chmod 755 /system/xbin/dnsproxy2 /system/etc/init.d/20dnsproxy2'"
adb shell "su -c 'mount -o ro,remount /system'"
adb shell "su -c 'rm -f /data/local/tmp/dnsproxy2 /data/local/tmp/20dnsproxy2'"
adb reboot

exit 0
