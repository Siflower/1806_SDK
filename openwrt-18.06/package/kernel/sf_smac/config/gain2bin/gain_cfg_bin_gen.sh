#!/bin/bash
#IFS=$'\n'
line_num=0
if [[ $1 = "" ]]; then
	echo "Usage:"
	echo "./gain_cfg_bin_gen.sh tx_gain_table.txt"
	echo "will reorder bin file context"
	exit
fi

#touch $1.bin
INPUTBIN=$1
#OUTPUTBIN=${INPUTBIN%.*}.bin
OUTPUTBIN=tx_adjust_gain_table.bin

if [ -e "$OUTPUTBIN" ]; then
	echo "$OUTPUTBIN is already exist, remove it!"
	rm -f $OUTPUTBIN
fi

echo "##### start #####"
cp $INPUTBIN ${INPUTBIN%.*}.bak
#sed -in '1,8d' $INPUTBIN
#sed -n -i '/TX_Gain: /p' $INPUTBIN
sed -in 's/.*TX_Gain: //g' $INPUTBIN
#sed -in '$d' $INPUTBIN

dos2unix $INPUTBIN

echo "gen $OUTPUTBIN ...";
touch $OUTPUTBIN;

for line in $(<$INPUTBIN)
do
	((data=${line:0:2}))
	data_hex=`printf "%02x" $data`
	echo $data_hex
#	printf \\x${data_hex:2:2} >> $OUTPUTBIN
	printf \\x${data_hex:0:2} >> $OUTPUTBIN
done

mv ${INPUTBIN%.*}.bak $INPUTBIN
rm -f tx_gain_table.txtn
echo "##### end  #####"
