L.ui.view.extend({
	refresh: 5000,

	getKernelLog: L.rpc.declare({
		object: 'luci2.system',
		method: 'dmesg',
		expect: { log: '' }
	}),

	execute: function() {
		return this.getKernelLog().then(function(log) {
			var ta = document.getElementById('syslog');
			var lines = log.replace(/\n+$/, '').split(/\n/);

			ta.rows = lines.length;
			ta.value = lines.reverse().join("\n");
			$(ta).prop('wrap', 'on');
			$(ta).css({
				border: 'none',
				background: 'white',
				'box-shadow': 'none',
				width: '730px',
				height: '1200px',
				resize: 'none',
				cursor: 'auto',
			});

		});
	}
});
