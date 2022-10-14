#!/bin/sh

onoff=$1

if [ "$onoff" == "0" ]; then
	uci set firewall.@defaults[0].flow_offloading=0
	uci set firewall.@defaults[0].flow_offloading_hw=0
	uci commit firewall
	fw3 -q reload
	echo vldclean > /sys/kernel/debug/hnat_debug
else
	uci set firewall.@defaults[0].flow_offloading=1
	uci set firewall.@defaults[0].flow_offloading_hw=1
	uci commit firewall
	fw3 -q reload
fi
