#!/bin/sh

mac=$1
name=$2
ip=$3

namemac=${mac//:/_}

if [ ! -f "/etc/config/bindtable" ]; then
        echo "" > /etc/config/bindtable
fi


if [ "x$ip" != "x" -a "x$mac" != "x" ]; then
        echo "$mac $ip" >> /etc/ethers
        uci set bindtable."$namemac"=device
        uci -q set bindtable."$namemac".name="$name"
        uci -q set bindtable."$namemac".mac="$mac"
        uci -q set bindtable."$namemac".ip="$ip"
fi

uci commit
/etc/init.d/dnsmasq restart &
/etc/init.d/network reload
