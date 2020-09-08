#!/bin/sh

LED_LB_SWITCH=/sys/module/sf16a18_lb_smac/parameters/led_status
LED_HB_SWITCH=/sys/module/sf16a18_hb_smac/parameters/led_status

run_led_switch(){
	lb_led=$(cat $LED_LB_SWITCH)
	hb_led=$(cat $LED_HB_SWITCH)
	if [[ $lb_led -eq 1 || $hb_led -eq 1  ]]
	then
		echo 0 > $LED_LB_SWITCH
		echo 0 > $LED_HB_SWITCH
		echo "led is off<<<<<<"
	else
		echo 1 > $LED_LB_SWITCH
		echo 1 > $LED_HB_SWITCH
		echo "led is on<<<<<<<"
	fi
}
echo "led_switch start"
run_led_switch
