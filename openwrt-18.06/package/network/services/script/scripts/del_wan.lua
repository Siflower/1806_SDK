#!/usr/bin/lua

--****************************
--Function:	Delete certain interface and its vlanid
--Usage:	Run del_iface.lua with the interface you want to delete.
--Example:	lua del_iface.lua wan4
--****************************

local uci = require "luci.model.uci"
local _uci_real  = cursor or _uci_real or uci.cursor()

local finish = 0	--make sure delete vlan only once
local iface = arg[1]
local ifname = _uci_real:get("network",iface,"ifname")
local vlanid = tonumber(string.match(ifname,"eth0%p(%d+)"))

_uci_real:foreach("network","switch_vlan",
function(s)
    if tonumber(s.vlan) == vlanid and finish == 0 then
        _uci_real:delete("network",s[".name"])
		finish = 1
    end
end)
_uci_real:delete("network",iface)
_uci_real:commit("network")
