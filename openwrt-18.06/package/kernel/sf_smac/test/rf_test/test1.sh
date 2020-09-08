#!/bin/ash
# Copyright (C) 2006 OpenWrt.org

. /lib/sfwifi.sh
. /usr/share/libubox/jshn.sh

firmware_remove(){
	#stop wifi first
	/sbin/wifi down
	sleep 1
	#unregist umac
	unload_smac
	#unregist lmac
	unload_firmware
	#unregist rf
	unload_rf
}

firmware_reload(){
	local band="$2"
	#reload firmware
	load_firmware
	#reload rf
	insmod_rf
	[ lb = "$band" ] && {
		insmod_umac_lb
	}
	[ hb = "$band" ] && {
		insmod_umac_hb
	}
	[ -z "$band" ] && {
		insmod_smac
	}
}

echo "---------------------clear dmesg---------------------"
dmesg -c > log2
sfwifi remove > log2
echo "---------------------test1 start----------------------"
echo "---------------------test1 log--------------------------" > log_rf
if [ -z "$1" ]
then 
	timer=1
else
	timer=$1
fi

while [ $timer -gt 0 ];
do
	timer=$(($timer - 1))
	firmware_reload
	sleep 1
	firmware_remove
	dmesg | grep -n "app status is bad" >> log_rf
	dmesg -c > log2
	grep -q "app status is bad" log2
	if [ "$?" -eq "0" ]
	then
		echo "timer:$timer" >> log_rf
		echo "failed" >> log_rf
		echo "----------------- test fail! (cat log_rf)----------------"
		sfwifi reset
		exit
	fi
	sleep 1
	echo "-------------timer is $timer -----------------------------"
done
sfwifi reset
echo "----------------- test success -----------------"
echo "success" >> log_rf
