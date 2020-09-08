#!/bin/ash
# Copyright (C) 2006 OpenWrt.org
#   
. phy_commands.sh
show_writes=1
show_reads=1
val=0
getCommand(){
	CommandValue="NOTaCommand"
	index=0
	for comm in $command_arr
	do
		if [ $index -eq $1 ]
		then	
			if [ "$comm" != "0" ] 
			then
				echo $comm
				return
			fi
			echo $CommandValue
			return
		fi
		index=$(($index+1))
	done
	echo $CommandValue
}

getIndex() {
	index=0
	for comm in $command_arr
	do
		if [ "$comm" == "$1" ]
		then	
			echo $index
			return
		fi
		index=$(($index+1))
	done
	echo $index
}

send_command() {
	val=$2;
	band=$((val>>12))
	if [ $band -eq 0 ]
	then
		addr=0x17a02880
	else
		addr=0x17a02880
	fi
	delim=" "
	count=0
	command="send_command "
	for data in "$@"
	do
		if [ $count -eq 0 ]
		then
			ix=$(getIndex $data)
			if [ $ix -gt -1 ]
			then
				if [ $1 == "CWA_TRX_SwitchMode" ]
				then
					wdata=$(printf "0x%04x" $((0x0000+$ix*16+$#-1)))
				else
					wdata=$(printf "0x%04x" $((0x8000+$ix*16+$#-1)))
				fi
				command=$command$(getCommand $ix)
			else
				command=$command$data
			fi
		else
			wdata=$(printf "0x%04x" $((data)))
			command=$command$delim$wdata
		fi
		devmem ${addr} 16 ${wdata} 
		if [ "$show_writes" -gt 0 ]
		then
			echo WR ${addr} ${wdata}
		fi
		addr=$(printf "0x%04x\n" $(($addr + 2)))
		count=$(($count+1))
	done
	echo "$command"
}

get_reply(){
	band=$((val>>12))
	if [ $band -eq 0 ]
	then
		addr=0x17a02894
	else
		addr=0x17a02894
	fi
	delim=" "
	result="........resp "
	error=""
	count=0
	if [ $# -eq 1 ]
	then
		count=$1

	else	
		count=1
	fi
	READY=0
	ERROR=0
	data=0
	timer=1000
	while [ $READY == 0 ]
	do 
		data=$(devmem ${addr} 16)
		if [ $show_reads -gt 0 ]
		then
			echo "RD" ${addr} ${data}
		fi
		if [ "$data" != "0x0000" ]
		then
			READY=1
		fi
		if [ $timer -eq 0 ]
		then
			READY=1
		fi
		timer=$((timer - 1))
	done
	if [ $(($data)) -ge $((0x8000)) ]
        then
        	ERROR=1
		result=$result"!!!ERROR"
	fi
	count=$(($data&0xF))
	comm=$((($data/16)&0xFF))
	CommandValue=$(getCommand $comm)
	result=$result$delim$CommandValue
	while [ $count -gt 0 ]
	do 
		addr=$(printf "0x%X\n" $(($addr + 2)))
		data=$(devmem ${addr} 16)
		if [ "$show_reads" -gt 0 ]
		then
			echo "RD" ${addr} ${data}
		fi
		result=$result$delim$data
		count=$(($count-1))
	done
	echo $result
}
