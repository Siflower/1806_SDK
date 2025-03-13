L.ui.view.extend({
	    do_cmd: L.rpc.declare({
        object: 'web.advance',
        method: 'cmd',
        params: [ 'cmd' ],
    }),

    execute: function() {
        var self = this;
        var m = new L.cbi.Map('wireless', {
            caption: L.tr("WifiSetting"),
            collabsible: true
        });

        var s = m.section(L.cbi.NamedSection, 'guest_radio0', {
            caption: L.tr("2.4G")
        });

        var e = s.option(L.cbi.CheckboxValue, 'disabled', {
            caption: L.tr("Whether to disable guest network"),
            optional: true
        });

        e = s.option(L.cbi.InputValue, 'ssid', {
            caption: L.tr("Wireless name"),
			datatype:     'ssid'
        });

        e = s.option(L.cbi.ListValue, 'encryption', {
                caption: L.tr("Encryption method"),
                initial: 'none'
            })
            .value('none', L.tr('No encryption'))
            .value('psk+ccmp', L.tr('WPA Personal (PSK)'))
            .value('psk2+ccmp', L.tr('WPA2 Personal (PSK)'))
            .value('psk-mixed', L.tr('WPA/WPA2 Personal (PSK) mixed'));

        e = s.option(L.cbi.PasswordValue, 'key', {
            caption: L.tr("Wireless password"),
            datatype: 'wpakey'
        }).depends('encryption', function(v) {
            return (v != 'none');
        });

        e = s.option(L.cbi.ListValue, 'network', {
                caption: L.tr("Whether to allow visitors to access intranet resources"),
                initial: 'guest'
            })
            .value('guest', L.tr("no"))
            .value('lan', L.tr("yes"));

        e = s.option(L.cbi.InputValue, 'limitupload', {
            caption: L.tr("Maximum upload speed of guest network"),
            datatype: 'uinteger',
            initial: '0',
            description: L.tr("KB/s (0 means no limit)")
        });

        e.save = function(sid) {
            var upload_value = this.formvalue(sid);
            L.uci.set('wireless', 'guest_radio0', 'limitupload', upload_value);
            L.uci.save();
        }

        e = s.option(L.cbi.InputValue, 'limitdownload', {
            caption: L.tr("Maximum download speed of guest network"),
            datatype: 'uinteger',
            initial: '0',
            description: L.tr("KB/s (0 means no limit)")
        });

        e.save = function(sid) {
            var download_value = this.formvalue(sid);
            L.uci.set('wireless', 'guest_radio0', 'limitdownload', download_value);
            L.uci.save();
        }

        e = s.option(L.cbi.ListValue, 'limittime', {
                caption: L.tr("Whether to set the guest network opening time"),
                initial: 'false'
            })
            .value('false', L.tr("no"))
            .value('true', L.tr("yes"));

        e.save = function(sid) {
            var limittime_value = this.formvalue(sid);
            L.uci.set('wireless', 'guest_radio0', 'limittime', limittime_value);
            L.uci.set('wireless', 'guest_radio0', 'limittimetype', 0);
            L.uci.save();
        }

        e = s.option(L.cbi.InputValue, 'remainingtime', {
            caption: L.tr("Opening hours"),
            initial: '1',
            datatype: 'range(1,24)',
            description: L.tr("(unit: h)Turn off the guest network when the specified time is reached")
        }).depends('limittime', function(v) {
            return (v != 'false');
        });

        e.save = function(sid) {
            var remainingtime_value = this.formvalue(sid);
            L.uci.set('wireless', 'guest_radio0', 'remainingtime', remainingtime_value);
            L.uci.save();
        }

        var s2 = m.section(L.cbi.NamedSection, 'guest_radio1', {
            caption: L.tr("5G")
        });

        var e2 = s2.option(L.cbi.CheckboxValue, 'disabled', {
            caption: L.tr("Whether to disable guest network"),
            optional: true
        });

        e2 = s2.option(L.cbi.InputValue, 'ssid', {
            caption: L.tr("Wireless name"),
			datatype:     'ssid'
        });

        e2 = s2.option(L.cbi.ListValue, 'encryption', {
                caption: L.tr("Encryption method"),
                initial: 'none'
            })
            .value('none', L.tr('No encryption'))
            .value('psk+ccmp', L.tr('WPA Personal (PSK)'))
            .value('psk2+ccmp', L.tr('WPA2 Personal (PSK)'))
            .value('psk-mixed', L.tr('WPA/WPA2 Personal (PSK) mixed'));

        e2 = s2.option(L.cbi.PasswordValue, 'key', {
            caption: L.tr("Wireless password"),
            datatype: 'wpakey'
        }).depends('encryption', function(v) {
            return (v != 'none');
        });


        e2 = s2.option(L.cbi.ListValue, 'network', {
                caption: L.tr("Whether to allow visitors to access intranet resources"),
                initial: 'guest'
            })
            .value('guest', L.tr("no"))
            .value('lan', L.tr("yes"));


        e2 = s2.option(L.cbi.InputValue, 'limitupload', {
            caption: L.tr("Maximum upload speed of guest network"),
            datatype: 'uinteger',
            initial: '0',
            description: L.tr("KB/s (0 means no limit)")
        });

        e2.save = function(sid) {
            var upload_value = this.formvalue(sid);
            L.uci.set('wireless', 'guest_radio1', 'limitupload', upload_value);
            L.uci.save();
        }

        e2 = s2.option(L.cbi.InputValue, 'limitdownload', {
            caption: L.tr("Maximum download speed of guest network"),
            datatype: 'uinteger',
            initial: '0',
            description: L.tr("KB/s (0 means no limit)")
        });

        e2.save = function(sid) {
            var download_value = this.formvalue(sid);
            L.uci.set('wireless', 'guest_radio1', 'limitdownload', download_value);
            L.uci.save();
        }

        e2 = s2.option(L.cbi.ListValue, 'limittime', {
                caption: L.tr("Whether to set the guest network opening time"),
                initial: 'false'
            })
            .value('false', L.tr("no"))
            .value('true', L.tr("yes"));

        e2.save = function(sid) {
            var limittime_value = this.formvalue(sid);
            L.uci.set('wireless', 'guest_radio1', 'limittime', limittime_value);
            L.uci.set('wireless', 'guest_radio1', 'limittimetype', 0);
            L.uci.save();
        }

        e2 = s2.option(L.cbi.InputValue, 'remainingtime', {
            caption: L.tr("Opening hours"),
            initial: '1',
            datatype: 'range(1,24)',
            description: L.tr("(unit: h)Turn off the guest network when the specified time is reached")
        }).depends('limittime', function(v) {
            return (v != 'false');
        });

        e2.save = function(sid) {
            var remainingtime_value = this.formvalue(sid);
            L.uci.set('wireless', 'guest_radio1', 'remainingtime', remainingtime_value);
            L.uci.save();
        }

        m.on('apply',function(){self.do_cmd("sleep 1; env -i; ubus call network reload ; wifi reload_legacy; sleep 3; gwifi start; gwifi speed_rst 0;gwifi speed_rst 1")});
        m.insertInto('#map');
    }
});
