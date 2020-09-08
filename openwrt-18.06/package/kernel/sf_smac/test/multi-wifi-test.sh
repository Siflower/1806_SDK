#!/bin/sh

setup_channel(){

	#set 24g channel

	channel_24g=$(($1 % 11))
	[ "$channel_24g" -eq 0 ] && channel_24g=1
	uci set wireless.radio0.channel="$channel_24g"
	ch_5g=36
	case "$(($1 % 4))" in
		1) ch_5g=48;;
		2) ch_5g=64;;
		3) ch_5g=149;;
		0) ch_5g=36;;
	esac

	#set 5g channel
	uci set wireless.radio1.channel="$ch_5g"

}


init_ap(){
	rm /etc/config/wireless
	/sbin/wifi config > /etc/config/wireless
	commit_wireless
	hostssid="wifi-test-$1"

	#set all enalbed
	for i in $(seq 0 3)
	do
		uci -q set wireless.@wifi-iface[$i].disabled='0'
	done

	for i in $(seq 4 7)
	do
		uci add wireless wifi-iface > /dev/null                 #......ap......
		r=$(($i%2))                                             #......radio num......
		uci -q batch <<-EOF
			set wireless.@wifi-iface[$i].ifname=wlan${i}
			set wireless.@wifi-iface[$i].device=radio${r}
			set wireless.@wifi-iface[$i].network=lan
			set wireless.@wifi-iface[$i].mode=ap
			set wireless.@wifi-iface[$i].ssid="${hostssid}${i}"
			set wireless.@wifi-iface[$i].encryption="psk2+ccmp"
			set wireless.@wifi-iface[$i].key=12345678
EOF
	done
	uci -q commit wireless
}

commit_wireless()
{
	uci commit wireless
}

sync_wifi_down()
{
	echo "wifi down--1">dev/ttyS0
	/sbin/wifi down
	counter=20
	while [ $counter -gt 0 ]; do
		counter=$(($counter - 1))
		num=`ifconfig | grep wlan | wc -l`
		if [ $num -eq 0 ]; then
			break
		fi
		sleep 1
	done
	sleep 1
	echo "wifi down--2">dev/ttyS0
}

/etc/init.d/subservice stop
/etc/init.d/syncservice stop
#set recovery env
for _dev in /sys/class/ieee80211/*; do
	[ -e "$_dev" ] || continue
	dev="${_dev##*/}"
	echo 1 > /sys/kernel/debug/ieee80211/$dev/rwnx/recovery_enable
done

timer=1000000
while [ $timer -gt 0 ]; do
	timer=$(($timer - 1))
	echo "test num+++++++++++++++++++++++++++++++++$timer" > /dev/ttyS0
	init_ap $1
	commit_wireless
	ubus call network reload
	sleep 2
	sync_wifi_down
	echo "wifi up++">dev/ttyS0
	/sbin/wifi up
	counter=60
	cost=0
	num=0
	while [ $counter -gt 0 ]; do
		counter=$(($counter - 1))
		num=`ifconfig | grep wlan | wc -l`
		if  [ $num -eq 8 ]; then
			break
		fi
		sleep 1
		cost=$(($cost + 1))
	done
	if  [ $num -ne 8 ]; then
		echo "########################test fail########################">/dev/ttyS0
		echo "wifi interface num1 is $num">/dev/ttyS0
		echo "################################################">/dev/ttyS0
		break
	fi
	echo "wait wifi link up0 $cost seconds!">/dev/ttyS0
	rm /etc/config/wireless
	/sbin/wifi config> /etc/config/wireless
	uci -q set wireless.@wifi-iface[1].disabled='0'
	uci -q set wireless.@wifi-iface[3].disabled='0'
	commit_wireless
	sleep 2
	ubus call network reload
	sync_wifi_down
	echo "wifi up++">dev/ttyS0
	/sbin/wifi up
	num=0
	cost=0
	counter=60
	while [ $counter -gt 0 ]; do
		counter=$(($counter - 1))
		num=`ifconfig | grep wlan | wc -l`
		if  [ $num -eq 4 ]; then
			break
		fi
		sleep 1
		cost=$(($cost + 1))
	done
	if  [ $num -ne 4 ]; then
		echo "########################test fail########################">/dev/ttyS0
		echo "wifi interface num2 is $num">/dev/ttyS0
		echo "################################################">/dev/ttyS0
		break
	fi
	echo "wait wifi link up1 $cost seconds!">/dev/ttyS0
done
