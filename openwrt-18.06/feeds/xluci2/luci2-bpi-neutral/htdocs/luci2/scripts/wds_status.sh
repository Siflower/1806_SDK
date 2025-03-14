#!/bin/sh

set_config(){
	device=`uci get wireless.wds.device`
	oc=`uci get wireless.$device.origin_channel`
	if [ -n "$oc" ]; then
		uci set wireless.$device.channel=$oc
	fi
	uci del wireless.wds
	ip=`uci get network.lan.oip`
	if [ -n "$ip" ]; then
		uci set network.lan.proto='static'
		uci set network.lan.ipaddr=$ip
		uci set network.lan.netmask='255.255.255.0'
		uci set network.lan.ip6assign='60'
	fi
	if [ "$device" == "radio0" ];then
		ossid=`uci get wireless.default_radio0.ossid`
		uci set wireless.default_radio0.ssid=$ossid
	else
		ossid=`uci get wireless.default_radio1.ossid`
		uci set wireless.default_radio1.ssid=$ossid
	fi

	uci commit
	/etc/init.d/network restart
}

check_lan_ip(){
	loop_count=12
	while [ $loop_count -gt 0 ]; do
		ip=`ubus call network.interface.lan status | grep "ipv4-address" -C 3 | grep '"address"' | awk -F '"' '{print $4}'`
		if [ -n "$ip" ] ;then
			hosts=$(uci get basic_setting.vendor.hosts 2>/dev/null)
			if [ -n "$hosts" ]; then
				/usr/sbin/dns_redirect.sh $hosts enable "$ip"
			else
				/etc/init.d/dnsmasq stop
			fi
			sh /sbin/internet_detect.sh 3
			exit 0
		else
			sleep 5
			loop_count=$((loop_count - 1))
		fi
	done

	if [ $loop_count -eq 0 ]; then
		set_config
		exit 0
	fi
}

loop_count=12

while [ $loop_count -gt 0 ]; do
    if [ "$1" == "wlan0-1" ]; then
        status=`cat /sys/kernel/debug/ieee80211/phy0/siwifi/repeater_status`
    else
        status=`cat /sys/kernel/debug/ieee80211/phy1/siwifi/repeater_status`
    fi

    encryption=`uci get wireless.wps.encryption`

    if [ "$encryption" == "" ] && [ "$status" == "repeater assoc!" ]; then
        check_lan_ip
        exit 0
    elif [ "$status" == "repeater eapol!" ]; then
        check_lan_ip
        exit 0
    else
        sleep 5
        loop_count=$((loop_count - 1))
    fi
done

if [ $loop_count -eq 0 ]; then
    wifi
    sleep 15
    loop_count=12
fi

while [ $loop_count -gt 0 ]; do
    if [ "$1" == "wlan0-1" ]; then
        status=`cat /sys/kernel/debug/ieee80211/phy0/siwifi/repeater_status`
    else
        status=`cat /sys/kernel/debug/ieee80211/phy1/siwifi/repeater_status`
    fi

    encryption=`uci get wireless.wps.encryption`

    if [ "$encryption" == "" ] && [ "$status" == "repeater assoc!" ]; then
        check_lan_ip
        exit 0
    elif [ "$status" == "repeater eapol!" ]; then
        check_lan_ip
        exit 0
    else
        sleep 5
        loop_count=$((loop_count - 1))
    fi
done

if [ $loop_count -eq 0 ]; then
    set_config
fi
