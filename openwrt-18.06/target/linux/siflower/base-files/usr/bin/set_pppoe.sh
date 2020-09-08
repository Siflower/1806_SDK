ethtool -s eth0 speed 100 duplex full autoneg off
echo 1 > /proc/sys/net/ipv4/ip_forward
iptables -t nat -F
iptables -t nat -A POSTROUTING -s 192.168.6.0/24 -o pppoe-wan -j MASQUERADE
#iptables -F
#iptables -I FORWARD 1 -m conntrack --ctstate NEW,RELATED,ESTABLISHED -j FLOWOFFLOAD --hw
route add default gw 192.168.8.1
