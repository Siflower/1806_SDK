#!/bin/sh

USB_DEV="/dev/ttyUSB2"
PCIE_DEV="/dev/stty_nr31"

# 拨号方式: usb(默认) / pcie
MODE="${1:-usb}"

if [ "$MODE" = "pcie" ]; then
    MODEM_DEV="$PCIE_DEV"
    NET_DEV="pcie0"
else
    MODEM_DEV="$USB_DEV"
    NET_DEV="usb0"
fi

send_at() {
    local CMD="$1" line got="" n=0
    while read -t 1 -r line <&3 2>/dev/null; do :; done
    printf '%s\r' "$CMD" >&3
    while read -t 1 -r line <&3 2>/dev/null; do
        got="$got$line
"
        n=$((n + 1))
    done
    if [ -n "$DIAL_DEBUG" ]; then
        printf '<< %s\n' "$CMD" >&2
        printf '%s' "$got" >&2
    fi
    printf '%s' "$got"
}

common_resp() {
    local CMD="$1"
    local RESP
    RESP=$(send_at "$CMD")
    if echo "$RESP" | grep -q "OK"; then
        return 0
    else
        return 1
    fi
}

wait_for_modem_ready() {
    local dev="$1"
    local timeout_sec="${2:-30}"
    local i=0
    local resp

    echo "等待 $dev 就绪..."

    while [ "$i" -lt "$timeout_sec" ]; do
        if [ -e "$dev" ]; then
            if [ "$dev" == "$USB_DEV" ]; then
                if command -v stty >/dev/null 2>&1; then
                    stty -F "$dev" 115200 raw -echo 2>/dev/null
                fi
            fi
            exec 3<>"$dev" 2>/dev/null
            if [ $? -eq 0 ]; then
                common_resp 'AT' && return 0
                exec 3>&-
                exec 3<&-
            fi
        fi

        sleep 2
        i=$((i + 1))
        [ $((i % 5)) -eq 0 ] && echo "等待 $dev 就绪... 已尝试 $i 次未获 AT OK"
    done

    return 1
}

get_sim_state() {
    local RESP1 RESP2 simstat cpin
    RESP1=$(send_at 'AT+QSIMSTAT?')
    RESP2=$(send_at 'AT+CPIN?')
    simstat=$(echo "$RESP1" | sed -n 's/.*+QSIMSTAT: [0-9]\+,\([0-9]\+\).*/\1/p' | tr -d '[:space:]')
    cpin=$(echo "$RESP2" | sed -n 's/.*+CPIN: \([A-Z]\+\).*/\1/p' | tr -d '[:space:]')
    [ "$simstat" = "1" ] && [ "$cpin" = "READY" ]
}

get_init_stat() {
    local RESP initstat
    RESP=$(send_at 'AT+QINISTAT')
    initstat=$(echo "$RESP" | sed -n 's/.*+QINISTAT: \([0-9]\+\).*/\1/p' | tr -d '[:space:]')
    [ "$initstat" = "7" ]
}

wait_cgact_success() {
    local i RESP
    for i in $(seq 1 5); do
        common_resp 'AT+CGACT=1,1' && return 0
        sleep 3
    done
    return 1
}

find_zone_index() {
    local name="$1" n
    local i=0
    while uci get firewall.@zone[$i] >/dev/null 2>&1; do
        n=$(uci get firewall.@zone[$i].name 2>/dev/null)
        [ "$n" = "$name" ] && { echo "$i"; return 0; }
        i=$((i + 1))
    done
    return 1
}

find_forwarding_index() {
    local src="$1" dest="$2" s d
    local i=0
    while uci get firewall.@forwarding[$i] >/dev/null 2>&1; do
        s=$(uci get firewall.@forwarding[$i].src 2>/dev/null)
        d=$(uci get firewall.@forwarding[$i].dest 2>/dev/null)
        [ "$s" = "$src" ] && [ "$d" = "$dest" ] && { echo "$i"; return 0; }
        i=$((i + 1))
    done
    return 1
}

setup_firewall() {
    local changed=0 i cur z f wi

    # 关闭 flow offloading
    for i in flow_offloading flow_offloading_hw; do
        cur=$(uci get firewall.@defaults[0].$i 2>/dev/null)
        if [ "$cur" != "0" ]; then
            uci set firewall.@defaults[0].$i=0
            changed=1
        fi
    done

    # wan zone 加入 wwan (若缺失)
    wi=$(find_zone_index wan)
    if [ -n "$wi" ]; then
        if ! uci get firewall.@zone[$wi].network 2>/dev/null | grep -qx wwan; then
            uci add_list firewall.@zone[$wi].network=wwan
            changed=1
        fi
    fi

    # 建立 wwan zone (若缺失)
    if ! find_zone_index wwan >/dev/null; then
        z=$(uci add firewall zone)
        uci set firewall.$z.name=wwan
        uci set firewall.$z.network=wwan
        uci set firewall.$z.input=REJECT
        uci set firewall.$z.output=ACCEPT
        uci set firewall.$z.forward=REJECT
        uci set firewall.$z.masq=1
        uci set firewall.$z.mtu_fix=1
        changed=1
    fi

    # guest -> wwan 转发 (若缺失)
    if ! find_forwarding_index guest wwan >/dev/null; then
        f=$(uci add firewall forwarding)
        uci set firewall.$f.src=guest
        uci set firewall.$f.dest=wwan
        changed=1
    fi

    if [ "$changed" = 1 ]; then
        uci commit firewall
        /etc/init.d/firewall restart
    fi
}

add_config() {
    cat >> /etc/config/network <<EOF
config interface 'wwan'
    option proto 'dhcp'
    option ifname '$NET_DEV'
EOF

    if [ "$MODE" = "pcie" ]; then
        cat >> /etc/config/network <<EOF

config interface 'dummy0'
    option proto 'dhcp'
    option device 'sipa_dummy0'
EOF
    fi
}

ensure_wwan_ifname() {
    # 兼容旧配置: 若已有 device 写法, 改写为 ifname
    if uci get network.wwan >/dev/null 2>&1; then
        local dev ifn
        dev=$(uci get network.wwan.device 2>/dev/null)
        ifn=$(uci get network.wwan.ifname 2>/dev/null)
        if [ -n "$dev" ]; then
            uci delete network.wwan.device
            uci set network.wwan.ifname="$NET_DEV"
            uci commit network
        elif [ "$ifn" != "$NET_DEV" ]; then
            uci set network.wwan.ifname="$NET_DEV"
            uci commit network
        fi
    fi
}

# 打开读写 fd
wait_for_modem_ready "$MODEM_DEV" || { echo "等待设备通信准备失败"; exit 1; }

get_sim_state || { echo "SIM卡检测失败"; exec 3>&-; exec 3<&-; exit 1; }
get_init_stat || { echo "模块初始化检测失败"; exec 3>&-; exec 3<&-; exit 1; }
common_resp 'AT+QCFG="autoapn",1' || { echo "启用自动 APN 选择失败"; exec 3>&-; exec 3<&-; exit 1; }
wait_cgact_success || { echo "激活 PDP 上下文失败,请检查天线是否连接"; exec 3>&-; exec 3<&-; exit 1; }
common_resp 'AT+QNETDEVCTL=1,1,1' || { echo "连接指定 PDP 建立失败"; exec 3>&-; exec 3<&-; exit 1; }

# 关闭 fd
exec 3>&-
exec 3<&-

ensure_wwan_ifname

EXIST=$(grep -c "option ifname '$NET_DEV'" /etc/config/network)
if [ "$EXIST" -eq 0 ]; then
    add_config
fi

setup_firewall
/etc/init.d/network restart