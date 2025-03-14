L.ui.view.extend({
	dns: L.rpc.declare({
		object: 'network.interface.wan',
		method: 'status',
		expect: {
			'dns-server': [],
		},
	}),
	cmd: L.rpc.declare({
		object: 'web.advance',
		method: 'cmd',
		params: ['cmd'],
	}),
	wifi_status: L.rpc.declare({
		object: 'network.device',
		method: 'status',
	}),
	read: L.rpc.declare({
		object: 'file',
		method: 'read',
		params: ['path'],
		expect: {
			data: '',
		},
	}),
	execute: function () {
		var self = this,
			on = true, ifhttp = false;
		$('table,tr:not(tr:last-child) td:nth-last-child(1),tr:not(tr:last-child) td:nth-last-child(2)').css('border-bottom', '1px solid #c9c9c9');
		$('table').css({
			width: '90%',
			left: '5%',
			position: 'relative',
			'text-align': 'center',
			'line-height': '35px',
		});
		$('#Internet table').css('border-top', '1px solid #c9c9c9');
		$('td').css('width', '30%');
		var icon = $('<div />').addClass('icon checkbox').css({
			width: '30px',
			height: '30px',
			display: 'block',
			'background-size': '700%',
			'margin-top': '4px',
			'background-position': '49% 42.12%',
			'margin-left': '0px',
			cursor: 'default',
		});
		$('td:last-child').addClass('row').prepend(icon);
		$('td:last-child').find(':last-child').css({
			width: '75%',
			'text-align': 'center',
			margin: '0',
		});
		self.cmd('test -f /usr/bin/httping && echo "$FILE exist" > /tmp/httping');
		self.read('/tmp/httping').then(function (http) {
			if (http) ifhttp = true;
			else $('.icon:nth(2)').next().text(L.tr('Tools Not Installed'));
			self.cmd('rm /tmp/httping');
		});
		function uplink() {
			var flag = 3, msg = 'No Connection';
			self.cmd("ping siflower.com.cn -A -w 10|grep loss|awk '{print $7}' > /tmp/ping");
			var r1 = setTimeout(function () {
				clearTimeout(r1);
				self.read('/tmp/ping').then(function (res) {
					if (res == '0%\n') {
						flag = 2;
						msg = '';
					}
					changes(flag, 1, L.tr(msg));
					self.cmd('rm /tmp/ping');
				});
			});
		}
		function DNS() {
			var flag = 3, msg = 'Diagnosis Fail';
			self.dns().then(function (dns) {
				if (dns[0]) {
						flag = 2;
						msg = '';
				}
				changes(flag, 2, L.tr(msg));
			});
		}
		function HTTP() {
			var flag = 3, msg = 'Diagnosis Fail';
			self.cmd("httping -g https://siflower.com.cn -c 10 | grep connects,|awk '{print $5}' > /tmp/httping");
			var r2 = setTimeout(function () {
				clearTimeout(r2);
				self.read('/tmp/httping').then(function (res) {
					if (res == '0.00%\n') {
						flag = 2;
						msg = '';
					}
					changes(flag, 3, L.tr(msg));
					self.cmd('rm /tmp/httping');
				});
			}, 15000);
		}
		function Key() {
			var flag = 3, msg='Error', i = 5;
			L.ui.setting(1, L.tr('Please short press the button within %ds.').format(i));
			var fresh = setInterval(function () {
				i--;
				$('.loading').remove();
				if (i > 0) {
					L.ui.setting(1, L.tr('Please short press the button within %ds.').format(i));
				} else clearInterval(fresh);
			}, 1000);
			var delay = setTimeout(function () {
				clearTimeout(delay);
				self.read('/tmp/key').then(function (key) {
					if (key == 'pressed\n') {
						flag = 2;
						msg = '';
					}
					changes(flag, 4, L.tr(msg));
					self.cmd('rm /tmp/key');
				})
			}, 5000);
		}
		function LED() {
			var flag = 3, msg = 'Error';
			var pop = $('<div />')
			.addClass('PopWin')
			.attr('id', 'tip')
			.append(
				$('<div />')
					.addClass('PopWin_box pop_absolute_center')
					.css('width', '500px')
					.css('margin','-160px -250px')
					.append($('<div />').addClass('popWin_title table_cell').css('width', '500px').text(L.tr('Tips')))
					.append(
						$('<div />')
							.addClass('PopWin_content')
							.append(
								$('<div />')
									.css({
										'text-align': 'center',
										padding: '0 30px',
										'margin-top': '20px',
										'font-size': '16px',
										color: 'rgb(89,89,89)',
									})
									.text(L.tr('Observe whether the LED blinks normally.')),
							),
					)
					.append(
						$('<div />')
							.addClass('PopWin_buttons')
							.css('width', '386px')
							.append(
								$('<div />')
									.addClass('btn fl')
									.css({
										width: '175px',
										height: '45px',
										'font-size': '16px',
										'line-height': '28px',
										margin: '7px',
									})
									.text(L.tr('no'))
									.click(function () { 
										$('#tip').remove();
										changes(flag, 5, L.tr(msg));
									}),
							)
							.append(
								$('<div />')
									.addClass('btn fl')
									.css({
										width: '175px',
										height: '45px',
										'font-size': '16px',
										'line-height': '28px',
										margin: '7px',
									})
									.text(L.tr('yes'))
									.click(function () { 
										$('#tip').remove();
										flag = 2;
										msg = '';
										changes(flag, 5, L.tr(msg));
									}),
							),
					),
			);
			$('body').append(pop);
		}
		function WiFi() {
			var flag = 3,
				msg;
			self.wifi_status().then(function (wifi) {
				if (wifi.wlan0 && wifi.wlan1 && wifi.wlan0.up && wifi.wlan1.up) {
					flag = 2;
					msg = 'All Wi-Fi On';
				}
				if (!wifi.wlan0 || !wifi.wlan0.up) msg = 'Wi-Fi 2.4G is Off';
				if (!wifi.wlan1 || !wifi.wlan1.up) msg = 'Wi-Fi 5G is Off';
				if ((!wifi.wlan0 && !wifi.wlan1) || (!wifi.wlan0.up && !wifi.wlan1.up)) msg = 'All Wi-Fi Off';
				changes(flag, 6, L.tr(msg));
			});
		}
		function changes(flag, pos, txt) {
			pos = Number(pos) - 1;
			if (pos == 2 && !ifhttp) {
				flag = 3;
				txt = '';
			}
			var now = $('.icon:nth(' + pos + ')');
			if (flag === 1) {//ING
				$(now).css('background-position', '49% 21.04%');
				txt = L.tr('Diagnosing');
			}
			if (flag === 2) {//DONE
				$(now).css('background-position', '49% 43.8%');
				if (!txt) txt = L.tr('OK');
			}
			if (flag === 3)//Error
				$(now).css({
					'background-position': '49% 24.1%',
					'background-size': '400%',
				});
			if (txt) $(now).next().text(txt);
			if (flag != 1) on = false;
		}
		function whendone(num) {
			num = Number(num);
			var set = setInterval(function () {
				if (!on) {
					clearInterval(set);
					on = true;
					switch (num) {
						case 2:
							DNS();
							break;
						case 3:
							if (ifhttp) HTTP();
							break;
						case 4:
							Key();
							break;
						case 5:
							var a = setTimeout(function () {
								clearTimeout(a);
								LED();
							}, 500);
							break;
						case 6:
							WiFi();
							break;
						case 7:
							$('#diagnose').text(L.tr('Start Diagnose')).removeAttr('disabled');
							break;
					}
					if (num < 7) {
						if (num != 5) changes(1, num, '');
						whendone(num + 1);
					}
				}
			}, 500);
		}
		$('#diagnose').click(function () {
			$(this).text(L.tr('Diagnosing')).attr('disabled', 'true');
			uplink();
			changes(1, 1, '');
			whendone(2);
		});
	},
});
