L.ui.view.extend({
	//Check if wifi is connected
	wifi_status: L.rpc.declare({
		object: 'web.wireless',
		method: 'wds_wifi_connect_status',
		params: ['radio'],
		expect: {
			status: '',
		},
	}),

	//Scan wifi
	getWifiList: L.rpc.declare({
		object: 'iwinfo',
		method: 'scan',
		params: ['device'],
		expect: { results: [] },
		filter: function (data) {
			//Make sure the SSID exists and is not an empty string
			data = data.filter(function (item) {
				return item.ssid && item.ssid.trim() !== '';
			});
			data.sort(function (a, b) {
				return b.signal - a.signal;
			});
			return data;
		},
	}),

	//Connect wifi
	wifiConnect: L.rpc.declare({
		object: 'web.wireless',
		method: 'wds_wifi_connect',
		params: ['device', 'ssid', 'bssid', 'encryption', 'key']
	}),
	//Add network and Connect wifi
    wifiConnect_new: L.rpc.declare({
		object: 'web.wireless',
		method: 'wds_wifi_connect_new',
		params: ['ssid', 'encryption', 'key', 'flag']
	}),

	//Disable wds
	wds_Disable: L.rpc.declare({
		object: 'web.wireless',
		method: 'wds_disable'
	}),

	//Enable wds
	wds_Enable: L.rpc.declare({
		object: 'web.wireless',
		method: 'wds_enable',
		params: ['ifname', 'ip']
	}),

	//Check lan IP
	lan_ip: L.rpc.declare({
		object: 'network.interface.lan',
		method: 'status',
		expect: { 'ipv4-address': [] },
		filter: function (data) {
			var ip = '0';
			if (data[0]) {
				ip = data[0].address;
			}
			return ip;
		},
	}),

	execute: function () {
		var self = this;
		$('#wds_en').css({
			width: '50px',
			height: '27px',
		});
		var if_start_check = 0;		//check flag, make sure only one check interval exist
		var y = 0;				//check counter
		var  stop = false;
		var lip, lip1, p1, p2;
		var e, flag_ssid;
		self.lan_ip().then(function (now) {
			lip = now;
			lip1 = lip.split('.');
			lip1[3] = '1';
			lip1[2] = String(Number(lip1[2]) + 1);
			lip1 = lip1.join('.');
		});

		var wifi_table, wifilist, list_all, en, wi, ifwifi, ifwi1;
		function loadbox(msg) {
			var load = $('<div />')
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
								.append($('<div />').addClass('loading_word').css('height', '61px').append($('<p />').text(msg))),
						),
				);
			return load;
		}

		$('body').append(loadbox(L.tr("Confirm Wi-Fi Repeater setting, please wait")));
		var all = setTimeout(function () {
			$('body').find('.loading').remove();
			clearTimeout(all);
			checkWDS();
		}, 500);

		$('#wds_en').click(function () {
			if ($('#wds_en').prop('class') == 'switch_on') {
				$('#wds_en').prop('class', 'switch_off');
				$('body').append(loadbox(L.tr("Disable Wi-Fi Repeater, please wait")));
				var count = setInterval(function () { }, 10000);
				for (var cl = 1; cl <= count; cl++) {
					clearInterval(cl);
				}
				self.wds_Disable().then(function () {
					setTimeout(function () {
						var ip = L.uci.get('network' , 'lan', 'oip');
						var q = setInterval(function () {
							$.ajax({
								type: 'GET',
								url: 'http://' + ip,
								dataType: 'jsonp',
								complete: function (jump) {
									if (jump.status == 200) {
										clearInterval(q);
										setTimeout(location.href = 'http://' + ip, 1000);
									}
								}
							});
						}, 500);
					}, 5000);
				});
			} else if ($('#wds_en').prop('class') == 'switch_off') {
				var pop = $('<div />')
					.addClass('PopWin')
					.attr('id', 'opentip')
					.append(
						$('<div />')
							.addClass('PopWin_box pop_absolute_center')
							.css('width', '500px')
							.css('margin', '-160px -250px')
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
											.text(L.tr("To switch from routing mode to Wi-Fi Repeater mode, please ensure that the network cable is unplugged. If your Wi Fi is turned off, clicking 'Continue' will automatically turn on Wi Fi. At this point, the connection between your router and browser will be disconnected. Please reconnect later.")),
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
												$('#opentip').remove();
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
												$('#opentip').remove();
												wds_on();
											}),
									),
							),
					);
				$('body').append(pop);
			}
		});
		$('.subject').on('click', '#next', function () {
			scan();
		});

		function scan() {
			$('.subject').empty().css('margin-bottom', '50px');
			wifi_table = $('<div />')
				.css({
					margin: '0px 28px',
					'border-radius': '12px',
					border: '1px solid #c9c9c9',
				})
				.append(
					$('<div />').text(L.tr("Please choose one below to connect")).css({
						'font-size': '18px',
						'text-align': 'center',
						color: '#262626',
						'line-height': '85px',
						height: '85px',
					}),
				)
				.append(
					$('<div />')
						.addClass('guidegray')
						.css({
							'border-top': '1px solid #C9C9C9',
							'padding-left': '20px',
							'text-align': 'left',
						})
						.append($('<div />').addClass('pull-left').text(L.tr("List of available wireless networks")))
						.append(
							$('<div />')
								.attr('id', 'wifi_scan')
								.text(L.tr("Rescan"))
								.css({
									cursor: 'pointer',
									'margin-right': '20px',
									color: '#FA6F43',
									float: 'right',
									'text-align': 'right',
								})
								.click(function () {
									scan();
								}),
						),
				)
				.append(
					$('<div />')
						.attr('id', 'wifilist')
						.prop('align', 'center')
						.css({
							'min-height': '220px',
						})
						.append($('<div />').addClass('ic-loading').css('margin', '50px auto 20px'))
						.append(
							$('<div />').text(L.tr("Scanning...")).css({
								'font-size': '12px',
								color: 'rgb(90,90,90)',
							}),
						),
				)
                .append(
					$('<div />')
						.addClass('guidegray')
						.css({
							'border-top': '1px solid #C9C9C9',
							'padding-left': '20px',
							'text-align': 'left',
						})
						.append($('<div />').addClass('pull-left').text(L.tr("Adding additional networks")))
						.append(
							$('<div />')
								.attr('id', 'wifi_sure')
								.text(L.tr("input"))
								.css({
									cursor: 'pointer',
									'margin-right': '20px',
									color: '#FA6F43',
									float: 'right',
									'text-align': 'right',
								})
								.click(function () {
									add_network();
								}),
						),
				);

			$('.subject').append(wifi_table);
			wifilist = $('<div />')
				.css({
					overflow: 'hidden',
					width: '680px',
				})
				.append(
					$('<div />')
						.addClass('Scroller')
						.css({
							width: '680px',
							'max-height': '300px',
						})
						.append(
							$('<div />')
								.addClass('scroller_container')
								.css({
									width: '692px',
									'max-height': '300px',
								})
								.append($('<div />').addClass('scroller_content')),
						),
				);
			self.getWifiList('wlan0').then(function (list) {
				if (list) {
					for (var i in list) {
						list[i].device = '0';
					}
					list_all = list;
					self.getWifiList('wlan1').then(function (list1) {
						if (list1) {
							for (var i in list1) {
								list1[i].device = '1';
							}
							list_all.push.apply(list_all, list1);
						}
						list_all = list_all.sort(function (a, b) {
							var aq = a.quality / a.quality_max;
							var bq = b.quality / b.quality_max;
							return bq - aq;
						});
						information(list_all);
						wifi_table.find('#wifilist').empty().append(wifilist);

						if (checkWifi()) {
							connect(wifilist.find('.gscrollitem:first'));
							$('#ssid_input').val(L.uci.get('wireless', 'wds', 'ssid'));
							$('#o_password_input').val(L.uci.get('wireless', 'wds', 'key'));
							var de = L.uci.get('wireless', 'wds', 'device');
							var e = L.uci.get('wireless', 'default_' + de, 'encryption');
							if (e == 'none') {
								$('#no_enc').prop('checked', 'true');
								$('#enc_ch').prop('class', 'checkbox_press').css('background-position', '49% 43.8%');
							}
						}

						wifilist.on('click', '.gscrollitem', function () {
							flag = 1;
							connect(this);
						});
					});
				}
			});
		}
		function connect(info) {
			var edit = $('<div />').attr('id', 'infobox').css({
				padding: '30px 0px',
				'border-top': '1px solid #C9C9C9',
				'text-align': 'left',
			});
			var id_in = $('<div />')
				.css({
					'margin-bottom': '10px',
					width: '650px',
					'margin-left': '20px',
				})
				.append(
					$('<div />')
						.addClass('InputComponent')
						.css('width', '300px')
						.append(
							$('<div />')
								.addClass('InputContent')
								.append($('<div />').text(L.tr("Wi-Fi Name")))
								.append(
									$('<div />')
										.addClass('inputBox')
										.append(
											$('<input />')
												.attr('id', 'ssid_input')
												.attr('autocomplete', 'off')
												.addClass('inputArea')
												.css({
													width: '200px',
													height: '37px',
													'text-align': 'left',
													border: '1px solid rgba(201,201,201,0.3)',
													'padding-left': '14px',
												})
												.attr('readonly', 'readonly')
												.attr('type', 'text')
												.val($(info).find('.ssid').text()),
										),
								),
						),
				)
			if($(info).find('.encryption').text() != 'Open') {
			var key_in = $('<div />')
				.css({
					'margin-bottom': '30px',
					width: '650px',
					'margin-left': '320px',
					'margin-top': '-48px',
				})
				.append(
					$('<div />')
						.addClass('InputComponent')
						.css('width', '300px')
						.append(
							$('<div />')
								.addClass('InputContent')
								.append($('<div />').text(L.tr("Wi-Fi Password")))
								.append(
									$('<div />')
										.addClass('inputBox')
										.append(
											$('<input />')
												.attr('id', 'o_password_input')
												.attr('autocomplete', 'off')
												.addClass('inputArea')
												.css({
													width: '200px',
													height: '37px',
													'text-align': 'left',
													border: '1px solid rgba(201,201,201,0.3)',
													'padding-left': '14px',
												})
												.attr('unselectable', 'on')
												.attr('type', 'password')
												.keyup(function () {
													$('#t4').remove();
													if($(info).find('.encryption').text() != 'Open' && $('#o_password_input').val().length < 1) {
														$('#t4').remove();
															$('#o_password_input').after(
																$('<div />')
																	.attr('id', 't4')
																	.css({
																		color: 'red',
																		'font-size': '12px',
																	})
																	.text(L.tr("Field must not be empty")),
															);
															e = false;
													} else {

														e = true;
													}
												}),
										)
										.append(
											$('<div />')
												.addClass('eye')
												.css('margin-left', '-70px')
												.append(
													$('<div />')
														.addClass('imgLayer eyeHidden')
														.mousedown(function () {
															$('#o_password_input').attr('type', 'text');
															$(this).prop('class', 'imgLayer eyeShow');
														})
														.mouseup(function () {
															$('#o_password_input').attr('type', 'password');
															$(this).prop('class', 'imgLayer eyeHidden');
														}),
												),
										),
								),
						),
				)
			}
			if ($(info).find('.encryption').text() == 'Open') {
				//key_in.find('#o_password_input').attr('readonly', 'readonly');
				e = true;
			}
			$('#infobox').remove();
            $('#infoboxxx').remove();
      		wifi_table.find('#wifilist').append(edit.append(id_in));
			if($(info).find('.encryption').text() != 'Open')
				wifi_table.find('#wifilist').append(edit.append(id_in).append(key_in));
			var btn_group = $('<div />')
				.css({
					'margin-top': '14px',
					position: 'relative',
					width: '735px',
					'text-align': 'center',
				})
				.append(//Cancel
					$('<div />').text(L.tr("Cancel")).addClass('btn-primary').attr('id', 'cancel').css({
						display: 'inline-block',
						width: '263px',
						height: '50px',
						'font-size': '16px',
						'line-height': '50px',
						'text-align': 'center',
						cursor: 'pointer',
						margin: '0 auto',
						'border-radius': '8px',
					}),
				)
				.append(//Connect
					$('<div />').text(L.tr("Connect")).addClass('btn-primary').attr('id', 'conn_wifi').css({
						'margin-left': '20px',
						display: 'inline-block',
						width: '263px',
						height: '50px',
						'font-size': '16px',
						'line-height': '50px',
						'text-align': 'center',
						cursor: 'pointer',
						'border-radius': '8px',
					}),
				);
			$('#infobox').append(btn_group);
			$('#cancel').click(function () {
				$('#infobox').remove();
			});
			$('#conn_wifi').click(function () {
				if (!e) return;
				$('body').append(loadbox(L.tr("Connecting, please wait.")));
				p1 = 'radio' + $(info).prop('device');

				self.wifiConnect('radio' + $(info).prop('device'), $(info).find('.ssid').text(), $(info).prop('bssid'), $(info).find('.encryption').text(), $('#o_password_input').val());
				if($(info).prop('device') == '0') L.globals.ifname = 'wlan0-1';
				else if($(info).prop('device') == '1') L.globals.ifname = 'wlan1-1';
				y = 0;
				if_start_check = 0;
				var count = setInterval(function () { }, 10000);
				for (var cl = 1; cl <= count; cl++) {
					clearInterval(cl);
				}
				setTimeout(function () {
					var checkif = setInterval(function () {
						if_start_check = 1;
						clearInterval(checkif);
						setTimeout(function () {
							$('body').find('.loading').remove();
							$('body').append(loadbox(L.tr("Checking connection, please wait.")));
							ifwi1 = setInterval(function () {
								if (stop) return;
								checkWificon(undefined);
							}, 500);
						}, 2000);
					}, 500);
				}, 2000);
			});
		}

		// make sure wds_Enable() only run once
		var en_count = 0;
		function checkWificon(info) {
			self.wifi_status(L.globals.ifname).then(function (status) {
				y++;
				if(status == 'ok'){
					if(en_count == 0) {
						en_count = 1;
						stop = true;
						clearInterval(ifwi1);
						$('body').find('.loading').remove();
						$('body').append(loadbox(L.tr("Connect success, ready to start Wi-Fi Repeater. Please refresh page two minutes later. If you cannot return the now page means the Wi-Fi Repeater connect success.")));
						self.lan_ip().then(function(lan_ip){
							self.wds_Enable(L.globals.ifname, lan_ip);
						});
					}
				}
				else if (y > 39) {
					L.session.startHeartbeat();
					stop = true;
					clearInterval(ifwi1);
					var nexts = $('<div />')
						.addClass('PopWin')
						.attr('id', 'nexts')
						.append(
							$('<div />')
								.addClass('PopWin_box pop_absolute_center')
								.css('width', '500px')
								.css('margin', '-160px -250px')
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
												.text(L.tr("Connect failed, continue to select WiFi?")),
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
												.text(L.tr('Cancel'))//Cancel
												.click(function () {
													$('#nexts').remove();
													$('body').find('.loading').remove();
													$('body').append(loadbox(L.tr("Recovering settings, please wait.")));//Connection Fail.
													var count = setInterval(function () { }, 10000);
													for (var cl = 1; cl <= count; cl++) {
														clearInterval(cl);
													}
													self.wds_Disable().then(function () {
														setTimeout(function () {
															var q = setInterval(function () {
																self.lan_ip().then(function (data) {
																	if (data == lip || data == lip1) {
																		clearInterval(q);
																		setTimeout(function () {
																			L.session.startHeartbeat();
																			location.reload(true);
																		}, 2000);
																	}
																});
															}, 500);
														}, 5000);
													});
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
													$('body').find('.loading').remove();
													$('#nexts').remove();
													stop = false;
													scan();
												}),
										),
								),
						);
					$('body').find('.PopWin').remove();
					$('body').append(nexts);
				}
			});
		}
		function information(list) {
			var a = wifilist.find('.scroller_content');
			for (var i = 0; i < list.length; i++) {
				var tag;
				if (list[i].device == '1') tag = '5G';
				else if (list[i].device == '0') tag = '2.4G';
				var enc = list[i].encryption;
				if (enc.enabled == false) enc = 'Open';
				else if (enc.wpa) {
					if (enc.wpa.length ==2 && enc.wpa[0] == 1 && enc.wpa[1] == 2) {
						enc = 'WPA/WPA2(PSK) MIXED';
					} else if (enc.wpa[0] == 2) {
						enc = 'WPA2(PSK)';
					} else if (enc.wpa[0] == 1) {
						enc = 'WPA(PSK)';
					}
				}
				if (enc == list[i].encryption) continue;
				if (list[i].quality && list[i].quality_max) {
					var qu = (list[i].quality / list[i].quality_max) * 100;
					if (qu >= 65) {
						if (enc == 'Open') qu = 'open3';
						else qu = 'lock3';
					} else if (qu >= 35) {
						if (enc == 'Open') qu = 'open2';
						else qu = 'lock2';
					} else {
						if (enc == 'Open') qu = 'open1';
						else qu = 'lock1';
					}
				}
				a.append(
					$('<div />')
						.addClass('gscrollitem')
						.css('position', 'relative')
						.css('border-bottom', 'none')
						.prop('device', list[i].device)
						.prop('channel', list[i].channel)
						.prop('bssid', list[i].bssid)
						.append($('<div />').addClass('fl rlistconnect'))
						.append($('<div />').addClass('fl text_left_right').append($('<div />').addClass('ssid').css('margin-top', '10px').css('height', '20px').text(list[i].ssid)).append($('<div />').addClass('encryption').css('margin-top', '4px').text(enc)))
						.append(
							$('<div />')
								.addClass('fr')
								.css('margin-right', '20px')
								.append($('<div />').addClass('tag').css({
									'position': 'absolute',
									'top': '15px',
									'right': '60px',
									'line-height': '26px',
									'border': '1px solid',
									'border-radius': '12px',
									'width': '40px',
									'font-size': '12px'
								}).text(tag))
								.append(
									$('<div />')
										.addClass('wifi' + qu)
										.css('margin-top', '15px'),
								),
						),
				);
            }
		}
        function add_network() {
	    var e = true;
            var edits = $('<div />').attr('id', 'infoboxxx').css({
				padding: '30px 0px',
				'border-top': '1px solid #C9C9C9',
				'text-align': 'left',
			    });
            var wifi_mode = $('<div />')
                .css({
                    'margin-bottom': '30px',
					width: '650px',
					margin: '10px 200px ',
                })
                .append(
                    $('<div />')
						.addClass('InputComponent')
						.css('width', '300px')
						.append(
							$('<div />')
								.addClass('InputContent')
								.append($('<div />').text(L.tr("Wi-Fi Mode")))
								.append(
									$('<div />')
										.addClass('inputBox')
										.append(
											$('<select />')
												.attr('id', 'mode_input')
												.attr('autocomplete', 'off')
												.addClass('inputArea')
												.css({
													width: '200px',
													height: '37px',
													'text-align': 'left',
													border: '1px solid rgba(201,201,201,0.3)',
													'padding-left': '14px',
												})
												.attr('type', 'wifi-mode')
                                                .append($('<option />').text(L.tr("2.4G")))
                                                .append($('<option />').text(L.tr("5G"))),
										),
								),
						),
                );
            var wifi_name = $('<div />')
				.css({
					'margin-bottom': '30px',
					width: '650px',
					margin: '10px 200px ',
				})
				.append(
					$('<div />')
						.addClass('InputComponent')
						.css({'width': '300px',})
						.append(
							$('<div />')
								.addClass('InputContent')
								.append($('<div />').text(L.tr("Wi-Fi Name")))
								.append(
									$('<div />')
										.addClass('inputBox')
										.append(
											$('<input />')
												.attr('id', 'ssid_input')
												.attr('autocomplete', 'off')
												.addClass('inputArea')
												.css({
													width: '200px',
													height: '37px',
													'text-align': 'left',
													border: '1px solid rgba(201,201,201,0.3)',
													'padding-left': '14px',
												})
												.attr('type', 'wifi-name')
												.on("input propertychange", function () {
													$('#t3').remove();
													if($('#ssid_input').val().length < 1) {
														$('#ssid_input').after(
															$('<div />')
																.attr('id', 't3')
																.css({
																	color: 'red',
																	'font-size': '12px',
																})
																.text(L.tr("Field must not be empty")),
														);
														flag_ssid = false;
													} else {
														flag_ssid = true;
													}
												}),
                                                                                ),
								),
						),
				);
            var wifi_password = $('<div />')
                .css({
                    'margin-bottom': '30px',
					width: '650px',
					margin: '10px 200px ',
                })
                .append(
					$('<div />')
						.addClass('InputComponent')
						.css('width', '300px')
						.append(
							$('<div />')
								.addClass('InputContent')
								.append($('<div />').text(L.tr("Wi-Fi Password")))
								.append(
									$('<div />')
										.addClass('inputBox')
										.append(
											$('<input />')
												.attr('id', 'password_input')
												.attr('autocomplete', 'off')
												.addClass('inputArea')
												.css({
													width: '200px',
													height: '37px',
													'text-align': 'left',
													border: '1px solid rgba(201,201,201,0.3)',
													'padding-left': '14px',
												})
												.attr('type', 'password')
												.attr('readonly','readonly')
												.on("input propertychange", function () {
													$('#t4').remove();
													if($('#password_input').val().length < 1) {
														$('#password_input').after(
															$('<div />')
																.attr('id', 't4')
																.css({
																	color: 'red',
																	'font-size': '12px',
																})
																.text(L.tr("Field must not be empty")),
														);
														e = false;
													} else {
														e = true;
													}

												})
												.click(function(){
													if ( $('#encryption_input').val() != 'Open' ) {
														$('#password_input').removeProp("readonly");
													}
													else {
														$('#t4').remove();
														$('#password_input').attr('readonly','readonly');
														$('#password_input').val("");
													}
												}),
										)
										.append(
											$('<div />')
												.addClass('eye')
												.css('margin-left', '-70px')
												.append(
													$('<div />')
														.addClass('imgLayer eyeHidden')
														.mousedown(function () {
															$('#password_input').attr('type', 'text');
															$(this).prop('class', 'imgLayer eyeShow');
														})
														.mouseup(function () {
															$('#password_input').attr('type', 'password');
															$(this).prop('class', 'imgLayer eyeHidden');
														}),
												),
										),
								),
						),
				);
            var wifi_encryption = $('<div />')
                .css({
                    'margin-bottom': '30px',
                    width: '650px',
                    margin: '10px 200px ',
                })
                .append(
                    $('<div />')
						.addClass('InputComponent')
						.css({'width': '400px',
							'position': 'relative',
							'left': '-100px',})
						.append(
							$('<div />')
								.addClass('InputContent')
								.append($('<div />').text(L.tr("Wi-Fi encryption")))
								.append(
									$('<div />')
										.addClass('inputBox')
										.append(
											$('<select />')
												.attr('id', 'encryption_input')
												.attr('autocomplete', 'off')
												.addClass('inputArea')
												.css({
													width: '200px',
													height: '37px',
													'text-align': 'left',
													border: '1px solid rgba(201,201,201,0.3)',
													'padding-left': '14px',
												})
												.attr('type', 'wifi-encryption')
												.append($('<option />').val("Open").text(L.tr("Open")))
												.append($('<option />').val("WPA(PSK)").text(L.tr("WPA(PSK)")))
												.append($('<option />').val("WPA2(PSK)").text(L.tr("WPA2(PSK)")))
												.append($('<option />').val("WPA/WPA2(PSK) MIXED").text(L.tr("WPA/WPA2(PSK) MIXED"))),
										),
								),
						),
                );
			var btnss = $('<div />')
				.css({
					'margin-top': '14px',
					position: 'relative',
					width: '735px',
					'text-align': 'center',
				})
				.append(//Cancel
					$('<div />').text(L.tr("Cancel")).addClass('btn-primary').attr('id', 'cancel').css({
						display: 'inline-block',
						width: '263px',
						height: '50px',
						'font-size': '16px',
						'line-height': '50px',
						'text-align': 'center',
						cursor: 'pointer',
						margin: '0 auto',
						'border-radius': '8px',
					}),
				)
				.append(//Connect
					$('<div />').text(L.tr("Connect")).addClass('btn-primary').attr('id', 'conn_wifi').css({
						'margin-left': '20px',
						display: 'inline-block',
						width: '263px',
						height: '50px',
						'font-size': '16px',
						'line-height': '50px',
						'text-align': 'center',
						cursor: 'pointer',
						'border-radius': '8px',
					}),
				);
            $('#infobox').remove();
            $('#infoboxxx').remove();
            wifi_table.append(edits); // edits == infoboxxx
            wifi_table.find('#infoboxxx').append(wifi_mode);
            wifi_table.find('#infoboxxx').append(wifi_name);
            wifi_table.find('#infoboxxx').append(wifi_encryption);
            wifi_table.find('#infoboxxx').append(wifi_password);
            wifi_table.find('#infoboxxx').append(btnss);

			//default open
			wifi_password.hide();
			$(document).ready(function() {
				// Listen for change events in the selection box
				$('#encryption_input').on('change', function() {
				  // Gets the value selected by the user
				  var selectedValue = $(this).val();
				  // Perform a specific action based on the value selected by the user
				  if (selectedValue === 'Open') {
					// When the user selects the unencrypted option
					wifi_password.hide();
				  } else {
					// When the user selects the encryption option
					wifi_password.show();
				  }
				});
			  });
			$('#cancel').click(function () {
				$('#infoboxxx').remove();
			});
			$('#conn_wifi').click(function ()
			{
				if (!e) return;
				if (!flag_ssid) return;
				$('body').append(loadbox(L.tr("Connecting, please wait.")));
				if($('#encryption_input').val() != 'Open' ){
					self.wifiConnect_new( $('#ssid_input').val(), $('#encryption_input').val(), $('#password_input').val(), $('#mode_input').val());
				}
				else{
					self.wifiConnect_new( $('#ssid_input').val(), $('#encryption_input').val(), ' ', $('#mode_input').val());
				}

				if( $('#mode_input').val() == '2.4G') L.globals.ifname = 'wlan0-1';
				else if( $('#mode_input').val() == '5G') L.globals.ifname = 'wlan1-1';
				y = 0;
				if_start_check = 0;
				setTimeout(function () {
					var checkif = setInterval(function () {
						if_start_check = 1;
						clearInterval(checkif);
						setTimeout(function () {
							$('body').find('.loading').remove();
							$('body').append(loadbox(L.tr("Checking connection, please wait.")));
							ifwi1 = setInterval(function () {
								if (stop) return;
								checkWificon(undefined);
							}, 500);
						}, 2000);
					}, 500);
				}, 5000);
			});
        }

		function wds_on() {
			$('#wds_en').prop('class', 'switch_on');
			var status = $('<div />')
				.addClass('repeater_edit')
				.css({
					'margin-top': '90px',
					position: 'relative',
					'margin-left': '65px'
				})
				.append($('<div />').addClass('home_network').css('margin-top', '-3px'))
				.append(
					$('<div />').addClass('repeaterline').css({
						position: 'absolute',
						top: '30px',
						width: '100px',
						left: '120px',
					}),
				)
				.append($('<div />').addClass('home_wds_center').css({
					'margin-top': '-100px',
					'margin-left': '200px'
				}))
				.append(
					$('<div />').attr('id', 'res').addClass('repeater_fail').css({
						position: 'absolute',
						top: '30px',
						right: '115px',
					}),
				)
				.append($('<div />').addClass('wds_router').css('position', 'absolute').css('right', '-58px'));

			var start = $('<div />')
				.css({
					'margin-top': '14px',
					position: 'relative',
					width: '735px',
					'text-align': 'center',
				})
				.append($('<div />').addClass('fl').css('width', '245px').prop('align', 'center').text(L.tr("Internet")))//Internet
				.append($('<div />').addClass('fl').css('width', '240px').prop('align', 'center').text(L.tr("Superior Router")))//Superior Router
				.append($('<div />').addClass('fr').css('width', '240px').prop('align', 'center').text(L.tr("Local Router")))//Local Router
				.append($('<div />').addClass('fl').css('width', '245px').css('height', '1px'))
				.append(
					$('<div />')
						.addClass('fl')
						.css({
							width: '240px',
							height: '20px',
							'font-size': '12px',
							'white-space': 'pre-wrap',
						})
						.prop('align', 'center')
						.attr('id', 'ssid'),
				)
				.append(
					$('<div />')
						.addClass('fl')
						.attr('id', 'selfssid')
						.css({
							width: '240px',
							'font-size': '12px',
							'white-space': 'pre-wrap',
							'unicode-bidi': 'bidi-override',
							direction: 'ltr',
						})
						.prop('align', 'center'),
				)
				.append($('<div />').addClass('rinfo').css({
					'right': '165px',
					'font-size': '12px',
					'width': '146px',
					'min-height': '16px',
					'position': 'absolute',
					'word-break': 'break-all',
					'top': '-40px'
				}).attr('align', 'center').append($('<span />').attr('id', 'rqu')).append($('<br />')).append($('<span />').attr('id', 'rch')))
				.append(
					$('<div />').addClass('btn-primary').attr('id', 'next').css({
						display: 'inline-block',
						width: '263px',
						height: '50px',
						'font-size': '16px',
						'line-height': '50px',
						'text-align': 'center',
						cursor: 'pointer',
						margin: '0 auto',
						'border-radius': '8px',
					}),
				);

				self.lan_ip().then(function (data) {
				var d = L.uci.get('wireless', 'wds', 'device');
				if (d == 'radio0') {
					start.find('#selfssid').text('2.4G SSID: ' + L.uci.get('wireless', 'default_radio0', 'ssid'));
				} else if (d == 'radio1') {
					start.find('#selfssid').text('5G SSID: ' + L.uci.get('wireless', 'default_radio1', 'ssid'));
				} else if (d == undefined) {
					start.find('#selfssid').text('2.4G SSID: ' + L.uci.get('wireless', 'default_radio0', 'ssid') + '\n5G SSID: ' + L.uci.get('wireless', 'default_radio1', 'ssid'));
				}
				proto = L.uci.get('network', 'lan', 'proto');
				if (data != '0' && proto != 'static') {
					status.find('#res').prop('class', 'repeater_sucess');
					start.find('#ssid').text('SSID: ' + L.uci.get('wireless', 'wds', 'ssid'));
					if (d == 'radio0') {
						start.find('#rch').text(L.tr("Channel: ") + L.uci.get('wireless', 'radio0', 'channel'));
					} else if (d == 'radio1') {
						start.find('#rch').text(L.tr("Channel: ") + L.uci.get('wireless', 'radio1', 'channel'));
					}
					//TODO 根据装置不同显示信号强度
					start.find('#next').remove();
				} else start.find('#next').text(L.tr("Next"));//Next
				if (!$('#repeater_edit').length && !$('#selfssid').length)
					$('.subject').append(status).append(start);
			});
		}

		function checkWifi() {
			wi = L.uci.get('wireless', 'wds', 'ssid');
			if (wi == undefined) {
				return false;
			} else {
				return true;
			}
		}
		function checkWDS() {
			proto = L.uci.get('network', 'lan', 'proto');
			self.lan_ip().then(function (data) {
				if (proto == 'dhcp' && data != '0') {
					wds_on();
				} else {
					$('#wds_en').prop('class', 'switch_off');
					if (checkWifi()) {
						$('body').append(loadbox(L.tr("Checking connection, please wait.")));
						p1 = L.uci.get('wireless', 'wds', 'device');
						p2 = L.uci.get('wireless', p1, 'origin_channel');
						ifwifi = setInterval(function () {
							checkWificon(undefined);
						}, 500);
					}
				}
			});
		}
		return 0;
	},
});
