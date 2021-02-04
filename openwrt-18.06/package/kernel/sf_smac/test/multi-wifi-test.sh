#!/bin/sh

setup_channel(){

        #set 24g channel

        channel_24g=$(($1 % 11))
        [ "$channel_24g" -eq 0 ] && channel_24g=1
        uci set wireless.radio0.channel="$channel_24g"
        ch_5g=36
        case "$(($1 % 4))" in
                1) ch_5g=48;;
                2) ch_5g=64;;
                3) ch_5g=149;;
                0) ch_5g=36;;
        esac

        #set 5g channel
        uci set wireless.radio1.channel="$ch_5g"

}


init_ap(){
        rm /etc/config/wireless
        touch /etc/config/wireless
        commit_wireless
        hostssid="wifi-test-$1"

        uci -q batch <<-EOF
                set wireless.radio0=wifi-device
                set wireless.radio0.type='mac80211'
                set wireless.radio0.country='CN'
                set wireless.radio0.txpower_lvl='2'
                set wireless.radio0.channel='1'
                set wireless.radio0.band='2.4G'
                set wireless.radio0.hwmode='11g'
                set wireless.radio0.noscan='0'
                set wireless.radio0.netisolate='0'
                set wireless.radio0.max_all_num_sta='64'
                set wireless.radio0.path='platform/11000000.wifi-lb'
                set wireless.radio0.htmode='HT20'
                set wireless.radio0.disabled='0'
EOF
        uci -q batch <<-EOF
                set wireless.radio1=wifi-device
                set wireless.radio1.type='mac80211'
                set wireless.radio1.country='CN'
                set wireless.radio1.txpower_lvl='2'
                set wireless.radio1.channel='161'
                set wireless.radio1.band='5G'
                set wireless.radio1.hwmode='11a'
                set wireless.radio1.noscan='0'
                set wireless.radio1.netisolate='0'
                set wireless.radio1.max_all_num_sta='64'
                set wireless.radio1.path='platform/17800000.wifi-hb'
                set wireless.radio1.htmode='VHT80'
                set wireless.radio1.disabled='0'
EOF
        for i in $(seq 0 7)
        do
                uci add wireless wifi-iface > /dev/null                 #......ap......
                r=$(($i%2))                                             #......radio num......
                uci -q batch <<-EOF
                        set wireless.@wifi-iface[$i].ifname=wlan${i}
                        set wireless.@wifi-iface[$i].device=radio${r}
                        set wireless.@wifi-iface[$i].network=lan
                        set wireless.@wifi-iface[$i].mode=ap
                        set wireless.@wifi-iface[$i].ssid="${hostssid}${i}"
                        set wireless.@wifi-iface[$i].encryption="psk2+ccmp"
                        set wireless.@wifi-iface[$i].key=12345678
EOF
        done
        uci -q commit wireless
}

commit_wireless()
{
        uci commit wireless
}

sync_wifi_down()
{
        echo "wifi down--1">dev/ttyS0
        /sbin/wifi down
        counter=60
        while [ $counter -gt 0 ]; do
                counter=$(($counter - 1))
                num=`ifconfig | grep wlan | wc -l`
                if [ $num -eq 0 ]; then
                        break
                fi
                echo "wait wifi down $counter s" > /dev/ttyS0
                sleep 1
        done
        sleep 1
        echo "wifi down--2">dev/ttyS0
}

/etc/init.d/subservice stop
/etc/init.d/syncservice stop
#set recovery env
for _dev in /sys/class/ieee80211/*; do
        [ -e "$_dev" ] || continue
        dev="${_dev##*/}"
        echo 1 > /sys/kernel/debug/ieee80211/$dev/siwifi/recovery_enable
done

timer=1000000
while [ $timer -gt 0 ]; do
        timer=$(($timer - 1))
        echo "test num+++++++++++++++++++++++++++++++++++++++$timer" > /dev/ttyS0
	prefix=`cat /proc/sys/kernel/random/uuid | cut -c1-8`
        init_ap $prefix
        commit_wireless
        ubus call network reload
        sleep 2
        sync_wifi_down
        echo "wifi up++">dev/ttyS0
        /sbin/wifi up
        counter=60
        cost=0
        num=0
        while [ $counter -gt 0 ]; do
                counter=$(($counter - 1))
                num=`ifconfig | grep wlan | wc -l`
                if  [ $num -eq 8 ]; then
                        break
                fi
                echo "wait wifi up $counter s" > /dev/ttyS0
                sleep 1
                cost=$(($cost + 1))
        done
        if  [ $num -ne 8 ]; then
                echo "########################test fail########################">/dev/ttyS0
                echo "wifi interface num1 is $num">/dev/ttyS0
                echo "################################################">/dev/ttyS0
                break
        fi
        echo "wait wifi link up0 $cost seconds!">/dev/ttyS0
        uci -q set wireless.@wifi-iface[0].disabled='1'
        uci -q set wireless.@wifi-iface[1].disabled='1'
        uci -q set wireless.@wifi-iface[2].disabled='1'
        uci -q set wireless.@wifi-iface[3].disabled='1'
        commit_wireless
        sleep 2
        ubus call network reload
        sync_wifi_down
        echo "wifi up++">dev/ttyS0
        /sbin/wifi up
        num=0
        cost=0
        counter=60
        while [ $counter -gt 0 ]; do
                counter=$(($counter - 1))
                num=`ifconfig | grep wlan | wc -l`
                if  [ $num -eq 4 ]; then
                        break
                fi
                echo "wait wifi up2 $counter s" > /dev/ttyS0
                sleep 1
                cost=$(($cost + 1))
        done
        if  [ $num -ne 4 ]; then
                echo "########################test fail########################">/dev/ttyS0
                echo "wifi interface num2 is $num">/dev/ttyS0
                echo "################################################">/dev/ttyS0
                break
        fi
        echo "wait wifi link up1 $cost seconds!">/dev/ttyS0
done
