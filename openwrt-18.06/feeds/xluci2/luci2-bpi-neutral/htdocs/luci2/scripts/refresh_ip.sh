#!/bin/sh

i=0
echo '' > /etc/ethers
sed -i '1d' /etc/ethers
while true
do
	device=`uci -q get bindtable.@device[$i]`
	if [ "$device" != "device" ]; then
		break
	fi
	mac=`uci -q get bindtable.@device[$i].mac`
	ip=`uci -q get bindtable.@device[$i].ip`
	echo "$mac $ip" >> /etc/ethers
	i=$((i+1))
done

/etc/init.d/dnsmasq restart &
/etc/init.d/network reload
