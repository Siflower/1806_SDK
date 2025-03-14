Class.extend({
	getSystemInfo: L.rpc.declare({
		object: 'system',
		method: 'info',
		expect: { '': { } }
	}),

	getBoardInfo: L.rpc.declare({
		object: 'system',
		method: 'board',
		expect: { '': { } }
	}),

	getFactoryInfo: L.rpc.declare({
		object: 'web.system',
		method: 'get_factory_info',
		params: ['name'],
		expect: { '': { } }
	}),

	getLanIp: L.rpc.declare({
		object: 'network.interface.lan',
		method: 'status',
		expect: { '': {} }
	}),

	getWanIp: L.rpc.declare({
		object: 'network.interface.wan',
		method: 'status',
		expect: { '': {} }
	}),

	get_basic_setting: L.rpc.declare({
		object: 'uci',
		method: 'get',
		params: ['config', 'section', 'option'],
		expect: { value: ' ' }
	}),

	getDiskInfo: L.rpc.declare({
		object: 'luci2.system',
		method: 'diskfree',
		expect: { '': { } }
	}),

	getLocaltime: L.rpc.declare({
		object: 'luci2.system',
		method: 'localtime'
	}),

	getLocaltimestamp: L.rpc.declare({
		object: 'luci2.system',
		method: 'localtime',
		filter: function(info) {
			var date = new Date();
			date.setFullYear(info.year);
			date.setMonth(info.mon);
			date.setDate(info.day);
			date.setHours(info.hour);
			date.setMinutes(info.min);
			date.setSeconds(info.sec);

			time = Math.floor(date.getTime() / 1000);
			return time;
		}
	}),

	getInfo: function(cb) {
		L.rpc.batch();

		this.getSystemInfo();
		this.getBoardInfo();
		this.getDiskInfo();

		this.getFactoryInfo('macaddr');
		this.getFactoryInfo('model_ver');
		this.getFactoryInfo('sn');
		this.get_basic_setting('basic_setting', 'vendor', 'hosts')
		this.getLanIp();
		this.getWanIp();

		return L.rpc.flush().then(function(info) {
			var rv = {};
			$.extend(rv, info[0]);
			$.extend(rv, info[1]);
			$.extend(rv, info[2]);
			rv.macaddr = info[3].result;
			rv.model_ver = info[4].result;
			rv.sn = info[5].result;
			rv.login_domain = info[6];

			var lan_ip_info = info[7]["ipv4-address"];
			if (lan_ip_info && lan_ip_info.length > 0) {
				rv.lan_ip = lan_ip_info[0]["address"];
			}

			var wan_ip_info = info[8]["ipv4-address"];
			if (wan_ip_info && wan_ip_info.length > 0) {
				rv.wan_ip = wan_ip_info[0]["address"];
			}

			return rv;
		});
	},


	initList: L.rpc.declare({
		object: 'luci2.system',
		method: 'init_list',
		expect: { initscripts: [ ] },
		filter: function(data) {
			data.sort(function(a, b) { return (a.start || 0) - (b.start || 0) });
			return data;
		}
	}),

	initEnabled: function(init, cb)
	{
		return this.initList().then(function(list) {
			for (var i = 0; i < list.length; i++)
				if (list[i].name == init)
					return !!list[i].enabled;

			return false;
		});
	},

	initRun: L.rpc.declare({
		object: 'luci2.system',
		method: 'init_action',
		params: [ 'name', 'action' ],
		filter: function(data) {
			return (data == 0);
		}
	}),

	initStart:   function(init, cb) { return L.system.initRun(init, 'start',   cb) },
	initStop:    function(init, cb) { return L.system.initRun(init, 'stop',    cb) },
	initRestart: function(init, cb) { return L.system.initRun(init, 'restart', cb) },
	initReload:  function(init, cb) { return L.system.initRun(init, 'reload',  cb) },
	initEnable:  function(init, cb) { return L.system.initRun(init, 'enable',  cb) },
	initDisable: function(init, cb) { return L.system.initRun(init, 'disable', cb) },


	performReboot: L.rpc.declare({
		object: 'luci2.system',
		method: 'reboot'
	})
});
