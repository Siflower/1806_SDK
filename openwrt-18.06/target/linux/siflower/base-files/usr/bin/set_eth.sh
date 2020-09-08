ifconfig eth0 up
vconfig add eth0 3
vconfig add eth0 4
ifconfig eth0.3 192.168.5.123
ifconfig eth0.4 192.168.6.1
ethtool -s eth0 speed 100 duplex full autoneg off
echo 1 > /proc/sys/net/ipv4/ip_forward
iptables -t nat  -F
iptables -t nat -A POSTROUTING -s 192.168.6.0/24 -o eth0.3 -j MASQUERADE
#iptables -I FORWARD 1 -m conntrack --ctstate RELATED,ESTABLISHED -j FLOWOFFLOAD
iptables -F
iptables -I FORWARD 1 -m conntrack --ctstate NEW,RELATED,ESTABLISHED -j FLOWOFFLOAD --hw
route add default gw 192.168.5.1
