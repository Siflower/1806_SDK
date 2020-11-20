#!/usr/bin/lua

--****************************
--function:	update image with /tmp/firmware.img
--usage:	put aim firmware into /tmp/firmware.img and run update.lua with ClearConfig
--example:	lua update.lua 1
--****************************

local ClearConfig = arg[1]
local nixio = require "nixio"

function fork_exec(command)
	local pid = nixio.fork()
	if pid > 0 then
		return
	elseif pid == 0 then
		nixio.chdir("/")

		local null = nixio.open("/dev/null", "w+")
		if null then
			nixio.dup(null, nixio.stderr)
			nixio.dup(null, nixio.stdout)
			nixio.dup(null, nixio.stdin)
			if null:fileno() > 2 then
				null:close()
			end
		end
		nixio.exec("/bin/sh", "-c", command)
	end
end

function image_supported()
	return (os.execute("sysupgrade -T /tmp/firmware.img >/dev/null") == 0)
end

if image_supported() then
	if tonumber(ClearConfig) == 1 then
		fork_exec("sleep 5; killall dropbear uhttpd; sleep 1; /sbin/sysupgrade -n /tmp/firmware.img")
	else
		fork_exec("sleep 5; killall dropbear uhttpd; sleep 1; /sbin/sysupgrade /tmp/firmware.img")
	end
end
