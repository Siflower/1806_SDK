#!/bin/sh

default_lan_proto=`uci get network.lan.proto`
default_ports_lan=`uci get network.@switch_vlan[0].ports`
default_ports_wan=`uci get network.@switch_vlan[1].ports`
default_ports=`uci get network.@switch_vlan[1].ports | awk -F ' ' '{print $2}'`
port_str=${default_ports_lan/$default_ports/$default_ports_wan}

set_wan() {
	if [ $1 -ne 0 ]; then
		port_n=`expr $1 - 1`' '
		port_lan=${port_str/$port_n/''}
		port_wan=${port_n}${default_ports}
	else
		port_lan=$default_ports_lan
		port_wan=$default_ports_wan
	fi
	uci set network.@switch_vlan[0].ports="$port_lan"
	uci set network.@switch_vlan[1].ports="$port_wan"
	uci commit
}

checkport() {
	a=0
	rv=0
	for i in `seq 1 5`
	do
		{
			udhcpc -n -q -t 1 -i eth0."$i">&- && rv=$i
			[ $rv -ne 0 ] && echo $rv > /tmp/wanlan_set
		}&
	done
	wait
	if [ -f "/tmp/wanlan_set" ]; then
		a=`cat /tmp/wanlan_set`
		rm /tmp/wanlan_set
	fi
	echo $a
}

init() {
	uci set network.lan.proto='dhcp'
	uci delete network.lan.type
	uci set network.@switch_vlan[0].ports="0 $default_ports"
	uci set network.@switch_vlan[1].ports="1 $default_ports"
	for i in `seq 3 5`
	do
		uci set network.wan"$i"='interface'
		uci set network.wan"$i".ifname=eth0."$i"
		uci set network.wan"$i".proto=dhcp
	done
	for i in `seq 3 5`
	do
		a=`uci add network switch_vlan`
		uci set network.$a.device="switch0"
		uci set network.$a.vlan="$i"
		uci set network.$a.ports="`expr $i - 1` $default_ports"
	done
	uci commit
	/etc/init.d/network restart
}

dinit() {
	for i in `seq 3 5`
	do
		uci delete network.wan"$i"
		uci delete network.@switch_vlan[2]
	done
	uci set network.lan.proto=$default_lan_proto
	uci set network.lan.type="bridge"
	uci commit
}

start() {
	init
	sleep 6
	port=0
	for i in `seq 1 3`
	do
		port=$(checkport)
		if [ $port -ne 0 ]; then
			set_wan $port
			break
		fi
		sleep 1
	done
	[ $port -eq 0 ] && set_wan 0
	dinit
	/etc/init.d/network restart
}

waiting() {
	rv=0
	while true
	do
		udhcpc -n -q -t 1 -i br-lan>&- && rv=1
		if [ $rv -eq 1 ]; then
			start
			rv=0
		fi
		sleep 3
	done
}

case $1 in
	start)
		start
		;;
	set)
		set_wan $2
		/etc/init.d/network restart
		;;
	wait)
		waiting
		;;
	*)
		echo "commond error"
		;;
esac
