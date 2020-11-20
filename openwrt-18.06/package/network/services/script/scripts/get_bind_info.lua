#!/usr/bin/lua

--****************************
--function:	Get bind information of certain interface
--usage:	Run get_bind_info.lua with interface name
--example:	lua get_bind_info.lua lan. 
--result:	If you get 'lan1 wlan1 wlan4',it means lan has bind to lan1(eth0.1), ssid 1 and  ssid 4.
--****************************

local uci = require "luci.model.uci"
local _uci_real  = cursor or _uci_real or uci.cursor()

function get_port_list(interface)
    local port_list = {}
    local tmp
    local ifname = _uci_real:get("network",interface,"ifname")
    if ifname ~= nil then
        for word in string.gmatch(ifname, "%S+") do
            tmp = string.sub(word, 6, -1)
            if tonumber(tmp) <= 5 then
                port_list[#port_list + 1] = "lan"..tmp
            end
        end
    end

	local i = 1
	_uci_real:foreach("wireless","wifi-iface",
	function(s)
		if s.network == interface and (s.disabled == nil or s.disabled == "0") then
			port_list[#port_list + 1] = "wlan"..tostring(i)
		end
		i = i + 1
	end)
	return table.concat(port_list," ")
end

local interface = string.gsub(arg[1],'w','l')
print(get_port_list(interface))
