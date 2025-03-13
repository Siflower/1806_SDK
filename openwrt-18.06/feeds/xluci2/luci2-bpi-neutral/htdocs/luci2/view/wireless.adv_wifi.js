L.ui.view.extend({
	wlan_channel_info: L.rpc.declare({
		object: 'web.wireless',
		method: 'get_channel_info',
		expect: { 'info': {} }
	}),

	wlan_htmodelist: L.rpc.declare({
		object: 'web.wireless',
		method: 'get_htmodelist',
		expect: { 'htmodelist': {} }
	}),

	set_txpower_lvl: L.rpc.declare({
		object: 'web.wireless',
		method: 'set_txpower_lvl',
		params: ['radio', 'txpower_lvl'],
	}),

	execute: function () {
		var self = this;
		var tasks = [];

		tasks.push(self.wlan_channel_info());
		tasks.push(self.wlan_htmodelist());

		Promise.all(tasks).then(function (data) {

			var m = new L.cbi.Map('wireless', {
				caption: L.tr('Advanced WiFi'),
				collabsible: true
			});

			var s1 = m.section(L.cbi.NamedSection, 'radio0', {
				caption: L.tr('2.4G'),
				teasers: ['_hidden', 'txpower', '_channel', '_hwmode', '_htmode1', '_htmode2']
			});

			var e = s1.option(L.cbi.CheckboxValue, '_hidden', {
				caption: L.tr('Start Broadcast')
			});

			e.ucivalue = function (sid) {
				var hidden = L.uci.get('wireless', 'default_radio0', 'hidden');
				if (hidden == '0') {
					return true;
				}
				else {
					return false;
				}
			};

			e.save = function (sid) {
				var val = Number(this.formvalue(sid));
				if (val == 1) {
					L.uci.set('wireless', 'default_radio0', 'hidden', '0');
				}
				else {
					L.uci.set('wireless', 'default_radio0', 'hidden', '1');
				}
			};

			var e1 = s1.option(L.cbi.ListValue, 'txpower', {
				caption: L.tr('WiFi power'),
				initial: '20'
			})
				.value('20', L.tr('high'))
				.value('10', L.tr('mid'))
				.value('5', L.tr('low'));

			e1.save = function (sid) {
				var val = Number(this.formvalue(sid));
				if (val == 5) {
					L.uci.set('wireless', 'radio0', 'txpower', '5');
					L.uci.set('wireless', 'radio0', 'txpower_lvl', '0');
					self.set_txpower_lvl("radio0","0");
				}
				else if (val == 10) {
					L.uci.set('wireless', 'radio0', 'txpower', '10');
					L.uci.set('wireless', 'radio0', 'txpower_lvl', '1');
					self.set_txpower_lvl("radio0","1");
				}
				else {
					L.uci.set('wireless', 'radio0', 'txpower', '20');
					L.uci.set('wireless', 'radio0', 'txpower_lvl', '2');
					self.set_txpower_lvl("radio0","2");
				}
			};

			e1 = s1.option(L.cbi.ListValue, '_channel', {
				caption: L.tr('Channel'),
				datatype: 'channel_24g'
			})
				.value('auto', L.tr('Automatic'))
				.value('1', L.tr('1'))
				.value('2', L.tr('2'))
				.value('3', L.tr('3'))
				.value('4', L.tr('4'))
				.value('5', L.tr('5'))
				.value('6', L.tr('6'))
				.value('7', L.tr('7'))
				.value('8', L.tr('8'))
				.value('9', L.tr('9'))
				.value('10', L.tr('10'))
				.value('11', L.tr('11'))
				.value('12', L.tr('12'))
				.value('13', L.tr('13'));

			e1.ucivalue = function (sid) {
				if (!data[0].channel_24g)
					data[0].channel_24g = L.uci.get('wireless', 'radio0', 'channel');
				return data[0].channel_24g;
			}

			e1.save = function (sid) {
				var channel_24g = this.formvalue(sid);
				L.uci.set('wireless', 'radio0', 'channel', channel_24g);
				L.uci.save();
			}

			var e2 = s1.option(L.cbi.ListValue, '_hwmode', {
				caption: L.tr('Mode'),
				initial: '11n'
			})
				.value('11n', L.tr('11n/g/b'))
				.value('11g', L.tr('11g/b'))
				.value('11b', L.tr('11b'));

			e2.ucivalue = function (sid) {
				var hwmode = L.uci.get('wireless', 'radio0', 'hwmode');
				var htmode = L.uci.get('wireless', 'radio0', 'htmode');
				if (hwmode == '11b') {
					return '11b';
				}
				else if (htmode == 'HT20' || htmode == 'HT40' || htmode == 'VHT20' || htmode == 'VHT40') {
					return '11n';
				}
				else {
					return '11g';
				}
			};

			e2.save = function (sid) {
				var val = this.formvalue(sid);
				if (val == '11n') {
					L.uci.set('wireless', 'radio0', 'hwmode', '11g');
				}
				else if (val == '11g') {
					L.uci.unset('wireless', 'radio0', 'ht_coex');
					L.uci.unset('wireless', 'radio0', 'htmode');
					L.uci.set('wireless', 'radio0', 'hwmode', '11g');
				}
				else {
					L.uci.unset('wireless', 'radio0', 'ht_coex');
					L.uci.unset('wireless', 'radio0', 'htmode');
					L.uci.set('wireless', 'radio0', 'hwmode', '11b');
				}
				L.uci.save();
			};

			var e3 = s1.option(L.cbi.ListValue, '_htmode1', {
				caption: L.tr('Band Width'),
				initial: 'HT20'
			})
				.value('HT20', L.tr('20MHZ'))
				.value('HT40', L.tr('40MHZ'))
				.value('auto', L.tr('Automatic'))
				.depends('_hwmode', '11n');

			e3.ucivalue = function (sid) {
				var htmode = L.uci.get('wireless', 'radio0', 'htmode');
				var ht_coex = L.uci.get('wireless', 'radio0', 'ht_coex');
				if (ht_coex == '1') {
					return 'auto';
				}
				else if (htmode == 'VHT20') {
					htmode = 'HT20';
				}
				else if (htmode == 'VHT40') {
					htmode = 'HT40';
				}
				else if (htmode != 'VHT20' && htmode != 'VHT40') {
					htmode = 'HT20';
				}
				return htmode;
			};

			e3.save = function (sid) {
				var val = this.formvalue(sid);
				var val2 = e2.formvalue(sid);
				if (val2 != '11n') {
					L.uci.unset('wireless', 'radio0', 'htmode');
					L.uci.unset('wireless', 'radio0', 'ht_coex');
					L.uci.set('wireless', 'radio0', 'noscan', '0');
				}
				else if (val2 == '11n' && val == 'HT20') {
					L.uci.set('wireless', 'radio0', 'htmode', 'VHT20');
					L.uci.set('wireless', 'radio0', 'ht_coex', '0');
					L.uci.set('wireless', 'radio0', 'noscan', '0');
				}
				else if (val2 == '11n' && val == 'HT40') {
					L.uci.set('wireless', 'radio0', 'htmode', 'VHT40');
					L.uci.set('wireless', 'radio0', 'ht_coex', '0');
					L.uci.set('wireless', 'radio0', 'noscan', '1');
				}
				else if (val2 == '11n' && val == 'auto') {
					L.uci.set('wireless', 'radio0', 'htmode', 'VHT40');
					L.uci.set('wireless', 'radio0', 'ht_coex', '1');
					L.uci.set('wireless', 'radio0', 'noscan', '0');
				}
				L.uci.save();
			};

			var s2 = m.section(L.cbi.NamedSection, 'radio1', {
				caption: L.tr('5G'),
				teasers: ['_hidden', 'txpower', '_channel', '_hwmode', '_htmode1', '_htmode2']
			});

			var e5 = s2.option(L.cbi.CheckboxValue, '_hidden', {
				caption: L.tr('Start Broadcast')
			});

			e5.ucivalue = function (sid) {
				var hidden = L.uci.get('wireless', 'default_radio1', 'hidden');
				if (hidden == '0') {
					return true;
				}
				else {
					return false;
				}
			};

			e5.save = function (sid) {
				var val = Number(this.formvalue(sid));
				if (val == 1) {
					L.uci.set('wireless', 'default_radio1', 'hidden', '0');
				}
				else {
					L.uci.set('wireless', 'default_radio1', 'hidden', '1');
				}
			};

			var e6 = s2.option(L.cbi.ListValue, 'txpower', {
				caption: L.tr('WiFi power'),
				initial: '25'
			})
				.value('25', L.tr('high'))
				.value('15', L.tr('mid'))
				.value('5', L.tr('low'));

			e6.save = function (sid) {
				var val = Number(this.formvalue(sid));
				if (val == 5) {
					L.uci.set('wireless', 'radio1', 'txpower', '5');
					L.uci.set('wireless', 'radio1', 'txpower_lvl', '0');
					self.set_txpower_lvl("radio1","0");
				}
				else if (val == 15) {
					L.uci.set('wireless', 'radio1', 'txpower', '15');
					L.uci.set('wireless', 'radio1', 'txpower_lvl', '1');
					self.set_txpower_lvl("radio1","1");
				}
				else {
					L.uci.set('wireless', 'radio1', 'txpower', '25');
					L.uci.set('wireless', 'radio1', 'txpower_lvl', '2');
					self.set_txpower_lvl("radio1","2");
				}
			};

			e6 = s2.option(L.cbi.ListValue, '_channel', {
				caption: L.tr('Channel'),
				datatype: 'channel_5g'
			})
				.value('auto', L.tr('Automatic'))
				.value('36', L.tr('36'))
				.value('40', L.tr('40'))
				.value('44', L.tr('44'))
				.value('48', L.tr('48'))
				.value('52', L.tr('52'))
				.value('56', L.tr('56'))
				.value('60', L.tr('60'))
				.value('64', L.tr('64'))
				.value('149', L.tr('149'))
				.value('153', L.tr('153'))
				.value('157', L.tr('157'))
				.value('161', L.tr('161'))
				.value('165', L.tr('165'));

			e6.ucivalue = function (sid) {
				if (!data[0].channel_5g)
					data[0].channel_5g = L.uci.get('wireless', 'radio1', 'channel');
				return data[0].channel_5g;
			}

			e6.save = function (sid) {
				var channel_5g = this.formvalue(sid);
				L.uci.set('wireless', 'radio1', 'channel', channel_5g);
				L.uci.save();
			}

			var e7 = s2.option(L.cbi.ListValue, '_hwmode', {
				caption: L.tr('Mode'),
				initial: '11ac'
			})
				.value('11ac', L.tr('11ac'))
				.value('11a', L.tr('11a only'))
				.value('11n', L.tr('11n only'));

			e7.ucivalue = function (sid) {
				var htmode = L.uci.get('wireless', 'radio1', 'htmode');
				if (htmode == 'HT20' || htmode == 'HT40') {
					return '11n';
				}
				else if (htmode == 'VHT20' || htmode == 'VHT40' || htmode == 'VHT80') {
					return '11ac';
				}
				else {
					return '11a';
				}
			};

			e7.save = function (sid) {
				var val = this.formvalue(sid);
				if (val == '11ac') {
					L.uci.set('wireless', 'radio1', 'hwmode', '11a');
				}
				else if (val == '11a') {
					L.uci.unset('wireless', 'radio1', 'htmode');
					L.uci.set('wireless', 'radio1', 'hwmode', '11a');
				}
				else {
					L.uci.set('wireless', 'radio1', 'hwmode', '11a');
				}
				L.uci.save();
			};

			var e8 = s2.option(L.cbi.ListValue, '_htmode1', {
				caption: L.tr('Band Width'),
			})
				.value('VHT20', L.tr('20MHZ'))
				.value('VHT40', L.tr('40MHZ'))
				.value('VHT80', L.tr('80MHZ'))
				.depends('_hwmode', '11ac');

			e8.ucivalue = function (sid) {
				var htmode = L.uci.get('wireless', 'radio1', 'htmode');
				if (htmode != 'VHT20' && htmode != 'VHT40' && htmode != 'VHT80') htmode = 'VHT80';
				return htmode;
			};

			e8.save = function (sid) {
				var val = this.formvalue(sid);
				var val2 = e7.formvalue(sid);
				if (val2 == '11a') {
					L.uci.unset('wireless', 'radio1', 'htmode');
				}
				else if (val2 == '11ac') {
					if (val == 'VHT20') {
						L.uci.set('wireless', 'radio1', 'htmode', 'VHT20');
					}
					else if (val == 'VHT40') {
						L.uci.set('wireless', 'radio1', 'htmode', 'VHT40');
					}
					else {
						L.uci.set('wireless', 'radio1', 'htmode', 'VHT80');
					}
				}
				L.uci.save();
			};

			var e9 = s2.option(L.cbi.ListValue, '_htmode2', {
				caption: L.tr('Band Width'),
			})
				.value('HT20', L.tr('20MHZ'))
				.value('HT40', L.tr('40MHZ'))
				.depends('_hwmode', '11n');

			e9.ucivalue = function (sid) {
				var htmode = L.uci.get('wireless', 'radio1', 'htmode');
				if (htmode != 'HT20' && htmode != 'HT40') htmode = 'HT20';
				return htmode;
			};

			e9.save = function (sid) {
				var val = this.formvalue(sid);
				var val2 = e7.formvalue(sid);
				if (val2 == '11n') {
					if (val == 'HT20') {
						L.uci.set('wireless', 'radio1', 'htmode', 'HT20');
					}
					else {
						L.uci.set('wireless', 'radio1', 'htmode', 'HT40');
					}
				}
				L.uci.save();
			};
			m.insertInto('#map').then(function () {
				if ($('#field_wireless_radio1_radio1__channel').val() == '165') {
					$('#field_wireless_radio1_radio1__htmode2').val('HT20');
					$('#field_wireless_radio1_radio1__htmode1').val('VHT20');
					$('#field_wireless_radio1_radio1__htmode2').find('[value="HT40"]').css('display', 'none');
					$('#field_wireless_radio1_radio1__htmode1').find('[value="VHT40"]').css('display', 'none');
					$('#field_wireless_radio1_radio1__htmode1').find('[value="VHT80"]').css('display', 'none');
				} else {
					$('#field_wireless_radio1_radio1__htmode2').find('[value="HT40"]').css('display', '');
					$('#field_wireless_radio1_radio1__htmode1').find('[value="VHT40"]').css('display', '');
					$('#field_wireless_radio1_radio1__htmode1').find('[value="VHT80"]').css('display', '');
				}
				$('#field_wireless_radio1_radio1__channel').click(function () {
					if ($('#field_wireless_radio1_radio1__channel').val() == '165') {
						$('#field_wireless_radio1_radio1__htmode2').val('HT20');
						$('#field_wireless_radio1_radio1__htmode1').val('VHT20');
						$('#field_wireless_radio1_radio1__htmode2').find('[value="HT40"]').css('display', 'none');
						$('#field_wireless_radio1_radio1__htmode1').find('[value="VHT40"]').css('display', 'none');
						$('#field_wireless_radio1_radio1__htmode1').find('[value="VHT80"]').css('display', 'none');
					} else {
						$('#field_wireless_radio1_radio1__htmode2').find('[value="HT40"]').css('display', '');
						$('#field_wireless_radio1_radio1__htmode1').find('[value="VHT40"]').css('display', '');
						$('#field_wireless_radio1_radio1__htmode1').find('[value="VHT80"]').css('display', '');
					}
				});
			});
		});
	}
});
