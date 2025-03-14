L.ui.view.extend({
	cmd: L.rpc.declare({
		object: 'web.advance',
		method: 'cmd',
		params: ['cmd'],
	}),
	execute: function () {
		var self = this;
		self.cmd('chmod +x /www/luci2/scripts/info.sh');
		function padStart(self, aim, rep) {
			for (var i = self.length; i < aim; i++) self = rep + self;
			return self;
		}
		function dateFormat(fmt, date) {
			let ret;
			const opt = {
				'Y+': date.getFullYear().toString(),
				'm+': (date.getMonth() + 1).toString(),
				'd+': date.getDate().toString(),
				'H+': date.getHours().toString(),
				'M+': date.getMinutes().toString(),
			};
			for (let k in opt) {
				ret = new RegExp('(' + k + ')').exec(fmt);
				if (ret) fmt = fmt.replace(ret[1], padStart(opt[k], ret[1].length, '0'));
			}
			return fmt;
		}
		$('#get').click(function () {
			L.ui.setting(1, L.tr('Collecting debugging information...'));
			self.cmd('sh /www/luci2/scripts/info.sh').then(function () {
				self.cmd('chmod 777 /www/info.tar.gz').then(function () {
					var time = dateFormat('YYYY-mm-dd-HHMM-', new Date());
					const link = document.createElement('a');
					link.href = 'info.tar.gz';
					link.download = time + 'info.tar.gz';
					L.ui.setting(false);
					link.click();
				});
			});
		});
	}
})
