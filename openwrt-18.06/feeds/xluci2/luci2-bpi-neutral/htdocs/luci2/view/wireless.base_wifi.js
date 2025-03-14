L.ui.view.extend({
	execute: function() {
		var self = this;
		var m = new L.cbi.Map('wireless', {
			caption:     L.tr('WifiSetting'),
			collabsible: true
		});

		var s = m.section(L.cbi.NamedSection, 'default_radio0', {
			caption:      L.tr('2.4G')
		});

		var e = s.option(L.cbi.CheckboxValue, 'disabled', {
			caption:      L.tr('禁用 wifi'),
			optional:     true
		});

		e = s.option(L.cbi.InputValue, 'ssid', {
			caption:      L.tr('无线名称'),
			datatype:     'ssid'
		});

		e = s.option(L.cbi.ListValue, 'encryption', {
			caption:	L.tr('加密方式'),
			initial:	'none'
		})
		.value('none', L.tr('No encryption'))
		.value('psk+ccmp', L.tr('WPA Personal (PSK)'))
		.value('psk2+ccmp', L.tr('WPA2 Personal (PSK)'))
		.value('psk-mixed', L.tr('WPA/WPA2 Personal (PSK) mixed'));

		e = s.option(L.cbi.PasswordValue, 'key', {
			caption:    L.tr('密码'),
			datatype:   'wpakey'
		}).depends('encryption', function(v) {
			return (v != 'none');
		});

		var s2 = m.section(L.cbi.NamedSection, 'default_radio1', {
			caption:      L.tr('5G')
		});

		var e2 = s2.option(L.cbi.CheckboxValue, 'disabled', {
			caption:      L.tr('禁用 wifi'),
			optional:     true
		});

		e2 = s2.option(L.cbi.InputValue, 'ssid', {
			caption:      L.tr('无线名称'),
			datatype:     'ssid'
		});

		e2 = s2.option(L.cbi.ListValue, 'encryption', {
			caption:	L.tr('加密方式'),
			initial:	'none'
		})
		.value('none', L.tr('No encryption'))
		.value('psk+ccmp', L.tr('WPA Personal (PSK)'))
		.value('psk2+ccmp', L.tr('WPA2 Personal (PSK)'))
		.value('psk-mixed', L.tr('WPA/WPA2 Personal (PSK) mixed'));

		e2 = s2.option(L.cbi.PasswordValue, 'key', {
			caption:    L.tr('密码'),
			datatype:   'wpakey'
		}).depends('encryption', function(v) {
			return (v != 'none');
		});

		m.insertInto('#map');
	}
});
