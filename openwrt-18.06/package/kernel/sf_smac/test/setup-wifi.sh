#!/bin/sh
echo "setup wifi $1 $2" > /dev/ttyS0

#for 86v like board detect dhcp status in service
if [ -f /etc/init.d/get_dhcpserver_status ];then
/etc/init.d/get_dhcpserver_status stop
fi
if [ -f /etc/init.d/get_dhcpserver_status ];then
/etc/init.d/get_ip_status stop
fi

lb_path=`ls sys/devices/platform/ | grep "lb"`
hb_path=`ls sys/devices/platform/ | grep "hb"`

get_channel_24g(){
	#set 24g channel
	channel_24g=$(($1 % 11))
	[ "$channel_24g" -eq 0 ] && channel_24g=1
}

get_channel_5g(){
	channel_5g=36
        case "$(($1 % 4))" in
        	1) channel_5g=161;;
        	2) channel_5g=48;;
        	3) channel_5g=149;;
        	0) channel_5g=36;;
	esac
}

prepare_common_wifi_config()
{
	ssid=$1
	cat <<EOF
	config wifi-device  radio0
	option type     mac80211
	option channel  ${channel_24g}
	option hwmode   11g
	option band     2.4G
	option country  'CN'
	option ht_coex  0
	option noscan   1
	option htmode 'HT20'
	option path 'platform/$lb_path'

	config wifi-iface default_radio0
	option device   radio0
	option ifname   wlan0
	option network  lan
	option mode     ap
	option ssid     OpenWrt-24g-${ssid}
	option encryption psk2+ccmp
	option key      12345678

	config wifi-device  radio1
	option type     mac80211
	option channel  ${channel_5g}
	option hwmode   11a
	option band     5G
	option country  'CN'
	option ht_coex  0
	option noscan   1
	option htmode 'VHT80'
	option path 'platform/$hb_path'

	config wifi-iface default_radio1
	option device   radio1
	option ifname   wlan1
	option network  lan
	option mode     ap
	option ssid     OpenWrt-5g-${ssid}
	option encryption psk2+ccmp
	option key      12345678

EOF

}

init_master(){
	rm /etc/config/wireless
	NUM_INT=$(($1 % 256))
	NUM=`printf "%02x" $NUM_INT`
	echo $NUM_INT > /etc/wifi_num
	get_channel_24g $1
	get_channel_5g $1
	prepare_common_wifi_config $1 > /etc/config/wireless
	#set 24g sta
	uci set wireless.@wifi-iface[0].mode='sta'
	uci set wireless.@wifi-iface[0].ifname='sfi0'
	uci set wireless.@wifi-iface[0].network='wwan'
	uci set wireless.@wifi-iface[0].ssid="OpenWrt-24g-$1"
	uci set wireless.@wifi-iface[0].bssid="12:16:88:21:19:$NUM"
	#set 5g ap mac addr&bssid
	uci set wireless.@wifi-iface[1].macaddr="14:16:88:21:19:$NUM"
	uci set wireless.@wifi-iface[1].ssid="OpenWrt-5g-$1"
	#set ip
	uci set network.lan.ipaddr="192.168.$NUM_INT.1"
	uci commit wireless
	uci commit network
	/etc/init.d/network restart
	sleep 3
	killall iperf
	iperf -s&
}

init_slave(){
	rm /etc/config/wireless
	NUM_INT=$(($1 % 256))
	NUM=`printf "%02x" $NUM_INT`
	echo $NUM_INT > /etc/wifi_num
	get_channel_24g $1
	get_channel_5g $1
	prepare_common_wifi_config $1 > /etc/config/wireless
	#set 5g sta
	uci set wireless.@wifi-iface[1].mode='sta'
	uci set wireless.@wifi-iface[1].ifname='sfi1'
	uci set wireless.@wifi-iface[1].network='wwan'
	uci set wireless.@wifi-iface[1].ssid="OpenWrt-5g-$1"
	uci set wireless.@wifi-iface[1].bssid="14:16:88:21:19:$NUM"
	#set 24g ap mac addr&bssid
	uci set wireless.@wifi-iface[0].macaddr="12:16:88:21:19:$NUM"
	uci set wireless.@wifi-iface[0].ssid="OpenWrt-24g-$1"
	#set ip
	uci set network.lan.ipaddr="192.100.$NUM_INT.1"
	uci commit wireless
	uci commit network
	/etc/init.d/network restart
	sleep 3
	killall iperf
	iperf -s&
}

case "$1" in
	master) init_master "$2";;
	slave) init_slave "$2";;
	*) init_master "$2";;
esac
