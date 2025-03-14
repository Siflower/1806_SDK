L.ui.view.extend({
	cmd: L.rpc.declare({
	object: 'web.advance',
	method: 'cmd',
	params: ['cmd'],
	}),

	wan_ipaddr: L.rpc.declare({
		object: 'network.interface.wan',
		method: 'status',
		expect: {},
	}),

	execute: function() {
		var self = this;
		var m = new L.cbi.Map('network', {
			caption:     L.tr('lan/dhcp Networksetting'),
			collabsible: true
		});

		var s = m.section(L.cbi.NamedSection, 'lan', {
//			caption:      L.tr('lan/dhcp network')
		});

		var wan_ip='';
		self.wan_ipaddr().then(function (info) {
			wan_ip=info['ipv4-address'][0]['address'];
		});

		var e = s.option(L.cbi.InputValue, 'ipaddr', {
			caption:      L.tr('lan/dhcp ipaddress'),
			datatype:     function(str) {

				if (!L.parseIPv4(str)){
					return L.tr('Must be a valid IPv4 address');
				}
				var ip = wan_ip.split('.');
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
					return L.tr('Must not be in the same segment as the IP address of Wan.');
				}
					return true;
			}
		});

		var n = new L.cbi.Map('dhcp', {
			caption:     L.tr('dhcpsetting'),
			collabsible: true,
			topline:     true
		});

		var s2 = n.section(L.cbi.NamedSection, 'lan', {
//			caption:      L.tr('dhcp')
		});

		var e2 = s2.option(L.cbi.CheckboxValue, 'ignore', {
			caption:      L.tr('disable dhcp'),
			optional:     true
		});

		var a2 = s2.option(L.cbi.InputValue, 'start', {
			caption:      L.tr('dhcp starting address'),
			datatype:     'range(1,255)'
		});

		var dhcpend = s2.option(L.cbi.InputValue, '__dhcpend', {
			caption:      L.tr('dhcp end address'),
			datatype:     function(v, sid ) {

				var start_now = a2.formvalue('lan');
				var start_now1 = Number(start_now);
				if (Number(v) > 255 || Number(v) < 1 || v.match(/^-?[0-9]+$/) == null)
				{
					return L.tr('Please enter a value in the range of (1,255)');
				}
				else if (start_now1 > v && v <= 255 && v >= 0)
				{
					return L.tr('Please enter a value greater than the dhcp start address');
				}
				else
				{
					return true;
				}
			}
		});

		dhcpend.ucivalue = function(sid) {
			var limit_now = this.ownerMap.get('dhcp', sid, 'limit');
			var start_now = this.ownerMap.get('dhcp', sid, 'start');
			var dhcpend_now = Number(limit_now)+Number(start_now)-1;
			return (dhcpend_now);
		};

		dhcpend.save = function(sid) {
			var dhcpend_now = this.ownerSection.fields.__dhcpend.formvalue(sid);
			var start_value = a2.formvalue(sid);
			var limit_value = Number(dhcpend_now)-Number(start_value)+1;
			var map = this.ownerMap;
			map.set('dhcp', sid, 'limit', limit_value);
			L.uci.save();
		}

		e2 = s2.option(L.cbi.InputValue, '_leasetime', {
			caption: L.tr('dhcp lease time'),
			datatype: 'min(1)',
			description: L.tr("h")
		});

		e2.ucivalue = function (sid) {
			var value = L.uci.get('dhcp', 'lan', 'leasetime');
			return value.split('h')[0];
		}

		e2.save = function(sid) {
			var value = this.formvalue(sid) + 'h';
			L.uci.set('dhcp', 'lan', 'leasetime', value);
			L.uci.save();
		}

		m.on('apply',function(){
			var getIp = $("#field_network_lan_lan_ipaddr").val();
			var getLanProto=L.uci.get('network', 'lan', 'proto');
			if (getLanProto == "dhcp") {
				L.uci.set('network', 'lan', 'ipaddr',getIp);
				L.uci.save();
			}
			else{
				self.cmd("/etc/init.d/network restart");
			}

			L.ui.setting(true, L.tr("Saving configuration..."));
			setTimeout(() => {
				location.replace(location.protocol+`//` + getIp);
			}, 6000);
		});
		n.insertInto('#amap');
		m.insertInto('#map');
	}
});
