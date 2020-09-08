#!/bin/sh
# Copyright (C) 2006 OpenWrt.org

. /usr/share/libubox/jshn.sh
. command_parser.sh

send_command CWA_APP_GetStatus 
get_reply

#band=0
#opmode=5
#send_command CWA_TRX_SwitchMode $(((band<<12) +  (opmode<<8) + 63))

if [ $1 == "hb" ]
then
	echo "HB TRX status:"
	band=0
	send_command CWA_TRX_GetOpMode $(((band<<12) +  0))
	get_reply

	send_command CWA_TRX_GetStatus $(((band<<12) +  0))
	get_reply

	send_command CWA_TRX_GetRFChannel $(((band<<12) +  0))
	get_reply
fi

if [ $1 == "lb" ]
then
	echo "LB TRX status:"
	band=1
	send_command CWA_TRX_GetOpMode $(((band<<12) +  0))
	get_reply

	send_command CWA_TRX_GetStatus $(((band<<12) +  0))
	get_reply

	send_command CWA_TRX_GetRFChannel $(((band<<12) +  0))
	get_reply
fi
