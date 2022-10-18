L.ui.view.extend({

	getProcessList: L.rpc.declare({
		object: 'iwinfo',
		method: 'scan',
		params: [ 'device' ],
		expect: { results: [ ] },
		filter: function(data) {
			data.sort(function(a, b) { return b.signal - a.signal });
			return data;
		}
	}),

	execute: function() {
		var self = this;
		var allow_signals = this.options.acls.status;
		var a = self.getProcessList('wlan0').then(function(list) {
			var procTable = new L.ui.table({
				columns: [ {
					caption: L.tr('SSID'),
					key:     'ssid'
				}, {
					caption: L.tr('BSSID'),
					key:     'bssid'
				}, {
					caption: L.tr('SIGNAL'),
					key:     'signal'
				}, {
					caption: L.tr('CHANNEL'),
					key:     'channel'
				}, {
					caption: L.tr('ENCRYPTION'),
					key:     'encryption',
					format:  function(v) {
						var encrypt = "Open";
						if(v.enabled == false) {
							encrypt = "Open";
						} else {
							if(v.wpa[0] == 1 && v.wpa[1] == 2){
								encrypt = "WPA/WPA2(PSK) MIXED";
							} else if(v.wpa[0] == 2){
								encrypt = "WPA2(PSK)";
							} else if(v.wpa[0] == 1){
								encrypt = "WPA(PSK)";
							}
						}
						return encrypt;
					}
				} ]
			});

			procTable.rows(list);
			procTable.insertInto('#scan_table');
		});
		var b = self.getProcessList('wlan1').then(function(list) {
			var procTable = new L.ui.table({
				columns: [ {
					caption: L.tr('SSID'),
					key:     'ssid'
				}, {
					caption: L.tr('BSSID'),
					key:     'bssid'
				}, {
					caption: L.tr('SIGNAL'),
					key:     'signal'
				}, {
					caption: L.tr('CHANNEL'),
					key:     'channel'
				}, {
					caption: L.tr('ENCRYPTION'),
					key:     'encryption',
					format:  function(v) {
						var encrypt = "Open";
						if(v.enabled == false) {
							encrypt = "Open";
						} else {
							if(v.wpa[0] == 1 && v.wpa[1] == 2){
								encrypt = "WPA/WPA2(PSK) MIXED";
							} else if(v.wpa[0] == 2){
								encrypt = "WPA2(PSK)";
							} else if(v.wpa[0] == 1){
								encrypt = "WPA(PSK)";
							}
						}
						return encrypt;
					}
				} ]
			});

			procTable.rows(list);
			procTable.insertInto('#scan5g_table');
		});
		return b;
	}
});
