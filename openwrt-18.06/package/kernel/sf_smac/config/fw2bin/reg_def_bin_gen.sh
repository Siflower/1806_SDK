#!/bin/bash
IFS=$'\n'
line_num=0
if [[ $1 = "" ]]; then
	echo "Usage:"
	echo "./reg_def_bin_gen.sh AET_REG_defaults.txt"
	echo "will reorder bin file context"
	exit
fi

#touch $1.bin
INPUTBIN=$1
OUTPUTBIN="rf_default_reg.bin"

if [ -e "$OUTPUTBIN" ]; then
	echo "$OUTPUTBIN is already exist, remove it!"
	rm -f $OUTPUTBIN
fi

echo "##### start #####"
cp $INPUTBIN ${INPUTBIN%.*}.bak
#sed -in '1,14d' $INPUTBIN
sed -n -i '/W /p' $INPUTBIN
sed -in '/W/{s/W /0x/;s/ //1}' $INPUTBIN

dos2unix $INPUTBIN
echo "gen $OUTPUTBIN ...";
touch $OUTPUTBIN;

for line in $(<$INPUTBIN)
do
	((data=${line:0:12}))
	data_hex=`printf "%08x" $data`
	echo "$data_hex"
	printf \\x${data_hex:6:2} >> $OUTPUTBIN
	printf \\x${data_hex:4:2} >> $OUTPUTBIN
	printf \\x${data_hex:2:2} >> $OUTPUTBIN
	printf \\x${data_hex:0:2} >> $OUTPUTBIN
done

mv ${INPUTBIN%.*}.bak $INPUTBIN
rm -f AET_REG_defaults.txtn
echo "##### end  #####"
