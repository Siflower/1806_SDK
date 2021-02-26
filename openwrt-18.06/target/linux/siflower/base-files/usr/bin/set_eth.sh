ifconfig eth1 up
vconfig add eth1 3
vconfig add eth1 4
ifconfig eth1.3 192.168.5.123
ifconfig eth1.4 up
ifconfig eth0 up
brctl addbr br-lan
brctl addif br-lan eth1.4
brctl addif br-lan eth0
ifconfig br-lan 192.168.4.1
#ifconfig eth1.4 192.168.4i.1
#ifconfig eth0 192.168.4.1
ethtool -s eth1 speed 100 duplex full autoneg off
echo 1 > /proc/sys/net/ipv4/ip_forward
iptables -t nat  -F
iptables -t nat -A POSTROUTING -s 192.168.4.0/24 -o eth1.3 -j MASQUERADE
#iptables -I FORWARD 1 -m conntrack --ctstate RELATED,ESTABLISHED -j FLOWOFFLOAD
iptables -F
#iptables -I FORWARD 1 -m conntrack --ctstate NEW,RELATED,ESTABLISHED -j FLOWOFFLOAD --hw
route add default gw 192.168.5.1
