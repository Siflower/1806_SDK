#!/bin/ash

sfwifi reset

#wait all ready

max=$3
last_cali_work_num=0

sleep 50

if [ $3 == "down" ]; then
	ifconfig wlan0 down
	ifconfig wlan1 down
	max=$4
fi
echo 100 > /sys/kernel/debug/aetnensis/recalibrate

while [ $max -gt 0 ]
do
	max=`expr ${max} - 1`

	SEED=`(tr -cd 0-9 </dev/urandom | head -c 8;)2>/dev/null`
	RND_NUM=`echo $SEED $1 $2|awk '{srand($1);printf "%d",rand()*10000%($3-$2)+$2}'`
	echo $RND_NUM

	echo $RND_NUM > /sys/kernel/debug/aetnensis/recalibrate
	sleep 3

	cali_work_num=`cat /sys/kernel/debug/aetnensis/stats | grep "calibration work time" | awk '{print $NF}'`
	if [ $last_cali_work_num -eq $cali_work_num ]; then
		echo "fail" > /tmp/rf_recali_cycle
		exit
	fi
	echo "$max" > /tmp/rf_recali_cycle
	last_cali_work_num=$cali_work_num
done
