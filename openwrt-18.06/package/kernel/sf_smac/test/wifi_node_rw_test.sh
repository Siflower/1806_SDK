#!/bin/sh
# auther: guangming
# time: 2021/08/13
# the purpose of the test is check whether the node can read and write normally
# You can make "./wifi_node_rw_test -help/-h" to view help 

# The address of test file
# 2.4G
addr1="/sys/kernel/debug/ieee80211/phy0/siwifi/"
# 5G
addr2="/sys/kernel/debug/ieee80211/phy1/siwifi/"
# RF deive
addr3="/sys/kernel/debug/aetnensis/"

# The status of wifi
wlan0_status=`ifconfig | grep wlan0`
wlan1_status=`ifconfig | grep wlan1`

# Long str (>512)
	test_str=123456789abcdef
for i in $(seq 1 33)
	do
	test_str="$test_str-123456789abcdef"
	done

# The function print the node where address
# $1:address
# $2:the name of log file
print_operation()
{
	echo "=========The node in this address: <<$1>>"========= >> $2.txt
	for file in `ls $1`
	do
		if [ -d $1${file} ];then
			continue
		fi
		echo "$file " >> $2.txt
	done
	return 0;
}

# The function read operation
# $1:address
# $2:the name of log file
read_operation()
{
	print_operation $1 $2
	for file in `ls  $1`
	do
		if [ -d $1${file} ];then
			continue
		fi
		if [ $file == irqnum ];then
			continue
		fi
		if [ $file == recalibrate ];then
			continue
		fi
		echo "---- Reading node ---- address: <<$1>> node: <<$file>> "
		cat $1${file}  >> /dev/null
		wlan0_status=`ifconfig | grep wlan0`
		wlan1_status=`ifconfig | grep wlan1`
		if [ -z "$wlan0_status" ];then
			echo " Failed to read node -- address: <<$1>> node: <<$file>> " >> bad$2.txt
		elif [ -z "$wlan1_status" ];then
			echo " Failed to read node -- address: <<$1>> node: <<$file>> " >> bad$2.txt
		else
			echo " Successfully(no error) read node -- address: <<$1>> node: <<$file>> " >> $2.txt
		fi
		#sleep 1
	done
	return 0;
}

# The function write operation
# $1:address
# $2:the value what want to write
# $3:the name of log file
write_operation()
{
	if [ ${#2} -gt 20 ];then
		str_name=long_str
	else
		str_name=$2
	fi
	print_operation $1 $3
	for file in `ls  $1`
	do
		if [ -d $1${file} ];then
			continue
		fi
		if [ $file == irqnum ];then
			continue
		fi
		if [ $file == recalibrate ];then
			continue
		fi
		echo "---- Writing <<$str_name>> to node ---- address: <<$1>> node: <<$file>> "
		echo $2 >> $1${file}
		wlan0_status=`ifconfig | grep wlan0`
		wlan1_status=`ifconfig | grep wlan1`
		if [ -z "$wlan0_status" ];then
			echo " Failed to write <<$str_name>> to node -- address: <<$1>> node: <<$file>> " >> bad$3.txt
		elif [ -z "$wlan1_status" ];then
			echo " Failed to write <<$str_name>> to node -- address: <<$1>> node: <<$file>>  " >> bad$3.txt
		else
			echo "Successfully(no error) write <<$str_name>> to node -- address: <<$1>> node: <<$file>> " >> $3.txt
		fi
		#sleep 1
	done
	return 0;
}

# The function test operation                                                  
# $1:the addr of test. "0" is addr1,"1" is addr2,"2" is addr3, if there are no parameters, it will be pushed out 
# $2:the operation of test. "r" is read,"2" is write,if there are no parameters, means read and write
test_operation()                                                                       
{
	if [ $1 -eq 0 ];then    # operate addr1
		if [ $2 == r ];then    # only read
			read_operation $addr1 res1
		elif [ $2 == w ];then  # only write
			write_operation $addr1 "" res1
			write_operation $addr1 0 res1
			write_operation $addr1 1 res1
			write_operation $addr1 VB7*s2d% res1
			write_operation $addr1 $test_str res1
		elif [ -z $2 ];then # read and write
			read_operation $addr1 res1
			write_operation $addr1 "" res1
			write_operation $addr1 0 res1
			write_operation $addr1 1 res1
			write_operation $addr1 VB7*s2d% res1
			write_operation $addr1 $test_str res1
		else    # Wrong input
			echo " Parameter error"
			exit 1
		fi
	elif [ $1 -eq 1 ];then  # operate addr2
		if [ $2 == r ];then    # only read
			read_operation $addr2 res2
		elif [ $2 == w ];then  # only write
		    write_operation $addr2 "" res2
			write_operation $addr2 0 res2
		    write_operation $addr2 1 res2
	        write_operation $addr2 VB7*s2d% res2
	        write_operation $addr2 $test_str res2
	    elif [ -z $2 ];then # read and write
	        read_operation $addr2 res2
			write_operation $addr2 "" res2
	        write_operation $addr2 0 res2
	        write_operation $addr2 1 res2
	        write_operation $addr2 VB7*s2d% res2
	        write_operation $addr2 $test_str res2
	    else    # Wrong input
		    echo " Parameter error"
			exit 1
	    fi
	elif [ $1 -eq 2 ];then  # operate addr3
		if [ $2 == r ];then    # only read
			read_operation $addr3 res3
	    elif [ $2 == w ];then  # only write
	        write_operation $addr3 "" res3
		    write_operation $addr3 0 res3
			write_operation $addr3 1 res3
			write_operation $addr3 VB7*s2d% res3
			write_operation $addr3 $test_str res3
		elif [ -z $2 ];then # read and write
			read_operation $addr3 res3
			write_operation $addr3 "" res3
			write_operation $addr3 0 res3
			write_operation $addr3 1 res3
			write_operation $addr3 VB7*s2d% res3
			write_operation $addr3 $test_str res3
		else    # Wrong input
			echo " Parameter error"
			exit 1
		fi
	else    # Wrong input
		echo " Parameter error"
		exit 1
		fi
		return 0
}

# The function of print help
print_help()
{
	echo "	================================================================="
	echo "	=	Usage:	./wifi_node_rw_test [parameter1] [parameter2]	="
	echo "	=	or	./wifi_node_rw_test [parameter3]		="
	echo "	=								="
	echo "	=	parameter1:						="
	echo "	=		-0			operate node of 2.4G	="
	echo "	=		-1			operate node of 5G	="
	echo "	=		-2			operate node of rf	="
	echo "	=	parameter2:						="
	echo "	=		-r			read operation		="
	echo "	=		-w			write operation		="
	echo "	=	parameter3:						="
	echo "	=		-help/-h		read operation		="
	echo "	=		-p			pressure test		="
	echo "	================================================================="
	exit 0
}

# THe function of pressure test
pressure_test()
{	
	while(true)
	do
		# Read addr1
		test_operation 0 r
		# Write addr1
		test_operation 0 w
		# Read addr2
		test_operation 1 r
		# Write addr2
		test_operation 1 w
		# Read addr3 except these nodes (irqnum recalibrate)
		test_operation 2 r
		# Write addr3 except these nodes (irqnum recalibrate)
		test_operation 2 w
	done
	return 0
}

# The begin of this script
if [ $1 == -help ]||[ $1 == -h ];then
	print_help
elif [ $1 == -0 ];then
	if [ $2 == -r ];then
		test_operation 0 r
	elif [ $2 == -w ];then
		test_operation 0 w
	else
		echo " Parameter error"
        exit 1
	fi
elif [ $1 == -1 ];then	
	if [ $2 == -r ];then
        test_operation 1 r
    elif [ $2 == -w ];then
        test_operation 1 w
    else
        echo " Parameter error"
        exit 1
    fi
elif [ $1 == -2 ];then
	if [ $2 == -r ];then
        test_operation 2 r
	 elif [ $2 == -w ];then
        test_operation 2 w
    else    
        echo " Parameter error"
        exit 1
    fi
elif [ $1 == -p ];then
	pressure_test
else
	echo " Parameter error"
        exit 1
fi

echo " Test end ~ "
exit 0
