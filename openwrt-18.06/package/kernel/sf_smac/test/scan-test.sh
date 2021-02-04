#!/bin/sh
timer=1000000
while [ $timer -gt 0 ]; do
    timer=$(($timer - 1))
    iw dev wlan1 scan > /tmp/3.txt
    sleep 5
done
