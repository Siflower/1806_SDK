#!/bin/sh

#for help
#1.this script function is changing .txt($cali_first_file) to .bin($cali_last_file)
#2.for every line,only data after "(gain)" will be changed
#3.the data start at 2048,end at 4096

#txpower calibrate table version
txpower_calibrate_table_version="XO"

#.txt table for human reading
cali_first_file="./txpower_calibrate_table_expa.txt"
if [ ! -e $cali_first_file ];then
	echo "the file $cali_first_file is not exist" && exit
fi

#.bin will be created
cali_last_file="./default_txpower_calibrate_expa_table.bin"
if [ -e $cali_last_file ];then
	rm $cali_last_file
fi

#1.start putting data at $data_start bytes,the front part will be inited with "ff"
#2.the bin will end at $data_end,the not used part will be filled with "ff"
#3.if the data is larger than data_end,the data_end is not used,then
#the size of bin = data_start + data size
data_start=2048
data_end=4096

#put $data_start(default 2048 means 0x0800) bytes "ff" in $cali_last_file
#put "V2 00" in $cali_last_file
init_table_head(){
echo "start generating txpower calibtration table(bin),please wait about 40 seconds..."
for total in `seq 1 $data_start`
do
	/usr/bin/printf "\xff" >> $cali_last_file
done
/usr/bin/printf "$txpower_calibrate_table_version" >> $cali_last_file
#the value of XO（0x00),will be read from factory instead of here
/usr/bin/printf "\x0\x0" >> $cali_last_file
total=$(($total+4))
}

#rules for generating high list tables (siwifi_factory_info.c sf_wifi_init_high_txpower_list)
#2.4g:define values of a channel,then copy them to each channel(channel 1-13)
#5g:define values of a channel,then copy them to channel 36-64
#5g:the value channel 100-144 149-165 are same as the normal list
fill_high_list_table(){
#2.4g
/usr/bin/printf "\x18" >> $cali_last_file
/usr/bin/printf "\x13\x13\x13\x13\x13\x13\x12\x12\x11\x11\x10\x0f" >> $cali_last_file
/usr/bin/printf "\x13\x13\x12\x12\x11\x11\x10\x0f" >> $cali_last_file
/usr/bin/printf "\x13\x13\x12\x12\x11\x11\x10\x0f" >> $cali_last_file
#5g
/usr/bin/printf "\x05" >> $cali_last_file
/usr/bin/printf "\x0e\x0e\x0d\x0d\x0d\x0c\x0c\x0b" >> $cali_last_file
/usr/bin/printf "\x0e\x0d\x0d\x0c\x0c\x0b\x0a\x09" >> $cali_last_file
/usr/bin/printf "\x0e\x0d\x0d\x0c\x0c\x0b\x0a\x09" >> $cali_last_file
/usr/bin/printf "\x0e\x0d\x0d\x0c\x0b\x0b\x0a\x09\x08" >> $cali_last_file
/usr/bin/printf "\x0e\x0d\x0d\x0c\x0b\x0b\x0a\x09\x08\x07" >> $cali_last_file
/usr/bin/printf "\x0e\x0d\x0d\x0c\x0b\x0b\x0a\x09\x08\x07" >> $cali_last_file
total=$(($total+83))
}

#there will be $data_end(default 4096 means 0x0fff) bytes,fill the last part with "ff"
fill_table_tail(){
if [ "$txpower_calibrate_table_version"x == "V2"x ];then
	fill_high_list_table
fi
while [ $total -lt $data_end ]
do
	/usr/bin/printf "\xff" >> $cali_last_file
	total=$(($total+1))
done
echo "Successfully generated calibration table(bin), size is $total"
}

enter_cali(){
if [ "$1" -lt "32" ];then
	hex_num=`printf "%x" $1`
	/usr/bin/printf "\x$hex_num" >> $cali_last_file
	total=$(($total+1))
else
	hex_num=`printf "%x" 31`
	echo "the value($tatal) is out of range,use 31 instead"
	/usr/bin/printf "\x$hex_num" >> $cali_last_file
	total=$(($total+1))
fi
}

_transform(){
while [ "$1" ]
do
	gain_num=$1
	echo $gain_num | grep -q '[^0-9]' && echo "$gain_num (total $total) is not a number,skip it" || enter_cali $gain_num
	shift
done
}

#Only lines with "gain" are selected
#the number(ranged 0-31) after "(gain)" will be put in $cali_last_file
transform(){
is_data=`echo $1 | grep "gain"`
if [ "$is_data"x != x ] ; then
	gain_num_line=${1#*(gain)}
	_transform $gain_num_line
fi
}

init_table_head
while read line
do
	transform "$line"
done < $cali_first_file
fill_table_tail
