#!/bin/sh
# @log=0 do not log.
# @cnt means stations number
log=0
cnt=0
parse_time=0
[ "$1" == "log" ] && log=1
#read addr&ip from wldevlist
get_addr() {
	local time=`date`
	local ip
	local mac_addr
	uci show wldevlist | grep "device" | sed 's/=.*$//g' > /addr.txt
	echo "##############test start at $time################ " > /tmp/deauth.txt
	while read line
	do
		online=`uci -q get ${line}.online`
		if [ "$online" == 1  ]; then
			ip=`uci -q get ${line}.ip`
			mac_addr=`echo "$line" | sed 's/^.*\.//g'`""
			eval "ip$cnt=$ip"
			eval "addr$cnt=$mac_addr"
			eval "echo \"station: $cnt mac_addr \$addr$cnt ip \$ip$cnt >>>>>>>>>>>\" >> /tmp/deauth.txt"
			let "cnt++"
		fi
	done < /addr.txt
	echo "ping connect test started, $cnt stations in total. cat /tmp/deauth.txt to check the result." > /dev/console
}

ping_3times() {
	local time=0
	local ans=1
	until [ "$time" -gt 2 -o "$ans" == 0 ]
	do
		ping -c 1 $1 >/dev/null 2>&1
		ans="$?"
		[ "$log" == 1 ] && echo ">>>>time: $time ping: $ans (0,success 1,fail) ip: $ip <<<<<<<" > /dev/console
		let "time++"
	done
	return "$ans"
}

#parse base on $cnt
parse_station() {
	local time
	local ip
	local mac_addr
	local num=$cnt
	let "num--"
	until [ $num -lt 0 ]
	do
		eval "mac_addr=\$addr$num"
		eval "ip=\$ip$num"
		#ping -c 1 ${ip} >/dev/null 2>&1
		ping_3times "$ip"
		[ "$?" == 1 ] && {
			time=`date`
			#TODO what if deauth.txt takes up too much memory?
			echo "$time: ping $ip failed, mac_addr is ${mac_addr}     " >> /tmp/deauth.txt
		}
		let "num--"
	done
}

get_addr
while true
do
	[ "$log" == 1 ] && echo "---------------------------parse: $parse_time--------------------------" > /dev/console
	parse_station
	let "parse_time++"
	sleep 1
	case "$parse_time" in
		*000)
			fail=`cat /tmp/deauth.txt | grep "failed" -c`
			[ "$fail" -gt 1000 ] && {
				echo "too much stations ping failed! exit! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" > /dev/console
				exit 1
			}
			;;
	esac
done
