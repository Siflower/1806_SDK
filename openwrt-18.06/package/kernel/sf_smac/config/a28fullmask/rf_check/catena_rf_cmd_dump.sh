#!/bin/ash


dump_reg(){
    start_addr=$1
    stop_addr=$2
    rs_reg=$3
	if [ -z "$4" ];then
		OUTPUTTXT=rf_cmd_dump.txt
	else
		OUTPUTTXT=rf_cmd_dump_$4.txt
	fi

	while [ $start_addr -le $stop_addr ]
    do
    offset=`printf "0x%04x" $start_addr`
    addr=`expr $start_addr + $start_addr`
    addr=`expr 297795584 + $addr`
    if [ $rs_reg == "1" ]; then
        data=`devmem $addr 16`
        #simulate delay
        data=`devmem 297803776 16`
        data=`devmem 297805794 16`
    else
        data=`devmem $addr 16`
    fi
    addr_x=`printf "0x%04x" $addr`
    data_x=`printf "0x%04x" $data`
    line_x=$offset"        "$data_x
    echo $line_x >> $OUTPUTTXT
	start_addr=`expr ${start_addr} + 1`
    done
}

rm rf_cmd_dump.txt -rf
#cmd  logging buffer address 3800~3C00
dump_reg 14336 15360 0 $1
