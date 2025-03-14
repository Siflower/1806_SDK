L.ui.view.extend({

	get_status: L.rpc.declare({
		object: 'web.network',
		method: 'getstatus',
		expect: { }
	}),

	get_ipaddr: L.rpc.declare({
		object: 'network.interface.wan',
		method: 'status',
		expect: { }
	}),

	execute: function() {
		var self = this;
		var m = new L.cbi.Map('network', {
			caption:     L.tr("WAN"),
			collabsible: true
		});

		var s = m.section(L.cbi.NamedSection, 'wan', {
			caption:      L.tr("WAN port settings")
		});

		var wanstatus = s.option(L.cbi.DummyValue, '__wanstatus', {
                        caption:        L.tr("WAN status")
                });


		wanstatus.load = function(sid)
		{
			var id = this.id(sid);
			self.get_status().then(function(info) {
				var stat = info.status;
				var protonow = L.uci.get('network', 'wan', 'proto');
				var ipaddrnow = L.uci.get('network', 'wan', 'ipaddr');
				if (stat != 0 && stat != 1) {
					setTimeout(function () {
						parent.location.reload();
                                                }, 3000);
                                        $('#' + id).text(L.tr("Testing"));
					stat = info.status;
				}
				else if (stat == 0 || stat == 1) {
					if (protonow != "bridge") {
						self.get_ipaddr().then(function(ip) {
							if(!ip){
								setTimeout(function () {
									parent.location.reload();
								}, 3000);
								$('#' + id).text(L.tr("Testing"));
							}
							else {
								var ipstatus = 0;
								if (stat == 1) {
									if (protonow == "pppoe") {
									}
									else {
										var ipexist = $.isEmptyObject(ip["ipv4-address"][0].address);
										if (ipexist != true)
											ipstatus = ip["ipv4-address"][0].address;
										else {}
									}
								}

								// wan status in dhcp mode
								if (protonow == 'dhcp' && stat == 0)
									$('#' + id).text(L.tr("The network cable is not connected"));
								else if (protonow == 'dhcp' && stat == 1 && ipstatus != 0)
									$('#' + id).text(L.tr("Connected and can go online normally"));
								else if (protonow == 'dhcp' && stat == 1 && ipstatus == 0)
									$('#' + id).text(L.tr("Can't connect to the internet"));

								// wan status in static mode
								else if (protonow == 'static' && stat == 1 && typeof(ipaddrnow) != 'undefined')
									$('#' + id).text(L.tr("Connected and can go online normally"));
								else if (protonow == 'static' && stat == 0 && typeof(ipaddrnow) != 'undefined')
									$('#' + id).text(L.tr("The network cable is not connected"));

								// wan status in pppoe mode
								else if (protonow == 'pppoe' && stat == 1 )
									$('#' + id).text(L.tr("The network cable is connected"));
								else if (protonow == 'pppoe' && stat == 0 )
									$('#' + id).text(L.tr("The network cable is not connected"));
							}
						});
					}
					else if (protonow == "bridge") {
						if (protonow == 'bridge' && stat == 1 )
							$('#' + id).text(L.tr("Currently in bridge mode"));
						else if (protonow == 'bridge' && stat == 0 )
							$('#' + id).text(L.tr("The network cable is not connected"));
					}
				}
			});
		};

		p = s.option(L.cbi.ListValue, 'proto', {
			caption:	L.tr("Mode"),
			initial:	'dhcp'
		})
		.value('dhcp', L.tr("DHCP"))
		.value('static', L.tr("STATIC"))
		.value('pppoe', L.tr("PPPOE"))
		.value('bridge', L.tr("Bridge"))

		p.save = function(sid){
			var value = this.formvalue(sid);
			var ori_value = L.uci.get('network', 'wan', 'proto');
			if(ori_value == 'bridge' && value != 'bridge'){
				L.uci.unset('network', 'wan', 'disabled');
				L.uci.set('network', 'lan', 'proto', 'static');
			}
			if(value == 'pppoe'){
				L.uci.unset('network', 'wan', 'ipaddr');
				L.uci.unset('network', 'wan', 'gateway');
				L.uci.unset('network', 'wan', 'netmask');
				L.uci.set('network', 'wan', 'proto', 'pppoe');
			} else if(value == 'static') {
				L.uci.unset('network', 'wan', 'username');
				L.uci.unset('network', 'wan', 'password');
				L.uci.unset('network', 'wan', 'auto');
				L.uci.set('network', 'wan', 'proto', 'static');
			} else if(value == 'dhcp') {
				L.uci.unset('network', 'wan', 'ipaddr');
				L.uci.unset('network', 'wan', 'gateway');
				L.uci.unset('network', 'wan', 'netmask');
				L.uci.unset('network', 'wan', 'username');
				L.uci.unset('network', 'wan', 'password');
				L.uci.unset('network', 'wan', 'auto');
				L.uci.set('network', 'wan', 'proto', 'dhcp');
			} else if(value == 'bridge') {
				L.uci.unset('network', 'wan', 'ipaddr');
				L.uci.unset('network', 'wan', 'gateway');
				L.uci.unset('network', 'wan', 'netmask');
				L.uci.unset('network', 'wan', 'username');
				L.uci.unset('network', 'wan', 'password');
				L.uci.unset('network', 'wan', 'auto');
				L.uci.set('network', 'wan', 'proto', 'bridge');
				L.uci.set('network', 'wan', 'disabled', '1');
				L.uci.set('network', 'lan', 'ifname', 'eth0.1 eth0.2');
				L.uci.set('network','lan','proto','dhcp');
			}
			L.uci.save();
		}

		var ipdata = s.option(L.cbi.InputValue, 'ipaddr', {
			caption:      L.tr("IP address"),
			datatype:    'ip4addr'
		}).depends('proto', function(v) { return (v == 'static');});

		e = s.option(L.cbi.InputValue, 'netmask', {
			caption:      L.tr("Subnet mask"),
			datatype:    'netmask4'
		}).depends('proto', function(v) { return (v == 'static');});

		e = s.option(L.cbi.InputValue, 'gateway', {
			caption:      L.tr("Default gateway"),
			datatype:    function(v) {
				var data_ipaddr = ipdata.formvalue('wan');
				var segment = data_ipaddr.slice(0,9);
				if (segment != v.slice(0,9))
					return L.tr("The gateway must be in the same network segment as the ip address");
				else if (segment == v.slice(0,9) && v.slice(9,10) != ".")
					return L.tr("Please enter the standard ipv4 format");
				else if (v.slice(10) <= 255 && v.slice(10) >= 1)
					return true;
				else
					return L.tr("Please enter the standard ipv4 format");
			}
		}).depends('proto', function(v) { return (v == 'static');});

		e = s.option(L.cbi.InputValue, 'username', {
			caption:      L.tr("Broadband account")
		}).depends('proto', function(v) { return (v == 'pppoe');});

		e = s.option(L.cbi.PasswordValue, 'password', {
			caption:      L.tr("Broadband password")
		}).depends('proto', function(v) { return (v == 'pppoe');});

		e = s.option(L.cbi.CheckboxValue, 'auto', {
			caption:      L.tr("Auto connect")
		}).depends('proto', function(v) { return (v == 'pppoe');});

		var manualdns = s.option(L.cbi.CheckboxValue, '__manualdns', {
			caption:      L.tr("Manually set up DNS server"),
			optional:     true
		}).depends('proto', function(v) { return (v == 'dhcp' || v == 'pppoe');});

		manualdns.ucivalue = function(sid) {
			var peerdns_value = L.uci.get('network', 'wan', 'peerdns');
			if (peerdns_value == 1)
				return false;
			else if (typeof(peerdns_value) == 'undefined')
				return false;
			else if (peerdns_value == 0)
				return true;
		};

		manualdns.save = function(sid) {
			var mdns = manualdns.formvalue(sid);
			var proto_value = p.formvalue(sid);
			if (proto_value != 'static') {
				if(mdns == 0) {
					L.uci.set('network', 'wan', 'peerdns', 1);
					L.uci.unset('network', 'wan', 'dns');
				}
				else
					L.uci.set('network', 'wan', 'peerdns', 0);
			}
			else if (proto_value == 'static') { }
			L.uci.save();
		};

		var dns1 = s.option(L.cbi.InputValue, '__dns1', {
			caption:      L.tr("Preferred DNS server"),
			datatype:     'ip4addr'
		}).depends('__manualdns', function(v) {
			var tmp = false;
			var tmp_proto = p.formvalue('wan');
			if(tmp_proto == "static")
				tmp = true;
			return v || tmp;
		});

		var dns2 = s.option(L.cbi.InputValue, '__dns2', {
			caption:      L.tr("Standby DNS server"),
			optional:     true,
			datatype:     'ip4addr',
			description:  L.tr("Optional")
		}).depends('__manualdns', function(v) {
			var tmp = false;
			var tmp_proto = p.formvalue('wan');
			if(tmp_proto == "static")
				tmp = true;
			return v || tmp;
		});

		dns1.ucivalue = function(sid) {
			var dns_value = L.uci.get("network", "wan", "dns");
			var str;
			if (dns_value){
				str = dns_value.indexOf(" ");
				if(str == -1)
					return dns_value;
				else
					return dns_value.substring(0,str);
			}
		};

		dns2.ucivalue = function(sid) {
			var dns_value = L.uci.get("network", "wan", "dns");
			var str;
			if (dns_value){
				str = dns_value.indexOf(" ");
				if(str < dns_value.length-1 && str > 0)
					return dns_value.substring(str+1);
			}
		};

		dns1.save = dns2.save =  function(sid) {
			var dnsv1 = dns1.formvalue(sid);
			var dnsv2 = dns2.formvalue(sid);
			var mdns = manualdns.formvalue(sid);
			var proto_value = p.formvalue(sid);
			if (proto_value != 'static') {
				if(!mdns)
					L.uci.unset('network', 'wan', 'dns');
				else{
					if(dnsv2){
						var dnsv = dnsv1 + ' ' + dnsv2;
						L.uci.set('network', 'wan', 'dns', dnsv);
					} else
						L.uci.set('network', 'wan', 'dns', dnsv1);
				}
			}
			else if (proto_value == 'static') {
				L.uci.unset('network', 'wan', 'peerdns');
				if(dnsv2){
					var dnsv = dnsv1 + ' ' + dnsv2;
					L.uci.set('network', 'wan', 'dns', dnsv);
					} 
				else
					L.uci.set('network', 'wan', 'dns', dnsv1);
			}
			L.uci.save();
		};

		pmtu = s.option(L.cbi.InputValue, 'mtu', {
			caption:      L.tr("Packet MTU byte"),
			initial:      1500,
			description:  L.tr("The default is 1500, if not necessary, please do not modify")
		});

		pmtu.ucivalue = function(sid) {
						var mtu_value = L.uci.get("network", "wan", "mtu");
						if (mtu_value)
							return mtu_value-4;
						else
							return 1500;
				};

		function accAdd(arg1,arg2){
			var r1,r2,m;
			try{r1=arg1.toString().split(".")[1].length}catch(e){r1=0};
			try{r2=arg2.toString().split(".")[1].length}catch(e){r2=0};
			m=Math.pow(10,Math.max(r1,r2));
			return (arg1*m+arg2*m)/m;
		};

		pmtu.save = function(sid) {
			var pamtu = pmtu.formvalue(sid);
			if(pamtu > 38 && pamtu < 9000)
				L.uci.set('network', 'wan', 'mtu', accAdd(pamtu,4));
			else
				L.uci.set('network', 'wan', 'mtu', 1504);
			L.uci.save();
		};

		uni = s.option(L.cbi.CheckboxValue, 'unicast', {
			caption:      L.tr("Open unicast to get ip"),
			optional:     true,
			description:  L.tr("Do not choose under normal circumstances")
		}).depends('proto', function(v) { return (v == 'dhcp');});

		uni.save = function(sid) {
			var un = uni.formvalue(sid);
			var tp = false;
			var tp_proto = p.formvalue('wan');
			if(tp_proto == "dhcp")
				tp = true;
			if(tp){
				if(un)
					L.uci.set('network', 'wan', 'unicast', 1);
				else
					L.uci.set('network', 'wan', 'unicast', 0);
			}else
				L.uci.unset('network', 'wan', 'unicast');
			L.uci.save();
		};

		wanspeed = s.option(L.cbi.ListValue, 'speed', {
			caption:    L.tr("WAN port rate setting"),
			initial:    'auto',
			description: L.tr("When using a fixed rate, the peer should also be set to a fixed rate")
		})
		.value('auto', L.tr("Auto-negotiation"))
		.value('10mfull', L.tr("10M full-duplex"))
		.value('10mhalf', L.tr("10M half-duplex"))
		.value('100mfull', L.tr("100M full-duplex"))
		.value('100mhalf', L.tr("100M half-duplex"))
		.value('1000mfull', L.tr("1000M full-duplex"));

		wanspeed.save = function(sid) {
			var wansp = wanspeed.formvalue(sid);
			L.uci.set('network', 'wan', 'speed', wansp);
			L.uci.save();
		};

		servicename = s.option(L.cbi.InputValue, 'service', {
			caption:      L.tr("Service Name"),
			optional:     true
		}).depends('proto', function(v) { return (v == 'pppoe');});

		servicename.save = function(sid) {
			var sname = servicename.formvalue(sid);
			var tmp = false;
			var tmp_proto = p.formvalue('wan');
			if(tmp_proto == "pppoe")
				tmp = true;
			if(tmp && sname)
				L.uci.set('network', 'wan', 'service', sname);
			else
				L.uci.unset('network', 'wan', 'service');
			L.uci.save();
		};

		servername = s.option(L.cbi.InputValue, 'ac', {
			caption:      L.tr("Server name"),
			optional:     true
		}).depends('proto', function(v) { return (v == 'pppoe');});

		servername.save = function(sid) {
			var svname = servername.formvalue(sid);
			var tmp = false;
			var tmp_proto = p.formvalue('wan');
			if(tmp_proto == "pppoe")
				tmp = true;
			if(tmp && svname)
				L.uci.set('network', 'wan', 'ac', svname);
			else
				L.uci.unset('network', 'wan', 'ac');
			L.uci.save();
		};

		uad = s.option(L.cbi.CheckboxValue, '__useoperatoraddress', {
			caption:      L.tr("Use the ip address specified by the operator"),
			optional:     true
		}).depends('proto', function(v) { return (v == 'pppoe');});

		uad.ucivalue  = function(sid) {
			var po = L.uci.get("network", "wan", "pppd_options");
			if(po)
				return true;
			else
				return false;
		};

		uad.save = function(sid) {
			var ua = uad.formvalue(sid);
			var tmp = false;
			var tmp_proto = p.formvalue('wan');
			if(tmp_proto == "pppoe")
				tmp = true;
			if(tmp){
				if(ua)
					L.uci.set('network', 'wan', 'fixipEnb', 1);
				else
					L.uci.set('network', 'wan', 'fixipEnb', 0);
			}else
				L.uci.unset('network', 'wan', 'fixipEnb');
			L.uci.save();
		};

		operatoraddress = s.option(L.cbi.InputValue, 'pppd_options', {
			caption:      L.tr("The ip address specified by the operator")
		}).depends('__useoperatoraddress', function(v) { return v;});

		operatoraddress.ucivalue  = function(sid) {
			var po = L.uci.get("network", "wan", "pppd_options");
			if(po)
				return po;
		};

		operatoraddress.save = function(sid) {
			var ua = uad.formvalue(sid);
			var oad;
			if(ua){
				oad = operatoraddress.formvalue(sid);
				L.uci.set('network', 'wan', 'pppd_options', oad);
			}else
				L.uci.unset('network', 'wan', 'pppd_options');
			L.uci.save();
		};

		m.insertInto('#map');
		}
});
