L.ui.view.extend({
	wifi_info: L.rpc.declare({
		object: 'iwinfo',
		method: 'info',
		params: ['device'],
		expect: { ssid: '' },
		filter: function (data) {
			return data;
		},
	}),
	net_speed: L.rpc.declare({
		object: 'luci2.network.bwmon',
		method: 'statistics',
		params: ['device'],
		expect: {
			statistics: {},
		},
	}),
	net_sta: L.rpc.declare({
		object: 'network.interface.wan',
		method: 'status',
		expect: {},
	}),
	repeater_sta: L.rpc.declare({
		object: 'network.interface.lan',
		method: 'status',
		expect: {},
	}),
	sys_info: L.rpc.declare({
		object: 'system',
		method: 'info',
	}),
	reboot: L.rpc.declare({
		object: 'system',
		method: 'reboot',
	}),

	execute: function () {
		var self = this;
		var fresh;
		var list = ['wireless', 'network', 'devlist', 'wldevlist'];
		L.network.load().then(function () {
			L.network.refreshInterfaceStatus().then(function () {
				var networks = L.network.getInterfaces();
				for (var i = 0; i < networks.length; i++) {
					var network = networks[i];
					if (network.options.name == 'lan') {
						var lip4 = network.getIPv4Addrs();
					}
				}
				if ($('#lan_ip').text() == '')
					$('#lan_ip').text(lip4);
			});
		});
		L.uci.load(list).then(function () {
			fresht();
			p_wifi();
			p_dev();
			p_net();
			var all = setInterval(function () {
				var now = L.getHash('view');
				if (now != 'home') clearInterval(all);
				fresht();
				p_wifi();
				p_dev();
			}, 60000);

			var fresh = setInterval(function () {
				var now = L.getHash('view');
				if (now != 'home') clearInterval(fresh);
				p_net();
			}, 1000);

			$('#reboot_btnId').click(function () {
				setTimeout(function(){
					location.reload(true);
				}, 70000);
				var pop = $('<div />')
				.addClass('PopWin')
				.attr('id', 'ifreboot')
				.append(
					$('<div />')
						.addClass('PopWin_box pop_absolute_center')
						.css('width', '500px')
						.css('margin','-160px -250px')
						.append($('<div />').addClass('popWin_title table_cell').css('width', '500px').text(L.tr("Tips")))
						.append(
							$('<div />')
								.addClass('PopWin_content')
								.append(
									$('<div />')
										.css({
											'text-align': 'center',
											padding: '0 30px',
											'margin-top': '20px',
											'font-size': '12px',
											color: 'rgb(89,89,89)',
										})
										.text(L.tr("Click Continue to confirm reboot, which may interrupt the connection")),
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
										.text(L.tr("Cancel"))
										.click(function () {
											$('#ifreboot').remove();
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
										.text(L.tr("Continue"))
										.click(function () {
											$('#ifreboot').remove();
											startreboot();
										}),
								),
						),
				);
				$('body').append(pop);
			function startreboot() {
				$('body').append(
					$('<div />')
						.addClass('loading')
						.append(
							$('<div />')
								.addClass('loading_content absolute_center')
								.append(
									$('<div />')
										.addClass('loading_box default clearboth')
										.append(
											$('<div />').addClass('loading_gif').css({
												'margin-right': '20px',
												right: '0',
											}),
										)
										.append($('<div />').addClass('loading_word').css('height', '61px').append($('<p />').text(L.tr("Rebooting...")))),
								),
						),
				);
				self.reboot();
			}
			});
			$('.router_wan').click(function () {
				$('#internet').click();
			});
			$('.router_more').click(function () {
				$('#more').click();
			});
			$('.router_deviceCount').click(function () {
				if(proto.value !== "bridge"){
					$('#device').click();
				}
			});
			function fresht() {
				self.sys_info().then(function (sys) {
					var t = sys.uptime;
					var t1 = 0,
						t2 = 0,
						t3 = 0;
					var d = '';
					if (t < 60) {
						t1 = t;
						d = t1 + L.tr(" s");
					} else if (t >= 60) {
						t1 = ~~(t / 60);
						if (t1 >= 60) {
							t2 = ~~(t1 / 60);
							t1 = t1 - 60 * t2;
							if (t2 >= 24) {
								t3 = ~~(t2 / 24);
								t2 = t2 - 24 * t3;
							}
						}
						d = t3 + L.tr(" days") + ' ' + t2 + L.tr(" hours") + ' ' + t1 + L.tr(" mins");
					}
					$('#sys_t').text(d);
				});
			}

			function p_dev() {
				L.uci.callLoad('devlist').then(function (dev) {
					L.uci.callLoad('wldevlist').then(function (wld) {
						var dc = 0;
						for (var a in dev) {
							if (L.uci.get('devlist', a, 'online') == '1') dc++;
						}
						for (var b in wld) {
							if (L.uci.get('wldevlist', b, 'online') == '1') dc++;
						}
						$('#dev_co').text(dc);
					});
				});
			}
			function p_wifi() {
				$('body').find('.wifi_24g_hover').css('margin-left', '5px');
				$('body').find('.wifi_5g_hover').css('margin-left', '5px');
				$('body')
					.find('.wifi_24g_box')
					.click(function () {
						$('#wifi').click();
					});
				$('body')
					.find('.wifi_5g_box')
					.click(function () {
						$('#wifi').click();
					});
				self.wifi_info('wlan0').then(function (ip_24g) {
					if (ip_24g != '') {
						$('body').find('.wifi_24g_hover').parent().prop('class', 'wifi_24g wifi_24g_able');
						$('body').find('.wifi_24g_hover').text(ip_24g);
					} else {
						$('body').find('.wifi_24g_hover').parent().prop('class', 'wifi_24g wifi_24g_disable');
						$('body').find('.wifi_24g_hover').remove();
					}
				});
				self.wifi_info('wlan1').then(function (ip_5g) {
					$('body').find('.wifi_5g_hover').text(L.uci.get('wireless', 'default_radio1', 'ssid'));
					if (ip_5g != '') {
						$('body').find('.wifi_5g_hover').parent().prop('class', 'wifi_5g wifi_5g_able');
						$('body').find('.wifi_5g_hover').text(ip_5g);
					} else {
						$('body').find('.wifi_5g_hover').parent().prop('class', 'wifi_5g wifi_5g_disable');
						$('body').find('.wifi_5g_hover').remove();
					}
				});
			}
			function p_net() {
				function updates() {
					self.net_speed('eth0.2').then(function (history) {
						var tx = history.tx_bytes[history.tx_bytes.length - 1];
						var rx = history.rx_bytes[history.rx_bytes.length - 1];
						if (tx) {
							tx = tx * 8;
							if (tx < 1000) {
								$('#ups').html('%.2m'.format(tx));
								$('#upu').text('bps');
							} else if (tx > 1048576) {
								$('#ups').html('%.2m'.format(tx / 1000000));
								$('#upu').text('Mbps');
							} else {
								$('#ups').html('%.2m'.format(tx / 1000));
								$('#upu').text('Kbps');
							}
						} else {
							$('#ups').text('0');
						}
						if (rx) {
							rx = rx * 8;
							if (rx < 1000) {
								$('#downs').html('%.2m'.format(rx));
								$('#downu').text('bps');
							} else if (rx > 1000000) {
								$('#downs').html('%.2m'.format(rx / 1000000));
								$('#downu').text('Mbps');
							} else {
								$('#downs').html('%.2m'.format(rx / 1000));
								$('#downu').text('Kbps');
							}
						} else {
							$('#downs').text('0');
						}
					});
				}
				self.net_sta().then(function (wan) {
					if(L.uci.get('network', 'wan', 'proto') == 'bridge'){
						var p1 = $('body').find('.content_status');
								$('body').find('.router_more').remove();
								var p2 = $('body').find('.fun_name');
								var p3 = $('body').find('.hone_connect');
								$(p1)
									.prop('class', 'content_status_repeater')
									.prepend(
										$('<div />')
										.addClass('repeater_center_box fl')
												.append($('<div />').addClass('home_wds_center').css('margin-left','-50px'))
											.append(
												$('<div />')
													.addClass('repeater_status')
													.css('left', '120px')
													.append(
														$('<div />')
															.css({
																"width": "100px",
																"height": "2px",
																"background": "#FA6F43",
																"position": "absolute",
																"top": "16px"
															})
													)
													.append(
														$('<div />').css({
															'background-image': 'url("luci2/res/siflower.png")',
															'margin-left': '120px',
															'width': '90px',
															'height': '90px',
															'position': 'relative',
															'margin-top': '-25px',
															'background-position': '50% 62.2%',
															'background-size': '300%'
														}),
													),
											),
									)
									.prepend($('<div />').addClass('repeater_line_box fl').append($('<div />').addClass('home_network').css('margin-left','-60px'))
										.append($('<div />').addClass('repeater_line left_115')));
								$(p1).find('.router_wan').remove();
								$(p1).find('.router_wifi').prop('class','router_wifi fl').css('margin-left', '100px');
								$(p1).find('.router_deviceCount').prop('class','router_deviceCount fl').css('margin-left', '0px');
								$(p2).empty().prop('class', 'repeater_name');
								$(p2)
									.append(
										$('<p />')
											.css({
												'margin-left': '42px',
												float: 'left',
											})
											.text(L.tr("Internet")),
									)
									.append(
										$('<p />')
											.css({
												'margin-left': '108px',
												float: 'left',
											})
											.text(L.tr("Superior Router")),
									)
									.append(
										$('<p />')
											.css({
												'margin-left': '95px',
												float: 'left',
											})
											.text(L.tr("This device")),
									)
									.append(
										$('<p />')
											.css({
												'margin-left': '114px',
												float: 'left',
											})
											.text(L.tr("Devices")),
									);
								$(p3).prop('class', 'hone_connect hone_connect_repeater');
								$(p3).find('.wanip').remove();
								$(p3).find('.system').css('border', 'none');
								$(p3).find('.wantime').find('.descripe').text(L.tr("Bridge access method"));
								$(p3)
									.find('.wantime')
									.find('.content')
									.text(L.tr("Wired bridging"));
					}else if (wan == undefined || (L.uci.get('network', 'lan', 'proto') == 'dhcp')) {
						self.repeater_sta().then(function (lan) {
							if (lan.up == false) {
								var wwanfail='<div class="home_network"></div><div id="conn_sta" class="repeater_connect marginleft_15 fl left_130"></div><p id = "conn_info" class="marginleft_15 left_130" >'+ L.tr("Error interface, please check the configuration.")+'</p>';
								$('.router_wan').empty().html(wwanfail);
								$('#wan_ip').text(L.tr("No internet connection"));
							} else {
								var p1 = $('body').find('.content_status');
								$('body').find('.router_more').remove();
								var p2 = $('body').find('.fun_name');
								var p3 = $('body').find('.hone_connect');
								$(p1)
									.prop('class', 'content_status_repeater')
									.prepend(
										$('<div />')
											.addClass('repeater_center_box fl')
												.append($('<div />').addClass('home_wds_center').css('margin-left','-50px'))
											.append(
												$('<div />')
													.addClass('repeater_status repeater_status_line')
													.css('left', '120px')
													.append(
														$('<div />')
															.addClass('repeater_status info')
															.css({
																color: 'rgb(51,51,51)',
																'word-break': 'break-word',
																width: '120px',
																'margin-left': '-10px',
																'line-height': '15px',
															})
															.append($('<div />').attr('id', 're_in')),
													)
													.append(
														$('<div />')
															.addClass('repeater_bridgestrenth repeater_wireless_box')
															.css({
																color: 'rgb(51,51,51)',
																'word-break': 'break-all',
																width: '120px',
																'margin-left': '-10px',
																'line-height': '15px',
															})
															.append(
																$('<p />')
																	.css('white-space', 'pre-wrap')
																	.text('SSID :' + L.uci.get('wireless', 'wds', 'ssid')),
															),
													)
													.append(
														$('<div />').css({
															'background-image': 'url("luci2/res/siflower.png")',
															'margin-left': '120px',
															'width': '90px',
															'height': '90px',
															'position': 'relative',
															'margin-top': '-25px',
															'background-position': '50% 62.2%',
															'background-size': '300%'
														}),
													),
											),
									)
									.prepend($('<div />').addClass('repeater_line_box fl').append($('<div />').addClass('home_network').css('margin-left','-60px'))
										.append($('<div />').addClass('repeater_line left_115')));
								$(p1).find('.router_wan').remove();
								$(p1).find('.router_wifi').prop('class','router_wifi fl').css('margin-left', '100px');
								$(p1).find('.router_deviceCount').prop('class','router_deviceCount fl').css('margin-left', '0px');
								$(p2).empty().prop('class', 'repeater_name');
								$(p2)
									.append(
										$('<p />')
											.css({
												'margin-left': '42px',
												float: 'left',
											})
											.text(L.tr("Internet")),
									)
									.append(
										$('<p />')
											.css({
												'margin-left': '108px',
												float: 'left',
											})
											.text(L.tr("Main route")),
									)
									.append(
										$('<p />')
											.css({
												'margin-left': '95px',
												float: 'left',
											})
											.text(L.tr("This device")),
									)
									.append(
										$('<p />')
											.css({
												'margin-left': '114px',
												float: 'left',
											})
											.text(L.tr("Devices")),
									);
								$(p3).prop('class', 'hone_connect hone_connect_repeater');
								$(p3).find('.wanip').remove();
								$(p3).find('.system').css('border', 'none');
								$(p3).find('.wantime').find('.descripe').text(L.tr("Relay"));
								var type = L.uci.get('wireless', 'wds', 'device');
								if (type == 'radio0') {
									type = '2.4G ';
								} else if (type == 'radio1') {
									type = '5G ';
								}
								$(p3)
									.find('.wantime')
									.find('.content')
									.text(type + L.tr("Wi-Fi access"));
							}
						});
					} else if (wan.up == true) {
						var net = $('#conn_sta').parent();
						$('#conn_sta').remove();
						$('#conn_info').remove();
						$('#wan_ip').text(wan['ipv4-address'][0]['address']);
						$(net)
							.append(
								$('<div />')
									.addClass('router_up_speed marginleft_2 left_175')
									.append($('<i />').addClass('arrow_up fl'))
									.append($('<div />').addClass('fl').css('direction', 'ltr').append($('<span />').css('display', 'inline').attr('id', 'ups')).append($('<em />').attr('id', 'upu').text('Kbps'))),
							)
							.append($('<div />').addClass('router_wan_line left_150'))
							.append(
								$('<div />')
									.addClass('router_down_speed left_175')
									.append($('<i />').addClass('arrow_down fl'))
									.append($('<div />').addClass('fl').css('direction', 'ltr').append($('<span />').css('display', 'inline').attr('id', 'downs')).append($('<em />').attr('id', 'downu').text('Kbps'))),
							);
							updates();
					} else if (wan.up == false) {
						var noconn='<div class="home_network"></div><div id="conn_sta" class="repeater_connect marginleft_15 fl left_130"></div><p id = "conn_info" class="marginleft_15 left_130" >'+ L.tr("No internet connection, please check the configuration.") +'</p>';
						$('.router_wan').empty().html(noconn);
						$('#wan_ip').text(L.tr("No internet connection"));
					}
				});
			}
		});
	},
});


