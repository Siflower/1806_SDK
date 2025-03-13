L.ui.view.extend({
	get_routert: L.rpc.declare({
		object: 'system',
		method: 'info',
		expect: {},
		filter: function (data) {
			if (data) return data.localtime;
		}
	}),
	cmd: L.rpc.declare({
		object: 'web.advance',
		method: 'cmd',
		params: ['cmd'],
	}),
	read: L.rpc.declare({
		object: 'file',
		method: 'read',
		params: ['path'],
		expect: {
			data: '',
		},
	}),
	write: L.rpc.declare({
		object: 'file',
		method: 'write',
		params: ['path', 'data', 'append'],
	}),
	execute: function () {
		var self = this, localt, routert, fit = false;
		/*display css*/
		$('.tb-no-border').css({
			'border-spacing': '0px',
			'border-collapse': 'collapse',
			border: 'medium none',
			'margin-left': '70px',
			'margin-bottom': '5px',
		});
		$('table').find('td').css({
			'vertical-align': 'middle',
			'text-align': 'center',
		});
		$('.hourList').css({
			color: 'rgb(181,181,181)',
			'padding-left': '0px',
			'font-size': '12px',
			'list-style': 'outside none none',
			'text-align': 'left',
		});
		$('.hourList').find('li').css({
			display: 'inline-block',
			'text-align': 'center',
			'list-style-type': 'none',
			width: '21px',
			'user-select': 'none'
		});
		$('.hourList').find('span').css({
			width: '18px',
			height: '18px',
			color: 'white',
			display: 'inline-block',
			cursor: 'pointer',
			'border-radius': '100%',
			background: '#FA6F43',
			'line-height': '18px',
		});
		$('.weekList').css({
			float: 'left',
			color: 'rgb(80,80,80)',
			'font-size': '12px',
			'list-style': 'outside none none',
			'padding-left': '0px',
		});
		$('.weekList').find('li').css({
			'font-weight': 'bold',
			cursor: 'pointer',
			height: '21px',
			'line-height': '21px',
			'user-select': 'none'
		});
		$('td').css({
			border: 'medium none',
			'white-space': 'nowrap',
		});
		$('.tableWeek').css({
			float: 'left',
			'margin-top': '5px',
		});
		$('.weekTd').css({
			border: '1px solid rgb(227,230,233)',
			'font-size': '0px',
			padding: '1px',
			height: '15px',
		});
		$('.tableICell').css({
			display: 'inline-block',
			height: '15px',
			width: '15px',
			'background-color': 'white',
			cursor: 'pointer',
		});
		$('.TimeTip').css({
			'margin-right': '85px',
			'text-align': 'right',
			'font-size': '12px',
		});
		$('.TimeTip').find('i').css({
			height: '15px',
			width: '15px',
			'vertical-align': 'middle',
			background: '#FA6F43',
			display: 'inline-block',
			'margin-left': '5px',
		});
		/*init*/
		$('.tableICell').prop('sel', 0).css('background-color', 'white');
		/*Get Rules*/
		setwifis();
		getrules();
		timefail();
		function flag(now) {
			if (!$(now).prop('sel')) $(now).css('background-color', '#FB6666');
			else $(now).css('background-color', 'white');
			$(now).prop('sel', !$(now).prop('sel'));
		}

		function setwifis() {
			self.cmd('if [ ! -f "/tmp/wifitiming" ]; then touch /tmp/wifitiming && chmod +x /tmp/wifitiming; fi').then(function () {
				var shell = 'if [ $1 == "down" ]; then\n	uci set wireless.default_radio0.disabled="1"\n	uci set wireless.default_radio1.disabled="1"\nelif [ $1 == "up" ]; then\n	uci set wireless.default_radio0.disabled="0"\n	uci set wireless.default_radio1.disabled="0"\nfi\nuci commit\nwifi reload'
				self.write('/tmp/wifitiming', shell);
			});
		}

		function chosen(here, x, y) {
			var now;
			if (!here) {
				if (!x) {
					for (var a = 1; a < 25; a++) {
						now = $('#weekTb')
							.find('tr:nth-child(' + y + ')')
							.find('td:nth-child(' + a + ')')
							.find('.tableICell');
						flag(now);
					}
				} else if (!y) {
					for (var a = 1; a < 8; a++) {
						now = $('#weekTb')
							.find('tr:nth-child(' + a + ')')
							.find('td:nth-child(' + x + ')')
							.find('.tableICell');
						flag(now);
					}
				} else {
					now = $('#weekTb')
						.find('tr:nth-child(' + y + ')')
						.find('td:nth-child(' + x + ')')
						.find('.tableICell');
					flag(now);
				}
			} else {
				now = here;
				flag(now);
			}
		}

		function getrules() {
			self.read('/etc/crontabs/root').then(function (r) {
				var rules = r.split('\n');
				if (rules) {
					for (var i = 0; i < rules.length; i++) {
						var temp = rules[i].split(' ');
						if (temp[temp.length - 1] == 'up') {
							var down = rules[i - 1].split(' ');
							var up = rules[i].split(' ');
							down = down.splice(1, 4).map(Number);
							down.splice(1, 2);
							up = up.splice(1, 4).map(Number);
							up.splice(1, 2);
							var tl = up[1] - down[1];
							if (tl < 0) tl += 7;
							tl = tl * 24 + up[0] - down[0];
							down[1] = (down[1] + 6) % 7;
							for (var x = down[0], y = down[1], count = 0; count < tl; count++) {
								chosen(0, x + 1, y + 1);
								x++;
								if (x > 23) {
									y = (y + 1) % 7;
									x = 0;
								}
							}
						}
					}
				}
			});
		}

		function timefail() {
			self.get_routert().then(function (time) {
				localt = Math.trunc(Date.now() / 1000);
				routert = time - 8 * 60 * 60;
				if (routert < localt + 30 && routert > localt - 30) fit = true;
				if (!fit) $('#warning').show();
				else $('#warning').hide();
			});
		}

		function checknow() {
			self.get_routert().then(function (time) {
				routert = (time - 8 * 60 * 60) * 1000;
				var d = new Date(routert);
				var y = d.getDay();
				if (y == 0) y = 7;
				var a = d.getHours() + 1;
				var now = $('#weekTb')
					.find('tr:nth-child(' + y + ')')
					.find('td:nth-child(' + a + ')')
					.find('.tableICell');
				if (now.prop('sel')) self.cmd('/tmp/wifitiming down');
				else self.cmd('/tmp/wifitiming up');
			});
		}

		function setrules() {
			var status = false;
			let rules = new Array();
			for (var i = 1; i < 8; i++) {
				var now = $('#weekTb')
					.find('tr:nth-child(' + i + ')')
					.find('i:first');
				for (var j = 0; j < 24; j++) {
					if ($(now).prop('sel') != status) {
						var w = i % 7;
						if (!status) rules.push('0 ' + j + ' * * ' + w + ' /tmp/wifitiming down');
						else rules.push('0 ' + j + ' * * ' + w + ' /tmp/wifitiming up');
						status = !status;
					}
					now = $(now).parent().next().children();
				}
			}
			checknow();
			self.cmd('sed -i "/wifitiming down/d" /etc/crontabs/root');
			self.cmd('sed -i "/wifitiming up/d" /etc/crontabs/root');
			rules = rules.join('\n');
			self.write('/etc/crontabs/root', rules + '\n', true);
			self.cmd('/etc/init.d/cron restart');
		}
		$('.tableICell').click(function () {
			chosen(this);
		});
		$('.tableICell').on('mousedown', function () {
			$('.tableICell').on('mouseover', function () {
				$(this).click();
			});
		});
		$('.tableICell').on('mouseup', function () {
			$('.tableICell').off('mouseover');
		});
		$('#timeWeek').find('li').click(function () {
			chosen(0, 0, $(this).index() + 1);
		});
		$('#timeHour').find('span').click(function () {
			chosen(0, $(this).parent().index() + 1);
		});
		$('#save').click(function () {
			setrules();
			L.ui.setting(L.tr('Saving...'));
			var tip = setTimeout(function () {
				clearTimeout(tip);
				L.ui.setting('', false);
			}, 1500);
		});
	},
});
