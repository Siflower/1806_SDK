#!/bin/ash
# Copyright (C) 2006 OpenWrt.org

echo "------------------------clear dmesg---------------------------"
dmesg -c > log2
echo "-------------------------test2 start---------------------------" 
echo "~~~~~~~~~~~~~~~~~~~~~test2 log~~~~~~~~~~~~~~~~~~~~~~~~" > log_rf
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
	sleep 2
	for j in 1 2 3; 
	do
		for i in 5180 5190 5200 5210 5220 5230 5240 5260 5280 5300 5320 5500 5520 5540 5560 5580 5600 5580 5600 5620 5640 5660 5700 5720 5745 5765 5785 5805 5825;do
		ate_cmd wlan1 fastconfig -n 100 -l 1024 -f $i -c $i -w $j -u $j -m 4 -i 8 -g 1 -p 31 -t
		sleep 1
		ate_cmd wlan1 fastconfig -s
		sleep 1
		dmesg | grep -n "Rsp Error" >> log_rf
		dmesg -c > log2
		grep -q "Rsp Error" log2
		if [ "$?" -eq "0" ]
		then
			echo "error in Channel:$i Bw:$j" >> log_rf
			echo "timer:$timer failed" >> log_rf
			sfwifi reset
			exit
		fi
		grep -q "check status failed" log2
		if [ "$?" -eq "0" ]
		then
			grep -n -C 4 "check status failed" log2 >> log_rf
			echo "timer:$timer HB" >>log_rf
			sfwifi reset
			exit
		fi
		
		sleep 1
		done
	done

	for j in 1 2;
	do
		for i in 2412 2417 2422 2427 2432 2437 2442 2447 2452 2457 2462 2467 2472 2484;do
		ate_cmd wlan0 fastconfig -n 100 -l 1024 -f $i -c $i -w $j -u $j -m 2 -i 7 -g 1 -p 31 -t
		sleep 1
		ate_cmd wlan0 fastconfig -s
		sleep 1
		dmesg | grep -n "Rsp Error" >> log_rf
		dmesg -c > log2
		grep -q "Rsp Error" log2
		if [ "$?" -eq "0" ]
		then
			echo "error in Channel:$i Bw:$j" >> log_rf
			echo "timer:$timer failed" >>log_rf
			sfwifi reset
			exit
		fi
		
		grep -q "check status failed" log2
		if [ "$?" -eq "0" ]
		then
			grep -n -C 4 "check status failed" log2 >> log_rf
			echo "timer:$timer LB" >>log_rf
			sfwifi reset
			exit
		fi
		sleep 1
		done
	done

	echo "-------------timer is $timer -----------------------------"
done
sfwifi reset
echo "----------------- test success -----------------"
echo "success" >> log_rf
