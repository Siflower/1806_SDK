#!/bin/sh
uci set wireless.@wifi-iface[1].mode='sta'
uci set wireless.@wifi-iface[1].network='wwan'
uci set wireless.@wifi-iface[1].ssid='OpenWrt-5G-xxxx'
uci set wireless.@wifi-iface[1].bssid='XX:XX:XX:XX:XX:XX'
uci set network.lan.ipaddr='192.168.5.1'
uci commit wireless
uci commit network
/etc/init.d/network restart
ifconfig wlan1 192.168.4.55
