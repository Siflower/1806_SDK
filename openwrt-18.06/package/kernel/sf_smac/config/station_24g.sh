#!/bin/sh
uci set wireless.@wifi-iface[0].mode='sta'
uci set wireless.@wifi-iface[0].network='wwan'
uci set wireless.@wifi-iface[0].ssid='OpenWrt-xxxx'
uci set wireless.@wifi-iface[0].bssid='XX:XX:XX:XX:XX:XX'
uci set network.lan.ipaddr='192.168.5.1'
uci commit wireless
uci commit network
/etc/init.d/network restart
