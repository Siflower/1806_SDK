PUBDIRPATH=./
uboot_img=""
openwrt_img=""
pcba_img=""
factory_img=""
prj=
flash_size_mb=
not_cover_factory=

if [ -n "$1" ]; then
	prj=$1
fi

if [ "$1" != "ocn" ]; then
	echo "first param need ocn"
	exit 1
fi

if [ -n "$2" ]; then
	flash_size_mb=$2
fi

if [ -n "$3" ]; then
	not_cover_factory=$3
	echo "not_cover_factory=$not_cover_factory"
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
	isopenwrt=$(echo $line | grep "eoc-wifi")
	if [ "$isopenwrt" != "" ];then
		if [ "$openwrt_img" != "" ];then
			echo "too many openwrt image found!!!"
			exit 1
		fi
		openwrt_img=$line
		continue
	fi
	isback=$(echo $line | grep "back")
	if [ "$isback" != "" ];then
		if [ "$openwrt_back" != "" ];then
			echo "too many openwrt image back found!!!"
			exit 1
		fi
		openwrt_back=$line
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

if [ "$openwrt_back" = "" ];then
	echo "openwrt image back not found!!!"
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
elif [ "$flash_size_mb" = "32" ]; then
flash_size=32M
#TODO other size, altered once
FACTORY_OFFSET=589824
#FACTORY_OFFSET=576k
UBOOT_MAX_SIZE=$FACTORY_OFFSET
OPENWRT_OFFSET=3801088
#OPENWRT_OFFSET=3712k
OPENWRT_BACK_OFFSET=12189696
#OPENWRT_BACK_OFFSET=11904k
PCBA_OFFSET=33030144
#PCBA_OFFSET=32m-512k
OPENWRT_MAX_SIZE=8388608
#OPENWRT_MAX_SIZE=8m
PCBA_MAX_SIZE=524288
#PCBA_MAX_SIZE=512k
else
echo "error flash size!!!"
exit 1
fi

PCBT_OFFSET=23
FACTORY_PCBT_OFFSET=`expr $FACTORY_OFFSET + $PCBT_OFFSET`

echo "pcbt offset is $FACTORY_PCBT_OFFSET"
openwrt_size=`wc -c < $openwrt_img`
openwrt_back_size=`wc -c < $openwrt_back`
back_size=`wc -c < $openwrt_back`
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
echo "openwrt_back= $openwrt_back size=$openwrt_back_size"
if [ $openwrt_back_size -gt $OPENWRT_MAX_SIZE ]; then
	echo "openwrt back img size is too large"
	exit 1
fi
echo "pcba = $pcba_img size=$pcba_size"
if [ $pcba_size -gt $PCBA_MAX_SIZE ]; then
	echo "pcba img size is too large"
	exit 1
fi
echo "factory=$factory_img size=$factory_size"

if [ "$not_cover_factory" != "1" ];then
	output_file=${prj}_output_$(date +"%Y%m%d").bin

	rm -rf ./$output_file
	dd if=/dev/zero of=./$output_file bs=$flash_size count=1
	dd if=./$uboot_img of=./$output_file seek=0 skip=0 bs=1 count=$uboot_size conv=notrunc
	dd if=./$factory_img of=./$output_file seek=$FACTORY_OFFSET skip=0 bs=1 count=$factory_size conv=notrunc
	dd if=./$openwrt_img of=./$output_file seek=$OPENWRT_OFFSET skip=0 bs=1 count=$openwrt_size conv=notrunc
	dd if=./$openwrt_back of=./$output_file seek=$OPENWRT_BACK_OFFSET skip=0 bs=1 count=$back_size conv=notrunc
	dd if=./$pcba_img of=./$output_file seek=$PCBA_OFFSET skip=0 bs=1 count=$pcba_size conv=notrunc
	#hardware version flag
	printf "hv" | dd of=./$output_file bs=1 count=2 seek=589851 conv=notrunc
	#wifi-eoc hardware version
	printf "3.1.00\0" | dd of=./$output_file bs=1 count=7 seek=589853 conv=notrunc
	#product key flag
	printf "pk" | dd of=./$output_file bs=1 count=2 seek=589943 conv=notrunc
	#product key
	printf "928d446fc28aa03a2eaaf07d7f5a7dd9" | dd of=./$output_file bs=1 count=32 seek=589945 conv=notrunc
	#rom type flag
	printf "rt" | dd of=./$output_file bs=1 count=2 seek=589983 conv=notrunc
	#rom type
	printf "\x29" | dd of=./$output_file bs=1 count=1 seek=589985 conv=notrunc
	#hardware feature
	printf "\xfe" | dd of=./$output_file bs=1 count=1 seek=589921 conv=notrunc
	#wifi_sn
	printf "1111111111111111" | dd of=./$output_file bs=1 count=16 seek=590129 conv=notrunc
	#openwrt version
	openwrt_version=${openwrt_img%.bin}
	openwrt_version_count=`expr ${#openwrt_version} + 1`
	printf "$openwrt_version\0" | dd of=./$output_file bs=1 count=$openwrt_version_count seek=590358 conv=notrunc
	printf "PCBT" | dd of=./$output_file bs=1 count=4 seek=$FACTORY_PCBT_OFFSET conv=notrunc
else
	echo "not_cover_factory=$not_cover_factory"
	#first file only contain uboot
	first_output_file=${prj}_output_$(date +"%Y%m%d")_1.bin
	second_output_file=${prj}_output_$(date +"%Y%m%d")_2.bin
	rm -rf ./$first_output_file ./$second_output_file
	dd if=/dev/zero of=./$first_output_file bs=$uboot_size count=1
	# second_size = 32m -640k = 0x2000000 - 0xa0000
	second_size=32899072
	dd if=/dev/zero of=./$second_output_file bs=$second_size count=1

	dd if=./$uboot_img of=./$first_output_file seek=0 skip=0 bs=1 count=$uboot_size conv=notrunc

	#OPENWRT_OFFSET=3712k-640k = 3072k = 3m
	OPENWRT_OFFSET=3145728
	#OPENWRT_BACK_OFFSET=11904k-640k = 11m
	OPENWRT_BACK_OFFSET=11534336
	#PCBA_OFFSET=32m-512k-640k
	PCBA_OFFSET=32374784
	dd if=./$openwrt_img of=./$second_output_file seek=$OPENWRT_OFFSET skip=0 bs=1 count=$openwrt_size conv=notrunc
	dd if=./$openwrt_back of=./$second_output_file seek=$OPENWRT_BACK_OFFSET skip=0 bs=1 count=$back_size conv=notrunc
	dd if=./$pcba_img of=./$second_output_file seek=$PCBA_OFFSET skip=0 bs=1 count=$pcba_size conv=notrunc
fi

md5sum ./$uboot_img ./$openwrt_img ./$openwrt_back
echo "openwrt bin size=$openwrt_size"
echo "openwrt back bin size=$openwrt_back_size"
echo "uboot bin size=$uboot_size"
echo "the final bin is $output_file $first_output_file $second_output_file"
)
