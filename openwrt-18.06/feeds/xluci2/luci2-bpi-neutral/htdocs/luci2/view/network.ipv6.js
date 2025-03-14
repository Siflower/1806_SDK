L.ui.view.extend({
	execute: function() {
		var self = this;
		var m = new L.cbi.Map('network', {
			caption:        L.tr('IPV6'),
			collabsible: true
		});
		var s = m.section(L.cbi.NamedSection, 'wan6', {
			caption:      L.tr('WAN')
		});

		var ip6 = s.option(L.cbi.CheckboxValue, "__en", {
			caption: L.tr("Enable"),
			optional: true,
		});
		ip6.ucivalue = function (sid) {
			var enable = L.uci.get("network", "wan6", "disabled");
			if (enable == 1) {
				return false;
			} else {
				return true;
			}
		};
		ip6.save = function (sid) {
			var enable = ip6.formvalue(sid);
			var wan_proto = L.uci.get("network", "wan", "proto");
			if (enable == false) {
				L.uci.set("network", "wan6", "disabled", "1");
				L.uci.set("dhcp", "lan", "dhcpv6", "disabled");
				L.uci.set("dhcp", "lan", "ra", "disabled");
			} else {
				L.uci.unset("network", "wan6", "disabled");
				if (wan_proto == "pppoe") {
					L.uci.set("dhcp", "lan", "dhcpv6", "server");
					L.uci.set("dhcp", "lan", "ra", "server");
				} else {
					L.uci.set("dhcp", "lan", "dhcpv6", "relay");
					L.uci.set("dhcp", "lan", "ra", "relay");
				}
			}
			L.uci.save();
		};

		var p = s.option(L.cbi.ListValue, 'proto', {
			caption:        L.tr('Proto')
		})
			.value('dhcpv6', L.tr('Auto'))
			.value('static', L.tr('Manual'));
		p.save = function(sid){
			var proto = this.formvalue(sid);
			if(proto == "dhcpv6"){
				L.uci.unset("network","wan6","ip6addr");
				L.uci.unset("network","wan6","ip6gw");
			}
			L.uci.set("network","wan6","proto",proto);
			L.uci.save();
		}
		addr = s.option(L.cbi.InputValue, 'ip6addr', {
			caption:        L.tr('IP Address'),
			datatype:    'cidr6'
		}).depends('proto',function(v){
			return(v != 'dhcpv6');
		});
		gw = s.option(L.cbi.InputValue, 'ip6gw', {
			caption:        L.tr('Gateway'),
			datatype:    'ip6addr'
		}).depends('proto',function(v){
			return(v != 'dhcpv6');
		});
		var manualdns = s.option(L.cbi.ListValue, '__dns', {
			caption:        L.tr('Type Of DNS'),
			initial:        'dhcpv6'
		})
			.value('auto', L.tr('Auto'))
			.value('manual', L.tr('Manual'));
		manualdns.save = function(sid){
			var dns_status = this.formvalue(sid);
			if(dns_status == "auto"){
				L.uci.unset("network","wan6","dns");
			}
			L.uci.save();
		}
		manualdns.ucivalue = function(sid){
			var ifdns = L.uci.get("network","wan6","dns");
			if(ifdns){
				return "manual";
			}else{
				return "auto";
			}
		};
		zdns = s.option(L.cbi.InputValue, "__zdns", {
			caption:        L.tr('DNS Master Server Address'),
			datatype:    'ip6addr'
		}).depends('__dns',function(v){
			return(v != 'auto');
		});
		cdns = s.option(L.cbi.InputValue, "__cdns", {
			caption:        L.tr('DNS Slave Server Address'),
			datatype:    'ip6addr',
			optional:    true
		}).depends('__dns',function(v){
			return(v != 'auto');
		});
		zdns.ucivalue = function(sid){
			var dns_value = L.uci.get("network", "wan6", "dns");
			var str;
			if(dns_value){
				str = dns_value.indexOf(" ");
				if(str == -1)
					return dns_value;
				else
					return dns_value.substring(0,str);
			}
		};
		cdns.ucivalue = function(sid){
			var dns_value = L.uci.get("network", "wan6", "dns");
			var str;
			if(dns_value){
				str = dns_value.indexOf(" ");
				if(str < dns_value.length-1 && str > 0)
					return dns_value.substring(str+1);
			}
		};
		cdns.save = zdns.save = function(sid){
			var z1dns = zdns.formvalue(sid);
			var c1dns = cdns.formvalue(sid);
			var dns_status = manualdns.formvalue(sid);
			if(dns_status == "manual"){
				if(c1dns){
					var sdns = z1dns + ' ' + c1dns;
					L.uci.set("network","wan6","dns", sdns);
				}
				else
					L.uci.set("network","wan6","dns", z1dns)
			}
			L.uci.save();
		};


		var d = new L.cbi.Map("dhcp", {
			emptytop: '0px'
		});

		var l = d.section(L.cbi.NamedSection, "lan", {
			caption: L.tr("LAN"),
		});
		var rd = l
			.option(L.cbi.ListValue, "__ra", {
				caption: L.tr("Router Broadcast"),
				datatype: "string",
				initial: "1",
			})
			.value("1", L.tr("Auto"))
			.value("0", L.tr("Disable"));
		rd.save = function (sid) {
			var ra = rd.formvalue(sid);
			if (ra == "0") {
				L.uci.set("dhcp", "lan", "ra_default", "");
			} else if (ra == "1") {
				L.uci.set("dhcp", "lan", "ra_default", "1");
			}
			L.uci.save();
		};
		rd.ucivalue = function (sid) {
			var ra = L.uci.get("dhcp", "lan", "ra_default");
			if (ra == undefined) return "0";
			else return ra;
		};
		var dhcpm = "";
		var dm = l
			.option(L.cbi.ListValue, "__dhcpmode", {
				caption: L.tr("DHCP Mode"),
				initial: "auto",
			})
			.value("auto", L.tr("Auto"))
			.value("manual", L.tr("Manual"))
			.value("disable", L.tr("Disable"));
			dm.save = function (sid) {
				var dhcpmode = dm.formvalue(sid);
				var wan_proto = L.uci.get("network", "wan", "proto");
				if (dhcpmode == "auto") {
					if (wan_proto == "pppoe") {
						L.uci.set("dhcp", "lan", "dhcpv6", "server");
						L.uci.set("dhcp", "lan", "ra", "server");
					} else {
						L.uci.set("dhcp", "lan", "dhcpv6", "relay");
						L.uci.set("dhcp", "lan", "ra", "relay");
					}
					L.uci.unset("network", "wan6", "ip6prefix");
					L.uci.unset("dhcp", "lan", "dns");
					L.uci.unset("dhcp", "lan", "domain");
				} else if (dhcpmode == "disable") {
					L.uci.set("dhcp", "lan", "dhcpv6", "disabled");
					L.uci.set("dhcp", "lan", "ra", "disabled");
					L.uci.unset("network", "wan6", "ip6prefix");
					L.uci.unset("dhcp", "lan", "dns");
					L.uci.unset("dhcp", "lan", "domain");
					dhcpm = "2";
				} else if (dhcpmode == "manual") {
					if (wan_proto == "pppoe") {
						L.uci.set("dhcp", "lan", "dhcpv6", "server");
						L.uci.set("dhcp", "lan", "ra", "server");
					} else {
						L.uci.set("dhcp", "lan", "dhcpv6", "relay");
						L.uci.set("dhcp", "lan", "ra", "relay");
					}
					dhcpm = "3";
				}
				L.uci.save();
			};
			dm.ucivalue = function (sid) {
				var lan_dhcpv6 = L.uci.get("dhcp", "lan", "dhcpv6");
				var wan_ip6prefix = L.uci.get("network", "wan6", "ip6prefix");
				if (lan_dhcpv6 == "server" || lan_dhcpv6 == "relay") {
					if (wan_ip6prefix == undefined || wan_ip6prefix == "auto") {
						return "auto";
					} else {
						return "manual";
					}
				} else if (lan_dhcpv6 == "disabled") {
					return "disable";
				};
			}
		var prefix = l
			.option(L.cbi.InputValue, "__ip6pre", {
				caption: L.tr("Prefix"),
				datatype: "cidr6",
			})
			.depends("__dhcpmode", function (v) {
				return v == "manual";
			});
		var preferred_lifetime = l
			.option(L.cbi.InputValue, "__prelt", {
				caption: L.tr("Preferred Lifetime"),
				datatype: "range(60,4294967295)",
			})
			.depends("__dhcpmode", function (v) {
				return v == "manual";
			});
		var valid_lifetime = l
			.option(L.cbi.InputValue, "__vlt", {
				caption: L.tr("Valid Lifetime"),
				datatype: "range(60,4294967295)",
			})
			.depends("__dhcpmode", function (v) {
				return v == "manual";
			});

		prefix.ucivalue = function (sid) {
			var get_str = L.uci.get("network", "wan6", "ip6prefix");
			if (get_str == undefined) {
				return get_str;
			}
			var x = get_str.indexOf(",");
			var pf = get_str.substring(0, x);
			return pf;
		};

		preferred_lifetime.ucivalue = function (sid) {
			var get_str = L.uci.get("network", "wan6", "ip6prefix");
			if (get_str == undefined) {
				return get_str;
			}
			var y = get_str.indexOf(",");
			get_str = get_str.substring(y + 1, get_str.length);
			y = get_str.indexOf(",");
			var plt = get_str.substring(0, y);
			return plt;
		};

		valid_lifetime.ucivalue = function (sid) {
			var get_str = L.uci.get("network", "wan6", "ip6prefix");
			if (get_str == undefined) {
				return get_str;
			}
			var y = get_str.indexOf(",");
			get_str = get_str.substring(y + 1, get_str.length);
			y = get_str.indexOf(",");
			var vlt = get_str.substring(y + 1, get_str.length);
			return vlt;
		};

		valid_lifetime.save =
			preferred_lifetime.save =
			prefix.save =
				function (sid) {
					var pf = prefix.formvalue(sid);
					var plt = preferred_lifetime.formvalue(sid);
					var vlt = valid_lifetime.formvalue(sid);

					if (pf && plt && vlt && dhcpm == "3") {
						var set_str = pf + "," + plt + "," + vlt;
						L.uci.set("network", "wan6", "ip6prefix", set_str);
						L.uci.save();
					}
				};

		var dns1 = l
			.option(L.cbi.InputValue, "__dns", {
				caption: L.tr("DNS Master Server Address"),
				datatype: "ip6addr",
			})
			.depends("__dhcpmode", function (v) {
				return v == "manual";
			});
		dns1.save = function (sid) {
			if (dm.formvalue(sid) == "manual") {
				var dns = dns1.formvalue(sid);
				L.uci.set("dhcp", "lan", "dns", dns);
				L.uci.save();
			}
		};
		dns1.ucivalue = function (sid) {
			var dns = L.uci.get("dhcp", "lan", "dns");
			return dns;
		};

		var dns2 = l
			.option(L.cbi.InputValue, "__domain", {
				caption: L.tr("DNS Slave Server Address"),
				datatype: "ip6addr",
				optional: true,
			})
			.depends("__dhcpmode", function (v) {
				return v == "manual";
			});
		dns2.save = function (sid) {
			if (dm.formvalue(sid) == "manual") {
				var domain = dns2.formvalue(sid);
				L.uci.set('dhcp', 'lan', 'domain', domain);
				L.uci.save();
			}
		};
		dns2.ucivalue = function (sid) {
			var domain = L.uci.get("dhcp", "lan", "domain");
			return domain;
		};
		m.insertInto("#map");
		d.insertInto("#map2");
	},
});


