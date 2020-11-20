#!/usr/bin/lua

--****************************
--function:	Get linkstatus of wan
--usage:	Run get_wan_status.lua and you will get the result info
--example:	lua get_bind_info.lua
--****************************

local uci = require "luci.model.uci"
local _uci_real  = cursor or _uci_real or uci.cursor()


function get_connect()
	local ntm = require "luci.model.network".init()
	local wan = ntm:get_wan_networks()[1]
	local ipaddr
	local proto = _uci_real:get("netowrk","wan","proto")

	if proto == static then
		ipaddr = _uci_real:get("netowrk","wan","ipaddr")
	else
		if wan then
			ipaddr = wan:ipaddr()
		end
	end

	local connect = ipaddr == nil and 0 or 1
	return connect
end

function get_linkstatus()
	local ports
	local vlanid = string.match(_uci_real:get("network","wan","ifname"),"eth0%p(%d)")
	_uci_real:foreach("network","switch_vlan",
	function(s)
		if _uci_real:get("network",s[".name"],"vlan") == vlanid then
			ports = _uci_real:get("network",s[".name"],"ports")
			return
		end
	end)
	local phy = "phy"..string.sub(ports,0,1)
	local cmd = "cat sys/kernel/debug/gsw_debug | grep "..phy.." | awk -F ' ' '{print $3}'"
	local file = io.popen(cmd)
	local linkstatus = tonumber(file:read("*n"))
	file:close()
	return linkstatus
end

if get_linkstatus() == 0 then
	print("No link")
elseif get_connect() == 0 then
	print("Linked but not connected, get no ipaddr")
else
	print("conncet success")
end
