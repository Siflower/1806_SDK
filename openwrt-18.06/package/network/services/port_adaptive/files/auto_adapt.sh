#!/bin/sh

default_ports_lan=`uci get network.@switch_vlan[0].ports`
default_ports_wan=`uci get network.@switch_vlan[1].ports`
default_ports=`uci get network.@switch_vlan[1].ports | awk -F ' ' '{print $2}'`
port_str=${default_ports_lan/$default_ports/$default_ports_wan}

set_wan() {
	if [ $1 -ne 0 ]; then
		port_n=`expr $1 - 1`' '
		port_lan=${port_str/$port_n/''}
		port_wan=${port_n}${default_ports}
	fi
	uci set network.@switch_vlan[0].ports="$port_lan"
	uci set network.@switch_vlan[1].ports="$port_wan"
	uci commit
}

dinit() {
	cp /tmp/adapt_network /etc/config/network
	rm /tmp/adapt_network
}

kill_all() {
	pid_fd=`ps | grep "send" |grep "eth0." | grep -v "grep" | awk -F ' ' '{print $1}'`
	kill $pid_fd
}

checkport() {
	a=0
	rv=0
	for i in `seq 1 5`
	do
		{
			send eth0."$i" &
		}&
	done
	wait
	sleep 1
	kill_all
}

init() {
	cp /etc/config/network /tmp/adapt_network
	echo " " > /etc/config/network
	for i in `seq 1 5`
	do
		uci set network.wan"$i"='interface'
		uci set network.wan"$i".ifname=eth0."$i"
		uci set network.wan"$i".proto=dhcp
	done

	a=`uci add network switch`
	uci set network.$a.name="switch0"
	uci set network.$a.reset="1"
	uci set network.$a.enable_vlan="1"

	for i in `seq 1 5`
	do
		a=`uci add network switch_vlan`
		uci set network.$a.device="switch0"
		uci set network.$a.vlan="$i"
		uci set network.$a.ports="`expr $i - 1` $default_ports"
	done
	uci commit
	/etc/init.d/network reload
}

start() {
	a=0
	init
	rm /tmp/dhcp_iface
	for i in `seq 1 3`
	do
		checkport
		if [ -f "/tmp/dhcp_iface" ]; then
			a=`cat /tmp/dhcp_iface | awk -F '.' '{print $2}'`
			rm /tmp/dhcp_iface
			break
		fi
		sleep 1
	done
	dinit
	if [ "$a" -ne 0 ];then
		set_wan $a
	fi
	/etc/init.d/network reload &
}

case $1 in
	start)
		start
		;;
	set)
		set_wan $2
		/etc/init.d/network reload
		;;
	*)
		echo "commond error"
		;;
esac
