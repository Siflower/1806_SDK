#!/usr/bin/lua

--****************************
--Function:	Set vlanid related to certain wan
--Usage:	Run change_vlan.lua with wan_name and vlanid you want to set. '1' means to add switch_vlan if not find vlanid
--Example:	lua change_vlan.lua wan4 1099 1
--****************************

local uci = require "luci.model.uci"
local _uci_real  = cursor or _uci_real or uci.cursor()

function get_port_str()
	local port_str
	local result
	_uci_real:foreach("network","switch_vlan",
	function(s)
		port_str =  _uci_real:get("network",s[".name"],"ports")
		if port_str then
			if result == nil then
				port_str = string.match(port_str,"(%d+)t")
				result = port_str.."t"
			end
		end
	end)
	return result
end

function add_switch(vlanid)
    local s_name = _uci_real:add("network","switch_vlan")
	local default_port = "4 "..get_port_str()
    _uci_real:set("network",s_name,"vlan",vlanid)
    _uci_real:set("network",s_name,"device","switch0")
    _uci_real:set("network",s_name,"ports",default_port)
end

function set_vlanid(wan,new_vlanid)
	local finish = 0
	local ifname = _uci_real:get("network",wan,"ifname")
	local old_vlanid = tonumber(string.match(ifname,"eth0%p(%d+)"))

	if string.find(ifname,"bridge") then
		new_ifname = "bridge-eth0."..new_vlanid
	else
		new_ifname = "eth0."..new_vlanid
	end

	_uci_real:set("network",wan,"ifname",new_ifname)
	_uci_real:foreach("network","switch_vlan",
	function(s)
		if tonumber(s.vlan) == old_vlanid and finish == 0 then
			_uci_real:set("network",s[".name"],"vlan",new_vlanid)
			finish = 1
		end
	end)

	if finish == 0 and arg[3] == 1 then
		add_switch(new_vlanid)
	end

	_uci_real:save("network")
	_uci_real:commit("network")
end

local wan = arg[1]
local new_vlanid = arg[2]
if _uci_real:get("network",wan) == nil then
	print("no "..wan.." in network")
else
	set_vlanid(wan,new_vlanid)
end

