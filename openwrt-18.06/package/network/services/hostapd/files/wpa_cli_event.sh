#!/bin/ash
#echo "~$1~$2~$3~" > /dev/ttyS0
. /usr/share/led-button/wps_func.sh

wds_if="$1"
path_led="/sys/class/leds/siwifi-"
wps_enabled=0

if [ -e "/tmp/dual_band_test" ]; then
	exit 0
fi

check_wps() {
	wps_status=0
	[ -f /tmp/wps_status ] && {
		wps_status=`cat /tmp/wps_status`
	}
}

prepare_params() {
	case $wds_if in
		sfi0)
			trig=phy0
			band=24g
			;;
		sfi1)
			trig=phy1
			band=5g
			;;
	esac
}

del_clients() {
	local cnt
	local device
	local clients

	# eth devices
	if [[ "$1" == "0" || "$1" == "2" ]]; then
		ifconfig eth0 down
		sleep 1
		ifconfig eth0 up
	fi

	# wifi devices
	if [[ "$1" == "1" || "$1" == "2" ]]; then
		cnt=`ubus list | grep -c hostapd`
		while [ $cnt -gt 0 ]
		do
			local ccnt
			#echo "cnt is $cnt" > /dev/ttyS0
			device=`ubus list | grep hostapd | awk 'NR=="'$cnt'" {print $1}'`
			#echo "device is $device" > /dev/ttyS0
			#clients=`ubus call $device get_clients | awk '/:..:/ {sub(/.$/,"",$1); print $1}'`
			clients=`ubus call $device get_clients | awk '/:..:/ {sub(/\":/,"",$1);sub(/\"/,"",$1); print $1}'`
			#echo "clients is $clients" > /dev/ttyS0
			ccnt=`echo "$clients" | grep -c ""`
			while [ $ccnt -gt 0 ]
			do
				local cli
				cli=`echo "$clients" | awk 'NR=="'$ccnt'" {print $1}'`
				#echo "cli is $cli" > /dev/ttyS0
				[ -n "$cli" ] && ubus call $device del_client '{"addr":"'$cli'", "reason":3, "deauth": True}'
				let ccnt=$ccnt-1
			done
			let cnt=$cnt-1
		done
	fi
}

if [[ "$wds_if" == "sfi0" || "$wds_if" == "sfi1" ]]; then
	check_wps
	prepare_params

	if [ "$2" == "WPS-SUCCESS" ]; then
		[ "$wps_status" == 0 ] && {
			echo "$wds_if" > /tmp/wps_status
			#echo "wps~select~$wds_if~" > /dev/ttyS0
		}
		#TODO  LED control should spilt in func.
	fi

	if [ "$2" == "WPS-FAIL" ]; then
		#echo "~$1~$2~rm wps_status" > /dev/ttyS0
		uci_delete_wireless_iface "sfi0"
		uci_delete_wireless_iface "sfi1"
		# acturally we do not need delete network here, just in case.
		uci_delete_network
		uci commit wireless
		output=`wifi reload`
		[ -f /tmp/wps_status ] && rm /tmp/wps_status
	fi

	if [ "$2" == "CONNECTED" ]; then
		if [ "$wps_status" != 0 ]; then
			if [ "$wds_if" != "$wps_status" ]; then
				#echo "wps $wps_status now $wds_if so exit 0" > /dev/ttyS0
				exit 0
			fi
			#shall we change wps_status here to avoid setting more than once?
			/usr/share/led-button/wps_config.sh $wds_if $band
			exit 0
		fi

		#echo "wpa_cli_evt: connected" > /dev/ttyS0
		[ -f "tmp/wds_sta_status" ] && busy=`cat /tmp/wds_sta_status`
		while [ "$busy" == "b" ]
		do
			busy=`cat /tmp/wds_sta_status`
		done
		echo "b" > /tmp/wds_sta_status

		# disable lan dhcp server
		uci set dhcp.lan.ignore=1
		uci commit dhcp
		output=`/etc/init.d/dnsmasq reload`

		sta_status=`uci get network.stabridge`
		[ "$sta_status" = "interface" ] && set_channel "$wds_if"

		if [ -d "$path_led""$trig""::tx" ]; then
			echo "$trig""tx" > "$path_led""$trig""::tx"/trigger
		fi
		if [ -d "$path_led""$trig""::rx" ]; then
			echo "$trig""rx" > "$path_led""$trig""::rx"/trigger
		fi
		echo "0" > /tmp/wds_sta_status
	fi

	if [ "$2" == "DISCONNECTED" ]; then
		[ "$wps_status" != 0 ] && exit 0
		#echo "wpa_cli_evt: disconnected" > /dev/ttyS0
		[ -f "tmp/wds_sta_status" ] && busy=`cat /tmp/wds_sta_status`
		while [ "$busy" == "b" ]
		do
			busy=`cat /tmp/wds_sta_status`
		done
		echo "b" > /tmp/wds_sta_status

		# enable lan dhcp server
		uci set dhcp.lan.ignore=0
		uci commit dhcp
		output=`/etc/init.d/dnsmasq reload`

		# kick out devices, 0 for eth devices, 1 for wifi, 2 for all
		del_clients 2

		is_enabled=`ifconfig | grep "$1"`
		if [ "$is_enabled" == "" ]; then
			echo "1" > /tmp/wds_sta_status
			exit 0
		fi

		if [ -d "$path_led""$trig""::tx" ]; then
			echo "timer" > "$path_led""$trig""::tx"/trigger
			echo "100" > "$path_led""$trig""::tx"/delay_on
			echo "100" > "$path_led""$trig""::tx"/delay_off
		fi
		if [ -d "$path_led""$trig""::rx" ]; then
			echo "timer" > "$path_led""$trig""::tx"/trigger
			echo "100" > "$path_led""$trig""::tx"/delay_on
			echo "100" > "$path_led""$trig""::tx"/delay_off
		fi

		echo "1" > /tmp/wds_sta_status

	fi

	# This is called by wds_sta_is_disconnected() in wirelessnew.lua
	if [ "$2" == "RECONNECT" ]; then
		[ -f "tmp/wds_sta_status" ] && busy=`cat /tmp/wds_sta_status`
		while [ "$busy" == "b" ]
		do
			busy=`cat /tmp/wds_sta_status`
		done
		echo "b" > /tmp/wds_sta_status

		wpa_cli reconfigure
		wpa_cli scan
		result=`wpa_cli scan_result | grep $3 -i`
		if [ "$result""x" != "x" ]; then
			# host exists, try again
			echo "1" > /tmp/wds_sta_status
		else
			# no host
			echo "2" > /tmp/wds_sta_status
		fi
	fi
fi
#echo "wpa_cli_event done!" > /dev/ttyS0
