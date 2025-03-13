L.ui.view.extend({

	callSet: L.rpc.declare({
		object: 'uci',
		method: 'set',
		params: ['config', 'section', 'values']
	}),

	callLoad: L.rpc.declare({
		object: 'uci',
		method: 'get',
		params: ['config', 'section', 'option'],
	}),

	callCommit: L.rpc.declare({
		object: 'uci',
		method: 'commit',
		params: ['config']
	}),

	callShow: L.rpc.declare({
		object: 'uci',
		method: 'get',
		params: ['config']
	}),

	callAdd: L.rpc.declare({
		object: 'uci',
		method: 'add',
		params: ['config', 'type', 'name', 'values']
	}),

	callDel: L.rpc.declare({
		object: 'uci',
		method: 'delete',
		params: ['config', 'section', 'option']
	}),

	wifi_control_open: function () {
		var self = this;
		self.callLoad('wireless', 'default_radio0', 'macfilter').then(function (value) {
			if (value.value != undefined) {
				$('#wifi_control').addClass('switch_on').removeClass('switch_off');
				$('#wifi_control').prop('status', '0');
				$('#type').show();
			}
			else {
				$('#wifi_control').addClass('switch_off').removeClass('switch_on');
				$('#wifi_control').prop('status', '1');
				$('#type').hide();
				$('#table').hide();
			}
		});
		$('#wifi_control').click(function () {
			var status = $('#wifi_control').prop('status');
			if (status == 1) {
				$('#wifi_control').addClass('switch_on').removeClass('switch_off');
				self.callSet('wireless', 'default_radio0', { 'macfilter': 'deny' });
				self.callSet('wireless', 'default_radio1', { 'macfilter': 'deny' });
				$('#wifi_control').prop('status', '0');
				$('#black_list_check').prop('status', '1');
				$('#black_list_checkbox').prop('class', 'checkbox_press');
				$('#white_list_check').prop('status', '0');
				$('#white_list_checkbox').prop('class', 'checkbox');
				$('#type').show();
				$('#table').show();
				$('#black_list_table').show();
				$('#white_list_table').hide();
				self.add_black_list();
				self.get_black_list();
			}
			else {
				var delPromises = [
					self.callDel('wireless', 'default_radio0', 'macfilter'),
					self.callDel('wireless', 'default_radio0', 'maclist'),
					self.callDel('wireless', 'default_radio1', 'macfilter'),
					self.callDel('wireless', 'default_radio1', 'maclist')
				];
				Promise.all(delPromises).then(function() {
					L.uci.callCommit('wireless').then(function () {
						L.ui.setting(true);
						setTimeout(function () {
							$('#wifi_control').addClass('switch_off').removeClass('switch_on');
							$('#wifi_control').prop('status', '1');
							$('#type').hide();
							$('#table').hide();
							L.ui.setting(false);
						}, 1000 * 10);
					})
				})
			}
		});
	},

	wifi_control_type: function () {
		var self = this;
		self.callLoad('wireless', 'default_radio0', 'macfilter').then(function (value) {
			if (value.value == 'allow') {
				$('#white_list_check').prop('status', '1');
				$('#white_list_checkbox').prop('class', 'checkbox_press');
				$('#black_list_checkbox').prop('class', 'checkbox');
				$('#black_list_check').prop('status', '0');
				$('#table').show();
				$('#white_list_table').show();
				$('#black_list_table').hide();
				self.get_white_list();
				self.add_white_list();
			}
			else if (value.value == 'deny') {
				$('#black_list_check').prop('status', '1');
				$('#black_list_checkbox').prop('class', 'checkbox_press');
				$('#white_list_checkbox').prop('class', 'checkbox');
				$('#white_list_check').prop('status', '0');
				$('#table').show();
				$('#black_list_table').show();
				$('#white_list_table').hide();
				self.get_black_list();
				self.add_black_list();
			}
		});
		$('#white_list_check').click(function () {
			var status = $('#white_list_check').prop('status');
			if (status == 0) {
				$('#white_list_checkbox').prop('class', 'checkbox_press');
				$('#black_list_checkbox').prop('class', 'checkbox');
				$('#white_list_check').prop('status', '1');
				$('#black_list_check').prop('status', '0');
				$('#table').show();
				$('#white_list_table').show();
				$('#black_list_table').hide();
				var promises = [
					self.callSet('wireless', 'default_radio0', { 'macfilter': 'allow' }),
					self.callDel('wireless', 'default_radio0', 'maclist'),
					self.callSet('wireless', 'default_radio1', { 'macfilter': 'allow' }),
					self.callDel('wireless', 'default_radio1', 'maclist')
				];
				Promise.all(promises).then(function() {
					L.uci.callCommit('wireless').then(function () {
						L.ui.setting(true);
						setTimeout(function () {
							self.get_white_list();
							self.add_white_list();
							L.ui.setting(false);
						}, 1000 * 10);
					})
				})
			}
		});
		$('#black_list_check').click(function () {
			var status = $('#black_list_check').prop('status');
			if (status == 0) {
				$('#black_list_checkbox').prop('class', 'checkbox_press');
				$('#white_list_checkbox').prop('class', 'checkbox');
				$('#black_list_check').prop('status', '1');
				$('#white_list_check').prop('status', '0');
				$('#table').show();
				$('#black_list_table').show();
				$('#white_list_table').hide();
				var promises = [
					self.callSet('wireless', 'default_radio0', { 'macfilter': 'deny' }),
					self.callDel('wireless', 'default_radio0', 'maclist'),
					self.callSet('wireless', 'default_radio1', { 'macfilter': 'deny' }),
					self.callDel('wireless', 'default_radio1', 'maclist'),
				];
				Promise.all(promises).then(function() {
					L.uci.callCommit('wireless').then(function () {
						L.ui.setting(true);
						setTimeout(function () {
							self.get_black_list();
							self.add_black_list();
							L.ui.setting(false);
						}, 1000 * 10);
					})
				})
			}
		});
	},

	add_black_list: function () {
		var self = this;
		$('#add_black_list_btn').click(function () {
			$('#table').find('.PopWin').remove();
			$('#table').append($('<div />').addClass('PopWin').attr('id', 'popwin')
				.append($('<div />').addClass('PopWin_box pop_absolute_center').css({ 'width': '700px', 'margin': '-241px -350px' })
					.append($('<div />').addClass('popWin_title table_cell').css('width', '700px').text(L.tr('Add blacklist')))
					.append($('<div />').addClass('PopWin_content').attr('id', 'content')
						.append($('<div />').css('margin-top', '30px')
							.append($('<div />').addClass('row InputContent')
								.append($('<div />').css('font-size', '15px').text(L.tr('Select device')))
								.append($('<div />').css('width', '263px').attr('id', 'mac')
									.append($('<div />').addClass('select_box border_1px paddingleft_14').css({ 'height': '37px', 'line-height': '37px' }).attr('id', 'select_device')
										.append($('<input />').attr('id', 'device_select').addClass('inputArea paddingright_14').val(L.tr('Manual set')).css({ 'border': 'none', 'width': '200px', 'height': '30px', 'outline': 'none' }).prop('readonly', true))
										.append(self.get_device_list())))))
						.append($('<div />').css('margin-top', '20px').attr('id', 'mac_input')
							.append($('<div />').addClass('InputComponent')
								.append($('<div />').addClass('InputContent row')
									.append($('<div />').text(L.tr('MAC address')))
									.append($('<div />').attr('id', 'macaddr').addClass('inputBox')
										.append($('<input />').addClass('inputArea paddingleft_14').attr('id', 'macaddr_input').css({ 'width': '243px', 'height': '37px', 'text-align': 'left', 'border': '1px solid rgb(201,201,201)', 'padding-right': '14px', 'outline': 'none' }))))
								.append($('<div />').addClass('row InputDesc')
									.append($('<div />').addClass('marginleft_215').text(L.tr('MAC address format:XX:XX:XX:XX:XX:XX')))))))
					.append($('<div />').css('margin-top', '20px').attr('id', 'input')
						.append($('<div />').addClass('InputComponent')
							.append($('<div />').addClass('InputContent row')
								.append($('<div />').text(L.tr('hostname')))
								.append($('<div />').addClass('inputBox')
									.append($('<input />').addClass('inputArea paddingleft_14').attr('id', 'hostname_input').css({ 'width': '243px', 'height': '37px', 'text-align': 'left', 'border': '1px solid rgb(201,201,201)', 'padding-right': '14px', 'outline': 'none' }))))))
					.append($('<div />').addClass('PopWin_buttons').css('width', '386px')
						.append($('<div />').addClass('button fl button_common_short').attr('id', 'calcelipserver').text(L.tr('Cancel')).css('margin', '7px'))
						.append($('<div />').addClass('button fl button_common_short').attr('id', 'postserver').text(L.tr('Ok')).css('margin', '7px')))));
			$('#calcelipserver').click(function () {
				$('#popwin').remove();
			});
			$('#postserver').click(function () {
				var macaddr_list = [];
				if (L.globals.flag == 0) {
					var macaddr = $('#macaddr_input').val().split(':');
					if (macaddr.length != 6) {
						window.alert(L.tr('Please input the true macaddr!'));
						return false;
					}
					for (var i = 0; i < 6; i++) {
						var tmp = parseInt(macaddr[i], 16);
						if (isNaN(tmp)) {
							window.alert(L.tr('Please input the true macaddr!'));
							return false;
						}
						else if (tmp < 0 || tmp > 255) {
							window.alert(L.tr('Please input the true macaddr!'));
							return false;
						}
					}
					macaddr_list[0] = $('#macaddr_input').val().split(':').join('_');
					var hostname = $('#hostname_input').val();
					if (hostname == undefined)
						hostname = 'anonymous';
					self.callAdd('wldevlist', 'device', macaddr_list[0], { 'mac': macaddr_list[0], 'hostname': hostname });
					self.callCommit('wldevlist');
					L.globals.flag = 1;
				}
				else if (L.globals.flag == 1) {
					macaddr_list[0] = $('#device_select').val();
					L.globals.flag = 0;
				}
				self.set_black_list(macaddr_list)
				$('#popwin').remove();
			});
		});
	},

	add_white_list: function () {
		var self = this;
		$('#add_white_list_btn').click(function () {
			$('#table').find('.PopWin').remove();
			$('#table').append($('<div />').addClass('PopWin').attr('id', 'popwin')
				.append($('<div />').addClass('PopWin_box pop_absolute_center').css({ 'width': '700px', 'margin': '-241px -350px' })
					.append($('<div />').addClass('popWin_title table_cell').css('width', '700px').text(L.tr('Add whitelist')))
					.append($('<div />').addClass('PopWin_content').attr('id', 'content')
						.append($('<div />').css('margin-top', '30px')
							.append($('<div />').addClass('row InputContent')
								.append($('<div />').css('font-size', '15px').text(L.tr('Select device')))
								.append($('<div />').css('width', '263px').attr('id', 'mac')
									.append($('<div />').addClass('select_box border_1px paddingleft_14').css({ 'height': '37px', 'line-height': '37px' }).attr('id', 'select_device')
										.append($('<input />').attr('id', 'device_select').addClass('inputArea paddingright_14').val(L.tr('Manual set')).css({ 'border': 'none', 'width': '200px', 'height': '30px', 'outline': 'none' }).prop('readonly', true))
										.append(self.get_device_list())))))
						.append($('<div />').css('margin-top', '20px').attr('id', 'mac_input')
							.append($('<div />').addClass('InputComponent')
								.append($('<div />').addClass('InputContent row')
									.append($('<div />').text(L.tr('MAC address')))
									.append($('<div />').attr('id', 'macaddr').addClass('inputBox')
										.append($('<input />').addClass('inputArea paddingleft_14').attr('id', 'macaddr_input').css({ 'width': '243px', 'height': '37px', 'text-align': 'left', 'border': '1px solid rgb(201,201,201)', 'padding-right': '14px', 'outline': 'none' }))))
								.append($('<div />').addClass('row InputDesc')
									.append($('<div />').addClass('marginleft_215').text(L.tr('MAC address format:XX:XX:XX:XX:XX:XX')))))))
					.append($('<div />').css('margin-top', '20px').attr('id', 'input')
						.append($('<div />').addClass('InputComponent')
							.append($('<div />').addClass('InputContent row')
								.append($('<div />').text(L.tr('hostname')))
								.append($('<div />').addClass('inputBox')
									.append($('<input />').addClass('inputArea paddingleft_14').attr('id', 'hostname_input').css({ 'width': '243px', 'height': '37px', 'text-align': 'left', 'border': '1px solid rgb(201,201,201)', 'padding-right': '14px', 'outline': 'none' }))))))
					.append($('<div />').addClass('PopWin_buttons').css('width', '386px')
						.append($('<div />').addClass('button fl button_common_short').attr('id', 'calcelipserver').text(L.tr('Cancel')).css('margin', '7px'))
						.append($('<div />').addClass('button fl button_common_short').attr('id', 'postserver').text(L.tr('Ok')).css('margin', '7px')))));
			$('#calcelipserver').click(function () {
				$('#popwin').remove();
			});
			$('#postserver').click(function () {
				var macaddr_list = [];
				if (L.globals.flag == 0) {
					var macaddr = $('#macaddr_input').val().split(':');
					if (macaddr.length != 6) {
						window.alert(L.tr('Please input the true macaddr!'));
						return false;
					}
					for (var i = 0; i < 6; i++) {
						var tmp = parseInt(macaddr[i], 16);
						if (isNaN(tmp)) {
							window.alert(L.tr('Please input the true macaddr!'));
							return false;
						}
						else if (tmp < 0 || tmp > 255) {
							window.alert(L.tr('Please input the true macaddr!'));
							return false;
						}
					}
					macaddr_list[0] = $('#macaddr_input').val().split(':').join('_');
					var hostname = $('#hostname_input').val();
					if (hostname == undefined)
						hostname = 'anonymous';
					self.callAdd('wldevlist', 'device', macaddr_list[0], { 'mac': macaddr_list[0], 'hostname': hostname });
					self.callCommit('wldevlist');
					L.globals.flag = 1;
				}
				else if (L.globals.flag == 1) {
					macaddr_list[0] = $('#device_select').val();
					L.globals.flag = 0;
				}
				self.set_white_list(macaddr_list)
				$('#popwin').remove();
			});
		});
	},

	get_device_list: function () {
		var self = this;
		L.globals.flag = 0;
		self.callShow('wldevlist').then(function (value) {
			var select_list = [];
			for (var i in value) {
				var obj = value[i];
				for (var j in obj) {
					select_list.push({
						hostname: obj[j].hostname,
						macaddr: obj[j].mac,
					});
				}
			}
			for (var i = 0; i < select_list.length; i++) {
				if (select_list[i].hostname == undefined) {
					select_list[i].hostname = "anonymous";
				}
				select_list[i].macaddr = select_list[i].macaddr.split('_').join(':');
			}
			$('#select_device').find('.list_fixed').remove();
			$('#select_device').append($('<div />').addClass('list_fixed border_1px').attr('id', 'device_list').css({ 'max-height': '222px', 'height': '222px', 'display': 'none' })
				.append($('<ul />').attr('id', 'select_list').addClass('select_list').css({ 'margin-right': '-12px', 'padding-right': '0px', 'height': '100%', 'overflow': 'auto' })
					.append($('<li />').attr('id', 'select_input').addClass('paddingleft_10 paddingright_15 active').text(L.tr('Manual set')))))
			for (var i = 0; i < select_list.length; i++) {
				$('#select_list').append($('<li />').attr('id', select_list[i].macaddr).addClass('paddingleft_10 paddingright_15 active').text(select_list[i].macaddr));
			}
			$('#device_select').blur(function () {
				var text = $('#device_select').val();
				if (text != L.tr('Manual set')) {
					$('#mac_input').hide();
				}
				$('#device_list').hide();
			});
			$('#device_select').mousedown(function () {
				L.globals.flag = 0;
				$('#device_list').show();
				$('#mac_input').show();
				$('ul#select_list').on("mousedown", "li", function () {
					$('#device_list').hide();
					if (this.id == 'select_input') {
						L.globals.flag = 0;
						$('#device_select').val(L.tr('Manual set'));
						$('#mac_input').show();
						$('#macaddr_input').val('');
						$('#hostname_input').val('');
					}
					else {
						$('#device_select').val(this.id);
						$('#mac_input').hide();
						for (var i = 0; i < select_list.length; i++) {
							if (select_list[i].macaddr == this.id)
								$('#hostname_input').val(select_list[i].hostname);
						}
						L.globals.flag = 1;
					}
				});
			});
		})
	},

	set_black_list: function (macaddr_list) {
		var self = this;
		macaddr_list[0] = macaddr_list[0].split("_").join(":");
		self.callLoad('wireless', 'default_radio0', 'maclist').then(function (values) {
			var black_list = [];
			if (values.value != undefined) {
				for (var i = 0; i < values.value.length; i++) {
					black_list[i] = values.value[i];
				}
				black_list.push(macaddr_list[0]);
				macaddr_list = [];
				for (var i = 0; i < black_list.length; i++) {
					if (macaddr_list.indexOf(black_list[i]) < 0) {
						macaddr_list.push(black_list[i]);
					}
				}
			}
			self.callSet('wireless', 'default_radio0', { 'macfilter': 'deny', 'maclist': macaddr_list });
			self.callSet('wireless', 'default_radio1', { 'macfilter': 'deny', 'maclist': macaddr_list });
			self.callCommit('wireless').then(function () {
				L.ui.setting(true);
				setTimeout(function () {
					// L.file.exec('ifup', ['lan']);
					self.get_black_list();
					L.ui.setting(false);
				}, 1000 * 10);
			})
		});
	},

	set_white_list: function (macaddr_list) {
		var self = this;
		macaddr_list[0] = macaddr_list[0].split("_").join(":");
		self.callLoad('wireless', 'default_radio0', 'maclist').then(function (values) {
			var white_list = [];
			if (values.value != undefined) {
				for (var i = 0; i < values.value.length; i++) {
					white_list[i] = values.value[i];
				}
				white_list.push(macaddr_list[0]);
				macaddr_list = [];
				for (var i = 0; i < white_list.length; i++) {
					if (macaddr_list.indexOf(white_list[i]) < 0) {
						macaddr_list.push(white_list[i]);
					}
				}
			}
			self.callSet('wireless', 'default_radio0', { 'macfilter': 'allow', 'maclist': macaddr_list });
			self.callSet('wireless', 'default_radio1', { 'macfilter': 'allow', 'maclist': macaddr_list });
			self.callCommit('wireless').then(function () {
				L.ui.setting(true);
				setTimeout(function () {
					// L.file.exec('ifup', ['lan']);
					self.get_white_list();
					L.ui.setting(false);
				}, 1000 * 10);
			})
		});
	},

	get_black_list: function () {
		var self = this;
		self.callLoad('wireless', 'default_radio0', 'macfilter').then(function (values) {
			if (values.value == 'deny') {
				L.uci.callLoad('wireless').then(function(data){
				console.log(data)
				self.callLoad('wireless', 'default_radio0', 'maclist').then(function (values) {
					var black_list = [];
					if (values.value != undefined) {
						for (var i = 0; i < values.value.length; i++) {
							black_list.push({
								hostname: "anonymous",
								macaddr: values.value[i]
							});
						}
						for (var i = 0; i < black_list.length; i++) {
							black_list[i].macaddr = black_list[i].macaddr.split('_').join(':');
						}
						self.callShow('wldevlist').then(function (value) {
							var select_list = [];
							for (var i in value) {
								var obj = value[i];
								for (var j in obj) {
									select_list.push({
										hostname: obj[j].hostname,
										macaddr: obj[j].mac,
									});
								}
							}
							for (var i = 0; i < select_list.length; i++) {
								if (select_list[i].hostname == undefined) {
									select_list[i].hostname = "anonymous";
								}
								select_list[i].macaddr = select_list[i].macaddr.split('_').join(':');
							}
							for (var i = 0; i < select_list.length; i++) {
								for (var j = 0; j < black_list.length; j++) {
									if (select_list[i].macaddr == black_list[j].macaddr)
										black_list[j].hostname = select_list[i].hostname;
								}
							}
							$('#black_list_table').find('.listtable').remove();
							for (var i = 0; i < black_list.length; i++) {
								$('#black_list_table').append($('<div />')
									.append($('<div >').addClass('border_top listtable height_70 text_center')
										.append($('<div />').addClass('pull-left border_1px_right height_70').css('width', '240px')
											.append($('<input />').css({ 'margin-top': '25px', 'text-align': 'center', 'border': 'none', 'outline': 'none', 'width': '200px' }).val(black_list[i].hostname).prop('readonly', 'true')))
										.append($('<div />').addClass('pull-left border_1px_right height_70').css('width', '238px')
											.append($('<input />').css({ 'margin-top': '25px', 'text-align': 'center', 'border': 'none', 'outline': 'none', 'width': '200px' }).val(black_list[i].macaddr).prop('readonly', 'true')))
										.append($('<div />').addClass('pull-left border_1px_right height_70').css('width', '100px')
											.append($('<div />').addClass('rateButton edit_big').css({ 'margin-top': '15px', 'margin-right': '10px' }).click(function () {
												$(this).parent().prev().prev().children().removeProp('readonly').css({ 'border': '1px solid rgb(201,201,201', 'width': '150px', 'height': '30px', 'padding-right': '14px', 'border-radius': '8px', 'margin-top': '18px' });
												$(this).parent().prev().children().removeProp('readonly').css({ 'border': '1px solid rgb(201,201,201', 'width': '150px', 'height': '30px', 'padding-right': '14px', 'border-radius': '8px', 'margin-top': '18px' });
												var mac = $(this).parent().prev().children().val().split(':').join('_');
												var mac_old = $(this).parent().prev().children().val();
												$(this).parent().prev().prev().children().blur(function () {
													$(this).css('border', 'none').prop('readonly', 'true');
													var hostname = $(this).val();
													self.callSet('wldevlist', mac, { 'hostname': hostname });
													self.callCommit('wldevlist');
													L.ui.setting(true);
													setTimeout(function () {
														L.ui.setting(false);
													}, 1000);
												});
												$(this).parent().prev().children().blur(function () {
													$(this).css('border', 'none').prop('readonly', 'true');
													var mac_modify = $(this).val();
													self.callSet('wldevlist', mac_modify, { 'mac': mac_modify });
													self.callCommit('wldevlist');
													self.callLoad('wireless', 'default_radio0', 'maclist').then(function (values) {
														var mac_list = values.value;
														for (var i = 0; i < mac_list.length; i++) {
															if (mac_list[i] == mac_old) {
																mac_list[i] = mac_modify;
															}
														}
														self.callSet('wireless', 'default_radio0', { 'maclist': mac_list });
														self.callSet('wireless', 'default_radio1', { 'maclist': mac_list });
														self.callCommit('wireless');
														L.ui.setting(true);
														setTimeout(function () {
															L.ui.setting(false);
														}, 1000);
													});
												});
											})))
										.append($('<div />').addClass('pull-left border_1px_right height_70').css({ 'width': '100px', 'border': 'none' })
											.append($('<div />').addClass('ic-del').css({ 'margin-top': '20px', 'margin-left': '33px' }).click(function () {
												var mac_tmp = $(this).parent().prev().prev().children().val();
												var mac0 = "wireless.default_radio0.maclist=" + mac_tmp;
												var mac1 = "wireless.default_radio1.maclist=" + mac_tmp;
												L.file.exec('uci', ['del_list', mac0]);
												L.file.exec('uci', ['del_list', mac1]);
												L.file.exec('uci', ['commit']).then(function () {
													L.file.exec('ifup', ['lan']);
												})
												L.ui.setting(true);
												setTimeout(function () {
													L.ui.setting(false);
													self.get_black_list();
												}, 1000 * 10);
											})))))
							}
						})
					}
					else {
						$('#black_list_table').find('.listtable').remove();
					}
				})
				})
			}
			else {
				$('#black_list_table').find('.listtable').remove();
			}
		})
	},

	get_white_list: function () {
		var self = this;
		self.callLoad('wireless', 'default_radio0', 'macfilter').then(function (values) {
			if (values.value == 'allow') {
				self.callLoad('wireless', 'default_radio0', 'maclist').then(function (values) {
					var white_list = [];
					if (values.value != undefined) {
						for (var i = 0; i < values.value.length; i++) {
							white_list.push({
								hostname: "anonymous",
								macaddr: values.value[i]
							});
						}
						for (var i = 0; i < white_list.length; i++) {
							white_list[i].macaddr = white_list[i].macaddr.split('_').join(':');
						}
						self.callShow('wldevlist').then(function (value) {
							var select_list = [];
							for (var i in value) {
								var obj = value[i];
								for (var j in obj) {
									select_list.push({
										hostname: obj[j].hostname,
										macaddr: obj[j].mac,
									});
								}
							}
							for (var i = 0; i < select_list.length; i++) {
								if (select_list[i].hostname == undefined) {
									select_list[i].hostname = "anonymous";
								}
								select_list[i].macaddr = select_list[i].macaddr.split('_').join(':');
							}
							for (var i = 0; i < select_list.length; i++) {
								for (var j = 0; j < white_list.length; j++) {
									if (select_list[i].macaddr == white_list[j].macaddr)
										white_list[j].hostname = select_list[i].hostname;
								}
							}
							$('#white_list_table').find('.listtable').remove();
							for (var i = 0; i < white_list.length; i++) {
								$('#white_list_table').append($('<div />')
									.append($('<div >').addClass('border_top listtable height_70 text_center')
										.append($('<div />').addClass('pull-left border_1px_right height_70').css('width', '240px')
											.append($('<input />').css({ 'margin-top': '25px', 'text-align': 'center', 'border': 'none', 'outline': 'none', 'width': '200px' }).val(white_list[i].hostname).prop('readonly', 'true')))
										.append($('<div />').addClass('pull-left border_1px_right height_70').css('width', '238px')
											.append($('<input />').css({ 'margin-top': '25px', 'text-align': 'center', 'border': 'none', 'outline': 'none', 'width':'200px' }).val(white_list[i].macaddr).prop('readonly', 'true')))
										.append($('<div />').addClass('pull-left border_1px_right height_70').css('width', '100px')
											.append($('<div />').addClass('rateButton edit_big').css({ 'margin-top': '15px', 'margin-right': '10px' }).click(function () {
												$(this).parent().prev().prev().children().removeProp('readonly').css({ 'border': '1px solid rgb(201,201,201', 'width': '150px', 'height': '30px', 'padding-right': '14px', 'border-radius': '8px', 'margin-top': '18px' });
												$(this).parent().prev().children().removeProp('readonly').css({ 'border': '1px solid rgb(201,201,201', 'width': '150px', 'height': '30px', 'padding-right': '14px', 'border-radius': '8px', 'margin-top': '18px' });
												var mac = $(this).parent().prev().children().val().split(':').join('_');
												var mac_old = $(this).parent().prev().children().val();
												$(this).parent().prev().prev().children().blur(function () {
													$(this).css('border', 'none').prop('readonly', 'true');
													var hostname = $(this).val();
													self.callSet('wldevlist', mac, { 'hostname': hostname });
													self.callCommit('wldevlist');
													L.ui.setting(true);
													setTimeout(function () {
														L.ui.setting(false);
													}, 1000);
												});
												$(this).parent().prev().children().blur(function () {
													$(this).css('border', 'none').prop('readonly', 'true');
													var mac_modify = $(this).val();
													self.callSet('wldevlist', mac_modify, { 'mac': mac_modify });
													self.callCommit('wldevlist');
													self.callLoad('wireless', 'default_radio0', 'maclist').then(function (values) {
														var mac_list = values.value;
														for (var i = 0; i < mac_list.length; i++) {
															if (mac_list[i] == mac_old) {
																mac_list[i] = mac_modify;
															}
														}
														self.callSet('wireless', 'default_radio0', { 'maclist': mac_list });
														self.callSet('wireless', 'default_radio1', { 'maclist': mac_list });
														self.callCommit('wireless');
														L.ui.setting(true);
														setTimeout(function () {
															L.ui.setting(false);
														}, 1000 * 10);
													});
												});
											})))
										.append($('<div />').addClass('pull-left border_1px_right height_70').css({ 'width': '100px', 'border': 'none' })
											.append($('<div />').addClass('ic-del').css({ 'margin-top': '20px', 'margin-left': '33px' }).click(function () {
												var mac_tmp = $(this).parent().prev().prev().children().val();
												var mac0 = "wireless.default_radio0.maclist=" + mac_tmp;
												var mac1 = "wireless.default_radio1.maclist=" + mac_tmp;
												L.file.exec('uci', ['del_list', mac0]);
												L.file.exec('uci', ['del_list', mac1]);
												L.file.exec('uci', ['commit']).then(function () {
													L.file.exec('ifup', ['lan']);
												})
												L.ui.setting(true);
												setTimeout(function () {
													L.ui.setting(false);
													self.get_white_list();
												}, 1000 * 10);
											})))))
							}
						})
					}
					else {
						$('#white_list_table').find('.listtable').remove();
					}
				})
			}
			else {
				$('#white_list_table').find('.listtable').remove();
			}
		})
	},

	execute: function () {
		var self = this;
		self.wifi_control_open();
		self.wifi_control_type();
	}
});
