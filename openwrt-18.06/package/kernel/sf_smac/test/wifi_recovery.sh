#!/bin/sh

#set recovery env
for _dev in /sys/class/ieee80211/*; do
	[ -e "$_dev" ] || continue
	dev="${_dev##*/}"
	echo 1 > /sys/kernel/debug/ieee80211/$dev/siwifi/diags/force_trigger_type
	echo 1 > /sys/kernel/debug/ieee80211/$dev/siwifi/recovery_enable
done

timer=100000
while [ $timer -gt 0 ]; do
	timer=$(($timer - 1))
	for _phy in /sys/class/ieee80211/*; do
		[ -e "$_phy" ] || continue
		phy="${_phy##*/}"
		cat /sys/kernel/debug/ieee80211/$phy/siwifi/diags/mactrace
		sleep 1
		while [ 1 ]; do
			state=`cat /sys/kernel/debug/ieee80211/$phy/siwifi/run_state`
			if [ "$(($state&1))" -eq "0" ]; then
				echo "+++++++++++++++++++++++++++++++++++++++++++++++timer=$timer" > /dev/ttyS0
				break
			else
				sleep 1
			fi
		done
	done
done
