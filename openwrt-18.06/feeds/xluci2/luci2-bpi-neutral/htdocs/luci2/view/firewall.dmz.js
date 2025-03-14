L.ui.view.extend({

    execute: function() {
        var self = this;
        var m = new L.cbi.Map('basic_setting', {
            caption: L.tr('DMZsetting'),
            collabsible: true
        });

        var s = m.section(L.cbi.DummySection, '__dmz', {
//            caption: L.tr('DMZsetting'),
        });

        var enable = s.option(L.cbi.CheckboxValue, 'enable', {
            caption: L.tr('enable DMZ'),
            optional: true
        });

        enable.ucivalue = function(sid) {
            var enable_value = L.uci.get("basic_setting", "dmz", "enable");
            if (enable_value == 1) {
                return true;
            } else {
                return false;
            }
        };

        enable.save = function(sid) {
            var enable_value = enable.formvalue(sid);
            L.uci.add('basic_setting', 'setting', 'dmz');
            L.uci.set('basic_setting', 'dmz', 'enable', enable_value);
            L.uci.save();
        };

        var host = s.option(L.cbi.InputValue, 'host', {
            caption: L.tr('DMZip'),
	    datatype: 'ip4addr'
        }).depends('enable', function(v) {
            return (v != 0);
        });

        host.ucivalue = function(sid) {
            var dmz_value = L.uci.get("basic_setting", "dmz", "host");
            if (dmz_value) {
                return dmz_value;
            } else {
                return "";
            }
        };


        host.save = function(sid) {
            var enable_value = enable.formvalue(sid);
            var dmz_value = host.formvalue(sid);
            if (Number(enable_value) == 1) {
                L.uci.set('basic_setting', 'dmz', 'host', dmz_value);
                L.uci.add('firewall', 'redirect', 'dmz');
                L.uci.set('firewall', 'dmz', 'dest_ip', dmz_value);
                L.uci.set('firewall', 'dmz', 'src', 'wan');
                L.uci.set('firewall', 'dmz', 'proto', 'all');
                L.uci.save();
            }

            if (Number(enable_value) == 0) {
                L.uci.remove('firewall', 'dmz');
            }

            L.uci.save();
        }

        m.insertInto('#dmzmap');
    }
});
