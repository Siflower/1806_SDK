#!/bin/sh

. /lib/functions.sh

INSMOD="/sbin/insmod"
RMMOD="/sbin/rmmod"

load_firmware()
{
    cmd="/sbin/insmod startcore"
    eval $cmd
}

unload_firmware()
{
    cmd="/sbin/rmmod startcore"
    eval $cmd
}

insmod_rf() {
#   umount /sys/kernel/debug
#   mount -t debugfs none /sys/kernel/debug
    modprobe sf16a18_rf
    sleep 1
}

unload_rf() {
    cmd="/sbin/rmmod sf16a18_rf"
    eval $cmd
    sleep 1
}

insmod_umac(){
    cmd="/sbin/insmod $1"
    eval $cmd
}

insmod_mac80211(){
    modprobe mac80211
}

insmod_cfg80211(){
    modprobe cfg80211
}

insmod_fmac() {
    insmod_umac sf16a18_fmac
}

unload_fmac(){
    cmd="/sbin/rmmod sf16a18_fmac"
    eval $cmd
}
