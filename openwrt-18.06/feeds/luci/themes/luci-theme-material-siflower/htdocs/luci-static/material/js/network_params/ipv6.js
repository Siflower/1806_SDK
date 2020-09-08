function changeSelectBtn(id) {
    var select = document.getElementById(id);
    var index = select.selectedIndex;
    var length = select.length;
    for (var i = 0; i < length; i++) {
        var option = document.getElementById(select[i].value);
        if (option != null) {
            if (i == index) {
                option.style.display = "";
            } else {
                option.style.display = "none";
            }
        }
    }
}

function showTips() {
    document.getElementById("saveTipSucess").style.display = '';
    document.getElementById("saveTip").style.display = '';
}

function setWanDHCP(addr, gateway) {
    document.getElementById("wan_dhcp_addr").innerText = addr;
    document.getElementById("wan_dhcp_gateway").innerText = gateway;
}

function setWanStatic(addr, gateway) {
    console.log(addr);
    var index = addr.indexOf("/");
    var ip;
    var prefix;
    if (index != -1) {
        ip = addr.substring(0, index);
        prefix = addr.substring(index + 1);
    } else {
        ip = addr;
        prefix = 64;
    }
    console.log("ip is " + ip);
    document.getElementById("wan_static_addr").value = ip;
    document.getElementById("wan_static_prefix").value = prefix;
    document.getElementById("wan_static_gateway").value = gateway;
}

function setWanDnsManual(master, slave) {
    document.getElementById("wan_master").value = master;
    document.getElementById("wan_slave").value = slave;
}

function setWanDNSAuto(addr) {
    document.getElementById("wan_dns_addr").innerText = addr;
}

function setDHCPLifetime(prefered, valid) {
    document.getElementById("dhcp_prefered").value = prefered;
    document.getElementById("dhcp_valid").value = valid;
}

function setDHCPPrefix(addr, length) {
    document.getElementById("dhcp_prefix_ipaddr").value = addr;
    document.getElementById("dhcp_prefix_length").value = length;
}

function setDHCPDns(master, slave) {
    document.getElementById("dhcp_master").value = master;
    document.getElementById("dhcp_slave").value = slave;
}

function setSelection(mode, id) {
    document.getElementById(id).selectedIndex = mode;
}

function setDisabledSelection() {
    document.getElementById('dns_type').value = 'dns_auto';
    document.getElementById('wanv6_type').value = 'dhcp';
    document.getElementById('broadcast').value = 'broadcast_forbid';
    document.getElementById('dhcp_type').value = 'dhcp_auto';
}

function notifySelectionChange() {
    changeSelectBtn('wanv6_type');
    changeSelectBtn('dns_type');
    changeSelectBtn('dhcp_type');
    changeSelectBtn('broadcast');
}

function parsev6Addr(x) {
    if (x.match(/^([a-fA-F0-9:]+):(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})$/)) {
        var v6 = RegExp.$1, v4 = this.parseIPv4(RegExp.$2);
        if (!v4) {
            return null;
        }
        x = v6 + ':' + (v4[0] * 256 + v4[1]).toString(16)
            + ':' + (v4[2] * 256 + v4[3]).toString(16);
    }

    if (!x.match(/^[a-fA-F0-9:]+$/)) {
        return null;
    }

    var prefix_suffix = x.split(/::/);

    if (prefix_suffix.length > 2) {
        return null;
    }

    var prefix = (prefix_suffix[0] || '0').split(/:/);
    var suffix = prefix_suffix.length > 1 ? (prefix_suffix[1] || '0').split(/:/) : [];

    if (suffix.length ? (prefix.length + suffix.length > 7) : ((prefix_suffix.length < 2 && prefix.length < 8) || prefix.length > 8)) {
        return null;
    }

    var i, word;
    var words = [];

    for (i = 0, word = parseInt(prefix[0], 16); i < prefix.length; word = parseInt(prefix[++i], 16)) {
        if (prefix[i].length <= 4 && !isNaN(word) && word <= 0xFFFF)
            words.push(word);
        else
            return null;
    }

    for (i = 0; i < (8 - prefix.length - suffix.length); i++) {
        words.push(0);
    }

    for (i = 0, word = parseInt(suffix[0], 16); i < suffix.length; word = parseInt(suffix[++i], 16)) {
        if (suffix[i].length <= 4 && !isNaN(word) && word <= 0xFFFF)
            words.push(word);
        else
            return null;
    }

    return words;
}

function checkv6Addr(x, nomask) {
    var re = nomask ? /^([0-9a-fA-F:.]+)$/ : /^([0-9a-fA-F:.]+)(?:\/(\d{1,3}))?$/;
    var m = x.match(re);
    return m && parsev6Addr(m[1]) && (m[2] ? checkv6Musk(m[2]) : true);
}

function checkv6Musk(x) {
    if (x == null || x == "") {
        return false;
    }
    return !isNaN(x) && x >= 0 && x <= 128;
}

function checkLifetime(x) {
    if (x == null || x == "") {
        return false;
    }
    return !isNaN(x) && x >= 60 && x <= 4294967295;
}

function checkv6Prefix(x) {
    if (x == null || x == "") {
        return false;
    }
    return !isNaN(x) && x >= 16 && x <= 64;
}