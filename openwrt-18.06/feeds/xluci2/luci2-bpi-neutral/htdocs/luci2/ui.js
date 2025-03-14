(function() {
	var ui_class = {
		saveScrollTop: function()
		{
			this._scroll_top = $(document).scrollTop();
		},

		restoreScrollTop: function()
		{
			if (typeof(this._scroll_top) == 'undefined')
				return;

			$(document).scrollTop(this._scroll_top);

			delete this._scroll_top;
		},

		setting: function (enable, msg) {
			if (!msg)
				msg = L.tr('Saving configuration...');
			if (enable == false) {
				$('body').find('.loading').remove();
				return;
			}
			var set = $('<div />')
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
			$('body').append(set);
		},

		loading: function(enable, msg)
		{
			var win = $(window);
			var body = $('body');
			if (!msg)
				msg = L.tr('Loading data…');

			var state = this._loading || (this._loading = {
				modal: $('<div />')
					.css('z-index', 2000)
					.addClass('modal fade')
					.append($('<div />')
						.addClass('modal-dialog')
						.append($('<div />')
							.addClass('modal-content luci2-modal-loader')
							.append($('<div />')
								.addClass('modal-body'))))
					.appendTo(body)
					.modal({
						backdrop: 'static',
						keyboard: false
					})
			});

			state.modal.find('.modal-body').text(msg)

			state.modal.modal(enable ? 'show' : 'hide');
		},

		dialog: function(title, content, options)
		{
			var win = $(window);
			var body = $('body');
			var self = this;

			var state = this._dialog || (this._dialog = {
				dialog: $('<div />')
					.addClass('modal fade')
					.append($('<div />')
						.addClass('modal-dialog')
						.append($('<div />')
							.addClass('modal-content')
							.append($('<div />')
								.addClass('modal-header')
								.append('<h4 />')
									.addClass('modal-title'))
							.append($('<div />')
								.addClass('modal-body'))
							.append($('<div />')
								.addClass('modal-footer')
								.append(self.button(L.tr('Close'), 'primary')
									.click(function() {
										$(this).parents('div.modal').modal('hide');
									})))))
					.appendTo(body)
			});

			if (typeof(options) != 'object')
				options = { };

			if (title === false)
			{
				state.dialog.modal('hide');

				return state.dialog;
			}

			var cnt = state.dialog.children().children().children('div.modal-body');
			var ftr = state.dialog.children().children().children('div.modal-footer');

			ftr.empty().show();

			if (options.style == 'confirm')
			{
				ftr.append(L.ui.button(L.tr('Ok'), 'primary')
					.click(options.confirm || function() { L.ui.dialog(false) }));

				ftr.append(L.ui.button(L.tr('Cancel'), 'default')
					.click(options.cancel || function() { L.ui.dialog(false) }));
			}
			else if (options.style == 'close')
			{
				ftr.append(L.ui.button(L.tr('Close'), 'primary')
					.click(options.close || function() { L.ui.dialog(false) }));
			}
			else if (options.style == 'wait')
			{
				ftr.append(L.ui.button(L.tr('Close'), 'primary')
					.attr('disabled', true));
			}else if (options.style == 'onlyConfirm'){
				ftr.append(L.ui.button(L.tr('Ok'), 'primary')
					.click(options.confirm || function() { L.ui.dialog(false) }));
			}

			if (options.wide)
			{
				state.dialog.addClass('wide');
			}
			else
			{
				state.dialog.removeClass('wide');
			}

			state.dialog.find('h4:first').text(title);

			var data = state.dialog.data('bs.modal');
			if (data) {
				data.options.backdrop = options.backdrop;
				data.options.keyboard = options.keyboard;
				state.dialog.modal('show');
			} else {
				state.dialog.modal({
					backdrop: options.backdrop,
					keyboard: options.keyboard
				});
			}

			cnt.empty().append(content);

			return state.dialog;
		},

		upload: function(title, content, options)
		{
			var state = L.ui._upload || (L.ui._upload = {
				form: $('<form />')
					.attr('method', 'post')
					.attr('action', '/cgi-bin/luci-upload')
					.attr('enctype', 'multipart/form-data')
					.attr('target', 'cbi-fileupload-frame')
					.append($('<p />'))
					.append($('<input />')
						.attr('type', 'hidden')
						.attr('name', 'sessionid'))
					.append($('<input />')
						.attr('type', 'hidden')
						.attr('name', 'filename'))
					.append($('<input />')
						.attr('type', 'file')
						.attr('name', 'filedata')
						.addClass('cbi-input-file'))
					.append($('<div />')
						.css('width', '100%')
						.addClass('progress progress-striped active')
						.append($('<div />')
							.addClass('progress-bar')
							.css('width', '100%')))
					.append($('<iframe />')
						.addClass('pull-right')
						.attr('name', 'cbi-fileupload-frame')
						.css('width', '1px')
						.css('height', '1px')
						.css('visibility', 'hidden')),

				finish_cb: function(ev) {
					$(this).off('load');

					var body = (this.contentDocument || this.contentWindow.document).body;
					if (body.firstChild.tagName.toLowerCase() == 'pre')
						body = body.firstChild;

					var json;
					try {
						json = $.parseJSON(body.innerHTML);
					} catch(e) {
						json = {
							message: L.tr('Invalid server response received'),
							error: [ -1, L.tr('Invalid data') ]
						};
					};

					if (json.error)
					{
						L.ui.dialog(L.tr('File upload'), [
							$('<p />').text(L.tr('The file upload failed with the server response below:')),
							$('<pre />').addClass('alert-message').text(json.message || json.error[1]),
							$('<p />').text(L.tr('In case of network problems try uploading the file again.'))
						], { style: 'close' });
					}
					else if (typeof(state.success_cb) == 'function')
					{
						state.success_cb(json);
					}
				},

				confirm_cb: function() {
					var f = state.form.find('.cbi-input-file');
					var b = state.form.find('.progress');
					var p = state.form.find('p');

					if (!f.val())
						return;

					state.form.find('iframe').on('load', state.finish_cb);
					state.form.submit();

					f.hide();
					b.show();
					p.text(L.tr('File upload in progress …'));

					state.form.parent().parent().find('button').prop('disabled', true);
				}
			});

			state.form.find('.progress').hide();
			state.form.find('.cbi-input-file').val('').show();
			state.form.find('p').text(content || L.tr('Select the file to upload and press "%s" to proceed.').format(L.tr('Ok')));

			state.form.find('[name=sessionid]').val(L.globals.sid);
			state.form.find('[name=filename]').val(options.filename);

			state.success_cb = options.success;

			L.ui.dialog(title || L.tr('File upload'), state.form, {
				style: 'confirm',
				confirm: state.confirm_cb
			});
		},

		download: function()
		{
			var state = L.ui._download || (L.ui._download = {
				form: $('<form />')
					.attr('method', 'post')
					.attr('action', '/cgi-bin/luci-upload')
					.attr('enctype', 'multipart/form-data')
					.attr('target', 'cbi-fileupload-frame')
					.append($('<p />'))
						.attr('text', L.tr('OTA image file download in progress …'))
					.append($('<input />')
						.attr('type', 'hidden')
						.attr('name', 'sessionid'))
					.append($('<input />')
						.attr('type', 'hidden')
						.attr('name', 'filename'))
					.append($('<div />')
						.css('width', '100%')
						.addClass('progress progress-striped active')
						.append($('<div />')
							.addClass('progress-bar')
							.css('width', '100%')))
					.append($('<iframe />')
						.addClass('pull-right')
						.attr('name', 'cbi-fileupload-frame')
						.css('width', '1px')
						.css('height', '1px')
						.css('visibility', 'hidden')),
			});

			state.form.find('p').text(L.tr('OTA image file download in progress …'));
			state.form.parent().parent().find('button').prop('disabled', true);

			L.ui.dialog(L.tr('Firmware download'), state.form, {
				style: 'wait'
			});
		},

		reconnect: function(info)
		{
			var protocols = (location.protocol == 'https:') ? [ 'http', 'https' ] : [ 'http' ];
			var ports     = (location.protocol == 'https:') ? [ 80, location.port || 443 ] : [ location.port || 80 ];
			var address   = location.hostname.match(/^[A-Fa-f0-9]*:[A-Fa-f0-9:]+$/) ? '[' + location.hostname + ']' : location.hostname;
			var images    = $();
			var interval, timeout;

			L.ui.dialog(
				L.tr('Waiting for device'), [
					$('<p />').text(info || L.tr('Please stand by while the device is reconfiguring …')),
					$('<div />')
						.css('width', '100%')
						.addClass('progress progress-striped active')
						.append(
							$('<div />')
							.addClass('progress-bar')
							.css('width', '100%')
						)
				], { style: 'wait', backdrop: 'static', keyboard: false }
			);

			for (var i = 0; i < protocols.length; i++)
				images = images.add($('<img />').attr('url', protocols[i] + '://' + address + ':' + ports[i]));

			images.on('load', function() {
				var url = this.getAttribute('url');
				L.session.isAlive().then(function(access) {
					if (access) {
						window.clearTimeout(timeout);
						window.clearInterval(interval);
						L.ui.dialog(false);
						images = null;
					} else {
						setTimeout(function () {
							console.log(url)
							location.href = url;
						}, 100000)
					}
				});
			});

			interval = window.setInterval(function() {
				images.each(function() {
					this.setAttribute('src', this.getAttribute('url') + L.globals.resource + '/res/loading.gif?r=' + Math.random());
				});
			}, 5000);

			timeout = window.setTimeout(function() {
				window.clearInterval(interval);
				images.off('load');

				L.ui.dialog(
					L.tr('Device not responding'),
					L.tr('The device was not responding within 180 seconds, you might need to manually reconnect your computer or use SSH to regain access.'),
					{ style: 'close' }
				);
			}, 180000);
		},

		get_lang: function()
		{
			var deferred = $.Deferred();
			$.ajax(L.globals.resource + '/template/lang?fresh=' + Math.random() ,{
				cache:    true,
				dataType: 'text',
				success:  function(data) {
					var lang = data.substring(0,2);
					if(lang == "en") {
						L.g_language = "en"
					} else if(lang == "de") {
						L.g_language = "de"
					} else if(lang == "es") {
						L.g_language = "es"
					} else if(lang == "fr") {
						L.g_language = "fr"
					} else if(lang == "it") {
						L.g_language = "it"
					} else {
						L.g_language = "zh"
					}
					deferred.resolve();
				}
			});
			return deferred;
		},

		login: function(invalid)
		{
			var state = L.ui._login || (L.ui._login = {});

			if (!state.deferred || state.deferred.state() != 'pending')
				state.deferred = $.Deferred();

			/* try to find sid from hash */
			var sid = L.getHash('id');
			if (sid && sid.match(/^[a-f0-9]{32}$/))
			{
				L.globals.sid = sid;
				L.session.isAlive().then(function(access) {
					if (access)
					{
						L.session.startHeartbeat();
						state.deferred.resolve();
						L.uci.load(['wireless', 'network']).then(function () {
							if (L.uci.get('wireless', 'wds', 'ssid') && (L.uci.get('network', 'lan', 'proto') == 'dhcp')) L.globals.wds = true;
							else L.globals.wds = false;
							L.ui.loadTheme();
						});
					}
					else
					{
						L.setHash('id', undefined);
						L.ui.login();
					}
				});

				return state.deferred;
			}

			$.ajax(L.globals.resource + '/template/login.htm', {
				cache:    true,
				dataType: 'text',
				success:  function(data) {
					data = data.replace(/<%([#:=])?(.+?)%>/g, function(match, p1, p2) {
						p2 = p2.replace(/^\s+/, '').replace(/\s+$/, '');
						switch (p1)
						{
						case '#':
							return '';

						case ':':
							return L.tr(p2);

						case '=':
							return L.globals[p2] || '';

						default:
							return '(?' + match + ')';
						}
					});
					$('body').empty().append(data);
					let currlang = "";
					L.uci.callLoad('luci2').then(function(data){
						currlang = data.main.lang;
						if(data && data.languages){
							L.ui.callOption('luci2', "languages", currlang).then(function(v) {
								$(".login_langinfo").text(v)
							});
							$('.login_lang ul').empty();
							for(let key in data.languages){
								if(key.charAt(0) != "."){
									if(key != currlang){
										$('.login_lang ul').append($('<li>', {
											value: key,
											text: data.languages[key]
										}));
									}
								}
							}
						}
					});

					$('.login_langinfo').click(function(event) {
						event.stopPropagation();
						$('.login_lang ul').toggle();
					});

					$(document).click(function() {
						$('.login_lang ul').hide();
					});

					$(".login_lang").on("click","ul li",function(){
						let selectlang = $(this).attr("value");
						if(selectlang){
							if(selectlang == "en"){
								L.ui.do_cmd("echo en > /www/luci2/template/lang");
							}else if(selectlang == "zh_cn"){
								L.ui.do_cmd("echo zh > /www/luci2/template/lang");
							}

							L.uci.callSet('luci2', "main", { 'lang': selectlang});
                            L.uci.callCommit('luci2');
							location.reload(true);
						}
					})

					$('#userpassword_ctrl.inputArea.paddingleft_14').keypress(function (ev) {
						if (ev.which == 10 || ev.which == 13) {
							confirm_cb();
						}
					});
					var forget = 0;
					$('#forgetpwd_page_link').click(function () {
						if(forget == 0){
							forget = 1;
							$('#forgetpwd_page_tips').css('display','');
						} else {
							forget = 0;
							$('#forgetpwd_page_tips').css('display','none');
						}
					});
					function response_cb(response) {
						if (!response.ubus_rpc_session) {
							$('#login_error').css('display','');
							$('#userpassword_ctrl.inputArea.paddingleft_14').focus();
						} else {
							L.globals.sid = response.ubus_rpc_session;
							L.setHash('id', L.globals.sid);
							L.session.startHeartbeat();
							L.uci.load(['wireless', 'network']).then(function () {
								if (L.uci.get('wireless', 'wds', 'ssid') && (L.uci.get('network', 'lan', 'proto') == 'dhcp')) L.globals.wds = true;
								else L.globals.wds = false;
								L.ui.loadTheme();
							});
						}
					}
					function confirm_cb() {
						var u = 'root';
						var p = $('#userpassword_ctrl.inputArea.paddingleft_14').val();
						L.globals.sid = '00000000000000000000000000000000';
						L.session.login(u, p).then(response_cb);
					}
					$('#loginbtn').click(function () {
						confirm_cb();
					});

					state.deferred.resolve();
				}
			});

			return state.deferred;
		},

		callOption: L.rpc.declare({
			object: 'uci',
			method: 'get',
			params: ['config', 'section', 'option'],
			expect: { value: ' ' }
		}),

		callSet: L.rpc.declare({
			object: 'uci',
			method: 'set',
			params: ['config', 'section', 'values']
		}),

		callCommit: L.rpc.declare({
			object: 'uci',
			method: 'commit',
			params: ['config']
		}),

		do_cmd: L.rpc.declare({
			object: 'web.advance',
			method: 'cmd',
			params: [ 'cmd' ]
		}),

		initialPage:function(){
			L.ui.callOption('basic_setting', 'web_guide', 'inited').then(function (value) {
				if (value == '0'){
					let path = window.location.hash.split("view:")[1] || "guide_start";
					if(path == "guide_start"){
						localStorage.clear();
					}
					history.pushState({}, '', window.location.hash);
					L.ui.loadHtml(path,"body");

					$(window).on('hashchange', function() {
						let route = location.hash.split("view:")[1];
						if(path != route){
							L.ui.loadHtml(route,"body");
						}
					});
				}else{
					L.ui.login()
				}
			})
		},

		loadHtml:function(html,elem){
			let d = $.Deferred();
			let cname = L.toClassName(html);
			let views = L.views || (L.views = {});
			L.setHash('view', html);

			$.ajax(L.globals.resource + `/template/${html}.htm`, {
				cache: true,
				dataType: 'text',
				success: function (data) {
					data = data.replace(/<%([#:=])?(.+?)%>/g, function(match, p1, p2) {
						p2 = p2.replace(/^\s+/, '').replace(/\s+$/, '');
						switch (p1)
						{
						case '#':
							return '';

						case ':':
							return L.tr(p2);

						case '=':
							return L.globals[p2] || '';

						default:
							return '(?' + match + ')';
						}
					});

					$(elem).empty().append(data);
				},
			}).then(function () {
				let url = L.globals.resource + `/view/${html}.js`;
				$.ajax(url, {
					method: 'GET',
					cache: true,
					dataType: 'text',
				}).then(function (data) {
					let viewConstructorSource = ('(function(L, $) { ' + 'return %s' + '})(L, $);\n\n' + '//# sourceURL=%s').format(data, url);
					let viewConstructor = eval(viewConstructorSource);
					views[cname] = new viewConstructor({
						name: html,
						acls: {
							"read": {
								"uci": [
									"*"
								],
								"ubus": [
									"*"
								],
							},
							"write": {
								"uci": [
									"*"
								],
								"ubus": [
									"*"
								],
							}
						},
					});
					L.globals.currentView = views[cname];
					views[cname].render.apply(views[cname]);
					d.resolve();
				});
			});
			return d.promise();
		},

		cryptPassword: L.rpc.declare({
			object: 'luci2.ui',
			method: 'crypt',
			params: [ 'data' ],
			expect: { crypt: '' }
		}),


		mergeACLScope: function(acl_scope, scope)
		{
			if ($.isArray(scope))
			{
				for (var i = 0; i < scope.length; i++)
					acl_scope[scope[i]] = true;
			}
			else if ($.isPlainObject(scope))
			{
				for (var object_name in scope)
				{
					if (!$.isArray(scope[object_name]))
						continue;

					var acl_object = acl_scope[object_name] || (acl_scope[object_name] = { });

					for (var i = 0; i < scope[object_name].length; i++)
						acl_object[scope[object_name][i]] = true;
				}
			}
		},

		mergeACLPermission: function(acl_perm, perm)
		{
			if ($.isPlainObject(perm))
			{
				for (var scope_name in perm)
				{
					var acl_scope = acl_perm[scope_name] || (acl_perm[scope_name] = { });
					L.ui.mergeACLScope(acl_scope, perm[scope_name]);
				}
			}
		},

		mergeACLGroup: function(acl_group, group)
		{
			if ($.isPlainObject(group))
			{
				if (!acl_group.description)
					acl_group.description = group.description;

				if (group.read)
				{
					var acl_perm = acl_group.read || (acl_group.read = { });
					L.ui.mergeACLPermission(acl_perm, group.read);
				}

				if (group.write)
				{
					var acl_perm = acl_group.write || (acl_group.write = { });
					L.ui.mergeACLPermission(acl_perm, group.write);
				}
			}
		},

		callACLsCallback: function(trees)
		{
			var acl_tree = { };

			for (var i = 0; i < trees.length; i++)
			{
				if (!$.isPlainObject(trees[i]))
					continue;

				for (var group_name in trees[i])
				{
					var acl_group = acl_tree[group_name] || (acl_tree[group_name] = { });
					L.ui.mergeACLGroup(acl_group, trees[i][group_name]);
				}
			}

			return acl_tree;
		},

		callACLs: L.rpc.declare({
			object: 'luci2.ui',
			method: 'acls',
			expect: { acls: [ ] }
		}),

		getAvailableACLs: function()
		{
			return this.callACLs().then(this.callACLsCallback);
		},

		renderChangeIndicator: function()
		{
			return $('<ul />')
				.addClass('nav navbar-nav navbar-right')
				.append($('<li />')
					.append($('<a />')
						.attr('id', 'changes')
						.attr('href', '#')
						.append($('<span />')
							.addClass('label label-info')
							.hide())));
		},

		callMenuCallback: function(entries)
		{
			if (L.globals.wds) {
				for (var now in entries) {
					var check = now.indexOf('/');
					if (check != -1) check = now.slice(0, check);
					else check = now;
					if (check == 'advance' || check == 'network')
						delete entries[now];
					if (now == 'wireless/adv_wifi' || now == 'wireless/guest_wifi' || now == 'wireless/visit_control')
						delete entries[now];
				}
			}

			L.globals.mainMenu = new L.ui.menu();
			L.globals.mainMenu.entries(entries);

			$('#mainmenu').empty().append(L.globals.mainMenu.render(0, 1));
			$('#indicator').empty().append(L.ui.renderChangeIndicator());

			$('.navbar-fixed-side .dropdown').on('hidden.bs.dropdown', function() {
				$(this).addClass('side-open');
			});

			$('.navbar-fixed-side .dropdown').on('shown.bs.dropdown', function() {
				$('.side-open.dropdown').removeClass('side-open');
			});
		},

		callMenu: L.rpc.declare({
			object: 'luci2.ui',
			method: 'menu',
			expect: { menu: { } }
		}),

		renderMainMenu: function()
		{
			return this.callMenu().then(this.callMenuCallback);
		},

		renderViewMenu: function()
		{
			$('#viewmenu')
				.empty()
				.append(L.globals.mainMenu.render(2, 900));
		},

		renderView: function()
		{
			var node  = arguments[0];
			var name  = node.view.split(/\//).join('.');
			var cname = L.toClassName(name);
			var views = L.views || (L.views = { });
			var args  = [ ];

			for (var i = 1; i < arguments.length; i++)
				args.push(arguments[i]);

			if (L.globals.currentView)
				L.globals.currentView.finish();

			L.ui.renderViewMenu();
			L.setHash('view', node.view);

			if (views[cname] instanceof L.ui.view)
			{
				L.globals.currentView = views[cname];
				return views[cname].render.apply(views[cname], args);
			}

			var url = L.globals.resource + '/view/' + name + '.js';

			return $.ajax(url, {
				method: 'GET',
				cache: true,
				dataType: 'text'
			}).then(function(data) {
				try {
					var viewConstructorSource = (
						'(function(L, $) { ' +
							'return %s' +
						'})(L, $);\n\n' +
						'//# sourceURL=%s'
					).format(data, url);

					var viewConstructor = eval(viewConstructorSource);

					views[cname] = new viewConstructor({
						name: name,
						acls: node.write || { }
					});

					L.globals.currentView = views[cname];
					return views[cname].render.apply(views[cname], args);
				}
				catch(e) {
					alert('Unable to instantiate view "%s": %s'.format(url, e));
				};

				return $.Deferred().resolve();
			});
		},

		changeView: function()
		{
			var name = L.getHash('view');
			var node = L.globals.defaultNode;

			if (name && L.globals.mainMenu)
				node = L.globals.mainMenu.getNode(name);

			if (node)
			{
//				L.ui.loading(true);
				L.ui.renderView(node).then(function() {
					$('#mainmenu.in').collapse('hide');
//					L.ui.loading(false);
				});
			}
		},

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

		updateHostname: function()
		{
			return L.system.getBoardInfo().then(function(info) {
				if (info.hostname)
					$('#hostname').text(info.hostname);
			});
		},

		updateChanges: function()
		{
			return L.uci.changes().then(function(changes) {
				var n = 0;
				var html = '';

				for (var config in changes)
				{
					var log = [ ];

					for (var i = 0; i < changes[config].length; i++)
					{
						var c = changes[config][i];

						switch (c[0])
						{
						case 'order':
							log.push('uci reorder %s.<ins>%s=<strong>%s</strong></ins>'.format(config, c[1], c[2]));
							break;

						case 'remove':
							if (c.length < 3)
								log.push('uci delete %s.<del>%s</del>'.format(config, c[1]));
							else
								log.push('uci delete %s.%s.<del>%s</del>'.format(config, c[1], c[2]));
							break;

						case 'rename':
							if (c.length < 4)
								log.push('uci rename %s.<ins>%s=<strong>%s</strong></ins>'.format(config, c[1], c[2], c[3]));
							else
								log.push('uci rename %s.%s.<ins>%s=<strong>%s</strong></ins>'.format(config, c[1], c[2], c[3], c[4]));
							break;

						case 'add':
							log.push('uci add %s <ins>%s</ins> (= <ins><strong>%s</strong></ins>)'.format(config, c[2], c[1]));
							break;

						case 'list-add':
							log.push('uci add_list %s.%s.<ins>%s=<strong>%s</strong></ins>'.format(config, c[1], c[2], c[3], c[4]));
							break;

						case 'list-del':
							log.push('uci del_list %s.%s.<del>%s=<strong>%s</strong></del>'.format(config, c[1], c[2], c[3], c[4]));
							break;

						case 'set':
							if (c.length < 4)
								log.push('uci set %s.<ins>%s=<strong>%s</strong></ins>'.format(config, c[1], c[2]));
							else
								log.push('uci set %s.%s.<ins>%s=<strong>%s</strong></ins>'.format(config, c[1], c[2], c[3], c[4]));
							break;
						}
					}

					html += '<code>/etc/config/%s</code><pre class="uci-changes">%s</pre>'.format(config, log.join('\n'));
					n += changes[config].length;
				}

				if (n > 0)
					$('#changes')
						.click(function(ev) {
							L.ui.dialog(L.tr('Staged configuration changes'), html, {
								style: 'confirm',
								confirm: function() {
									L.uci.apply().then(function(){
										L.ui.updateChanges();
										L.ui.dialog(false);
									});
								}
							});
							ev.preventDefault();
						})
						.children('span')
							.show()
							.text(L.trcp('Pending configuration changes', '1 change', '%d changes', n).format(n));
				else
					$('#changes').children('span').hide();
			});
		},

		get_status: L.rpc.declare({
			object: 'web.network',
			method: 'getstatus',
			expect: { }
		}),

		get_ipaddr: L.rpc.declare({
			object: 'network.interface.wan',
			method: 'status',
			expect: { }
		}),

		command: L.rpc.declare({
			object: 'web.advance',
			method: 'cmd',
			params: ['cmd'],
		}),

		loadInternet: function(){
			var self = this;
			var m = new L.cbi.Map('network', {
				emptytop:    '0px'
			});

			var s = m.section(L.cbi.NamedSection, 'wan', {
				//caption:      L.tr("WAN port settings")
			});

			var wanstatus = s.option(L.cbi.DummyValue, '__wanstatus', {
                caption:        L.tr("WAN status"),
				initial:        L.tr("Testing")
            });

			function start_wanstatus_check(id)
			{
				clearInterval(L.wanstatus_interval);
				L.wanstatus_interval = setInterval(function() {
					if(L.getHash("view") != "internet"){
						clearInterval(L.wanstatus_interval);
						return;
					}
					var protonow = L.uci.get('network', 'wan', 'proto');
					if (protonow != "bridge") {
						self.get_status().then(function(info) {
							var stat = info.status;
							var ipaddrnow = L.uci.get('network', 'wan', 'ipaddr');
							if (stat != undefined) {
								if (stat == 0 ) {
									$('#' + id).text(L.tr("The network cable is not connected"));
								} else if(protonow == 'static') {
									$('#' + id).text(L.tr("The network cable is connected"));
								} else {
									self.get_ipaddr().then(function(data) {
										if(data){
											var ipexist = data["ipv4-address"];
											if (protonow == 'pppoe'){
												if(ipexist){
													$('#' + id).text(L.tr("pppoe connect success"));
												} else {
													$('#' + id).text(L.tr("pppoe connect failed"));
												}
											} else {
												if(ipexist){
													$('#' + id).text(L.tr("Connected and can go online normally"));
												} else {
													$('#' + id).text(L.tr("Can't connect to the internet"));
												}
											}
										} else {
											//if not get result
											$('#' + id).text(L.tr("Testing"));
										}
									});
								}
							} else {
								//if not get result
								$('#' + id).text(L.tr("Testing"));
							}
						});
					} else {
						$('#' + id).text(L.tr("Currently in bridge mode"));
					}
				}, 2000);
			};

			wanstatus.load = function(sid)
			{
				var id = this.id(sid);
				start_wanstatus_check(id);
			}

			p = s.option(L.cbi.ListValue, 'proto', {
				caption:	L.tr("Mode"),
				initial:	'dhcp'
			})
				.value('dhcp', L.tr("DHCP"))
				.value('static', L.tr("Static"))
				.value('pppoe', L.tr("PPPoE"))
				.value('bridge', L.tr("Bridge"))

			p.save = function(sid){
				var value = this.formvalue(sid);
				var ori_value = L.uci.get('network', 'wan', 'proto');
				if(ori_value == 'bridge' && value != 'bridge'){
					L.uci.unset('network', 'wan', 'disabled');
					L.uci.set('network', 'lan', 'proto', 'static');
					L.uci.load('dhcp').then(function(data) {
						L.uci.unset('dhcp', 'lan', 'ignore');
						L.uci.save();
					});
				}
				if(ori_value == 'pppoe' && value != 'pppoe'){
					L.uci.set('network', 'wan6', 'reqprefix', 'auto');
					L.uci.set('network', 'wan6', 'reqaddress', 'try');
					L.uci.load('dhcp').then(function(data) {
						L.uci.set('dhcp', 'wan', 'dhcpv6', 'relay');
						L.uci.set('dhcp', 'wan', 'ra', 'relay');
						L.uci.set('dhcp', 'wan', 'ndp', 'relay');
						L.uci.set('dhcp', 'wan', 'master', '1');
						L.uci.set('dhcp', 'lan', 'ndp', 'relay');
						L.uci.set('dhcp', 'lan', 'ra_management', '1');
						L.uci.set('dhcp', 'lan', 'dhcpv6', 'relay');
						L.uci.set('dhcp', 'lan', 'ra', 'relay');
						L.uci.unset('dhcp', 'lan', 'ra_default');
						L.uci.save();
					});
				}
				if(ori_value != 'pppoe' && value == 'pppoe'){
					L.uci.unset('network', 'wan6', 'reqprefix');
					L.uci.unset('network', 'wan6', 'reqaddress');
					L.uci.load('dhcp').then(function(data) {
						L.uci.unset('dhcp', 'wan', 'dhcpv6');
						L.uci.unset('dhcp', 'wan', 'ra');
						L.uci.unset('dhcp', 'wan', 'ndp');
						L.uci.unset('dhcp', 'wan', 'master');
						L.uci.unset('dhcp', 'lan', 'ndp');
						L.uci.unset('dhcp', 'lan', 'ra_management');
						L.uci.set('dhcp', 'lan', 'dhcpv6', 'server');
						L.uci.set('dhcp', 'lan', 'ra', 'server');
						L.uci.set('dhcp', 'lan', 'ra_default', '1');
						L.uci.save();
					});
				}
				if(value == 'pppoe'){
					var id = wanstatus.id(sid);
					L.uci.unset('network', 'wan', 'ipaddr');
					L.uci.unset('network', 'wan', 'gateway');
					L.uci.unset('network', 'wan', 'netmask');
					L.uci.set('network', 'wan', 'proto', 'pppoe');
					var ppp_times = 0;
					setTimeout(function(){
						$('#' + id).text(L.tr("Testing"));
						clearInterval(L.wanstatus_interval);
						L.ui.setting(true, L.tr("pppoe connecting..."));
						ppp_interval = setInterval(function() {
							ppp_times = ppp_times + 1;
							if(ppp_times > 10){
								clearInterval(ppp_interval);
								L.ui.setting(false);
								L.ui.setting(true, L.tr("pppoe connect failed"));
								$('body').removeClass("loading_gif");
								setTimeout(function(){
									L.ui.setting(false);
									start_wanstatus_check(id);
									$('#' + id).text(L.tr("pppoe connect failed"));
								}, 3000);
							}
							self.get_ipaddr().then(function(data){
								var ppp_status = data["ipv4-address"];
								if(ppp_status)
									ppp_status = data["ipv4-address"][0].address;
								if (ppp_status){
									clearInterval(ppp_interval);
									L.ui.setting(false);
									L.ui.setting(true, L.tr("pppoe connect success"));
									$('body').removeClass("loading_gif");
									setTimeout(function(){
										$('#' + id).text(L.tr("pppoe connect success"));
										start_wanstatus_check(id);
										L.ui.setting(false);
									}, 2000);
								}
							});
						}, 1000);
					}, 2000);
				} else if(value == 'static') {
					L.uci.unset('network', 'wan', 'auto');
					L.uci.set('network', 'wan', 'proto', 'static');
				} else if(value == 'dhcp') {
					L.uci.unset('network', 'wan', 'ipaddr');
					L.uci.unset('network', 'wan', 'gateway');
					L.uci.unset('network', 'wan', 'netmask');
					L.uci.unset('network', 'wan', 'auto');
					L.uci.set('network', 'wan', 'proto', 'dhcp');
				} else if(value == 'bridge') {
					L.uci.unset('network', 'wan', 'ipaddr');
					L.uci.unset('network', 'wan', 'gateway');
					L.uci.unset('network', 'wan', 'netmask');
					L.uci.unset('network', 'wan', 'auto');
					L.uci.set('network', 'wan', 'proto', 'bridge');
					L.uci.set('network', 'wan', 'disabled', '1');
					L.uci.set('network', 'lan', 'ifname', 'eth0.1 eth0.2');
					L.uci.set('network','lan','proto','dhcp');
					L.uci.load('dhcp').then(function() {
						L.uci.set('dhcp', 'lan', 'ignore', '1');
						L.uci.save();
					});
				}
				L.uci.save();
			}

			var lanip='';
			self.lan_ip().then(function (ip) {
				lanip = ip;
			});

			var ipdata = s.option(L.cbi.InputValue, 'ipaddr', {
				caption:      L.tr("IP address"),
				datatype:     function(str) {

				if (!L.parseIPv4(str)){
					return L.tr('Must be a valid IPv4 address');
				}
				var ip = lanip.split('.');
				var parts = str.split('.');
				var netmask = '255.255.255.0'.split('.');
				var res0 = parseInt(parts[0]) & parseInt(netmask[0]);
				var res1 = parseInt(parts[1]) & parseInt(netmask[1]);
				var res2 = parseInt(parts[2]) & parseInt(netmask[2]);
				var res3 = parseInt(parts[3]) & parseInt(netmask[3]);
				var res_gw0 = parseInt(ip[0]) & parseInt(netmask[0]);
				var res_gw1 = parseInt(ip[1]) & parseInt(netmask[1]);
				var res_gw2 = parseInt(ip[2]) & parseInt(netmask[2]);
				var res_gw3 = parseInt(ip[3]) & parseInt(netmask[3]);

				if (res0 === res_gw0 && res1 === res_gw1 && res2 === res_gw2 && res3 === res_gw3) {
					return L.tr('Must not be in the same segment as the IP address of Lan.');
				}
					return true;
				}
			}).depends('proto', function(v) { return (v == 'static');});

			e = s.option(L.cbi.InputValue, 'netmask', {
				caption:      L.tr("Subnet mask"),
				datatype:    'netmask4'
			}).depends('proto', function(v) { return (v == 'static');});

			function check(v) {
				if (v.length == 0) return L.tr('Field must not be empty');
				if (L.parseIPv4(v)) {
					var ip = $('#field_network_wan_wan_ipaddr').val().split('.');
					var gateway = v.split('.');
					var netmask = $('#field_network_wan_wan_netmask').val().split('.');

					var res0 = parseInt(ip[0]) & parseInt(netmask[0]);
					var res1 = parseInt(ip[1]) & parseInt(netmask[1]);
					var res2 = parseInt(ip[2]) & parseInt(netmask[2]);
					var res3 = parseInt(ip[3]) & parseInt(netmask[3]);
					var res_gw0 = parseInt(gateway[0]) & parseInt(netmask[0]);
					var res_gw1 = parseInt(gateway[1]) & parseInt(netmask[1]);
					var res_gw2 = parseInt(gateway[2]) & parseInt(netmask[2]);
					var res_gw3 = parseInt(gateway[3]) & parseInt(netmask[3]);

					if (res0 == res_gw0 && res1 == res_gw1 && res2 == res_gw2 && res3 == res_gw3)
						return true;

					else return L.tr('Must be in the same segment as the IP address.');
				} else return L.tr('Must be a valid IPv4 address');
			}

			var gateway = s.option(L.cbi.InputValue, 'gateway', {
				caption:      L.tr("Default gateway"),
				datatype:    function(v) {
					return check(v);
				}
			}).depends('proto', function(v) { return (v == 'static');});

			e = s.option(L.cbi.InputValue, 'username', {
				caption:      L.tr("Broadband account")
			}).depends('proto', function(v) { return (v == 'pppoe');});

			e = s.option(L.cbi.PasswordValue, 'password', {
				caption:      L.tr("Broadband password")
			}).depends('proto', function(v) { return (v == 'pppoe');});

			e = s.option(L.cbi.CheckboxValue, 'auto', {
				caption:      L.tr("Auto connect")
			}).depends('proto', function(v) { return (v == 'pppoe');});

			var manualdns = s.option(L.cbi.CheckboxValue, '__manualdns', {
				caption:      L.tr("Manually set up DNS server"),
				optional:     true
			}).depends('proto', function(v) { return (v == 'dhcp' || v == 'pppoe');});

			manualdns.ucivalue = function(sid) {
				if (L.uci.get('network', 'wan', 'peerdns') == 0) return true;
				return false;
			};

			manualdns.save = function(sid) {
				var mdns = manualdns.formvalue(sid);
				if (p.formvalue(sid) != 'static') {
					L.uci.set('network', 'wan', 'peerdns', 1 ^ mdns);
					if (!mdns) L.uci.unset('network', 'wan', 'dns');
				}
				L.uci.save();
			};

			var dns1 = s.option(L.cbi.InputValue, '__dns1', {
				caption:      L.tr("Preferred DNS server"),
				datatype:     'ip4addr'
			}).depends('__manualdns', function(v) {
				var proto = p.formvalue('wan');
				if(proto == "bridge"){
					return false;
				}
				return v || proto == "static";
			});

			var dns2 = s.option(L.cbi.InputValue, '__dns2', {
				caption:      L.tr("Standby DNS server(Optional)"),
				optional:     true,
				datatype:     'ip4addr',
			}).depends('__manualdns', function(v) {
				var proto = p.formvalue('wan');
				if(proto == "bridge"){
					return false;
				}
				return v || proto == "static";
			});

			dns1.ucivalue = function(sid) {
				var dns_value = L.uci.get("network", "wan", "dns");
				var split = 0;
				if (dns_value) {
					split = dns_value.indexOf(" ");
					if (split == -1) return dns_value;
					else return dns_value.substring(0, split);
				}
			};

			dns2.ucivalue = function(sid) {
				var dns_value = L.uci.get("network", "wan", "dns");
				var split = 0;
				if (dns_value) {
					split = dns_value.indexOf(" ");
					if (split != -1) return dns_value.substring(split + 1);
				}
			};

			dns1.save = dns2.save =  function(sid) {
				var dnsv1 = dns1.formvalue(sid);
				var dnsv2 = dns2.formvalue(sid);
				if (dnsv2) dnsv1 += ' ' + dnsv2;
				if (p.formvalue(sid) != 'static') {
					if (!manualdns.formvalue(sid)) L.uci.unset('network', 'wan', 'dns');
					else L.uci.set('network', 'wan', 'dns', dnsv1);
				} else {
					L.uci.unset('network', 'wan', 'peerdns');
					L.uci.set('network', 'wan', 'dns', dnsv1);
				}
				L.uci.save();
			};

			var internaldns = s.option(L.cbi.CheckboxValue, '__internaldns', {
				caption:      L.tr("Enable internal dns server"),
				optional:     true
			});

			internaldns.ucivalue = function(sid) {
				var value = L.uci.get("network", "wan", "internal_server")
				if (value == 1) return true;
				return false;
			};

			internaldns.save = function(sid) {
				var idns = internaldns.formvalue(sid);
				if(idns == true) {
					self.command("/sbin/internal_server 1");
				}
				else
					self.command("/sbin/internal_server 0");
			};

			pmtu1 = s.option(L.cbi.InputValue, '_mtu1', {
				caption:      L.tr("Packet MTU byte"),
				initial:      1500,
				datatype:     'range(1280,1500)',
				description:  L.tr("The default is 1500, if not necessary, please do not modify")
			}).depends('proto', function(v) { return (v != 'pppoe');});
			pmtu1.ucivalue = function(sid) {
				var mtu_value = L.uci.get("network", "wan_dev", "mtu");
				var proto = L.uci.get("network", "wan", "proto");
				if (mtu_value && proto != "pppoe")
					return mtu_value-4;
				else
					return 1500;
			};
			pmtu1.save = function(sid) {
				var pamtu = pmtu1.formvalue(sid);
				var proto = p.formvalue(sid);
				if(proto != 'pppoe') {
					if (pamtu > 38 && pamtu < 9000) L.uci.set('network', 'wan_dev', 'mtu', parseInt(pamtu) + 4);
					else L.uci.set('network', 'wan_dev', 'mtu', 1504);
					L.uci.save();
				}
			};
			pmtu2 = s.option(L.cbi.InputValue, '_mtu2', {
				caption:      L.tr("Packet MTU byte"),
				initial:      1500,
				datatype:     'range(46,9000)',
				description:  L.tr("The default is 1480, if not necessary, please do not modify")
			}).depends('proto', function(v) { return (v == 'pppoe');});
			pmtu2.ucivalue = function(sid) {
				var mtu_value = L.uci.get("network", "wan_dev", "mtu");
				var proto = L.uci.get("network", "wan", "proto");
				if (mtu_value && proto == "pppoe")
					return mtu_value-4;
				else
					return 1480;
			};
			pmtu2.save = function(sid) {
				var pamtu = pmtu2.formvalue(sid);
				var proto = p.formvalue(sid);
				if(proto == 'pppoe') {
					if (pamtu > 38 && pamtu < 9000)
						L.uci.set('network', 'wan_dev', 'mtu', parseInt(pamtu) + 4);
					else
						L.uci.set('network', 'wan_dev', 'mtu', 1484);
					L.uci.save();
				}
			};

			uni = s.option(L.cbi.CheckboxValue, 'unicast', {
				caption:      L.tr("Open unicast to get ip"),
				optional:     true,
				description:  L.tr("Do not choose under normal circumstances")
			}).depends('proto', function(v) { return (v == 'dhcp');});

			uni.save = function(sid) {
				var un = uni.formvalue(sid);
				if (p.formvalue('wan') == "dhcp") L.uci.set('network', 'wan', 'unicast', un);
				else L.uci.unset('network', 'wan', 'unicast');
				L.uci.save();
			};

			wanspeed = s.option(L.cbi.ListValue, 'speed', {
				caption:    L.tr("WAN port rate setting"),
				initial:    'auto',
				description: L.tr("When using a fixed rate, there is no need to set the peer synchronization to a fixed rate")
			})
			.value('auto', L.tr("Auto-negotiation"))
			.value('10mfull', L.tr("10M full-duplex"))
			.value('10mhalf', L.tr("10M half-duplex"))
			.value('100mfull', L.tr("100M full-duplex"))
			.value('100mhalf', L.tr("100M half-duplex"))
			.value('1000mfull', L.tr("1000M full-duplex"));

			wanspeed.save = function(sid) {
				var do_cmd = L.rpc.declare({object: 'web.advance', method: 'cmd', params: [ 'cmd' ]});
				var get_Port =  L.rpc.declare({object: 'luci2.advance', method: 'getport', expect: { }});
				var wan_dev = L.uci.get('network', 'wan', 'ifname').split(".")[0];
				var wansp = wanspeed.formvalue(sid);
				get_Port().then(function(result){
					var wan_port = result.port - 1;
					var wan_dev = "eth0";
					do_cmd("echo phyad "+ wan_port +" > /sys/kernel/debug/gmac_debug");
					if(wansp == "auto"){
						do_cmd("ethtool -s " + wan_dev + " autoneg on");
					}else if(wansp == "10mfull"){
						do_cmd("ethtool -s " + wan_dev + " speed 10 duplex full");
					}else if(wansp == "10mhalf"){
						do_cmd("ethtool -s " + wan_dev + " speed 10 duplex half");
					}else if(wansp == "100mfull"){
						do_cmd("ethtool -s " + wan_dev + " speed 100 duplex full");
					}else if(wansp == "100mhalf"){
						do_cmd("ethtool -s " + wan_dev + " speed 100 duplex half");
					}else if(wansp == "1000mfull"){
						do_cmd("ethtool -s " + wan_dev + " speed 1000 duplex full");
					}
				});
				L.uci.set('network', 'wan', 'speed', wansp);
				L.uci.save();
			};

			servicename = s.option(L.cbi.InputValue, 'service', {
				caption:      L.tr("Service Name"),
				optional:     true
			}).depends('proto', function(v) { return (v == 'pppoe');});

			servicename.save = function(sid) {
				var sname = servicename.formvalue(sid);
				if (p.formvalue('wan') == 'pppoe' && sname) L.uci.set('network', 'wan', 'service', sname);
				else L.uci.unset('network', 'wan', 'service');
				L.uci.save();
			};

			servername = s.option(L.cbi.InputValue, 'ac', {
				caption:      L.tr("Server name"),
				optional:     true
			}).depends('proto', function(v) { return (v == 'pppoe');});

			servername.save = function(sid) {
				var svname = servername.formvalue(sid);
				if (p.formvalue('wan') == "pppoe" && svname) L.uci.set('network', 'wan', 'ac', svname);
				else L.uci.unset('network', 'wan', 'ac');
				L.uci.save();
			};

			uad = s.option(L.cbi.CheckboxValue, '__useoperatoraddress', {
				caption:      L.tr("Use the ip address specified by the operator"),
				optional:     true
			}).depends('proto', function(v) { return (v == 'pppoe');});

			uad.ucivalue  = function(sid) {
				return !!L.uci.get("network", "wan", "pppd_options");
			};

			uad.save = function(sid) {
				var ua = uad.formvalue(sid);
				if (p.formvalue('wan') == 'pppoe') L.uci.set('network', 'wan', 'fixipEnb', 0 ^ ua);
				else L.uci.unset('network', 'wan', 'fixipEnb');
				L.uci.save();
			};

			operatoraddress = s.option(L.cbi.InputValue, 'pppd_options', {
				caption:      L.tr("The ip address specified by the operator"),
				datatype:	  'ipaddr'
			}).depends('__useoperatoraddress', function(v) { return v && p.formvalue('wan') == 'pppoe';});

			operatoraddress.save = function(sid) {
				var oad = operatoraddress.formvalue(sid);
				if (uad.formvalue(sid) && p.formvalue('wan') == 'pppoe') L.uci.set('network', 'wan', 'pppd_options', oad);
				else L.uci.unset('network', 'wan', 'pppd_options');
				L.uci.save();
			};
			m.insertInto('#mainPage').then(function () {
				$('#mainPage').css('width', '800px');
				$('#node_network_wan_wan___default__').css({
					width: '700px',
					'margin-left': '30px',
				});
				$('.form-group').children('div:last-child').css({
					width: '350px',
					'margin-top': '7px',
					'text-align': 'left',
					color: 'gray',
					'font-size': '12px',
					'margin-left': '220px',
				});
				$('#field_network_wan_wan_ipaddr').keyup(function () {
					gateway.setError($('#field_network_wan_wan_gateway'), check($('#field_network_wan_wan_gateway').val()));
				});
				$('#field_network_wan_wan_netmask').keyup(function () {
					gateway.setError($('#field_network_wan_wan_gateway'), check($('#field_network_wan_wan_gateway').val()));
				});
			});

			m.on('apply',function(){
				let get_val = $("#field_network_wan_wan_proto").val();
				let origin_proto = L.uci.get('network', 'wan', 'origin_proto');
				let get_http = "";

				L.ui.callOption('basic_setting', 'vendor', 'hosts').then(function (value) {
					if (value.length > 1){
						get_http = 'http://'+value;
					}

					if(get_val == "bridge" && origin_proto !="bridge"){
						L.ui.setting(false);
						var form = $('<p />').text(L.tr(`The LAN IP will change when switching from other modes to bridge mode.Guest WiFi will be turned off.Please log in using the domain name "%s" in two minutes`).format(get_http));
						L.ui.dialog(L.tr('Tips'), form, {
								style: 'onlyConfirm',
								confirm: function() {
										L.ui.dialog(false);
										L.ui.setting(true);
								},
						});
						self.do_cmd("/usr/sbin/ip_monitor.sh &");
					}else if (get_val != "bridge" && origin_proto =="bridge"){
						L.ui.setting(false);
						var form = $('<p />').text(L.tr(`The LAN IP will change when switching from bridge mode to other modes.Please log in using the domain name "%s" in two minutes`).format(get_http));
						L.ui.dialog(L.tr('Tips'), form, {
								style: 'onlyConfirm',
								confirm: function() {
									L.ui.dialog(false);
									L.ui.setting(true);
								},
						});
						self.do_cmd("/usr/sbin/ip_monitor.sh &");
					}
				})
				self.command("/etc/init.d/dnsmasq restart; /etc/init.d/network restart").then(function(v){
					setTimeout(() => {
						L.ui.setting(false);
					}, 1000*40);
				});
			});
		},

		board: L.rpc.declare({
			object: 'system',
			method: 'info',
			expect: {
				memory: {},
			},
		}),

		loadBasewifi: function(){
			var self = this;
			var m = new L.cbi.Map('wireless', {
				emptytop:    '0px'
			});

			var s = m.section(L.cbi.NamedSection, 'default_radio0', {
				caption:      L.tr('2.4G')
			});

			var e = s.option(L.cbi.CheckboxValue, 'disabled_hostapd', {
				caption:      L.tr('Disabled Wifi'),
				optional:     true
			});

			e = s.option(L.cbi.InputValue, 'ssid', {
				caption:      L.tr('Wireless name'),
				datatype:     'ssid'
			});

			e = s.option(L.cbi.ListValue, 'encryption', {
				caption:	L.tr('Encryption method'),
				initial:	'none'
			})
			.value('none', L.tr('No encryption'))
			.value('psk+ccmp', L.tr('WPA Personal (PSK)'))
			.value('psk2+ccmp', L.tr('WPA2 Personal (PSK)'))
			.value('psk-mixed', L.tr('WPA/WPA2 Personal (PSK) mixed'))
			.value('sae', L.tr('WPA3 Personal (SAE)'))
			.value('sae-mixed', L.tr('WPA2/WPA3 mixed'));

			e.save = function (sid) {
				let radio_2_4 = $('#field_wireless_default_radio0_default_radio0_disabled_hostapd').is(':checked');
				if(radio_2_4){
					L.ui.do_cmd(`hostapd_cli -i wlan0 disable`);
				}else{
					L.ui.do_cmd(`hostapd_cli -i wlan0 enable`);
				}
				var encrypt = this.formvalue(sid);
				L.uci.set('wireless', 'default_radio0', 'encryption', encrypt);
				if (encrypt == 'sae')
					L.uci.set('wireless', 'default_radio0', 'ieee80211w', 2);
				else if (encrypt == 'sae-mixed')
					L.uci.set('wireless', 'default_radio0', 'ieee80211w', 1);
				else L.uci.unset('wireless', 'default_radio0', 'ieee80211w');
				L.uci.save();
			}

			e = s.option(L.cbi.PasswordValue, 'key', {
				caption:    L.tr('Password'),
				datatype:   'wpakey'
			}).depends('encryption', function(v) {
				return (v != 'none');
			});

			var s2 = m.section(L.cbi.NamedSection, 'default_radio1', {
				caption:      L.tr('5G')
			});

			var e2 = s2.option(L.cbi.CheckboxValue, 'disabled_hostapd', {
				caption:      L.tr('Disabled Wifi'),
				optional:     true
			});

			e2 = s2.option(L.cbi.InputValue, 'ssid', {
				caption:      L.tr('Wireless name'),
				datatype:     'ssid'
			});

			e2 = s2.option(L.cbi.ListValue, 'encryption', {
				caption:	L.tr('Encryption method'),
				initial:	'none'
			})
			.value('none', L.tr('No encryption'))
			.value('psk+ccmp', L.tr('WPA Personal (PSK)'))
			.value('psk2+ccmp', L.tr('WPA2 Personal (PSK)'))
			.value('psk-mixed', L.tr('WPA/WPA2 Personal (PSK) mixed'))
			.value('sae', L.tr('WPA3 Personal (SAE)'))
			.value('sae-mixed', L.tr('WPA2/WPA3 mixed'));

			e2.save = function (sid) {
				let radio_5 = $('#field_wireless_default_radio1_default_radio1_disabled_hostapd').is(':checked');
				if(radio_5){
					L.ui.do_cmd(`hostapd_cli -i wlan1 disable`);
				}else{
					L.ui.do_cmd(`hostapd_cli -i wlan1 enable`);
				}
				var encrypt = this.formvalue(sid);
				L.uci.set('wireless', 'default_radio1', 'encryption', encrypt);
				if (encrypt == 'sae')
					L.uci.set('wireless', 'default_radio1', 'ieee80211w', 2);
				else if (encrypt == 'sae-mixed')
					L.uci.set('wireless', 'default_radio1', 'ieee80211w', 1);
				else L.uci.unset('wireless', 'default_radio1', 'ieee80211w');
				L.uci.save();
			}

			e2 = s2.option(L.cbi.PasswordValue, 'key', {
				caption:    L.tr('Password'),
				datatype:   'wpakey'
			}).depends('encryption', function(v) {
				return (v != 'none');
			});

			self.board().then(function (data) {
				m.insertInto('#mainPage').then(function () {
					$('#mainPage').css('width', '800px');
					$('#node_wireless_default_radio0_default_radio0___default__').css({
						width: '700px',
						'margin-left': '30px',
					});
					$('#node_wireless_default_radio1_default_radio1___default__').css({
						width: '700px',
						'margin-left': '30px',
					});
					if (Number(data.total) <= 64*1024*1024) {
						$('#field_wireless_default_radio0_default_radio0_encryption').find('[value="sae"]').css('display', 'none');
						$('#field_wireless_default_radio0_default_radio0_encryption').find('[value="sae-mixed"]').css('display', 'none');
						$('#field_wireless_default_radio1_default_radio1_encryption').find('[value="sae"]').css('display', 'none');
						$('#field_wireless_default_radio1_default_radio1_encryption').find('[value="sae-mixed"]').css('display', 'none');
					}
					else {
						$('#field_wireless_default_radio0_default_radio0_encryption').find('[value="sae"]').css('display', '');
						$('#field_wireless_default_radio0_default_radio0_encryption').find('[value="sae-mixed"]').css('display', '');
						$('#field_wireless_default_radio1_default_radio1_encryption').find('[value="sae"]').css('display', '');
						$('#field_wireless_default_radio1_default_radio1_encryption').find('[value="sae-mixed"]').css('display', '');
					}
				});
			});
		},

		save: L.rpc.declare({
			object: 'file',
			method: 'write',
			params: ['path', 'data', 'append'],
		}),
		cmdhistory: L.rpc.declare({
			object: 'file',
			method: 'read',
			params: ['path'],
			expect: {
				data: '',
			},
		}),
		getlog: L.rpc.declare({
			object: 'luci2.system',
			method: 'syslog',
			expect: { log: '' }
		}),
		loadDebug: function () {
			var self = this;
			var debugwin = $('<div />')
				.addClass('PopWin')
				.attr('id', 'debugwin')
				.append(
					$('<div />')
						.addClass('PopWin_box pop_absolute_center')
						.css('width', '1000px')
						.css('height', '600px')
						.css('margin', '-300px -500px')
						.append(
							$('<div />').attr('id', 'clearhistory').addClass('ic-del').css({
								'background-size': '450%',
								position: 'absolute',
								right: '20px',
								top: '20px',
								'z-index': '100000000',
							}),
						)
						.append(
							$('<div />')
								.addClass('popWin_title table_cell')
								.css({
									width: '1000px',
									position: 'relative',
								})
								.text(L.tr('Debug'))
								.append(
									$('<div />')
										.css({
											'font-size': '12px',
											color: 'rgb(89,89,89)',
											'margin-top': '15px',
											'margin-bottom': '-15px',
										})
										.text(L.tr('Here you can enter commands to debug.')),
								),
						)
						.append(
							$('<div />')
								.addClass('PopWin_content')
								.css({
									position: 'relative',
									width: '950px',
									left: '25px',
									'margin-top': '15px',
								})
								.append(
									$('<div />')
										.css({
											'font-size': '12px',
											position: 'absolute',
											width: '460px',
											'word-wrap': 'break-word',
										})
										.append(
											$('<div />').attr('id', 'log').css({
												overflow: 'hidden auto',
												'word-break': 'break-word',
												'min-height': '17.15px',
												'max-height': '410px',
											}),
										)
										.append(
											$('<div />')
												.css({
													'text-align': 'center',
													'margin-top': '20px',
													'font-size': '12px',
													color: 'rgb(89,89,89)',
													position: 'relative',
												})
												.text(L.tr('Above is the system log.')),
										),
								)
								.append(
									$('<div />')
										.css({
											'font-size': '12px',
											position: 'absolute',
											width: '460px',
											right: '0px',
											top: '0px',
											'word-wrap': 'break-word',
										})
										.append(
											$('<div />').attr('id', 'ch').css({
												overflow: 'hidden auto',
												'word-break': 'break-word',
												'min-height': '17.15px',
												'max-height': '377.3px',
											}),
										)
										.append($('<div />').text('>').css('width', '5px'))
										.append(
											$('<input />').attr('id', 'cmd').css({
												height: '17.15px',
												border: 'none',
												outline: 'none',
												position: 'relative',
												'margin-left': '10px',
												top: '-17.15px',
												'z-index': '1000000000',
											}),
										)
										.append(
											$('<div />')
												.css({
													'text-align': 'center',
													'margin-top': '20px',
													'font-size': '12px',
													color: 'rgb(89,89,89)',
													position: 'relative',
												})
												.text(L.tr('Enter the command in the upper window and press [Enter] to execute the command or press [Esc] to close the debug window.')),
										),
								),
						),
				);

			$('body').append(debugwin);
			var i = 0;
			var cmd, cmd0;
			self.cmdhistory('/tmp/debugcmd').then(function (oldcommand) {
				if (oldcommand) {
					oldcommand = '$ ' + oldcommand.replace(/\n/g, '\n$ ');
					$('#ch').prop('innerText', oldcommand + '\n=====Command History=====');
				} else {
					$('#ch').prop('innerText', '=====No Command History=====');
				}
				cmd0 = $('#ch').prop('innerText').replace('\n=====Command History=====', '');
				cmd0 = cmd0.replace('=====No Command History=====', '');
				cmd0 = cmd0.replace(/\$ /g, '');
				cmd0 = cmd0.split('\n');
				i = cmd0.length;
			});
			self.getlog().then(function (log) {
				log = log.split('\n');
				$('#log').prop('innerText', log.reverse().join('\n'));
			});

			$('#cmd').keypress(function (e) {
				if (e.which == 13) {
					cmd = $('#cmd').val();
					$('#cmd').val('');
					if ($('#ch').prop('innerText') == '=====No Command History=====')
						self.save('/tmp/debugcmd', cmd, true);
					else self.save('/tmp/debugcmd', '\n' + cmd, true);
					$('#ch').prop('innerText', $('#ch').prop('innerText') + '\n$ ' + cmd);
					cmd0 = $('#ch').prop('innerText').replace('\n=====Command History=====', '');
					cmd0 = cmd0.replace('=====No Command History=====', '');
					cmd0 = cmd0.replace(/\$ /g, '');
					cmd0 = cmd0.split('\n');
					i = cmd0.length;
					self.command(cmd);
					self.getlog().then(function (log) {
						log = log.split('\n');
						$('#log').prop('innerText', log.reverse().join('\n'));
					});
					var logfresh = setTimeout(function () {
						clearTimeout(logfresh);
						self.getlog().then(function (log) {
							log = log.split('\n');
							$('#log').prop('innerText', log.reverse().join('\n'));
						});
					}, 5000);
				}
			});
			$('#cmd').keydown(function (e) {
				if (e.which == 38) {
					if (i > 0) {
						i--;
						$('#cmd').val(cmd0[i]);
					} else {
						i = 0;
					}
				} else if (e.which == 40) {
					if (i < cmd0.length) {
						i++;
						$('#cmd').val(cmd0[i]);
					} else {
						$('#cmd').val('');
					}
				}
			});
			$('body').keydown(function (e) {
				if (e.which == 27) {
					$('.PopWin').remove();
				}
			});

			$('#clearhistory').click(function () {
				self.command('rm /tmp/debugcmd');
				$('#ch').prop('innerText', '=====No Command History=====');
			});
		},
		loadTheme: function() {
			var self = this;
			var d = $.Deferred();
			var wds = L.globals.wds;
			self.renderMainMenu().then(function () {
				L.globals.defaultNode = L.globals.mainMenu._nodes.childs.status;
				function check_ifex(check) {
					for (var a in L.globals.mainMenu._nodes.childs) {
						if (check == L.globals.mainMenu._nodes.childs[a].view) return true;
						else {
							var s = L.globals.mainMenu._nodes.childs[a].view.split('/');
							s = s[0];
							if (check == s) return true;
							else {
								for (var b in L.globals.mainMenu._nodes.childs[a].childs) {
									if (check == L.globals.mainMenu._nodes.childs[a].childs[b].view) return true;
								}
							}
						}
					}
					return false;
				}

				$.ajax(L.globals.resource + '/template/theme.htm', {
					cache: true,
					dataType: 'text',
					success: function (data) {
						data = data.replace(/<%([#:=])?(.+?)%>/g, function(match, p1, p2) {
							p2 = p2.replace(/^\s+/, '').replace(/\s+$/, '');
							switch (p1)
							{
							case '#':
								return '';
	
							case ':':
								return L.tr(p2);
	
							case '=':
								return L.globals[p2] || '';
	
							default:
								return '(?' + match + ')';
							}
						});
						$('body').empty().append(data);
					},
				}).then(function () {
					//checkwds
					if (wds) {
						$('#internet').find('*').css('opacity', '0.3');
						$('#device').find('*').css('opacity', '0.3');
					}
					//Action to open debug window
					var if_debug = 0;
					$('.siflower_logo').click(function () {
						if_debug = 1;
						setTimeout(function () {
							if_debug = 0;
						}, 5000);
					});
					$('.productname').click(function () {
						if_debug++;
						if (if_debug == 2) L.ui.loadDebug();
						else if_debug = 0;
					});

					//getBoardInfo to show version in footer
					L.system.getBoardInfo().then(function(info){
						$('#theme_version').text(L.tr("Software Version:") + (info.release.description.split('rel_')[1]).split('_')[0]);
					});

					cv();
					$(window).on('hashchange', function () {
						cv();
					});
					$('#home').click(function () {
						L.setHash('view', 'home');
					});
					$('#internet').click(function () {
						if (wds) return;
						L.setHash('view', 'internet');
					});
					$('#wifi').click(function () {
						L.setHash('view', 'base_wifi');
					});
					$('#device').click(function () {
						if (wds) return;
						L.setHash('view', 'device');
					});
					$('#more').click(function () {
						L.setHash('view', 'status/overview');
					});
					$('#logout_btn').click(function () {
						location.replace('/');
					});

					function cv() {
						var v = L.getHash('view');
						var flag = check_ifex(v);
						if (v == 'home' || v == undefined) {
							$('#home_').removeClass('small_icon menu_home');
							$('#internet_').removeClass('big_icon menu_internet_big');
							$('#wifi_').removeClass('big_icon menu_wifi_big');
							$('#device_').removeClass('big_icon menu_device_big');
							$('#more_').removeClass('big_icon menu_more_big');

							$('#home_').addClass('big_icon menu_home_big');
							$('#internet_').addClass('small_icon menu_internet');
							$('#wifi_').addClass('small_icon menu_wifi');
							$('#device_').addClass('small_icon menu_device');
							$('#more_').addClass('small_icon menu_more');
							L.setHash('view', 'home');
							L.ui.loadHome();
						} else if (v == 'internet' && !wds) {
							$('#home_').removeClass('big_icon menu_home_big');
							$('#internet_').removeClass('small_icon menu_internet');
							$('#wifi_').removeClass('big_icon menu_wifi_big');
							$('#device_').removeClass('big_icon menu_device_big');
							$('#more_').removeClass('big_icon menu_more_big');

							$('#home_').addClass('small_icon menu_home');
							$('#internet_').addClass('big_icon menu_internet_big');
							$('#wifi_').addClass('small_icon menu_wifi');
							$('#device_').addClass('small_icon menu_device');
							$('#more_').addClass('small_icon menu_more');

							$('#morePage').empty().append($('<div />').attr('id', 'mainPage').addClass('container').css('background-color', 'white'));
							L.ui.loadInternet();
						} else if (v == 'base_wifi') {
							$('#home_').removeClass('big_icon menu_home_big');
							$('#internet_').removeClass('big_icon menu_internet_big');
							$('#wifi_').removeClass('small_icon menu_wifi');
							$('#device_').removeClass('big_icon menu_device_big');
							$('#more_').removeClass('big_icon menu_more_big');

							$('#home_').addClass('small_icon menu_home');
							$('#internet_').addClass('small_icon menu_internet');
							$('#wifi_').addClass('big_icon menu_wifi_big');
							$('#device_').addClass('small_icon menu_device');
							$('#more_').addClass('small_icon menu_more');

							$('#morePage').empty().append($('<div />').attr('id', 'mainPage').addClass('container').css('background-color', 'white'));
							L.ui.loadBasewifi();
						} else if (v == 'device' && !wds) {
							$('#home_').removeClass('big_icon menu_home_big');
							$('#internet_').removeClass('big_icon menu_internet_big');
							$('#wifi_').removeClass('big_icon menu_wifi_big');
							$('#device_').removeClass('small_icon menu_device');
							$('#more_').removeClass('big_icon menu_more_big');

							$('#home_').addClass('small_icon menu_home');
							$('#internet_').addClass('small_icon menu_internet');
							$('#wifi_').addClass('small_icon menu_wifi');
							$('#device_').addClass('big_icon menu_device_big');
							$('#more_').addClass('small_icon menu_more');

							L.ui.loadDevice();
						} else if(flag) {
							$('#home_').removeClass('big_icon menu_home_big');
							$('#internet_').removeClass('big_icon menu_internet_big');
							$('#wifi_').removeClass('big_icon menu_wifi_big');
							$('#device_').removeClass('big_icon menu_device_big');
							$('#more_').removeClass('small_icon menu_more');

							$('#home_').addClass('small_icon menu_home');
							$('#internet_').addClass('small_icon menu_internet');
							$('#wifi_').addClass('small_icon menu_wifi');
							$('#device_').addClass('small_icon menu_device');
							$('#more_').addClass('big_icon menu_more_big');
							L.ui.loadMore().then(function () {
								L.ui.load();
								L.setHash('view', v);
								self.changeView();
							});
						} else {
							$('#home_').removeClass('small_icon menu_home');
							$('#internet_').removeClass('big_icon menu_internet_big');
							$('#wifi_').removeClass('big_icon menu_wifi_big');
							$('#device_').removeClass('big_icon menu_device_big');
							$('#more_').removeClass('big_icon menu_more_big');

							$('#home_').addClass('big_icon menu_home_big');
							$('#internet_').addClass('small_icon menu_internet');
							$('#wifi_').addClass('small_icon menu_wifi');
							$('#device_').addClass('small_icon menu_device');
							$('#more_').addClass('small_icon menu_more');
							L.setHash('view', 'home');
							L.ui.loadHome();
						}
					}

					d.resolve();
				});
			});
			return d.promise();
		},

		loadMore: function() {
			var self = this;
			var d = $.Deferred();

			$('head').append($("<link>")
				.attr('type', 'text/css')
				.attr('rel', 'stylesheet')
				.attr('href', L.globals.resource + '/css/theme.css'));

			$.ajax(L.globals.resource + '/template/moreMenu.htm', {
				cache:    true,
				dataType: 'text',
				success:  function(data) {
					$('#morePage').empty().append(data);
				}
			}).then(function() {
				d.resolve();
			});
			return d.promise();
		},

		loadHome: function() {
			var self = this;
			var d = $.Deferred();
			var cname = L.toClassName('home');
			var views = L.views || (L.views = {});

			$.ajax(L.globals.resource + '/template/home.htm', {
				cache: true,
				dataType: 'text',
				success: function (data) {
					data = data.replace(/<%([#:=])?(.+?)%>/g, function(match, p1, p2) {
						p2 = p2.replace(/^\s+/, '').replace(/\s+$/, '');
						switch (p1)
						{
						case '#':
							return '';

						case ':':
							return L.tr(p2);

						case '=':
							return L.globals[p2] || '';

						default:
							return '(?' + match + ')';
						}
					});

					$('#morePage').empty().append(data);
				},
			}).then(function () {

				var url = L.globals.resource + '/view/home.js';
				$.ajax(url, {
					method: 'GET',
					cache: true,
					dataType: 'text',
				}).then(function (data) {
					var viewConstructorSource = ('(function(L, $) { ' + 'return %s' + '})(L, $);\n\n' + '//# sourceURL=%s').format(data, url);

					var viewConstructor = eval(viewConstructorSource);

					views[cname] = new viewConstructor({
						name: 'home',
						acls: {
							"read": {
								"uci": [
									"*"
								],
								"ubus": [
									"*"
								],
							},
							"write": {
								"uci": [
									"*"
								],
								"ubus": [
									"*"
								],
							}
						},
					});
					L.globals.currentView = views[cname];
					views[cname].render.apply(views[cname]);
					d.resolve();
				});
			});
			return d.promise();
		},

		loadDevice: function() {
			var self = this;
			var d = $.Deferred();
			var do_cmd = L.rpc.declare({object: 'web.advance', method: 'cmd', params: [ 'cmd' ]});
			var device_data = [];
			var device_num = 0;
			var localtime = 0;
			L.system.getLocaltimestamp().then(function(info){
				localtime = info;
			});


			function update_device_speed(){
				var get_device_speed = L.rpc.declare({object: 'web.advance', method: 'get_device_speed', expect:{ 'speeds' : [] }});
				get_device_speed().then(function(speeds){
					for(let dev_mac in speeds){
						var mac_id = speeds[dev_mac].macaddr.replace(/:/g, '_')+'_downspeed';
						var down_speed = Number(speeds[dev_mac].ups ? Math.round(speeds[dev_mac].downs / 1024) : "0")
						$('#'+mac_id).text( down_speed + ' KB/s');
					}
				});
			}

			function getdevinfo()
			{
				L.rpc.batch();

				L.uci.callLoad('devlist');
				L.uci.callLoad('wldevlist');

				return L.rpc.flush().then(function(info){
					var rv = { };

					$.extend(rv, info[0]);
					$.extend(rv, info[1]);

					return rv;
				});
			}

			function update_device_ip(){
				var get_device_ip = L.rpc.declare({
					object: 'luci2.network',
					method: 'arp_table',
					expect: { entries: [ ] },
					filter: function(data) {
						var mac_ip = { };
						for (var i = 0; i < data.length; i++)
							if (data[i].macaddr != '00:00:00:00:00:00')
								mac_ip[data[i].macaddr] = data[i].ipaddr;
						return mac_ip;
					}
				});

				get_device_ip().then(function(mac_ip){
					for(let dev_mac in mac_ip){
						var mac_id = dev_mac.toUpperCase().replace(/:/g, '_')+'_ip';
						$('#'+mac_id).text(mac_ip[dev_mac]);
					}
				});
			}

			function get_device_data(){
				//clean data before get
				device_data = [];
				var defer = $.Deferred();
				L.uci.callLoad('devlist').then(function(dev_data){
					L.uci.callLoad('wldevlist').then(function(wldev_data){
						device_data = Object.assign(dev_data, wldev_data);
						defer.resolve();
					});
				});
				return defer.promise();
			};

			//dev_name : devlist or wldevlist; data : html data
			function list_device(dev_name, data, processedMacs) {
				L.uci.callLoad(dev_name).then(function(results){
					for(let dev_mac in results){
						if(results[dev_mac].online == "1"){
							if (processedMacs.has(dev_mac)) {
								continue;
							}
							processedMacs.add(dev_mac);

							var device_htm = data.replace(/Num/g,dev_mac);

							//ip mac
							$('#device_list').append(device_htm);
							$('#'+dev_mac+'_hostname').text(results[dev_mac]["hostname"] ? results[dev_mac]["hostname"] : L.tr("anonymous host"));
							$('#'+dev_mac+'_mac').text((results[dev_mac]["mac"] ? results[dev_mac]["mac"] : "undefined").replace(/_/g, ":"));

							//time
							var uptime = Number(results[dev_mac]["latest_time"] ? results[dev_mac]["latest_time"] : "0");
							var dev_time = localtime - uptime;
							if(uptime == 0 || dev_time < 0)
								dev_time = 0;
							var days = Math.floor(dev_time/86400).toString();
							var hours = Math.floor((dev_time - days*86400)/3600).toString();
							var mins = Math.floor((dev_time - days*86400 - hours*3600)/60).toString();
							$('#'+dev_mac+'_devicetime').text("%s%s %s%s %s%s".format(days, L.tr(" days"), hours, L.tr(" hours"), mins, L.tr(" mins")));

							//forbid devices
							var sw_btn = $('#'+dev_mac+'_switchbtn');
							if(results[dev_mac]["internet"] == "1"){
								sw_btn.prop("status","0").addClass('btn_off').removeClass('btn_on');
							} else {
								sw_btn.prop("status","1").addClass('btn_on').removeClass('btn_off');
							}
							sw_btn.click(function(){
								L.ui.setting(true);
								var forbidden = $('#'+dev_mac+'_switchbtn').prop('status');
								var mac = results[dev_mac]["mac"].replace(/_/g, ":");
								var speedlimit = get_speedlimit(results, dev_mac);
								var cmd;
								if(forbidden == "1"){
									//forbidden = 1 now. after click, not forbit
									L.uci.callSet(dev_name, dev_mac, {"internet":"1"});
									L.uci.callCommit(dev_name);
									$('#'+dev_mac+'_switchbtn').prop("status","0").addClass('btn_off').removeClass('btn_on');
									if(speedlimit == "0"){
										cmd = "echo 3 %s > /proc/ts; aclscript c_net %s 1; aclscript c_lan %s 1".format(mac, mac, mac);
									}
									else{
										cmd = "aclscript c_net %s 1; aclscript c_lan %s 1".format(mac, mac);
									}
								} else {
									L.uci.callSet(dev_name, dev_mac, {"internet":"0"});
									L.uci.callCommit(dev_name);
									$('#'+dev_mac+'_switchbtn').prop("status","1").addClass('btn_on').removeClass('btn_off');
									cmd = "echo 2 %s > /proc/ts; aclscript c_net %s 0; aclscript c_lan %s 0".format(mac, mac, mac);
								}
								//loading.gif disappear too fast so when setting(false) we use '.then(...)'
								do_cmd(cmd).then(function(){L.ui.setting(false);});
							});

							function get_speedlimit(results, dev_mac) {
								var speedlimit = results[dev_mac]["limitdown"] ? results[dev_mac]["limitdown"] : '0';
								if(speedlimit == "-1"){
									speedlimit = "0";
								}
								return speedlimit;
							}
							//speed and limit
							var speedlimit = get_speedlimit(results, dev_mac);
							var speedlimit_en = results[dev_mac]["speedlimit"] ? results[dev_mac]["speedlimit"] : '0';
							var limit_text = speedlimit_en == "0" ? L.tr("No limit") : (speedlimit + " KB/s");
							$('#' + dev_mac + '_downspeed').text('0 KB/s');
							$('#' + dev_mac + '_speedlimit').text(limit_text);
							$('#' + dev_mac + '_changerateId').click(function () {
								var mac = results[dev_mac]["mac"].replace(/_/g, ":");
								var speedlimit = get_speedlimit(results, dev_mac);
								//onkeypress event(keyCode==13) can also cause blur event and change speedlimit
								$('#' + dev_mac + '_speedlimit').text("").append('<input id="' + dev_mac
									+ '_inputlimit" onkeypress=" if(event.keyCode==13) { this.blur();}" maxlength="5" value="' + Number(speedlimit)
									+ '" type="text" class="inputArea paddingleft_14" style="width: 100px; height: 30px; text-align: center;'
									+ ' border: 1px solid rgb(201, 201, 201); padding-right: 14px; border-radius: 8px; outline: none;"> KB/s');
								$('#' + dev_mac + '_changerateId').attr('style', 'display: none;');
								$('#' + dev_mac + '_inputlimit').blur(function () {
									//in this function, need to set uci config first
									var mac = results[dev_mac]["mac"].replace(/_/g, ":");
									var new_speedlimit = $('#' + dev_mac + '_inputlimit').val();
									// 检查是否为非数字，如果是则将 new_speedlimit 设置为 "0"
									if (isNaN(new_speedlimit) || new_speedlimit.trim() === "") {
										new_speedlimit = "0";
									}
									var speedlimit = get_speedlimit(results, dev_mac);
									var speedlimit_en = results[dev_mac]["speedlimit"] ? results[dev_mac]["speedlimit"] : '0';
									var cmd;
									if(new_speedlimit == speedlimit) {
										console.log('not change');
										L.ui.loadDevice();
										return false;
									} else if (new_speedlimit == "0") {
										console.log("speed del");
										var forbidden_limit = $('#'+dev_mac+'_switchbtn').prop('status');
										L.ui.setting(true);
										L.uci.callSet(dev_name, dev_mac, {"limitdown":"-1", "limitup":"-1", "speedlimit":"0"});
										L.uci.callCommit(dev_name);
										if(forbidden_limit == "0"){
											cmd = "pctl speed del %s; echo 3 %s > /proc/ts;".format(mac, mac);
										}
										else{
											cmd = "pctl speed del %s; aclscript c_net %s 0; aclscript c_lan %s 0".format(mac, mac, mac);
										}
									}else {
										L.ui.setting(true);
										L.uci.callSet(dev_name, dev_mac, {"limitdown":new_speedlimit, "limitup":"-1", "speedlimit":"1"});
										L.uci.callCommit(dev_name);
										do_cmd("echo 2 %s > /proc/ts; echo 4 %s > /proc/ts;".format(mac, mac));
										if (speedlimit_en == "1") {
											console.log("speed update");
											//already has limit, update
											cmd = "pctl speed update %s -1 %d".format(mac, Number(new_speedlimit));
										} else {
											console.log("speed add");
											//from no limit to limit, add
											cmd = "pctl speed add %s -1 %d".format(mac, Number(new_speedlimit));
										}
									}
									do_cmd(cmd).then(function(){L.ui.setting(false);});
									L.ui.loadDevice();
								});
							});
						}
					}
				}).then(function(){
					update_device_ip();
				});
			}

			$.ajax(L.globals.resource + '/template/device.htm', {
				cache:    true,
				dataType: 'text',
				success:  function(data) {
					data = data.replace(/<%([#:=])?(.+?)%>/g, function(match, p1, p2) {
						p2 = p2.replace(/^\s+/, '').replace(/\s+$/, '');
						switch (p1)
						{
						case '#':
							return '';

						case ':':
							return L.tr(p2);

						case '=':
							return L.globals[p2] || '';

						default:
							return '(?' + match + ')';
						}
					});

					$('#morePage').empty().append(data);
				}
			}).then(function() {
				$.ajax(L.globals.resource + '/template/device_line.htm', {
					cache:    true,
					dataType: 'text',
					success:  function(data) {
						// Prevent the existence of identical items in devlist and wldevst
						var processedMacs = new Set();
						list_device('wldevlist', data, processedMacs);
						list_device('devlist', data, processedMacs);
					}
				});
			}).then(function(){
				//get online device num first. flag of new device comes
				get_device_data().then(function(){
					var n = 0;
					for(let m in device_data){
						if(device_data[m].online == "1"){
							n = n + 1;
						}
					}
					device_num = n;
				});
			}).then(function(){
				//Update datas every 2 second. Clear old interval before create new one.
				clearInterval(self.deice_interval);
				self.deice_interval = setInterval(function () {
					//update ip and speed
						update_device_ip();
						update_device_speed();
					//leave this page and clear interval
					if(L.getHash("view") != "device"){
						clearInterval(self.deice_interval);
						return;
					}
					//When new device comes, clear interval and load this page again
					get_device_data().then(function(){
						var n = 0;
						for(let m in device_data){
							if(device_data[m].online == "1"){
								$('#'+ m +'_hostname').text(device_data[m]["hostname"] ? device_data[m]["hostname"] : L.tr("anonymous host"));
								L.system.getLocaltimestamp().then(function(time){
									var uptime = Number(device_data[m]["latest_time"] ? device_data[m]["latest_time"] : "0");
									var dev_time = time - uptime;
									if(uptime == 0 || dev_time < 0)
										dev_time = 0;
									var days = Math.floor(dev_time/86400).toString();
									var hours = Math.floor((dev_time - days*86400)/3600).toString();
									var mins = Math.floor((dev_time - days*86400 - hours*3600)/60).toString();
									$('#'+ m +'_devicetime').text("%s%s %s%s %s%s".format(days, L.tr(" days"), hours, L.tr(" hours"), mins, L.tr(" mins")));
								});
								n = n + 1;
							}
						}
						if(device_num != n){
							clearInterval(self.deice_interval);
							L.ui.loadDevice();
						}
					});
				}, 3000);
			});
			return d.promise();
		},

		load: function()
		{
			var self = this;

//			self.loading(true);

			$.when(
				L.session.updateACLs(),
				self.updateHostname(),
				self.updateChanges(),
				self.renderMainMenu(),
				L.network.load()
			).then(function() {
/*				self.renderView(L.globals.defaultNode).then(function() {
//					self.loading(false);
				});

				$(window).on('hashchange', function() {
					self.changeView();
				});*/
			});
		},

		button: function(label, style, title)
		{
			style = style || 'default';

			return $('<button />')
				.attr('type', 'button')
				.attr('title', title ? title : '')
				.addClass('btn btn-' + style)
				.text(label);
		},

		icon: function(src, alt, title)
		{
			if (!src.match(/\.[a-z]+$/))
				src += '.png';

			if (!src.match(/^\//))
				src = L.globals.resource + '/icons/' + src;

			var icon = $('<img />')
				.attr('src', src);

			if (typeof(alt) !== 'undefined')
				icon.attr('alt', alt);

			if (typeof(title) !== 'undefined')
				icon.attr('title', title);

			return icon;
		}
	};

	ui_class.AbstractWidget = Class.extend({
		i18n: function(text) {
			return text;
		},

		label: function() {
			var key = arguments[0];
			var args = [ ];

			for (var i = 1; i < arguments.length; i++)
				args.push(arguments[i]);

			switch (typeof(this.options[key]))
			{
			case 'undefined':
				return '';

			case 'function':
				return this.options[key].apply(this, args);

			default:
				return ''.format.apply('' + this.options[key], args);
			}
		},

		toString: function() {
			return $('<div />').append(this.render()).html();
		},

		insertInto: function(id) {
			return $(id).empty().append(this.render());
		},

		appendTo: function(id) {
			return $(id).append(this.render());
		},

		on: function(evname, evfunc)
		{
			var evnames = L.toArray(evname);

			if (!this.events)
				this.events = { };

			for (var i = 0; i < evnames.length; i++)
				this.events[evnames[i]] = evfunc;

			return this;
		},

		trigger: function(evname, evdata)
		{
			if (this.events)
			{
				var evnames = L.toArray(evname);

				for (var i = 0; i < evnames.length; i++)
					if (this.events[evnames[i]])
						this.events[evnames[i]].call(this, evdata);
			}

			return this;
		}
	});

	ui_class.view = ui_class.AbstractWidget.extend({
		_fetch_template: function()
		{
			return $.ajax(L.globals.resource + '/template/' + this.options.name + '.htm', {
				method: 'GET',
				cache: true,
				dataType: 'text',
				success: function(data) {
					data = data.replace(/<%([#:=])?(.+?)%>/g, function(match, p1, p2) {
						p2 = p2.replace(/^\s+/, '').replace(/\s+$/, '');
						switch (p1)
						{
						case '#':
							return '';

						case ':':
							return L.tr(p2);

						case '=':
							return L.globals[p2] || '';

						default:
							return '(?' + match + ')';
						}
					});

					$('#maincontent').empty().append(data);
				}
			});
		},

		execute: function()
		{
			throw "Not implemented";
		},

		render: function()
		{
			var container = $('#maincontent');

			container.empty();

			if (this.title)
				container.append($('<h2 />').append(this.title));

			if (this.description)
				container.append($('<p />').append(this.description));

			var self = this;
			var args = [ ];

			for (var i = 0; i < arguments.length; i++)
				args.push(arguments[i]);

			return this._fetch_template().then(function() {
				return L.deferrable(self.execute.apply(self, args));
			});
		},

		repeat: function(func, interval)
		{
			var self = this;

			if (!self._timeouts)
				self._timeouts = [ ];

			var index = self._timeouts.length;

			if (typeof(interval) != 'number')
				interval = 5000;

			var setTimer, runTimer;

			setTimer = function() {
				if (self._timeouts)
					self._timeouts[index] = window.setTimeout(runTimer, interval);
			};

			runTimer = function() {
				L.deferrable(func.call(self)).then(setTimer, setTimer);
			};

			runTimer();
		},

		finish: function()
		{
			if ($.isArray(this._timeouts))
			{
				for (var i = 0; i < this._timeouts.length; i++)
					window.clearTimeout(this._timeouts[i]);

				delete this._timeouts;
			}
		}
	});

	ui_class.menu = ui_class.AbstractWidget.extend({
		init: function() {
			this._nodes = { };
		},

		entries: function(entries)
		{
			for (var entry in entries)
			{
				var path = entry.split(/\//);
				var node = this._nodes;

				for (i = 0; i < path.length; i++)
				{
					if (!node.childs)
						node.childs = { };

					if (!node.childs[path[i]])
						node.childs[path[i]] = { };

					node = node.childs[path[i]];
				}

				$.extend(node, entries[entry]);
			}
		},

		sortNodesCallback: function(a, b)
		{
			var x = a.index || 0;
			var y = b.index || 0;
			return (x - y);
		},

		firstChildView: function(node)
		{
			if (node.view)
				return node;

			var nodes = [ ];
			for (var child in (node.childs || { }))
				nodes.push(node.childs[child]);

			nodes.sort(this.sortNodesCallback);

			for (var i = 0; i < nodes.length; i++)
			{
				var child = this.firstChildView(nodes[i]);
				if (child)
				{
					for (var key in child)
						if (!node.hasOwnProperty(key) && child.hasOwnProperty(key))
							node[key] = child[key];

					return node;
				}
			}

			return undefined;
		},

		handleClick: function(ev)
		{
			L.setHash('view', ev.data);

			ev.preventDefault();
			this.blur();
		},

		a_renderN: function(i, sid, dt_sid, dd_sid, li_sid) {
                        $('#mainmenu').on("click", "#" + sid, function () {
                                                $('.' + "_focusall").removeClass('focus').removeClass('on_focus');
                                                $("." + "_icall").removeClass('on_ic').removeClass('ic-menu_up').addClass('ic-menu_down');
                                                $("." + "_ddall").removeClass('on_dd').addClass('hide');
                                                $(this).addClass('focus').addClass('on_focus');
                                                $("#" + sid + "_ic").removeClass('ic-menu_down').addClass('ic-menu_up').addClass('on_ic');
                                                $("." + dd_sid + "_dd" + ".hide").removeClass('hide').addClass('on_dd');
                                        });
                        $('#mainmenu').on("click", "." + dt_sid, function () {
                                                $(this).removeClass('on_focus').removeClass('focus');
                                                $("#" + sid + "_ic" + "." + "on_ic").removeClass('ic-menu_up').removeClass("on_ic").addClass('ic-menu_down');
                                                $("." + dd_sid + "_dd" + ".on_dd").removeClass('on_dd').addClass('hide');

                                        });

                        $('#mainmenu').on("click", "." + li_sid + "_li" , function () {
                                                $("._liall" ).removeClass('on_li').removeClass('active');
                                                $("." + li_sid + "_li" + ".on_li").removeClass('on_li').removeClass('active');
                                                $(this).addClass('active').addClass('on_li');
                                        });
                },

                renderNodes: function(childs, level, min, max)
                {
                        var nodes = [ ];
                        var v_cut = 0;
                        var vi_cut = 0;

                        var list = $('<ul />');

                        if (level == 0) {

                                for (var node in childs)
                        {
                                var child = this.firstChildView(childs[node]);
                                if (child)
                                        nodes.push(childs[node]);
                        }

                        nodes.sort(this.sortNodesCallback);

                                for (var i = 0; i < nodes.length; i++) {


                                        if (!L.globals.defaultNode)
                                        {
                                                var v = L.getHash('view');
                                                if (!v || v == nodes[i].view)
                                                        L.globals.defaultNode = nodes[i];
                                        }

                                        v_cut = nodes[i].view.indexOf("/");
                                        $('#mainmenu')
                                                .append($('<div />')
                                                        .addClass("menu_item ")
                                                        .append($('<div />')
                                                                .attr('id', nodes[i].title)
                                                                .addClass("dt")
								.addClass(nodes[i].view.substring(0, v_cut) + "_dtview")
                                                                .addClass("_focusall")
                                                                .text(L.tr(nodes[i].title) )
                                                                .append($('<div />')
                                                                        .attr('id', nodes[i].title + "_ic")
                                                                        .addClass("_icall")
                                                                        .addClass("ic-menu-arrow ic-menu_down ")
                                                                        .addClass(nodes[i].view.substring(0, v_cut) + "_icview")
                                                                ))
                                                        .append(this.renderNodes(nodes[i].childs, level + 1)));
                                v_cut = nodes[i].view.indexOf("/");
                                this.a_renderN(i, nodes[i].title, "on_focus", nodes[i].view.substring(0, v_cut), nodes[i].view.substring(0, v_cut) );

                                var vi_arr = L.getHash().view;
                                if (typeof(vi_arr) == "undefined")
                                        L.setHash('view', "home");
                                else {
                                var vi_cut = vi_arr.indexOf("/");
                                var vi_first = vi_arr.substring(0, vi_cut);
                                var vi_last = vi_arr.substring(vi_cut+1);

                                $("." + vi_first + "_dtview").addClass('focus').addClass('on_focus');
                                $("." + vi_first + "_dd").removeClass('hide').addClass('on_dd');
                                $("." + vi_first + "_icview").removeClass('ic-menu_down').addClass('ic-menu_up').addClass('on_ic');
                                $("." + vi_last + "_liview").addClass('active').addClass('on_li');}

                                }
                        }
                        else if (level == 1) {

                        for (var node in childs)
                        {
                                var child = this.firstChildView(childs[node]);
                                if (child)
                                        nodes.push(childs[node]);
                        }

                        nodes.sort(this.sortNodesCallback);
                                for (var i = 0; i < nodes.length; i++) {

                                        if (!L.globals.defaultNode)
                                        {
                                                var v = L.getHash('view');
                                                if (!v || v == nodes[i].view)
                                                        L.globals.defaultNode = nodes[i];
                                        }
                                        v_cut = nodes[i].view.indexOf("/");
                                        list.addClass('dd hide _ddall').addClass(nodes[i].view.substring(0, v_cut) + "_dd");
                                        var item = $('<li />')
                                                .attr('id', 'li_a_s' + i)
                                                .addClass("_liall")
                                                .addClass(nodes[i].view.substring(v_cut + 1) + "_liview")
                                                .addClass(nodes[i].view.substring(0, v_cut) + "_li")
                                                .text(L.tr(nodes[i].title))
                                                .appendTo(list);

                                        if (nodes[i].childs && level < max)
                                        {
                                        }
                                        else
                                        {
						list.find('#li_a_s' + i).click(nodes[i].view, this.handleClick);
                                        }
                                }
                        }

                        return list.get(0);
                },

		render: function(min, max)
		{
			var top = min ? this.getNode(L.globals.defaultNode.view, min) : this._nodes;
			return this.renderNodes(top.childs, 0, min, max);
		},

		getNode: function(path, max)
		{
			var p = path.split(/\//);
			var n = this._nodes;

			if (!n.childs)
				return undefined;

			if (typeof(max) == 'undefined')
				max = p.length;

			if (max > p.length)
				max = p.length;

			for (var i = 0; i < max; i++)
			{
				if (!n.childs[p[i]])
					return undefined;

				n = n.childs[p[i]];
			}

			return n;
		}
	});

	ui_class.table = ui_class.AbstractWidget.extend({
		init: function()
		{
			this._rows = [ ];
		},

		row: function(values)
		{
			if ($.isArray(values))
			{
				this._rows.push(values);
			}
			else if ($.isPlainObject(values))
			{
				var v = [ ];
				for (var i = 0; i < this.options.columns.length; i++)
				{
					var col = this.options.columns[i];

					if (typeof col.key == 'string')
						v.push(values[col.key]);
					else
						v.push(null);
				}
				this._rows.push(v);
			}
		},

		rows: function(rows)
		{
			for (var i = 0; i < rows.length; i++)
				this.row(rows[i]);
		},

		render: function(id)
		{
			var fieldset = document.createElement('fieldset');
				fieldset.className = 'cbi-section';

			if (this.options.caption)
			{
				var legend = document.createElement('legend');
				$(legend).append(this.options.caption);
				fieldset.appendChild(legend);
			}

			var table = document.createElement('table');
				table.className = 'table table-condensed table-hover';

			var has_caption = false;
			var has_description = false;

			for (var i = 0; i < this.options.columns.length; i++)
				if (this.options.columns[i].caption)
				{
					has_caption = true;
					break;
				}
				else if (this.options.columns[i].description)
				{
					has_description = true;
					break;
				}

			if (has_caption)
			{
				var tr = table.insertRow(-1);
					tr.className = 'cbi-section-table-titles';

				for (var i = 0; i < this.options.columns.length; i++)
				{
					var col = this.options.columns[i];
					var th = document.createElement('th');
						th.className = 'cbi-section-table-cell';

					tr.appendChild(th);

					if (col.width)
						th.style.width = col.width;

					if (col.align)
						th.style.textAlign = col.align;

					if (col.caption)
						$(th).append(col.caption);
				}
			}

			if (has_description)
			{
				var tr = table.insertRow(-1);
					tr.className = 'cbi-section-table-descr';

				for (var i = 0; i < this.options.columns.length; i++)
				{
					var col = this.options.columns[i];
					var th = document.createElement('th');
						th.className = 'cbi-section-table-cell';

					tr.appendChild(th);

					if (col.width)
						th.style.width = col.width;

					if (col.align)
						th.style.textAlign = col.align;

					if (col.description)
						$(th).append(col.description);
				}
			}

			if (this._rows.length == 0)
			{
				if (this.options.placeholder)
				{
					var tr = table.insertRow(-1);
					var td = tr.insertCell(-1);
						td.className = 'cbi-section-table-cell';

					td.colSpan = this.options.columns.length;
					$(td).append(this.options.placeholder);
				}
			}
			else
			{
				for (var i = 0; i < this._rows.length; i++)
				{
					var tr = table.insertRow(-1);

					for (var j = 0; j < this.options.columns.length; j++)
					{
						var col = this.options.columns[j];
						var td = tr.insertCell(-1);

						var val = this._rows[i][j];

						if (typeof(val) == 'undefined')
							val = col.placeholder;

						if (typeof(val) == 'undefined')
							val = '';

						if (col.width)
							td.style.width = col.width;

						if (col.align)
							td.style.textAlign = col.align;

						if (typeof col.format == 'string')
							$(td).append(col.format.format(val));
						else if (typeof col.format == 'function')
							$(td).append(col.format(val, i));
						else
							$(td).append(val);
					}
				}
			}

			this._rows = [ ];
			fieldset.appendChild(table);

			return fieldset;
		}
	});

	ui_class.grid = ui_class.AbstractWidget.extend({
		init: function()
		{
			this._rows = [ ];
		},

		row: function(values)
		{
			if ($.isArray(values))
			{
				this._rows.push(values);
			}
			else if ($.isPlainObject(values))
			{
				var v = [ ];
				for (var i = 0; i < this.options.columns.length; i++)
				{
					var col = this.options.columns[i];

					if (typeof col.key == 'string')
						v.push(values[col.key]);
					else
						v.push(null);
				}
				this._rows.push(v);
			}
		},

		rows: function(rows)
		{
			for (var i = 0; i < rows.length; i++)
				this.row(rows[i]);
		},

		createCell: function(col, classNames)
		{
			var sizes = [ 'xs', 'sm', 'md', 'lg' ];

			var cell = $('<div />')
				.addClass('cell clearfix');

			if (classNames)
				cell.addClass(classNames);

			if (col.nowrap)
				cell.addClass('nowrap');

			if (col.align)
				cell.css('text-align', col.align);

			for (var i = 0; i < sizes.length; i++)
				cell.addClass((col['width_' + sizes[i]] > 0)
					? 'col-%s-%d'.format(sizes[i], col['width_' + sizes[i]])
					: 'hidden-%s'.format(sizes[i]));

			if (col.hidden)
				cell.addClass('hidden-%s'.format(col.hidden));

			return cell;
		},

		render: function(id)
		{
			var fieldset = $('<fieldset />')
				.addClass('cbi-section');

			if (this.options.caption)
				fieldset.append($('<legend />').append(this.options.caption));

			var grid = $('<div />')
				.addClass('luci2-grid luci2-grid-hover');

			if (this.options.condensed)
				grid.addClass('luci2-grid-condensed');

			var has_caption = false;
			var has_description = false;

			var sizes = [ 'xs', 'sm', 'md', 'lg' ];

			for (var i = 0; i < sizes.length; i++)
			{
				var size = sizes[i];
				var width_unk = 0;
				var width_dyn = 0;
				var width_rem = 12;

				for (var j = 0; j < this.options.columns.length; j++)
				{
					var col = this.options.columns[j];
					var k = i, width = NaN;

					do { width = col['width_' + sizes[k++]]; }
						while (isNaN(width) && k < sizes.length);

					if (isNaN(width))
						width = col.width;

					if (isNaN(width))
						width_unk++;
					else
						width_rem -= width, col['width_' + size] = width;

					if (col.caption)
						has_caption = true;

					if (col.description)
						has_description = true;
				}

				if (width_unk > 0)
					width_dyn = Math.floor(width_rem / width_unk);

				for (var j = 0; j < this.options.columns.length; j++)
					if (isNaN(this.options.columns[j]['width_' + size]))
						this.options.columns[j]['width_' + size] = width_dyn;
			}

			if (has_caption)
			{
				var row = $('<div />')
					.addClass('row')
					.appendTo(grid);

				for (var i = 0; i < this.options.columns.length; i++)
				{
					var col = this.options.columns[i];
					var cell = this.createCell(col, 'caption')
						.appendTo(row);

					if (col.caption)
						cell.append(col.caption);
				}
			}

			if (has_description)
			{
				var row = $('<div />')
					.addClass('row')
					.appendTo(grid);

				for (var i = 0; i < this.options.columns.length; i++)
				{
					var col = this.options.columns[i];
					var cell = this.createCell(col, 'description')
						.appendTo(row);

					if (col.description)
						cell.append(col.description);
				}
			}

			if (this._rows.length == 0)
			{
				if (this.options.placeholder)
					$('<div />')
						.addClass('row')
						.append($('<div />')
							.addClass('col-md-12 cell placeholder clearfix')
							.append(this.options.placeholder))
						.appendTo(grid);
			}
			else
			{
				for (var i = 0; i < this._rows.length; i++)
				{
					var row = $('<div />')
						.addClass('row')
						.appendTo(grid);

					for (var j = 0; j < this.options.columns.length; j++)
					{
						var col = this.options.columns[j];
						var cell = this.createCell(col, 'content')
							.appendTo(row);

						var val = this._rows[i][j];

						if (typeof(val) == 'undefined')
							val = col.placeholder;

						if (typeof(val) == 'undefined')
							val = '';

						if (typeof col.format == 'string')
							cell.append(col.format.format(val));
						else if (typeof col.format == 'function')
							cell.append(col.format(val, i));
						else
							cell.append(val);
					}
				}
			}

			this._rows = [ ];

			return fieldset.append(grid);
		}
	});

	ui_class.hlist = ui_class.AbstractWidget.extend({
		render: function()
		{
			if (!$.isArray(this.options.items))
				return '';

			var list = $('<span />');
			var sep = this.options.separator || ' | ';
			var items = [ ];

			for (var i = 0; i < this.options.items.length; i += 2)
			{
				if (typeof(this.options.items[i+1]) === 'undefined' ||
				    this.options.items[i+1] === '')
					continue;

				items.push(this.options.items[i], this.options.items[i+1]);
			}

			for (var i = 0; i < items.length; i += 2)
			{
				list.append($('<span />')
						.addClass('nowrap')
						.append($('<strong />')
							.append(items[i])
							.append(': '))
						.append(items[i+1])
						.append(((i+2) < items.length) ? sep : ''))
					.append(' ');
			}

			return list;
		}
	});

	ui_class.progress = ui_class.AbstractWidget.extend({
		render: function()
		{
			var vn = parseInt(this.options.value) || 0;
			var mn = parseInt(this.options.max) || 100;
			var pc = Math.floor((100 / mn) * vn);

			var text;

			if (typeof(this.options.format) == 'string')
				text = this.options.format.format(this.options.value, this.options.max, pc);
			else if (typeof(this.options.format) == 'function')
				text = this.options.format(pc);
			else
				text = '%.2f%%'.format(pc);

			return $('<div />')
				.addClass('progress')
				.append($('<div />')
					.addClass('progress-bar')
					.addClass('progress-bar-info')
					.css('width', pc + '%'))
				.append($('<small />')
					.text(text));
		}
	});

	ui_class.devicebadge = ui_class.AbstractWidget.extend({
		render: function()
		{
			var l2dev = this.options.l2_device || this.options.device;
			var l3dev = this.options.l3_device;
			var dev = l3dev || l2dev || '?';

			var span = document.createElement('span');
				span.className = 'badge';

			if (typeof(this.options.signal) == 'number' ||
				typeof(this.options.noise) == 'number')
			{
				var r = 'none';
				if (typeof(this.options.signal) != 'undefined' &&
					typeof(this.options.noise) != 'undefined')
				{
					var q = (-1 * (this.options.noise - this.options.signal)) / 5;
					if (q < 1)
						r = '0';
					else if (q < 2)
						r = '0-25';
					else if (q < 3)
						r = '25-50';
					else if (q < 4)
						r = '50-75';
					else
						r = '75-100';
				}

				span.appendChild(document.createElement('img'));
				span.lastChild.src = L.globals.resource + '/icons/signal-' + r + '.png';

				if (r == 'none')
					span.title = L.tr('No signal');
				else
					span.title = '%s: %d %s / %s: %d %s'.format(
						L.tr('Signal'), this.options.signal, L.tr('dBm'),
						L.tr('Noise'), this.options.noise, L.tr('dBm')
					);
			}
			else
			{
				var type = 'ethernet';
				var desc = L.tr('Ethernet device');

				if (l3dev != l2dev)
				{
					type = 'tunnel';
					desc = L.tr('Tunnel interface');
				}
				else if (dev.indexOf('br-') == 0)
				{
					type = 'bridge';
					desc = L.tr('Bridge');
				}
				else if (dev.indexOf('.') > 0)
				{
					type = 'vlan';
					desc = L.tr('VLAN interface');
				}
				else if (dev.indexOf('wlan') == 0 ||
						 dev.indexOf('ath') == 0 ||
						 dev.indexOf('wl') == 0)
				{
					type = 'wifi';
					desc = L.tr('Wireless Network');
				}

				span.appendChild(document.createElement('img'));
				span.lastChild.src = L.globals.resource + '/icons/' + type + (this.options.up ? '' : '_disabled') + '.png';
				span.title = desc;
			}

			$(span).append(' ');
			$(span).append(dev);

			return span;
		}
	});

	return Class.extend(ui_class);
})();
