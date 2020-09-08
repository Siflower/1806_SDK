#!/bin/bash
#IFS=$'\n'
line_num=0
if [[ $1 = "" ]]; then
	echo "Usage:"
	echo "./rf_bin_gen.sh rf_peme_bin.h"
	echo "will reorder bin file context"
	exit
fi

#touch $1.bin
INPUTBIN=$1
#OUTPUTBIN=${INPUTBIN%.*}.bin
OUTPUTBIN=rf_pmem.bin

if [ -e "$OUTPUTBIN" ]; then
	echo "$OUTPUTBIN is already exist, remove it!"
	rm -f $OUTPUTBIN
fi

echo "##### start #####"
cp $INPUTBIN ${INPUTBIN%.*}.bak
#sed -in '1,8d' $INPUTBIN
sed -n -i '/0x/p' $INPUTBIN
#sed -in '$d' $INPUTBIN
#sed -in '$d' $INPUTBIN

dos2unix $INPUTBIN

echo "gen $OUTPUTBIN ...";
touch $OUTPUTBIN;

for line in $(<$INPUTBIN)
do
	((data=${line:0:6}))
	data_hex=`printf "%04x" $data`
	echo $data_hex
	printf \\x${data_hex:2:2} >> $OUTPUTBIN
	printf \\x${data_hex:0:2} >> $OUTPUTBIN
done

mv ${INPUTBIN%.*}.bak $INPUTBIN
rm -f rf_pmem_bin.hn
echo "##### end  #####"
