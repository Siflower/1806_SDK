#!/bin/sh


switch_type=""
have_phy4=`cat sys/kernel/debug/gsw_debug |grep phy4`

if [ -n "$have_phy4=" ]; then
	switch_type="realtek8367c"
else
	switch_type="intel7084"
fi

vlan1=`uci get network.@switch_vlan[0].ports`

if [ "$switch_type" == "realtek8367c" -a "$vlan1" != "1 2 3 4 16t" ]; then
	# modify wanLinkStatus
	`sed -i 's/phy3/phy0/g'  /bin/wanLinkStatus`

	# regenerate network, use uci set network may not completely take effect
	`sed -i '13c "1:lan:1" "2:lan:1" "3:lan:1" "4:lan:1" "0:wan:2" "16t@eth0"' /etc/board.d/01_network`
	rm /etc/board.json /etc/config/network
	/bin/board_detect
	sleep 1
	/etc/init.d/network restart
elif [ "$switch_type" == "intel7084" -a "$vlan1" != "0 1 2 5t" ]; then
	# change wanLinkStatus
	`sed -i 's/phy0/phy3/g'  /bin/wanLinkStatus`

	# regenerate network, use uci set network may not completely take effect
	`sed -i '13c "0:lan:1" "1:lan:1" "2:lan:1" "3:wan:2" "5t@eth0"' /etc/board.d/01_network`
	rm /etc/board.json /etc/config/network
	/bin/board_detect
	sleep 1
	/etc/init.d/network restart
fi
