L.ui.view.extend({
	execute: function() {
		var self = this;
		return L.ui.getAvailableACLs().then(function(acls) {
			var m = new L.cbi.Map('rpcd', {
				caption:     L.tr('Change the login password'),
				description: L.tr('Manage user account passwords and access to the WebUI interface'),
				readonly:    !self.options.acls.users
			});

			var s = m.section(L.cbi.DummySection, '__login', {
//				caption:      L.tr('Accounts'),
			});

			var opassword = s.option(L.cbi.PasswordValue, 'opassword', {
				caption:     L.tr('Original Password'),
				optional:    true,
			});

			var password = s.option(L.cbi.PasswordValue, 'password', {
				caption:     L.tr('New Password'),
				optional:    true,
				datatype:    'rangelength(5,12)'
			});

			var repassword = s.option(L.cbi.PasswordValue, 'repassword', {
				caption:     L.tr('Confirmation'),
				optional:    true,
				datatype:     function(str, sid) {
					var password = '' + $('#field_rpcd___login___login_password').val()
					var val = '' + str;

					for(var i = 0; i < val.length; i ++)
					{
						var c = val.substr(i, 1);
						var ts = escape(c);
						if(ts.substring(0, 2) == "%u") {
							return L.tr('Must void Chinese characters');
						}
					}

					if (val.length >= 5 && val.length <= 12) {
						if(password == val) {
							return true;
						} else {
							return L.tr("Two password inputs must be consistent")
						}
					} else {
						return L.tr('Must be between 5 and 12 characters');
					}
				}
			});

			password.ucivalue = function(sid) {
				return '';
			}
			password.toggle = function(sid) {
				var id = '#' + this.id(sid);
				this.callSuper('toggle', sid);
			};

			password.save = function(sid) {
				var real_sid;
				var rpcd_users = L.uci.sections('rpcd');
				for(var i = 0; i < rpcd_users.length; i++) {
					if(L.uci.get('rpcd', rpcd_users[i]['.name'], 'username') == 'root') {
						real_sid = rpcd_users[i]['.name'];
						break;
					}
				}

				var pw = password.formvalue(sid);
				var repw = repassword.formvalue(sid);
				var opw = opassword.formvalue(sid);

				var pw_crypt = L.uci.get('rpcd', real_sid, 'password');
				var opw_crypt = ''
				return L.ui.cryptPassword(opw).then(function(crypt) {
					opw_crypt = crypt
					console.log(pw_crypt)
					console.log(opw_crypt)
					if(pw == undefined){
						L.ui.setting(false);
						L.ui.dialog(L.tr('Tips'), L.tr('Please input a password'), {
							style: 'close'
						});
						return
					}else if(repw == undefined){
						L.ui.setting(false);
						L.ui.dialog(L.tr('Tips'), L.tr('Please enter the confirmation password'), {
							style: 'close'
						});
						return
					}else if(repw != pw){
						L.ui.setting(false);
						L.ui.dialog(L.tr('Tips'), [
							$('<p />').text(L.tr('Two password inputs must be consistent'))
						], { style: 'close' });
						return
					}

					if (!pw || !real_sid) {
						L.ui.setting(false);
					return;
					}

					if (pw_crypt == opw_crypt) {
						return L.ui.cryptPassword(pw).then(function(crypt) {
							L.uci.set('rpcd', real_sid, 'password', crypt);
						});
					} else {
						L.ui.setting(false);
						L.ui.dialog(L.tr('Tips'), L.tr('Original password is wrong!'), {
							style: 'close'
						});
						return
					}
				})
			};

			m.on('save', function() {
				L.uci.changes().then(function(changes) {
					self.relogin = true;
				});
			});

			m.on('apply', function() {
				if (self.relogin) {
					location.replace('/');
					return;
				}
			});

			return m.insertInto('#map');
		});
	}
});
