L.ui.view.extend({
	refresh: 5000,

	getSystemLog: L.rpc.declare({
		object: 'luci2.system',
		method: 'syslog',
		expect: { log: '' }
	}),

	execute: function() {
		return this.getSystemLog().then(function(log) {
			var ta = document.getElementById('syslog');
			var lines = log.replace(/\n+$/, '').split(/\n/);

			ta.rows = lines.length;
			ta.value = lines.reverse().join("\n");
			$(ta).prop('wrap', 'on');
			$(ta).css({
				border: 'none',
				background: 'white',
				'box-shadow': 'none',
				'width': '730px',
				'height': '1200px',
				'resize': 'none',
				cursor: 'auto',
			});

			// 添加导出按钮的点击事件
			var exportBtn = document.getElementById('exportBtn');
			exportBtn.onclick = function() {
					var logContent = ta.value;
					var blob = new Blob([logContent], { type: 'text/plain' });
					var link = document.createElement('a');
					link.href = window.URL.createObjectURL(blob);
					link.download = 'system_log.txt';
					link.click();
			};
		});
	}
});
