L.ui.view.extend({

	handleLogout: function() {
		var form = $('<p />').text(L.tr('Really logout?'));
		L.ui.dialog(L.tr('Logout'), form, {
			style: 'confirm',
			confirm: function() {
				location.replace('/');  // 重定向到主页
			},
		});
	},
		
	execute: function() {
		var self = this;

		$('#btn_logout').click(function() {self.handleLogout();});

	}
});
