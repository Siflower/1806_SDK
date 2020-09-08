#!/bin/ash
# Copyright (C) 2006 OpenWrt.org

echo "------------------------clear dmesg---------------------------"
dmesg -c > log3
echo "-------------------------test3 start---------------------------" 
echo "~~~~~~~~~~~~~~~~~~~~~test3 log~~~~~~~~~~~~~~~~~~~~~~~~" > log_rf
if [ -z "$1" ]
then
	timer=1
else
	timer=$1
fi

while [ $timer -gt 0 ];
do
	timer=$(($timer - 1))
		killall -q iw
		iw wlan0 scan > log1 &
		iw wlan1 scan > log2 &
		sleep 2
		dmesg | grep -n "Rsp Error" >> log_rf
		dmesg -c > log3
		grep -q "Rsp Error" log3
		if [ "$?" -eq "0" ]
		then
			grep -q "002B Rsp Error" log3
			if [ "$?" -eq "0" ]
			then
				grep -n -C 4 "002B Rsp Error" log3 >> log_rf
			else
				echo "Rsp Error:timer:$timer" >>log_rf
				echo "failed" >> log_rf
				exit
			fi
		fi
		
		grep -q "RF is still busy" log3
		if [ "$?" -eq "0" ]
		then
			grep -n -C 4 "RF is still busy" log3 >> log_rf
			echo "timer:$timer" >>log_rf
			echo "failed" >> log_rf
			exit
		fi

		grep -q "check status failed" log3
		if [ "$?" -eq "0" ]
		then
			grep -n -C 4 "check status failed" log3 >> log_rf
			echo "-----timer:$timer----" >>log_rf
			echo "failed" >> log_rf
			exit
		fi
	echo "-------------timer is $timer -----------------------------"
done
echo "----------------- test success -----------------"
echo "success" >> log_rf
