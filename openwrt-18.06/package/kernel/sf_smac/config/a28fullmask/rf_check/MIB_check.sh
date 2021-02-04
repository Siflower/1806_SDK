#!/bin/sh
# Copyright (C) 2006 OpenWrt.org
timer=3
if [ $1 == "lb" ]
then
	if [ $2 == "rx" ]
	then
		while [ $timer -gt 0 ];
		do
			devmem 0x11080b80
			devmem 0x11080b84
			devmem 0x11080b88
			devmem 0x11080804
			devmem 0x11080808
			timer=$((timer - 1))
			echo "~~~~~~~~~~~~~~~"
			sleep 1
		done
	fi
	if [ $2 == "tx" ]
	then	
		devmem 0x11080b70
		devmem 0x11080b74	
	fi
fi

if [ $1 == "hb" ]
then
	if [ $2 == "rx" ]
	then
		while [ $timer -gt 0 ];
		do
			devmem 0x11480b80
			devmem 0x11480b84
			devmem 0x11480b88
			devmem 0x11480804
			devmem 0x11480808
			timer=$((timer - 1))
			echo "~~~~~~~~~~~~~~~"
			sleep 1
		done
	fi
	if [ $2 == "tx" ]
	then	
		devmem 0x11480b70
		devmem 0x11480b74
	fi
fi
