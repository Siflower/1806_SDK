#!/bin/ash
# Copyright (C) 2006 OpenWrt.org

. /lib/sfwifi.sh
. /usr/share/libubox/jshn.sh

echo "------------------------clear dmesg---------------------------"
dmesg -c > log3

ID=$(ps | grep "rf_hb_test" | grep -v "$0" | grep -v "grep" | awk '{print $1}')
if [ -n "$ID" ];then
	kill $ID
fi
ID=$(ps | grep "rf_lb_test" | grep -v "$0" | grep -v "grep" | awk '{print $1}')
if [ -n "$ID" ];then
	kill $ID
fi

echo "-------------------------test4 start---------------------------"
echo "-------------------------test4 log---------------------------" > log_rf
sfwifi remove > log3

if [ -z "$1" ]
then 
	max=8640
	echo "------------------it will last 1440 minute-----------------------"
else
	max=$(($1 * 3))
	echo "------------------it will last $1 minute-------------------------"
fi

load_firmware
insmod_rf
insmod_smac

rf_hb_test &
rf_lb_test &

timer=0
while [ $timer -lt $max ]
do	
	timer=$(($timer + 1))
	grep -q -E "rf_set_channel fail|aet_trx_init fail" log1
	if [ "$?" -eq "0" ]
	then
		echo "hb_test test failed,please check log1!" >> log_rf
		echo "hb_test test failed,please check log1!"
		break
	fi

	grep -q -E "rf_set_channel fail|aet_trx_init fail" log2
	if [ "$?" -eq "0" ]
	then
		echo "lb_test test failed,please check log2!" >> log_rf
		echo "lb_test test failed,please check log2!"
		break
	fi

	grep -q "test end" log1
	if [ "$?" -eq "0" ]
	then
		echo "hb_test end!"
		break
	fi

	grep -q "test end" log2
	if [ "$?" -eq "0" ]
	then
		echo "lb_test end!"
		break
	fi

	time=$(($timer * 20))
	echo "--------------running $time sec-------------- "
	sleep 20
done
ID=$(ps | grep "rf_hb_test" | grep -v "$0" | grep -v "grep" | awk '{print $1}')
if [ -n "$ID" ];then
	kill $ID
fi
ID=$(ps | grep "rf_lb_test" | grep -v "$0" | grep -v "grep" | awk '{print $1}')
if [ -n "$ID" ];then
	kill $ID
fi
rm log3 -rf

grep -q "failed" log_rf
if [ "$?" -eq "0" ]
then
	echo "failed" >> log_rf
else
	echo "success" >> log_rf
fi
sfwifi reset
exit
