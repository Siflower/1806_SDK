#!/bin/ash
# Copyright (C) 2006 OpenWrt.org

echo "------------------------clear dmesg---------------------------"
dmesg -c > log2
echo "-------------------------test6 start---------------------------" 
echo "~~~~~~~~~~~~~~~~~~~~~test6 log~~~~~~~~~~~~~~~~~~~~~~~~" > log_rf
if [ -z "$1" ]
then
	timer=1
else
	timer=$1
fi

while [ $timer -gt 0 ];
do
	timer=$(($timer - 1))
	ate_init
	dmesg -c > log2
	sleep 1
	ate_cmd wlan1 fastconfig -n 1000000 -l 1024 -f 5180 -c 5180 -w 1 -u 1 -m 2 -i 4 -g 0 -p 31 -t
	sleep 1
	ate_cmd wlan0 fastconfig -n 1000000 -l 1024 -f 2412 -c 2412 -w 0 -u 0 -m 0 -i 4 -g 0 -p 31 -t
	sleep 10
	dmesg -c >log2
	grep -c "sf_wifi_rf_irq_handle" log2
	if [ "$?" -gt "2" ]
	then
			ate_cmd wlan1 fastconfig -s
			ate_cmd wlan0 fastconfig -s
			echo "failed" >> log_rf
			sfwifi reset
			exit
	fi
	ate_cmd wlan1 fastconfig -s
	ate_cmd wlan0 fastconfig -s
	echo "-------------timer is $timer -----------------------------"
done
sfwifi reset
echo "success" >> log_rf
echo "------------success-----------------"
