#!/bin/sh
PUBDIRPATH=./
uboot_img=""
openwrt_img=""
pcba_img=""
factory_img=""
prj=
flash_size_mb=

if [ -n "$1" ]; then
	prj=$1
fi

if [ -n "$2" ]; then
	flash_size_mb=$2
fi

ls | ( while read line
do
	isuboot=$(echo $line | grep "uboot" | grep "$prj")
	if [ "$isuboot" != "" ];then
		if [ "$uboot_img" != "" ];then
			echo "too many uboot image found!!!"
			exit 1
		fi
		uboot_img=$line
		continue
	fi
	isopenwrt=$(echo $line | grep "openwrt" | grep "$prj")
	if [ "$isopenwrt" != "" ];then
		if [ "$openwrt_img" != "" ];then
			echo "too many openwrt image found!!!"
			exit 1
		fi
		openwrt_img=$line
		continue
	fi
	ispcba=$(echo $line | grep "pcba" | grep "$prj")
	if [ "$ispcba" != "" ];then
		if [ "$pcba_img" != "" ];then
			echo "too many pcba image found!!!"
			exit 1
		fi
		pcba_img=$line
		continue
	fi
	isfactory=$(echo $line | grep "factory")
	if [ "$isfactory" != "" ];then
		if [ "$factory_img" != "" ];then
			echo "too many factory image found!!!"
			exit 1
		fi
		factory_img=$line
		continue
	fi
done

if [ "$uboot_img" = "" ];then
	echo "uboot image not found!!!"
	exit 1
fi

if [ "$openwrt_img" = "" ];then
	echo "openwrt image not found!!!"
	exit 1
fi

if [ "$pcba_img" = "" ];then
	echo "pcba image not found!!!"
	exit 1
fi

if [ "$factory_img" = "" ];then
	echo "factory image not found!!!"
	exit 1
fi

if [ "$flash_size_mb" = "4" ]; then
flash_size=4M
FACTORY_OFFSET=200704
UBOOT_MAX_SIZE=$FACTORY_OFFSET
OPENWRT_OFFSET=204800
PCBA_OFFSET=3932160
OPENWRT_MAX_SIZE=3731456
PCBA_MAX_SIZE=262144
elif [ "$flash_size_mb" = "8" ]; then
flash_size=8M
FACTORY_OFFSET=589824
UBOOT_MAX_SIZE=$FACTORY_OFFSET
OPENWRT_OFFSET=655360
PCBA_OFFSET=7864320
OPENWRT_MAX_SIZE=7274496
PCBA_MAX_SIZE=524288
elif [ "$flash_size_mb" = "16" ]; then
flash_size=16M
FACTORY_OFFSET=589824
UBOOT_MAX_SIZE=$FACTORY_OFFSET
OPENWRT_OFFSET=655360
PCBA_OFFSET=16252928
OPENWRT_MAX_SIZE=15663104
PCBA_MAX_SIZE=524288
else
echo "error flash size!!!"
exit 1
fi

PCBT_OFFSET=23
FACTORY_PCBT_OFFSET=`expr $FACTORY_OFFSET + $PCBT_OFFSET`

echo "pcbt offset is $FACTORY_PCBT_OFFSET"
openwrt_size=`wc -c < $openwrt_img`
uboot_size=`wc -c < $uboot_img`
pcba_size=`wc -c < $pcba_img`
factory_size=`wc -c < $factory_img`
echo "uboot = $uboot_img size=$uboot_size"
if [ $uboot_size -gt $UBOOT_MAX_SIZE ]; then
	echo "uboot img size is too large"
	exit 1
fi
echo "openwrt_img = $openwrt_img size=$openwrt_size"
if [ $openwrt_size -gt $OPENWRT_MAX_SIZE ]; then
	echo "openwrt img size is too large"
	exit 1
fi
echo "pcba = $pcba_img size=$pcba_size"
if [ $pcba_size -gt $PCBA_MAX_SIZE ]; then
	echo "pcba img size is too large"
	exit 1
fi
echo "factory=$factory_img size=$factory_size"

output_file=${prj}_output_$(date +"%Y%m%d").bin

rm -rf ./$output_file
dd if=/dev/zero of=./$output_file bs=$flash_size count=1
dd if=./$uboot_img of=./$output_file seek=0 skip=0 bs=1 count=$uboot_size conv=notrunc
dd if=./$factory_img of=./$output_file seek=$FACTORY_OFFSET skip=0 bs=1 count=$factory_size conv=notrunc
dd if=./$openwrt_img of=./$output_file seek=$OPENWRT_OFFSET skip=0 bs=1 count=$openwrt_size conv=notrunc
dd if=./$pcba_img of=./$output_file seek=$PCBA_OFFSET skip=0 bs=1 count=$pcba_size conv=notrunc
printf "PCBT" | dd of=./$output_file bs=1 count=4 seek=$FACTORY_PCBT_OFFSET conv=notrunc

md5sum ./$uboot_img ./$openwrt_img
echo "openwrt bin size=$openwrt_size"
echo "uboot bin size=$uboot_size"
echo "the final bin is $output_file"
)
