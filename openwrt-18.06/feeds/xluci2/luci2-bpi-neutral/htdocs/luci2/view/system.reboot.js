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

	handleReboot: function () {
		var form = $('<p />').text(L.tr('Really reboot the device?'));
		L.ui.dialog(L.tr('Reboot'), form, {
			style: 'confirm',
			confirm: function () {
				L.system.performReboot().then(function () {
					L.ui.reconnect(L.tr('Device rebooting...'));
				});
			},
		});
	},

	execute: function () {
		var self = this;

		$('#btn_reboot').click(self.handleReboot);

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
		/*init初始化*/
		$('.tableICell').prop('sel', 0).css('background-color', 'white'); // 将所有表格单元格的 `sel` 属性设为 0，并将背景颜色设为白色

		/*设置定时重启*/
		setreboot(); // 调用设置重启计划的函数
		getrules();  // 调用获取规则函数
		timefail();  // 调用时间同步检测函数

		// 切换单元格选中状态并改变背景颜色
		function flag(now) {
			if (!$(now).prop('sel'))
				$(now).css('background-color', '#FB6666'); // 如果未选中，则背景变为红色
			else
				$(now).css('background-color', 'white');   // 如果选中，则背景变为白色
			$(now).prop('sel', !$(now).prop('sel'));       // 切换选中状态
		}

		// 设置重启计划
		function setreboot() {
			// 检查临时文件是否存在，不存在则创建一个用于保存定时脚本
			self.cmd('if [ ! -f "/etc/config/reboottiming" ]; then touch /etc/config/reboottiming && chmod +x /etc/config/reboottiming; fi').then(function () {
				var shell = 'if [ $1 == "reboot" ]; then\n    reboot\nfi'; // 脚本内容，当传入参数为“reboot”时执行重启命令
				self.write('/etc/config/reboottiming', shell); // 将脚本写入到 `/etc/config/reboottiming`
			});
		}

		// 选中指定单元格
		function chosen(here, x, y) {
			var now;
			if (!here) {
				if (!x) {
					for (var a = 1; a < 25; a++) { // 选择整行
						now = $('#weekTb')
							.find('tr:nth-child(' + y + ')')
							.find('td:nth-child(' + a + ')')
							.find('.tableICell');
						flag(now); // 切换选中状态
					}
				} else if (!y) {
					for (var a = 1; a < 8; a++) { // 选择整列
						now = $('#weekTb')
							.find('tr:nth-child(' + a + ')')
							.find('td:nth-child(' + x + ')')
							.find('.tableICell');
						flag(now); // 切换选中状态
					}
				} else {
					now = $('#weekTb')
						.find('tr:nth-child(' + y + ')')
						.find('td:nth-child(' + x + ')')
						.find('.tableICell');
					flag(now); // 切换单个单元格选中状态
				}
			} else {
				now = here; // 如果传入的是当前单元格，直接切换状态
				flag(now);
			}
		}

		// 获取当前计划规则
		function getrules() {
			self.read('/etc/crontabs/root').then(function (r) {
				var rules = r.split('\n'); // 读取 crontab 文件中的计划任务
				if (rules) {
					for (var i = 0; i < rules.length; i++) {
						if (rules[i].trim() === '') continue; // 跳过空白行
						var temp = rules[i].split(' ');
						if (temp.length >= 6 && temp[temp.length - 1] == 'reboot') { // 判断是否为重启任务
							var time = rules[i].split(' ').splice(0, 5).map(Number); // 获取五个时间字段
							time[4] = (time[4] + 6) % 7; // 将星期天设为0，调整星期字段
							chosen(0, time[1] + 1, time[4] + 1); // 选中对应时间的单元格（小时和星期）
						}
					}
				}
			});
		}


		// 检查本地时间和路由器时间是否同步
		function timefail() {
			self.get_routert().then(function (time) {
				localt = Math.trunc(Date.now() / 1000); // 获取本地时间戳
				routert = time - 8 * 60 * 60; // 路由器时间，减去8小时偏差
				if (routert < localt + 30 && routert > localt - 30) fit = true; // 如果路由器时间与本地时间相差小于30秒，则认为时间同步
				if (!fit) $('#warning').show(); // 时间不同步则显示警告
				else $('#warning').hide(); // 时间同步则隐藏警告
			});
		}

		// 检查当前时间是否符合重启计划
		function checknow() {
			self.get_routert().then(function (time) {
				routert = (time - 8 * 60 * 60) * 1000; // 将路由器时间转换为毫秒
				var d = new Date(routert);
				var y = d.getDay(); // 获取当前星期
				if (y == 0) y = 7;  // 将星期天设为7
				var a = d.getHours() + 1; // 获取当前小时
				var now = $('#weekTb')
					.find('tr:nth-child(' + y + ')')
					.find('td:nth-child(' + a + ')')
					.find('.tableICell');
				if (now.prop('sel')) self.cmd('/etc/config/reboottiming reboot'); // 如果当前单元格选中，则执行重启命令
			});
		}

		// 设置计划任务
		function setrules() {
			var status = false;
			let rules = new Array(); // 定义数组存放计划任务规则
			for (var i = 1; i < 8; i++) { // 遍历每一天
				var now = $('#weekTb')
					.find('tr:nth-child(' + i + ')')
					.find('i:first');
				for (var j = 0; j < 24; j++) { // 遍历每一小时
					if ($(now).prop('sel') != status) { // 如果状态发生变化，则添加计划任务
						var w = i % 7;
						if (!status)
							rules.push('0 ' + j + ' * * ' + w + ' /etc/config/reboottiming reboot'); // 添加定时重启任务
						status = !status;
					}
					now = $(now).parent().next().children(); // 继续遍历下一个单元格
				}
			}
			checknow(); // 检查当前是否符合计划任务的执行时间
			self.cmd('sed -i "/reboottiming reboot/d" /etc/crontabs/root'); // 删除已有的重启任务
			rules = rules.join('\n'); // 将规则数组转换为字符串
			self.write('/etc/crontabs/root', rules + '\n', true); // 将规则写入 crontab 文件
			self.cmd('/etc/init.d/cron restart'); // 重启 crontab 服务以应用新的任务
		}

		// 单元格点击事件，选中或取消选中
		$('.tableICell').click(function () {
			chosen(this);
		});

		// 鼠标按下时，开始选中多个单元格
		$('.tableICell').on('mousedown', function () {
			$('.tableICell').on('mouseover', function () {
				$(this).click();
			});
		});

		// 鼠标抬起时，停止选中多个单元格
		$('.tableICell').on('mouseup', function () {
			$('.tableICell').off('mouseover');
		});

		// 星期选择按钮的点击事件
		$('#timeWeek').find('li').click(function () {
			chosen(0, 0, $(this).index() + 1);
		});

		// 小时选择按钮的点击事件
		$('#timeHour').find('span').click(function () {
			chosen(0, $(this).parent().index() + 1);
		});

		// 保存按钮的点击事件
		$('#save').click(function () {
			setrules(); // 调用设置计划任务函数
			L.ui.setting(L.tr('Saving...')); // 显示“保存中...”的提示信息
			var tip = setTimeout(function () {
				clearTimeout(tip); // 在1.5秒后清除提示信息
				L.ui.setting('', false);
			}, 1500);
		});
	}
});
