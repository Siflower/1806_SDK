L.ui.view.extend({
	title: L.tr('WDS'),

	wds_Enable: L.rpc.declare({
		object: 'luci2.network',
		method: 'wds_enable',
		params: [ 'wdsip' ],
	}),

	wds_Disable: L.rpc.declare({
		object: 'luci2.network',
		method: 'wds_disable',
	}),

	wds_ip: L.rpc.declare({
		object: 'network.interface.wwan',
		method: 'status',
		expect: { 'ipv4-address': [ ] },
		filter: function(data) {
			var ip = "0";
			if(data[0]){
				ip = data[0].address;
			}
			return ip;
		}
	}),
	execute: function() {
		var self = this;
		$('#wds_en').click(function() {
			self.wds_Enable($("#wds_ip_input").val()).then(function() {});
			alert("WDS enable success. Please wait");
		});
		$('#wds_dis').click(function() {
			self.wds_Disable().then(function() {});
			alert("WDS enable success. Please wait");
		});
		self.wds_ip().then(function(ip) {
			if(ip != "0"){
				$('#wds_ip_input').val(ip);
			}
			return 0;
		});
		return 0;
	}
});
