#!/bin/ash


dump_reg(){
    start_addr=$1
    stop_addr=$2
    rs_reg=$3


    while [ $start_addr -le $stop_addr ]
    do
    offset=`printf "0x%04x" $start_addr`
    addr=`expr $start_addr + $start_addr`
    addr=`expr 396361728 + $addr`
    if [ $rs_reg == "1" ]; then
        data=`devmem $addr 16`
        #simulate delay
        data=`devmem 396369920 16`
        data=`devmem 396371938 16`
    else
        data=`devmem $addr 16`
    fi
    addr_x=`printf "0x%04x" $addr`
    data_x=`printf "0x%04x" $data`
    line_x=$offset"        "$data_x
    echo $line_x >> rf_reg.txt
    start_addr=`expr ${start_addr} + 1`
    done
}

rm rf_reg.txt -rf

if [ $1 = "all" ]; then
    #X2-bus address map & DIG_CTRL_SPACE
    #0 ~ 12FF
    dump_reg 0 4863 0
    #RSBUS register 1300 ~ 137E
    dump_reg 4864 4990 1
    #LB/HB_CMD_IF_BASE 1400 ~ 147F
    #dump_reg 5120 5247 0
    #DMA_TABLE 1600~19FF
    #DATA_RAM  2000~3FFF
    #PMEM      4000~5FFF
    #dump_reg 5632 24575 0
else
    dump_reg $1 $2
fi
