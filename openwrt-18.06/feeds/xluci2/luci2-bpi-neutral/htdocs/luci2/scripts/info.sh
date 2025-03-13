#!/bin/sh
cat_operation(){
	phy=$1
	file=$2
	cat /sys/kernel/debug/ieee80211/$phy/siwifi/$file > /tmp/info/$phy/$file
}
get_wifi_info(){
	phy=$1
	mkdir /tmp/info/$phy
	cat_operation $phy "txq"
	cat_operation $phy "stats"
	cat_operation $phy "trx_stats"
	cat_operation $phy "lmactx"
	cat_operation $phy "lmacrx"
	cat_operation $phy "hwq"
	cat_operation $phy "mib"
	echo 1 > /sys/kernel/debug/ieee80211/$phy/siwifi/enable_rc
	mkdir /tmp/info/$phy/rc
	for a in `ls /sys/kernel/debug/ieee80211/$phy/siwifi/rc/`
	do
		mkdir /tmp/info/$phy/rc/$a
		cat_operation $phy "rc/$a/stats"
		cat_operation $phy "rc/$a/rx_rate"
	done
}
mkdir /tmp/info
get_wifi_info "phy0"
get_wifi_info "phy1"
tar -zcvf /www/info.tar.gz /tmp/info
rm -r /tmp/info
