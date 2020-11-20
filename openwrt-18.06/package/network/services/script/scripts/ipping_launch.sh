#!/bin/sh

[ "$1" != "run" -a "$1" != "stop" ] && return

ipping_get() {
	local val=`uci get $1`
	echo ${val:-$2}
}

ipping_lunch() {
	#interface=`ipping_get ipping.@local[0].Interface`
	host=`ipping_get ipping.@local[0].Host`
	cnt=`ipping_get ipping.@local[0].NumberOfRepetitions 3`
	dsize=`ipping_get ipping.@local[0].DataBlockSize 64`
	timeout=`ipping_get ipping.@local[0].Timeout 1000`

	[ "$host" = "" ] && return
	timeout=$((timeout/1000))
	[ "$timeout" = "0" ] && timeout = "1"
	success_count=0
	avg_time_sum=0
	min=9999999
	max=0
	i=0

	while [ $i -lt $cnt ]; do
		let i++
		res=$(ping -q -c 1 -s $dsize -W $timeout $host 2>&1)
		ba=`echo "$res" | grep "bad address"`
		[ -n "$ba" ] && { uci set easycwmp.@local[0].DiagnosticsState=Error_CannotResolveHostName; return; }
		ba=`echo "$res" | grep "unknown host"`
		[ -n "$ba" ] && { uci set easycwmp.@local[0].DiagnosticsState=Error_CannotResolveHostName; return; }
		stc=`echo "$res" | grep "received"`
		[ -z "$stc" ] && { uci set easycwmp.@local[0].DiagnosticsState=Error_Other; return; }
		times=`echo "$res" | grep "min/avg/max"`
		[ -z "$times" ] && continue
		sc1=`echo $stc | awk '{print $4}'`
		sc1=${sc1:-0}
		success_count=$((success_count+sc1))
		times=`echo $times | awk -F'=' '{ print $2 }'`
		min_time=`echo $times | awk -F'[=/ ]' '{ print $1 }'`	
		avg_time=`echo $times | awk -F'[=/ ]' '{ print $2 }'`
		max_time=`echo $times | awk -F'[=/ ]' '{ print $3 }'`
		min_time=${min_time:-0}
		avg_time=${avg_time:-0}
		max_time=${max_time:-0}
		min_time=${min_time%.*}
		avg_time=${avg_time%.*}
		max_time=${max_time%.*}
		[ $min_time -lt $min ] && min=$min_time
		[ $max_time -gt $max ] && max=$max_time
		avg_time_sum=$((avg_time_sum+avg_time))
	done
	failure_count=$((cnt-success_count))
	[ $success_count -gt 0 ] && avg_time=$((avg_time_sum/success_count)) || avg_time=0
	min_time=$min
	max_time=$max

	uci set easycwmp.@local[0].DiagnosticsState=Complete
	uci set easycwmp.@local[0].SuccessCount=$success_count
	uci set easycwmp.@local[0].FailureCount=$failure_count
	uci set easycwmp.@local[0].AverageResponseTime=$avg_time
	uci set easycwmp.@local[0].MinimumResponseTime=$min_time
	uci set easycwmp.@local[0].MaximumResponseTime=$max_time
}

ipping_stop() {
	local pids=`ps | grep ipping_launch | grep -v grep | grep -v stop | awk '{print $2}'`
	[ -z "$pids" ] && pids=`ps | grep ipping_launch | grep -v grep | grep -v stop | awk '{print $2}'`
	if [ -n "$pids" ]; then
		kill -9 $pids 2>/dev/null
		uci set ipping.@local[0].DiagnosticsState=None
	fi
}
