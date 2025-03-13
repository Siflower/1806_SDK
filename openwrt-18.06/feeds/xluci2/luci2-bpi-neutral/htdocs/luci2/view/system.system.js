L.ui.view.extend({

	do_cmd: L.rpc.declare({
        object: 'web.advance',
        method: 'cmd',
        params: [ 'cmd' ]
    }),

	execute: function() {
		var self = this;

		var m = new L.cbi.Map('system', {
			caption:     L.tr('Date / Language'),
			description: L.tr('Here you can configure the basic aspects of your device like its hostname or the timezone.'),
			collabsible: true
		});

		var s = m.section(L.cbi.TypedSection, 'system', {
//			caption:     L.tr('System Properties'),
			teasers:     [ 'hostname', 'zonename', 'languages', 'themes' ],
			readonly:    !this.options.acls.system
		});

		var t = s.option(L.cbi.DummyValue, '__time', {
			caption:     L.tr('Local Time')
		});

		t.load = function(sid)
		{
			var id = this.id(sid);
			var uci = this.ucipath(sid);
			self.timezone = m.get(uci.config, uci.section, 'timezone');

			if (self.interval)
				return;

			L.system.getLocaltime().then(function(info) {
				var date = new Date();
				date.setFullYear(info.year);
				date.setMonth(info.mon);
				date.setDate(info.day);
				date.setHours(info.hour);
				date.setMinutes(info.min);
				date.setSeconds(info.sec);

				self.time = Math.floor(date.getTime() / 1000);

				self.interval = window.setInterval(function() {
					date.setTime(++(self.time) * 1000);
					$('#' + id).text('%04d/%02d/%02d %02d:%02d:%02d %s'.format(
						date.getFullYear(),
						date.getMonth() + 1,
						date.getDate(),
						date.getHours(),
						date.getMinutes(),
						date.getSeconds(),
						self.timezone
					));
				}, 1000);
			});
		};

		var z = s.option(L.cbi.ListValue, 'zonename', {
			caption:	L.tr('Timezone'),
			initial:	'UTC'
		});

		z.load = function(sid) {
			z.value('UTC');

			return $.getJSON(L.globals.resource + '/zoneinfo.json').then(function(zones) {
				var znames = [ ];

				for (var i = 0; i < zones.length; i++)
					for (var j = 5; j < zones[i].length; j++)
						znames.push(zones[i][j]);

				znames.sort();

				for (var i = 0; i < znames.length; i++)
					z.value(znames[i]);

				z.zones = zones;
			});
		};

		z.save = function(sid)
		{
			var uci = this.ucipath(sid);
			var val = this.formvalue(sid);

			if (!this.callSuper('save', sid))
				return false;

			for (var i = 0; i < z.zones.length; i++)
				for (var j = 5; j < z.zones[i].length; j++)
					if (z.zones[i][j] == val)
					{
						m.set(uci.config, uci.section, 'timezone', z.zones[i][0]);
						return true;
					}

			m.set(uci.config, uci.section, 'timezone', 'UTC');
			return true;
		};

		var lang_sid;
		var l = s.option(L.cbi.ListValue, 'languages', {
			caption:     L.tr('Language'),
			uci_package: 'luci2',
			uci_section: 'main',
			uci_option:  'lang',
			initial:    'zh_cn'
		});

		l.load = function(sid)
		{
			lang_sid = sid;
			this.choices = [];

			var langs = m.get('luci2', 'languages');
			for (var key in langs)
				if (key.charAt(0) != '.')
					this.value(key, langs[key]);
		};

		m.on('save', function() {
			var lang = l.formvalue(lang_sid);
			if(lang == "en")
				self.do_cmd("echo en > /www/luci2/template/lang");
			else if(lang == "zh_cn")
				self.do_cmd("echo zh > /www/luci2/template/lang");
			else if(lang == "de")
				cmd = "echo de > /www/luci2/template/lang"
			else if(lang == "es")
				cmd = "echo es > /www/luci2/template/lang"
			else if(lang == "fr")
				cmd = "echo fr > /www/luci2/template/lang"
			else if(lang == "it")
				cmd = "echo it > /www/luci2/template/lang"

			L.uci.changes().then(function(changes) {
				if (changes.luci2)
					self.relogin = true;
			});
		});

		m.on('apply', function() {
			if (self.relogin) {
				//location.href = '/';
				//don't need to relogin
				location.reload();
				return;
			}
			L.ui.setting(1, L.tr('Freshing Time...Please wait.'));

			L.system.getLocaltime().then(function(info) {
				var date = new Date();
				date.setFullYear(info.year);
				date.setMonth(info.mon);
				date.setDate(info.day);
				date.setHours(info.hour);
				date.setMinutes(info.min);
				date.setSeconds(info.sec);

				self.time = Math.floor(date.getTime() / 1000);
			})
			var delay = setTimeout(function () {
				location.reload(true);
				clearTimeout(delay);
			}, 2500);
		});

		return m.insertInto('#map');
	}
});
