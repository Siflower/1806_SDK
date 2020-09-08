#!/bin/sh
#env setup for arm platform
if [ -e $PWD/stop ]; then
    eval "rm -f $PWD/stop"
fi

if [ "$1" = "-h" ]; then
    echo "args is 2 5 others"
    echo "2for 2.4g  5 for 5g others for random"
    echo "default test is 5000 timesuse cmd"
    echo "sh auto_test.sh 2 2000"
    exit
fi

if test ! "$1"
then
    echo "miss arg "
    exit
fi

eval "mkdir /mnt/sda1/onoff_test"

# if [ -d "/mnt/sda1/onoff_test" ]; then
#     eval "touch on_off.log"
#     log_file="/mnt/sda1/onoff_test/on_off.log"
# else
	eval "touch on_off.log"
	log_file="$PWD/on_off.log"
# fi

cat /dev/null > $log_file

debug=true

log() {
		echo "$@" |tee -a $log_file
}
#hg means 5g
hgName="radio1"
hgAlias="5G"
hgInt=""
hgIdx=""

#lg means 2.4g
lgName="radio0"
lgAlias="2.4G"
lgInt=""
lgIdx=""
#env
sleep_time=3s
on_off_test_cycle=$2
test_int_idx=""
test_int=""
test_name=""
#mt7612e
#rai0
set_up_interface() {
    set=""
    #res1=`uci show wireless.@wifi-iface[0] |awk '/(device|ifname)/{print}'|awk -F = '{print $2}'`
    #res2=`uci show wireless.@wifi-iface[1] |awk '/(device|ifname)/{print}'|awk -F = '{print $2}'`
    #result="$res1"" ""$res2"
	result="radio0 wlan0 radio1 wlan1"
	log "$result"
    idx=0
    for var in $result; do
        case $var in
            $hgName)
                hgIdx=`expr ${idx} / 2`
                set=hgInt ;;
            $lgName)
                lgIdx=`expr ${idx} / 2`
                set=lgInt ;;
        esac
        eval "$set=$var"
        idx=`expr ${idx} + 1`
    done
}


check_disable_status() {
    return `uci get wireless.@wifi-device[${1}].disabled`
}

check_if_interface_exists() {
    list=`ifconfig |awk '/(HWaddr)/{print $1}'`
    for var in $list; do
        if [ ${1} = $var ]; then
           return 1
        fi
    done
    return 0
}

rand_l(){
	min=$1
	max=$(($2-$min+1))
	num=$(date +%s)
	echo "num=$num nax=$max min=$min" > /dev/ttyS0
	echo $(($num%$max+$min))
#	echo 0
}

#input index 0/1 (on/off)
turn_on_wifi() {
	eval "uci set wireless.@wifi-device[${1}].disabled=${2}"
	eval "uci commit"
	eval "wifi reload"
}

get_time() {
    echo `date -I'seconds'`
}
set_up_interface

#set up test args
set_up_test() {
    if [ $1 = 5 ]; then
        test_int_idx=$hgIdx
        test_int=$hgInt
        test_name=$hgName
    elif [ $1 = 2 ]; then
        test_int_idx=$lgIdx
        test_int=$lgInt
        test_name=$lgName
    else
	random=$(rand_l 0 1)
        if [ $random = 0 ]; then
            test_int_idx=$lgIdx
            test_int=$lgInt
            test_name=$lgName
        else
            test_int_idx=$hgIdx
            test_int=$hgInt
            test_name=$hgName
        fi
    fi
    log "test name=$test_name test_int=$test_int test_int_idx=$test_int_idx"
    if [ ! $test_int_idx  ] || [ ! $test_int ] ; then
        log "hgint $hgInt hgIdx $hgIdx lgInt $lgInt lgIdx $lgIdx"
        log " init interface info fail"
        return 0
    fi
    return 1
}
# turn_on_wifi $hgIdx 0
# check_if_interface_exists $hgInt
# echo "check result is  $?"
check_if_stop() {
    if [ -e $PWD/stop ]; then
        return 1
    else
        return 0
    fi
}

cycle_idx=0

until test "$cycle_idx" -eq "$on_off_test_cycle"
do
    log ""
    log "====cycle is $cycle_idx===="

    set_up_test $1
    if [ $? -eq 0 ]; then
        log "init test interface error"
        exit
    fi
    tmp_time=`get_time`
    log "start time is $tmp_time"

    check_if_stop
    if [ $? -eq 1 ]; then
        log "stop by user"
        exit
    fi

	check_disable_status $test_int_idx
	case "$?" in
		1)
			turn_on_wifi $test_int_idx 0
			sleep $sleep_time
			check_if_interface_exists $test_int
		if [ $? -eq 1 ]; then
				log "=======$test_name turn on success======="
			else
				time_t=0;
				check_if_interface_exists $test_int
				until [ $? -eq 1 ]
				do
					#turn_on_wifi $test_int_idx 0
					sleep $sleep_time
					time_t=`expr $time_t + 1`
					log "====cycle is $cycle_idx===$test_name turn on failed,continue $time_t times======="
					check_if_interface_exists $test_int
				done
				log "=======after retest $test_name turn on success======="
			fi
		;;
		*)
			turn_on_wifi $test_int_idx 1
			sleep $sleep_time
			check_if_interface_exists $test_int
			if [ $? -eq 0 ]; then
				log "=======$test_name turn off success======="
			else
				time_t=0;
				check_if_interface_exists $test_int
				until [ $? -eq 0 ]
				do
					#turn_on_wifi $test_int_idx 1
					sleep $sleep_time
					time_t=`expr $time_t + 1`
					log "====cycle is $cycle_idx===$test_name turn off failed,continue $time_t times======="
					check_if_interface_exists $test_int
				done
				log "=======after retest $test_name turn off success======="
			fi
		;;
	esac
	cycle_idx=`expr ${cycle_idx} + 1`
	sleep $sleep_time
done
