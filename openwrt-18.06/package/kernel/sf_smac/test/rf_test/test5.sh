#!/bin/ash
# Copyright (C) 2006 OpenWrt.org

. /lib/sfwifi.sh
. /usr/share/libubox/jshn.sh
#boot.sh contains some procedures found in siflower test scripts, but got lost beacuse of EVB board breakdown
. /bin/boot.sh
. /bin/command_parser.sh

echo "------------test5 log----------------" > log_rf
firmware_remove
firmware_reload

address=0x11c00001 #address is 0x11c00000-0x11c01299
value=0x0001
mode=0
cal_config=1

show_reads=0
show_writes=0

send_command_all() {
	val=0;
	band=$((val>>12))
	if [ $band -eq 0 ]
	then
		addr=0x11c02800
	else
		addr=0x11c02880
	fi
	delim=" "
	count=0
	command="send_command "
	for data in "$@"
	do
		if [ $count -eq 0 ]
		then
			ix=$(getIndex $data)
			if [ $ix -gt -1 ]
			then
				if [ $1 == "CWA_TRX_SwitchMode" ]
				then
					wdata=$(printf "0x%04x" $((0x0000+$ix*16+$#-1)))
				else
					wdata=$(printf "0x%04x" $((0x8000+$ix*16+$#-1)))
				fi
				command=$command$(getCommand $ix)
			else
				command=$command$data
			fi
		else
			wdata=$(printf "0x%04x" $((data)))
			command=$command$delim$wdata
		fi
		devmem ${addr} 16 ${wdata}
		if [ "$show_writes" -gt 0 ]
		then
			echo WR ${addr} ${wdata}
		fi
		addr=$(printf "0x%04x\n" $(($addr + 2)))
		count=$(($count+1))
	done
	echo "$command"
}

get_reply_all(){
	band=$((val>>12))
	if [ $band -eq 0 ]
	then
		addr=0x11c02814
	else
		addr=0x11c02894
	fi
	delim=" "
	result="........resp "
	error=""
	count=0
	if [ $# -eq 1 ]
	then
		count=$1

	else
		count=1
	fi
	READY=0
	ERROR=0
	data=0
	timer=1000
	while [ $READY == 0 ]
	do
		data=$(devmem ${addr} 16)
		if [ $show_reads -gt 0 ]
		then
			echo "RD" ${addr} ${data}
		fi
		if [ "$data" != "0x0000" ]
		then
			READY=1
		fi
		if [ $timer -eq 0 ]
		then
			READY=1
		fi
		timer=$((timer - 1))
	done
	if [ $(($data)) -ge $((0x8000)) ]
        then
        	ERROR=1
		result=$result"!!!ERROR"
		echo "failed in test all command!!!" >> log_rf
	fi
	count=$(($data&0xF))
	comm=$((($data/16)&0xFF))
	CommandValue=$(getCommand $comm)
	result=$result$delim$CommandValue
	while [ $count -gt 0 ]
	do
		addr=$(printf "0x%X\n" $(($addr + 2)))
		data=$(devmem ${addr} 16)
		if [ "$show_reads" -gt 0 ]
		then
			echo "RD" ${addr} ${data}
		fi
		result=$result$delim$data
		count=$(($count-1))
	done
	echo $result
	if [ -z "$1" ]
	then
		echo ""
	else
		if [ $data != $1 ]
		then
			echo "failed $1 $result" >> log_rf
			echo "------failed-------"
			sfwifi reset
			exit
		fi
	fi
}
#every command's parameter is there
#range t_appmode=(0/1/2/3/4 off/sleep/stdby_bootclk/stdby/idle_active)
#range t_ClkPllMode(0/1/2/3 clkpll_off/clkpll_on/clkpll_on_cal_vcofq clkpll_on_cal_vco)
#range t_Unlocked (0/1 locked/unlocked)
#range t_Flag (0/1 low/high)
#range t_AppCalConfig (1/2 CLK_PLL/AUX_ADC)
#range t_OffOn (0/1 off/on)
#range t_HkBaseClk(0/1 boot/lfc)
#range t_HkConfig(1/2/4/8 HK_TEMP/HK_TRX_HB/HK_TRX_LB/HK_TRX_APP is Temperature/TRX HB status/TRX LB status/APP status)
#range t_Trxid (0/1/2 hb/lb/none)
#range t_TrxModeSwitch (5/6 onrx/ontx)
#range t_Boolean (0/1 FALSE/TRUE)
#range t_TrxMode (0/1/2/3/4/5/6/7 off/onid/onidsx/onsx/onsxtrxid/onrx/ontx/ontrxcal)
#range t_TrxCalConfig(1/2/4/8/16/32/256/512 LBHB_SX_VCO/LBHB_TX_WBPLL_VCO/LBHB_RX_WBPLL_VCO/LBHB_RX_DCOC/LBHB_TX_DCOC/LBHB_RXChanBW/LBHB_TXChanBW/LBHB_TX_IQ/LBHB_RX_IQ/LBHB_TXPA)
#range t_AuxAdcConvSel (0/1/2 DO_SINGLE_CONV/START_CONT_CONV/STOP_CONT_CONV)
#range t_AdcInput (0/1/2/3/4/7 vss/atest1/atest2/atest3/atest4/off)

#in=inputparameter out=outputparameter
checktime=0
if [ $# -eq 0 ]
then
	echo "you should input the test num"
	return
fi

if [ $1 -lt 0 ]
then
	checktime=1
else
	checktime=$1
fi

while [ $checktime -gt 0 ]
do
echo "------$checktime-------"
#in address dw1[15:0] value dw2[15:0]
		send_command_all CWA_GEN_SetRegister $address $value
		get_reply_all
#in address dw1[15:0]
#out value rw1[15:0]
		send_command_all CWA_GEN_GetRegister $address
		get_reply_all $value
#out hw_version rw1[15:0] target_ver rw2[15:12] major_ver rw2[11:8] minor_ver rw2[7:0] build_nr rw3[15:0]
		send_command_all CWA_GEN_GetVersion
		get_reply_all 
#range t_appmode=(0/1/2/3/4 off/sleep/stdby_bootclk/stdby/idle_active)
#in mode dw1[2:0] t_appmode
		mode=0x0002
		send_command_all CWA_APP_SetOpMode $mode
		get_reply_all
#out mode rw1[2:0] t_appmode
		send_command_all CWA_APP_GetOpMode
		get_reply_all $mode
#in cal_config t_AppCalConfig dw1[3:0]
		cal_config=0x0001
		send_command_all CWA_APP_SetCalConfig $cal_config
		get_reply_all
#out cal_config rw1[3:0] t_AppCalConfig
		send_command_all CWA_APP_GetCalConfig
		get_reply_all $cal_config
#(no parameter) perform the cal defined by the present calibration configuration
		send_command_all CWA_APP_DoCalibration
		get_reply_all
#out cal_status rw1[3:0] t_AppCalConfig
		send_command_all CWA_APP_GetCalStatus
		get_reply_all $cal_config
#in mode dw1[2:0] t_appmode
		mode=4
		send_command_all CWA_APP_SetOpMode $mode
		get_reply_all
#out appmode rw1[14:12] t_appmode
#out clkpllmode rw[9:8] t_clkpllmode
#out unlocked rw1[4] t_unlocked
#out vtune_high rw1[3] t_Flag
#out vtune_low rw1[2] t_Flag
#out vco_toohigh rw1[1] t_Flag
#out vco_toolow rw1[0] t_Flag
		send_command_all CWA_APP_GetStatus
		get_reply_all
#in pospin dw1[11:8] t_AdcInput
#in negpin dw1[3:0] t_AdcInput
#		param=0x0100
#		send_command_all CWA_AUX_SetAdcInput $param
#		get_reply_all
#in conv_sel dw1[1:0] t_AuxAdcConvSel
#		para=0x0000
#		send_command_all CWA_AUX_DoAdcConv
#		get_reply_all
#		sleep 2
#out adc_result rw1[15:1] int
#out adc_old rw1[0] uint
#		send_command_all CWA_AUX_GetAdcResult
#		get_reply_all
#out pospin rw1[11:8]
#out negpin rw1[3:0]
#		send_command CWA_AUX_GetAdcInput
#		get_reply_all

#in hk_mode dw1[0] t_OffOn
#in hk_clock dw1[1 t_HkBaseClk
#in hk_task_config dw2[15:0] t_HkConfig
#in hk_update_time dw3[15:0] uint
#in hk_tempdiff_thres dw4[15:0] int
		param=3
		param2=0x000f
		param3=32768
		param4=0x0064
		send_command_all CWA_APP_SetHKConfig $param $param2 $param3 $param4
		get_reply_all
#out hk_mode rw1[0] t_OffOn
#out hk_clock rw1[1 t_HkBaseClk
#out hk_task_config rw2[15:0] t_HkConfig
#out hk_update_time rw3[15:0] uint
#out hk_tempdiff_thres rw4[15:0] int
		send_command_all CWA_APP_GetHKConfig
		get_reply_all $param4
#out temperature rw1[15:0] int
		send_command_all CWA_APP_GetTemperature
		get_reply_all
#in trxid dw1[15:12] t_Trxid
#in idle dw1[7:0]
#in rx dw2[7:0]
#in tx dw3[7:0]
#in pa dw4[7:0]
		param=0
		param2=2
		param3=3
		param4=0x0004
		send_command_all CWA_TRX_SetRfAppConfig $param $param2 $param3 $param4
		get_reply_all
#in trxid dw1[15:12] t_Trxid
#out idle rw1[7:0]
#out rx rw2[7:0]
#out tx rw3[7:0]
#out pa rw4[7:0]
		param=0
		send_command_all CWA_TRX_GetRfAppConfig $param
		get_reply_all $param4
#in trxid dw1[15:12] t_Trxid
#in rfch dw1[7:0] uint
#in ignore_cal_values dw2[15] t_Boolean
#in sx_vco_cap dw2[7:0] uint
#in sx_vco_cur dw3[15:8] uint
#in rx_wbpll_band dw3[7:4] uint
#in tx_wbpll_band dw3[3:0] uint
#If the ignore_cal_values parameter = 0 the 4 following parameters sx_vco_cap, sx_vco_cur, rx_wbpll_band and tx_wbpll_band is used together with the new RF channel number. The RF channel number corresponds to the channel
#number definitions of the WiFi standard.
		param=0x0024
		param2=0
		param3=0
		send_command_all CWA_TRX_SetRFChannel $param $param2 $param3
		get_reply_all
#in trxid dw1[15:12] t_Trxid
#out rfch rw1[7:0] unit
		param_g=0
		send_command_all CWA_TRX_GetRFChannel $param_g
		get_reply_all $param
#in trxid dw1[15:12] t_Trxid
#in chbw dw1[7:0] uint
		param=0x0014
		send_command_all CWA_TRX_SetChannelBW $param
		get_reply_all
#in trxid dw1[15:12]
#out chbw rw1[7:0]
		param_g=0
		send_command_all CWA_TRX_GetChannelBW $param_g
		get_reply_all $param
#in trxid dw1[15:12] t_Trxid
#in rxgain dw1[7:0] uint
		param=0x003F
		send_command_all CWA_TRX_SetRxGain $param
		get_reply_all
#in trxid dw1[15:12] t_Trxid
#out rxgain rw1[7:0] uint
		param_g=0
		send_command_all CWA_TRX_GetRxGain $param_g
		get_reply_all $param
#in trxid dw1[15:12] t_Trxid
#in txpwr dw1[7:0] uint
		param=0x001F
		send_command_all CWA_TRX_SetTxPower $param
		get_reply_all
#in trxid dw1[15:12] t_Trxid
#out txpwr rw1[7:0] uint
		param_g=0
		send_command_all CWA_TRX_GetTxPower $param_g
		get_reply_all $param
#in trxid dw1[15:12] t_Trxid
#in mode dw1[11:8]  t_TrxMode
#in gain_pwr dw1[7:0] uint
#in rfch dw2[15:8]
#in chbw dw2[7:0]
#out trxid rw1[15:12] t_Trxid
#out mode rw1[11:8] t_TrxMode
#out gain_pwr rw1[7:0] uint
#The CWA_TRX_SetOpMode command will set the <trxid>-TRX in operation mode <trx_mode> with RXGAIN/TXPOWER to <gain_pwr> and optionally the used RF channel to <rfch> and selected channel bandwidth to <chbw>. When the rfch and chbw input parameters are not used they are required to be set to zero (0).
		param=0x0100
		param2=0
		send_command_all CWA_TRX_SetOpMode $param $param2
		param=0x043F
		param2=0x2414
		send_command_all CWA_TRX_SetOpMode $param $param2
		get_reply_all
#in trxid dw1[15:12] t_Trxid
#out mode rw1[11:8] t_TrxMode
#out gain_pwr rw1[7:0] uint
		param_g=0
		send_command_all CWA_TRX_GetOpMode $param_g
		get_reply_all $param 
#in trxid dw1[15:12] t_Trxid
#in cal_config dw1[11:0] t_TrxCalConfig
		param=0x03FF
		send_command_all CWA_TRX_SetCalConfig $param
		get_reply_all
#in trxid dw1[15:12] t_Trxid
#out cal_config rw1[11:0] t_TrxCalConfig
		param_g=0
		send_command_all CWA_TRX_GetCalConfig $param_g
		get_reply_all $param
#in trxid dw1[15:12] t_Trxid
		param_g=0
		send_command_all CWA_TRX_DoCalibration $param_g
		get_reply_all
#in trxid dw1[15:12] t_Trxid
#out cal_config dw1[11:0] t_TrxCalConfig]
		param_g=0
		send_command_all CWA_TRX_GetCalStatus $param_g
		get_reply_all $param
#in trxid dw1[15:12] t_Trxid
#out trx_status rw1[15:0]
#Base band peak detector flag (bit 15)
#Detector flags for TX-WBPLL (bits 13:11):
#Detector flags for RX-WBPLL (bits 10:8)
#Detector flags for RF synth (SX) (bits 4:0)
		param=0
		send_command_all CWA_TRX_GetStatus $param
		get_reply_all

#in trxid dw1[15:12] t_Trxid
#in mode dw1[11:8] t_TrxModeSwitch
#in gain_pwr dw1[7:0] uint
		param=0x0500
		param2=0
		send_command_all CWA_TRX_SetOpMode $param $param2	
		param=0x0600
		send_command_all CWA_TRX_SwitchMode $param
		param_g=0
		send_command_all CWA_TRX_GetOpMode $param_g
		get_reply_all $param
		checktime=$(($checktime-1))
	done

grep -q "failed" log_rf
if [ "$?" -eq "0" ]
then
	echo "--------failed-----------"
	echo "failed" >> log_rf
else
	echo "--------success-----------"
	echo "success" >> log_rf
fi
sfwifi reset
