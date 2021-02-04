#!/bin/sh

run_master(){
#	echo ondemand > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
	WIFI_NUM=`cat /etc/wifi_num`
	ifconfig sfi0 192.100.$WIFI_NUM.55
	iperf -c 192.100.$WIFI_NUM.1 -d -i 1 -t 10000000
}

run_slave(){
#	echo ondemand > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
	WIFI_NUM=`cat /etc/wifi_num`
	ifconfig sfi1 192.168.$WIFI_NUM.55
	iperf -c 192.168.$WIFI_NUM.1 -d -i 1 -t 10000000
}

if [ -f /etc/init.d/get_dhcpserver_status ];then
/etc/init.d/get_dhcpserver_status stop
fi
if [ -f /etc/init.d/get_ip_status ];then
/etc/init.d/get_ip_status stop
fi

touch /tmp/dual_band_test
case "$1" in
	master) run_master;;
	slave) run_slave;;
	*) run_master;;
esac

rm /tmp/dual_band_test
