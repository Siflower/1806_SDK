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

