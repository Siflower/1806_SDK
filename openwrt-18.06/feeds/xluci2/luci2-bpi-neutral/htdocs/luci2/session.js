Class.extend({
	login: L.rpc.declare({
		object: 'session',
		method: 'login',
		params: [ 'username', 'password' ],
		expect: { '': { } }
	}),

	access: L.rpc.declare({
		object: 'session',
		method: 'access',
		params: [ 'scope', 'object', 'function' ],
		expect: { access: false }
	}),

	isAlive: function()
	{
		return L.session.access('ubus', 'session', 'access');
	},

	startHeartbeat: function()
	{
		this._hearbeatInterval = window.setInterval(function() {
			L.session.isAlive().then(function(alive) {
				if (!alive)
				{
					L.session.stopHeartbeat();
					L.ui.login(true);
				}

			});
		}, L.globals.timeout * 2);

		// 添加用户交互检测机制
		this.resetLogoutTimer();
		var self = this;
		['mousemove', 'keydown', 'click', 'scroll', 'touchstart'].forEach(function(event) {
			window.addEventListener(event, function() {
				self.resetLogoutTimer();
			});
		});
	},

	stopHeartbeat: function()
	{
		if (typeof(this._hearbeatInterval) != 'undefined')
		{
			window.clearInterval(this._hearbeatInterval);
			delete this._hearbeatInterval;
		}
		if (typeof(this._logoutTimer) !== 'undefined') {
			window.clearTimeout(this._logoutTimer);
			delete this._logoutTimer;
		}
	},

	// 重置5分钟不活动自动登出的定时器
	resetLogoutTimer: function() {
		var self = this;
		if (typeof(this._logoutTimer) !== 'undefined') {
			window.clearTimeout(this._logoutTimer);
		}
		this._logoutTimer = window.setTimeout(function() {
			self.logoutDueToInactivity();
		}, 300000); // 300000毫秒 = 5分钟
	},

	// 用户5分钟无操作后自动注销
	logoutDueToInactivity: function() {
		this.stopHeartbeat();
		L.ui.login(true); // 触发登录界面显示
		location.replace('/');
	},


	aclCache: { },

	callAccess: L.rpc.declare({
		object: 'session',
		method: 'access',
		expect: { '': { } }
	}),

	callAccessCallback: function(acls)
	{
		L.session.aclCache = acls;
	},

	updateACLs: function()
	{
		return L.session.callAccess()
			.then(L.session.callAccessCallback);
	},

	hasACL: function(scope, object, func)
	{
		var acls = L.session.aclCache;

		if (typeof(func) == 'undefined') {
			return (acls && acls[scope] && acls[scope][object]);
		}

		if (acls && acls[scope] && acls[scope][object]) {
			for (var i = 0; i < acls[scope][object].length; i++) {
				if (acls[scope][object][i] == func) {
					return true;
				}
			}
		}

		return false;
	}
});
