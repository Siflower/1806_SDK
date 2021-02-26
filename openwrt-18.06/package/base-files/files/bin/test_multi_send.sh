echo use 0 > /sys/kernel/debug/hnat_debug
echo test 0 10 1 0 1 1512 1 > /sys/kernel/debug/hnat_debug
for i in `seq 99`
do
	echo use $i > /sys/kernel/debug/hnat_debug
	echo test 0 10 1 0 1 1512 1 > /sys/kernel/debug/hnat_debug
	usleep 1
done
