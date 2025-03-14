L.ui.view.extend({
	board: L.rpc.declare({
		object: 'system',
		method: 'info',
		expect: {
			memory: {},
		},
	}),

	get_ipaddr: L.rpc.declare({
		object: 'network.interface.wan',
		method: 'status',
		expect: { }
	}),

	callSet: L.rpc.declare({
		object: 'uci',
		method: 'set',
		params: ['config', 'section', 'values']
	}),

	callCommit: L.rpc.declare({
		object: 'uci',
		method: 'commit',
		params: ['config']
	}),

	do_cmd: L.rpc.declare({
        object: 'web.advance',
        method: 'cmd',
        params: [ 'cmd' ]
    }),

	lan_ip: L.rpc.declare({
		object: 'network.interface.lan',
		method: 'status',
		expect: { 'ipv4-address': [] },
		filter: function (data) {
			var ip = '0';
			if (data[0]) {
				ip = data[0].address;
			}
			return ip;
		},
	}),

	execute: function() {
		var self = this;

		// 设置时区
		var m = new L.cbi.Map('system', {
			collabsible: true
		});
		
		var s = m.section(L.cbi.TypedSection, 'system', {});
		
		var t = s.option(L.cbi.DummyValue, '__time', {
			caption:     L.tr('Local Time')
		});
		
		t.load = function(sid)
		{
			var id = this.id(sid);
			var uci = this.ucipath(sid);
			self.timezone = m.get(uci.config, uci.section, 'timezone');
		
			if (self.interval)
				return;
		
			L.system.getLocaltime().then(function(info) {
				var date = new Date();
				date.setFullYear(info.year);
				date.setMonth(info.mon);
				date.setDate(info.day);
				date.setHours(info.hour);
				date.setMinutes(info.min);
				date.setSeconds(info.sec);
		
				self.time = Math.floor(date.getTime() / 1000);
		
				self.interval = window.setInterval(function() {
					date.setTime(++(self.time) * 1000);
					$('#' + id).text('%04d/%02d/%02d %02d:%02d:%02d %s'.format(
						date.getFullYear(),
						date.getMonth() + 1,
						date.getDate(),
						date.getHours(),
						date.getMinutes(),
						date.getSeconds(),
						self.timezone
					));
				}, 1000);
			});
		};
		
		var z_time = s.option(L.cbi.ListValue, 'zonename', {
			caption:	L.tr('Timezone'),
			initial:	'UTC'
		});
		
		z_time.load = function(sid) {
			z_time.value('UTC');
			return $.getJSON(L.globals.resource + '/zoneinfo.json').then(function(zones) {
				var znames = [ ];
		
				for (var i = 0; i < zones.length; i++)
					for (var j = 5; j < zones[i].length; j++)
						znames.push(zones[i][j]);
		
				znames.sort();
		
				for (var i = 0; i < znames.length; i++)
					z_time.value(znames[i]);
		
				z_time.zones = zones;
			});
		};
		
		z_time.save = function(sid){
			var uci = this.ucipath(sid);
			var val = this.formvalue(sid);
		
			if (!this.callSuper('save', sid))
				return false;
		
			for (var i = 0; i < z_time.zones.length; i++)
				for (var j = 5; j < z_time.zones[i].length; j++)
					if (z_time.zones[i][j] == val)
					{
						m.set(uci.config, uci.section, 'timezone', z_time.zones[i][0]);
						return true;
					}
		
			m.set(uci.config, uci.section, 'timezone', 'UTC');
			return true;
		};

		m.on('apply', function() {
			L.ui.setting(1, L.tr('Freshing Time...Please wait.'));
			L.system.getLocaltime().then(function(info) {
				var date = new Date();
				date.setFullYear(info.year);
				date.setMonth(info.mon);
				date.setDate(info.day);
				date.setHours(info.hour);
				date.setMinutes(info.min);
				date.setSeconds(info.sec);
				self.time = Math.floor(date.getTime() / 1000);
			}).then(function(){
				L.ui.setting(false)
				showPage("page_wan");
			})
		});

		m.insertInto('#time_set');

		// 模式选择
		var m_wan = new L.cbi.Map('network', {
			emptytop:    '0px'
		});

		var s_wan = m_wan.section(L.cbi.NamedSection, 'wan', {});

		p = s_wan.option(L.cbi.ListValue, 'proto', {
			caption:	L.tr("Mode"),
			initial:	'dhcp'
		})
			.value('dhcp', L.tr("DHCP"))
			.value('static', L.tr("Static"))
			.value('pppoe', L.tr("PPPoE"))
			.value('bridge', L.tr("Bridge"))

		p.save = function(sid){
			var value = this.formvalue(sid);
			var ori_value = L.uci.get('network', 'wan', 'proto');
			L.uci.set('network', 'wan', 'origin_proto',ori_value);
			if(ori_value == 'bridge' && value != 'bridge'){
				L.uci.unset('network', 'wan', 'disabled');
				L.uci.set('network', 'lan', 'proto', 'static');
				L.uci.set('network', 'lan', 'proto', 'static');
				L.uci.load('dhcp').then(function(data) {
					L.uci.unset('dhcp', 'lan', 'ignore');
					L.uci.save();
				});
			}
			if(value == 'pppoe'){
				L.uci.unset('network', 'wan', 'ipaddr');
				L.uci.unset('network', 'wan', 'gateway');
				L.uci.unset('network', 'wan', 'netmask');
				L.uci.set('network', 'wan', 'proto', 'pppoe');
				var ppp_times = 0;
				setTimeout(function(){
					$('#' + id).text(L.tr("Testing"));
					clearInterval(L.wanstatus_interval);
					L.ui.setting(true, L.tr("pppoe connecting..."));
					ppp_interval = setInterval(function() {
						ppp_times = ppp_times + 1;
						if(ppp_times > 10){
							clearInterval(ppp_interval);
							L.ui.setting(false);
							L.ui.setting(true, L.tr("pppoe connect failed"));
							$('body').removeClass("loading_gif");
							setTimeout(function(){
								L.ui.setting(false);
							}, 3000);
						}
						self.get_ipaddr().then(function(data){
							var ppp_status = data["ipv4-address"];
							if(ppp_status)
								ppp_status = data["ipv4-address"][0].address;
							if (ppp_status){
								clearInterval(ppp_interval);
								L.ui.setting(false);
								L.ui.setting(true, L.tr("pppoe connect success"));
								$('body').removeClass("loading_gif");
								setTimeout(function(){
									L.ui.setting(false);
								}, 2000);
							}
						});
					}, 1000);
				}, 2000);
			} else if(value == 'static') {
				L.uci.unset('network', 'wan', 'auto');
				L.uci.set('network', 'wan', 'proto', 'static');
			} else if(value == 'dhcp') {
				L.uci.unset('network', 'wan', 'ipaddr');
				L.uci.unset('network', 'wan', 'gateway');
				L.uci.unset('network', 'wan', 'netmask');
				L.uci.unset('network', 'wan', 'auto');
				L.uci.set('network', 'wan', 'proto', 'dhcp');
			} else if(value == 'bridge') {
				L.uci.unset('network', 'wan', 'ipaddr');
				L.uci.unset('network', 'wan', 'gateway');
				L.uci.unset('network', 'wan', 'netmask');
				L.uci.unset('network', 'wan', 'auto');
				L.uci.set('network', 'wan', 'proto', 'bridge');
				L.uci.set('network', 'wan', 'disabled', '1');
				L.uci.set('network', 'lan', 'ifname', 'eth0.1 eth0.2');
				L.uci.set('network','lan','proto','dhcp');
				L.uci.load('dhcp').then(function() {
					L.uci.set('dhcp', 'lan', 'ignore', '1');
					L.uci.save();
				});
			}
			L.uci.save();
		}

		var lanip='';
		self.lan_ip().then(function (ip) {
			lanip = ip;
		});

		var ipdata = s_wan.option(L.cbi.InputValue, 'ipaddr', {
			caption:      L.tr("IP address"),
			datatype:     function(str) {

			if (!L.parseIPv4(str)){
				return L.tr('Must be a valid IPv4 address');
			}
			var ip = lanip.split('.');
			var parts = str.split('.');
			var netmask = '255.255.255.0'.split('.');
			var res0 = parseInt(ip[0]) & parseInt(netmask[0]);
			var res1 = parseInt(ip[1]) & parseInt(netmask[1]);
			var res2 = parseInt(ip[2]) & parseInt(netmask[2]);
			var res3 = parseInt(ip[3]) & parseInt(netmask[3]);
			var res_gw0 = parseInt(parts[0]) & parseInt(netmask[0]);
			var res_gw1 = parseInt(parts[1]) & parseInt(netmask[1]);
			var res_gw2 = parseInt(parts[2]) & parseInt(netmask[2]);
			var res_gw3 = parseInt(parts[3]) & parseInt(netmask[3]);

			if (res0 === res_gw0 && res1 === res_gw1 && res2 === res_gw2 && res3 === res_gw3) {
				return L.tr('Must not be in the same segment as the IP address of Lan.');
			}
				return true;
			}
		}).depends('proto', function(v) { return (v == 'static');});

		e_wan = s_wan.option(L.cbi.InputValue, 'netmask', {
			caption:      L.tr("Subnet mask"),
			datatype:    'netmask4'
		}).depends('proto', function(v) { return (v == 'static');});

		function check(v) {
			if (v.length == 0) return L.tr('Field must not be empty');
			if (L.parseIPv4(v)) {
				var ip = $('#field_network_wan_wan_ipaddr').val().split('.');
				var gateway = v.split('.');
				var netmask = $('#field_network_wan_wan_netmask').val().split('.');

				var res0 = parseInt(ip[0]) & parseInt(netmask[0]);
				var res1 = parseInt(ip[1]) & parseInt(netmask[1]);
				var res2 = parseInt(ip[2]) & parseInt(netmask[2]);
				var res3 = parseInt(ip[3]) & parseInt(netmask[3]);
				var res_gw0 = parseInt(gateway[0]) & parseInt(netmask[0]);
				var res_gw1 = parseInt(gateway[1]) & parseInt(netmask[1]);
				var res_gw2 = parseInt(gateway[2]) & parseInt(netmask[2]);
				var res_gw3 = parseInt(gateway[3]) & parseInt(netmask[3]);

				if (res0 == res_gw0 && res1 == res_gw1 && res2 == res_gw2 && res3 == res_gw3)
					return true;

				else return L.tr('Must be in the same segment as the IP address.');
			} else return L.tr('Must be a valid IPv4 address');
		}

		var gateway = s_wan.option(L.cbi.InputValue, 'gateway', {
			caption:      L.tr("Default gateway"),
			datatype:    function(v) {
				return check(v);
			}
		}).depends('proto', function(v) { return (v == 'static');});

		e_wan = s_wan.option(L.cbi.InputValue, 'username', {
			caption:      L.tr("Broadband account")
		}).depends('proto', function(v) { return (v == 'pppoe');});

		e_wan = s_wan.option(L.cbi.PasswordValue, 'password', {
			caption:      L.tr("Broadband password")
		}).depends('proto', function(v) { return (v == 'pppoe');});

		e_wan = s_wan.option(L.cbi.CheckboxValue, 'auto', {
			caption:      L.tr("Auto connect")
		}).depends('proto', function(v) { return (v == 'pppoe');});

		var manualdns = s_wan.option(L.cbi.CheckboxValue, '__manualdns', {
			caption:      L.tr("Manually set up DNS server"),
			optional:     true
		}).depends('proto', function(v) { return (v == 'dhcp' || v == 'pppoe');});

		manualdns.ucivalue = function(sid) {
			if (L.uci.get('network', 'wan', 'peerdns') == 0) return true;
			return false;
		};

		manualdns.save = function(sid) {
			var mdns = manualdns.formvalue(sid);
			if (p.formvalue(sid) != 'static') {
				L.uci.set('network', 'wan', 'peerdns', 1 ^ mdns);
				if (!mdns) L.uci.unset('network', 'wan', 'dns');
			}
			L.uci.save();
		};

		var dns1 = s_wan.option(L.cbi.InputValue, '__dns1', {
			caption:      L.tr("Preferred DNS server"),
			datatype:     'ip4addr'
		}).depends('__manualdns', function(v) {
			var proto = p.formvalue('wan');
			if(proto == "bridge"){
				return false;
			}
			return v || proto == "static";
		});

		var dns2 = s_wan.option(L.cbi.InputValue, '__dns2', {
			caption:      L.tr("Standby DNS server(Optional)"),
			optional:     true,
			datatype:     'ip4addr',
		}).depends('__manualdns', function(v) {
			var proto = p.formvalue('wan');
			if(proto == "bridge"){
				return false;
			}
			return v || proto == "static";
		});

		dns1.ucivalue = function(sid) {
			var dns_value = L.uci.get("network", "wan", "dns");
			var split = 0;
			if (dns_value) {
				split = dns_value.indexOf(" ");
				if (split == -1) return dns_value;
				else return dns_value.substring(0, split);
			}
		};

		dns2.ucivalue = function(sid) {
			var dns_value = L.uci.get("network", "wan", "dns");
			var split = 0;
			if (dns_value) {
				split = dns_value.indexOf(" ");
				if (split != -1) return dns_value.substring(split + 1);
			}
		};

		dns1.save = dns2.save =  function(sid) {
			var dnsv1 = dns1.formvalue(sid);
			var dnsv2 = dns2.formvalue(sid);
			if (dnsv2) dnsv1 += ' ' + dnsv2;
			if (p.formvalue(sid) != 'static') {
				if (!manualdns.formvalue(sid)) L.uci.unset('network', 'wan', 'dns');
				else L.uci.set('network', 'wan', 'dns', dnsv1);
			} else {
				L.uci.unset('network', 'wan', 'peerdns');
				L.uci.set('network', 'wan', 'dns', dnsv1);
			}
			L.uci.save();
		};

		servicename = s_wan.option(L.cbi.InputValue, 'service', {
			caption:      L.tr("Service Name"),
			optional:     true
		}).depends('proto', function(v) { return (v == 'pppoe');});

		servicename.save = function(sid) {
			var sname = servicename.formvalue(sid);
			if (p.formvalue('wan') == 'pppoe' && sname) L.uci.set('network', 'wan', 'service', sname);
			else L.uci.unset('network', 'wan', 'service');
			L.uci.save();
		};

		servername = s_wan.option(L.cbi.InputValue, 'ac', {
			caption:      L.tr("Server name"),
			optional:     true
		}).depends('proto', function(v) { return (v == 'pppoe');});

		servername.save = function(sid) {
			var svname = servername.formvalue(sid);
			if (p.formvalue('wan') == "pppoe" && svname) L.uci.set('network', 'wan', 'ac', svname);
			else L.uci.unset('network', 'wan', 'ac');
			L.uci.save();
		};

		uad = s_wan.option(L.cbi.CheckboxValue, '__useoperatoraddress', {
			caption:      L.tr("Use the ip address specified by the operator"),
			optional:     true
		}).depends('proto', function(v) { return (v == 'pppoe');});

		uad.ucivalue  = function(sid) {
			return !!L.uci.get("network", "wan", "pppd_options");
		};

		uad.save = function(sid) {
			var ua = uad.formvalue(sid);
			if (p.formvalue('wan') == 'pppoe') L.uci.set('network', 'wan', 'fixipEnb', 0 ^ ua);
			else L.uci.unset('network', 'wan', 'fixipEnb');
			L.uci.save();
		};

		operatoraddress = s_wan.option(L.cbi.InputValue, 'pppd_options', {
			caption:      L.tr("The ip address specified by the operator"),
			datatype:	  'ipaddr'
		}).depends('__useoperatoraddress', function(v) { return v && p.formvalue('wan') == 'pppoe';});

		operatoraddress.save = function(sid) {
			var oad = operatoraddress.formvalue(sid);
			if (uad.formvalue(sid) && p.formvalue('wan') == 'pppoe') L.uci.set('network', 'wan', 'pppd_options', oad);
			else L.uci.unset('network', 'wan', 'pppd_options');
			L.uci.save();
			showPage("page_wireless");
		};

		m_wan.insertInto('#wan_set').then(function () {
			$('#wan_set').css('width', '800px');
			$('#node_network_wan_wan___default__').css({
				width: '700px',
				'margin-left': '30px',
			});
			$('.form-group').children('div:last-child').css({
				width: 'auto',
				'margin-top': '7px',
				'text-align': 'left',
				color: 'gray',
				'font-size': '12px',
				'margin-left': '210px',
			});
			$('#field_network_wan_wan_ipaddr').keyup(function () {
				gateway.setError($('#field_network_wan_wan_gateway'), check($('#field_network_wan_wan_gateway').val()));
			});
			$('#field_network_wan_wan_netmask').keyup(function () {
				gateway.setError($('#field_network_wan_wan_gateway'), check($('#field_network_wan_wan_gateway').val()));
			});
		});

		m_wan.on('apply',function(){
			L.ui.setting(true);
			let get_val = $("#field_network_wan_wan_proto").val();
			let origin_proto = L.uci.get('network', 'wan', 'origin_proto');
			if(get_val == "bridge" && origin_proto !="bridge"){
				L.ui.setting(false);
				var form = $('<p />').text(L.tr(`The LAN IP will change when switching from other modes to bridge mode.Please log in using the domain name (http://juovi.wifi/) in two minutes`));
				L.ui.dialog(L.tr('Tips'), form, {
						style: 'onlyConfirm',
						confirm: function() {
								L.ui.dialog(false);
								L.ui.setting(true);
						},
				});
				self.do_cmd("/usr/sbin/ip_monitor.sh &");
			}
			else if (get_val != "bridge" && origin_proto =="bridge"){
				L.ui.setting(false);
				var form = $('<p />').text(L.tr(`The LAN IP will change when switching from bridge mode to other modes.Please log in using the domain name (http://juovi.wifi/) in two minutes`));
				L.ui.dialog(L.tr('Tips'), form, {
						style: 'onlyConfirm',
						confirm: function() {
							L.ui.dialog(false);
							L.ui.setting(true);
						},
				});
				self.do_cmd("/usr/sbin/ip_monitor.sh &");
			}
			self.do_cmd("/etc/init.d/dnsmasq restart; /etc/init.d/network restart").then(function(v){
				setTimeout(() => {
					L.ui.setting(false);
				}, 1000*40);
			});
		});

		// 无线设置
		var m_wireless = new L.cbi.Map('wireless', {
			emptytop:    '0px'
		});

		var s_wireless = m_wireless.section(L.cbi.NamedSection, 'default_radio0', {
			caption:      L.tr('2.4G')
		});

		var e_wireless = s_wireless.option(L.cbi.CheckboxValue, 'disabled_hostapd', {
			caption:      L.tr('Disabled Wifi'),
			optional:     true
		});

		e_wireless = s_wireless.option(L.cbi.InputValue, 'ssid', {
			caption:      L.tr('Wireless name'),
			datatype:     'ssid'
		});

		e_wireless = s_wireless.option(L.cbi.ListValue, 'encryption', {
			caption:	L.tr('Encryption method'),
			initial:	'none'
		})
		.value('none', L.tr('No encryption'))
		.value('psk+ccmp', L.tr('WPA Personal (PSK)'))
		.value('psk2+ccmp', L.tr('WPA2 Personal (PSK)'))
		.value('psk-mixed', L.tr('WPA/WPA2 Personal (PSK) mixed'))
		.value('sae', L.tr('WPA3 Personal (SAE)'))
		.value('sae-mixed', L.tr('WPA2/WPA3 mixed'));

		e_wireless.save = function (sid) {
			let radio_2_4 = $('#field_wireless_default_radio0_default_radio0_disabled_hostapd').is(':checked');
				if(radio_2_4){
					L.ui.do_cmd(`hostapd_cli -i wlan0 disable`);
				}else{
					L.ui.do_cmd(`hostapd_cli -i wlan0 enable`);
				}
			var encrypt = this.formvalue(sid);
			L.uci.set('wireless', 'default_radio0', 'encryption', encrypt);
			if (encrypt == 'sae')
				L.uci.set('wireless', 'default_radio0', 'ieee80211w', 2);
			else if (encrypt == 'sae-mixed')
				L.uci.set('wireless', 'default_radio0', 'ieee80211w', 1);
			else L.uci.unset('wireless', 'default_radio0', 'ieee80211w');
			L.uci.save();
		}

		e_wireless = s_wireless.option(L.cbi.PasswordValue, 'key', {
			caption:    L.tr('Password'),
			datatype:   'wpakey'
		}).depends('encryption', function(v) {
			return (v != 'none');
		});

		var s2_wireless = m_wireless.section(L.cbi.NamedSection, 'default_radio1', {
			caption:      L.tr('5G')
		});

		var e2_wireless = s2_wireless.option(L.cbi.CheckboxValue, 'disabled_hostapd', {
			caption:      L.tr('Disabled Wifi'),
			optional:     true
		});

		e2_wireless = s2_wireless.option(L.cbi.InputValue, 'ssid', {
			caption:      L.tr('Wireless name'),
			datatype:     'ssid'
		});

		e2_wireless = s2_wireless.option(L.cbi.ListValue, 'encryption', {
			caption:	L.tr('Encryption method'),
			initial:	'none'
		})
		.value('none', L.tr('No encryption'))
		.value('psk+ccmp', L.tr('WPA Personal (PSK)'))
		.value('psk2+ccmp', L.tr('WPA2 Personal (PSK)'))
		.value('psk-mixed', L.tr('WPA/WPA2 Personal (PSK) mixed'))
		.value('sae', L.tr('WPA3 Personal (SAE)'))
		.value('sae-mixed', L.tr('WPA2/WPA3 mixed'));

		e2_wireless.save = function (sid) {
			let radio_5 = $('#field_wireless_default_radio1_default_radio1_disabled_hostapd').is(':checked');
			if(radio_5){
				L.ui.do_cmd(`hostapd_cli -i wlan1 disable`);
			}else{
				L.ui.do_cmd(`hostapd_cli -i wlan1 enable`);
			}
			var encrypt = this.formvalue(sid);
			L.uci.set('wireless', 'default_radio1', 'encryption', encrypt);
			if (encrypt == 'sae')
				L.uci.set('wireless', 'default_radio1', 'ieee80211w', 2);
			else if (encrypt == 'sae-mixed')
				L.uci.set('wireless', 'default_radio1', 'ieee80211w', 1);
			else L.uci.unset('wireless', 'default_radio1', 'ieee80211w');
			L.uci.save();
			showPage("page_password");
		}

		e2_wireless = s2_wireless.option(L.cbi.PasswordValue, 'key', {
			caption:    L.tr('Password'),
			datatype:   'wpakey'
		}).depends('encryption', function(v) {
			return (v != 'none');
		});

		self.board().then(function (data) {
			m_wireless.insertInto('#wireless_set').then(function () {
				$('#wireless_set').css('width', '800px');
				$('#node_wireless_default_radio0_default_radio0___default__').css({
					width: '700px',
					'margin-left': '30px',
				});
				$('#node_wireless_default_radio1_default_radio1___default__').css({
					width: '700px',
					'margin-left': '30px',
				});
				if (Number(data.total) <= 64*1024*1024) {
					$('#field_wireless_default_radio0_default_radio0_encryption').find('[value="sae"]').css('display', 'none');
					$('#field_wireless_default_radio0_default_radio0_encryption').find('[value="sae-mixed"]').css('display', 'none');
					$('#field_wireless_default_radio1_default_radio1_encryption').find('[value="sae"]').css('display', 'none');
					$('#field_wireless_default_radio1_default_radio1_encryption').find('[value="sae-mixed"]').css('display', 'none');
				}
				else {
					$('#field_wireless_default_radio0_default_radio0_encryption').find('[value="sae"]').css('display', '');
					$('#field_wireless_default_radio0_default_radio0_encryption').find('[value="sae-mixed"]').css('display', '');
					$('#field_wireless_default_radio1_default_radio1_encryption').find('[value="sae"]').css('display', '');
					$('#field_wireless_default_radio1_default_radio1_encryption').find('[value="sae-mixed"]').css('display', '');
				}
			});
		});

		var m_password = new L.cbi.Map('rpcd', {});

		var s_password = m_password.section(L.cbi.DummySection, '__login', {

		});

		var password = s_password.option(L.cbi.PasswordValue, 'password', {
			caption:     L.tr('Password'),
			optional:    true,
			datatype:     function(str, sid) {
				var password = '' + $('#field_rpcd___login___login_password').val()
				var val = '' + str;

				for(var i = 0; i < val.length; i ++)
				{
					var c = val.substr(i, 1);
					var ts = escape(c);
					if(ts.substring(0, 2) == "%u") {
						return L.tr('Must void Chinese characters');
					}
				}

				if (val.length >= 5 && val.length <= 12) {
					if(password == val) {
						return true;
					} else {
						return L.tr("Two password inputs must be consistent")
					}
				} else {
					return L.tr('Must be between 5 and 12 characters');
				}
			}
		});

		password.ucivalue = function(sid) {
			return '';
		}
		password.toggle = function(sid) {
			var id = '#' + this.id(sid);
			this.callSuper('toggle', sid);
		};

		password.save = function(sid) {
			var real_sid;
			var rpcd_users = L.uci.sections('rpcd');
			for(var i = 0; i < rpcd_users.length; i++) {
				if(L.uci.get('rpcd', rpcd_users[i]['.name'], 'username') == 'root') {
					real_sid = rpcd_users[i]['.name'];
					break;
				}
			}

			var pw = password.formvalue(sid);

			if (!pw || !real_sid)
				return;
			return L.ui.cryptPassword(pw).then(function(crypt) {
				L.uci.set('rpcd', real_sid, 'password', crypt);
			});
		};

		m_password.on('save', function() {
			L.uci.changes().then(function(changes) {
				self.relogin = true;
			});
		});

		m_password.insertInto('#password_set').then(function(){
			L.uci.callLoad('luci2').then(function(data) {
				var lang = data["main"].lang;
				$("#page_password").find(".common_button_long").text(L.tr("Finish"));
				$("#page_password").find(".common_button_long").css("right","46px");
				if(lang == "es") {
					$("#page_password").find(".backclick").css("right","160px");
				} else if(lang == "it") {
					$("#page_password").find(".backclick").css("right","155px");
				}
			})
		});

		function showPage(pageId) {
			const pages = document.querySelectorAll('.guidepage');
			pages.forEach(page => {
				page.classList.remove('page_active');
			});

			const activePage = document.getElementById(pageId);
			if (activePage) {
				activePage.classList.add('page_active');
			}

			localStorage.setItem('currentStep', pageId);
		}

		const currentStep = localStorage.getItem('currentStep') || 'page_time';
		showPage(currentStep);

		$(".backclick").off('click').on("click",function(){
			let elemb = $(this).attr("data-id");
			showPage(elemb);
		});

		$(".nextclick").off('click').on("click",function(){
			let elemn = $(this).attr("data-id");
			showPage(elemn);
		});

		$(".guideconfirm").off('click').on("click",function(){
			if (self.relogin) {
				L.ui.setting(true);
				self.callSet('basic_setting', 'web_guide', { 'inited': '1' });
				self.callCommit('basic_setting');
				L.ui.login().then(function(){
					history.replaceState(null, '', window.location.origin + window.location.pathname + window.location.search);
					location.reload(true)
				})
				L.ui.setting(false);
				return;
			}else{
				let getpassword = $("#field_rpcd___login___login_password").val();
				let form;
				if(getpassword){
					form = $('<p />').text(L.tr('Password not saved'));
				}else{
					form = $('<p />').text(L.tr('Password not set, please set and save'));
				}
				L.ui.dialog(L.tr('Tips'), form, {
					style: 'close'
				});
			}
		});
	}
});