L.ui.view.extend({

	execute: function() {
		var self = this;

		//differ in ac28 and evb
		L.system.getBoardInfo().then(function(info){
			model = info.model;

			var m = new L.cbi.Map('network', {
				caption: L.tr('IPTV'),
			});

			var s = m.section(L.cbi.DummySection, '__lan', {
			});

			var enable = s.option(L.cbi.CheckboxValue, 'iptv', {
				caption: L.tr('Enable IPTV'),
				initial: false
			});

		//enable
			enable.ucivalue = function(sid){
				var a = L.uci.get("network", "lan", "iptv");
				if(a == "1")
					return true;
				else
					return false;
			}

			enable.save = function(sid){
				var a = this.formvalue(sid);
				if(a)
					L.uci.set("network", "lan", "iptv", "1");
				else {
					var sw_sid = L.uci.sections("network","switch_vlan");
					if(model == "sf19a28-evb"){
						L.uci.set("network", sw_sid[0]['.name'], "ports", "1 2 3 4 16t");
						L.uci.set("network", sw_sid[1]['.name'], "ports", "0 16t");
					} else {
						L.uci.set("network", sw_sid[0]['.name'], "ports", "1 2 3 5t");
						L.uci.set("network", sw_sid[1]['.name'], "ports", "0 5t");
					}
					L.uci.set("network", "lan", "iptv", undefined);
					L.uci.remove("network", "lan2");
					L.uci.remove("network", "iptv");
				}
				L.uci.save();
			}

		//mode: router or bridge
			var mode = s.option(L.cbi.ListValue, '__mode', {
				caption: L.tr('Mode'),
				initial: "route"
			})
			.value("route",L.tr("Router mode"))
			.value("bridge",L.tr("Bridge mode"))
			.depends('iptv', true);

			mode.ucivalue = function(sid){
				var if_iptv = L.uci.get("network","lan2");
				if(if_iptv)
					return "bridge";
				else
					return "route";
			}

		//port: which port to be iptv
			var port = s.option(L.cbi.ListValue, '__port', {
				caption: L.tr('LAN interface'),
				description: L.tr("WAN is fixed at port 1"),
				initial: '1'
			})
			.depends('__mode', function(v){
				aa = enable.formvalue('__lan');
				if(enable.formvalue('__lan') == false){
					return false;
				}
				if(v == "bridge"){
					return true;
				}
				return false;
			});

			if(model == "sf19a28-ac28"){
				port.value("1",L.tr("port 2"))
					.value("2",L.tr("port 3"))
					.value("3",L.tr("port 4"))
			} else {
				port.value("1",L.tr("port 2"))
					.value("2",L.tr("port 3"))
					.value("3",L.tr("port 4"))
					.value("4",L.tr("port 5"))
			}

			port.ucivalue = function(sid){
				var v_section = L.uci.sections('network', 'switch_vlan');
				if(v_section.length > 2)
					return v_section[2].ports.split(' ')[0];
			}

		//vlan id
			var vlan = s.option(L.cbi.InputValue, '__vlanid', {
				caption: L.tr('VLAN id'),
				datatype: 'range(3,255)'
			})
			.depends('__mode', function(v){
				aa = enable.formvalue('__lan');
				if(enable.formvalue('__lan') == false){
					return false;
				}
				if(v == "bridge"){
					return true;
				}
				return false;
			});

			vlan.ucivalue = function(sid){
				var ifname = L.uci.get('network', 'lan2', 'ifname');
				if(ifname)
					return ifname.split('.')[1];
				else
					return undefined;
			}

		//802.1p, temporarily set to lan2
			var p = s.option(L.cbi.InputValue, '__8021p', {
				caption: L.tr('802.1p'),
				datatype: 'range(0,7)'
			})
			.depends('__mode', function(v){
				aa = enable.formvalue('__lan');
				if(enable.formvalue('__lan') == false){
					return false;
				}
				if(v == "bridge"){
					return true;
				}
				return false;
			});

			p.ucivalue = function(sid){
				return L.uci.get("network", "lan2", "8021p");
			};

		//set all config
			mode.save = function(sid){
				var iptv_mode = this.formvalue(sid);
				var iptv_en = enable.formvalue(sid);
				var set_port = Number(port.formvalue('__lan'));
				var set_vlan = vlan.formvalue('__lan');
				var set_8021p = p.formvalue('__lan');
				var sw_sid = L.uci.sections("network","switch_vlan");
				var port_end_str;	//16t or 5t
				var port_num;	//5 for evb; 4 for ac28
				if(iptv_en == true) {
					if(iptv_mode == "bridge") {
						L.uci.add("network","interface", "lan2");
						L.uci.set("network", "lan2", "proto","dhcp");
						L.uci.set("network", "lan2", "ifname","eth0." + set_vlan);
						L.uci.set("network", "lan2", "8021p", set_8021p);
						var new_sw_sid;
						var port_str = "";
						if(model == "sf19a28-ac28"){
							port_end_str = "5t";
							port_num = 4;
						} else {
							port_end_str = "16t";
							port_num = 5;
						}
						for(let i = 1; i < port_num; i++){
							if(i != set_port)
								port_str = port_str + i +  " ";
						}
						port_str = port_str + port_end_str;
						L.uci.set("network", sw_sid[0]['.name'], "ports", port_str);
						L.uci.add("network", "switch_vlan", "iptv");
						L.uci.set("network", "iptv", "device", "switch0");
						L.uci.set("network", "iptv", "vlan", set_vlan);
						L.uci.set("network", "iptv", "ports", set_port + " " + port_end_str);
					} else {
						if(model == "sf19a28-ac28"){
							L.uci.set("network", sw_sid[0]['.name'], "ports", "1 2 3 5t");
							L.uci.set("network", sw_sid[1]['.name'], "ports", "0 5t");
						}else{
							L.uci.set("network", sw_sid[0]['.name'], "ports", "1 2 3 4 16t");
							L.uci.set("network", sw_sid[1]['.name'], "ports", "0 16t");
						}
						L.uci.set("network", "lan2", undefined, undefined);
						L.uci.remove("network", "lan2");
						L.uci.remove("network", "iptv");
					}
					L.uci.save();
				}
			}

			m.insertInto('#map');
		});
	}
});
