echo src                192.168.6.123 62518 b0:83:fe:71:3b:0f >  /sys/kernel/debug/hnat_debug
echo dest               192.168.5.193 5001  10:16:88:c7:81:33 >  /sys/kernel/debug/hnat_debug
echo router     192.168.20.5 6001  10:16:88:11:11:11 10:16:88:33:33:33 >  /sys/kernel/debug/hnat_debug
echo vlan               0 0 >  /sys/kernel/debug/hnat_debug
echo other               1 0 >  /sys/kernel/debug/hnat_debug
echo rule               0 0 >  /sys/kernel/debug/hnat_debug
for i in `seq 99`
do
	echo src                192.168.6.$i $i b0:83:fe:71:3b:0f $i >  /sys/kernel/debug/hnat_debug
	echo dest               192.168.5.$i $i 10:16:88:c7:81:33 $i >  /sys/kernel/debug/hnat_debug
	echo router     192.168.20.5 $i 10:16:88:11:11:11 10:16:88:33:33:33 $i >  /sys/kernel/debug/hnat_debug
	echo vlan               0 0 $i >  /sys/kernel/debug/hnat_debug
	echo other               1 0 $i >  /sys/kernel/debug/hnat_debug
	echo rule               $i 0 >  /sys/kernel/debug/hnat_debug
	usleep 1
done
