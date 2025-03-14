L.ui.view.extend({

    execute: function() {

		function make_memory_info(v, total){
			return ('%d kB / %d kB (%d%%)').format(v / 1024, total / 1024, Math.round(v / total * 100));
		}

		L.system.getInfo().then(function(info) {
			$('#wan_ip_address').text(info.wan_ip);
			$('#lan_ip_address').text(info.lan_ip);
			$('#login_domain').text(info.login_domain);
			$('#model_ver').text(info.model_ver);
			$('#sn_code').text(info.sn);
			$('#mac_address').text(info.macaddr);
			var total = info.memory.total;
			$('#memory_1').attr("style", "background-color:#FA6F43; width: " + Math.round((info.memory.free + info.memory.buffered) / total * 100)+"%");
			$('#memory_1_text').text(make_memory_info(info.memory.free + info.memory.buffered, total));
			$('#memory_2').attr("style", "background-color:#FA6F43; width: " + Math.round(info.memory.free / total * 100)+"%");
			$('#memory_2_text').text(make_memory_info(info.memory.free, total));
			$('#memory_3').attr("style", "background-color:#FA6F43; width: " + Math.round(info.memory.shared / total * 100)+"%");
			$('#memory_3_text').text(make_memory_info(info.memory.shared, total));
			$('#memory_4').attr("style", "background-color:#FA6F43; width: " + Math.round(info.memory.buffered / total * 100)+"%");
			$('#memory_4_text').text(make_memory_info(info.memory.buffered, total));
		});
    }
});
