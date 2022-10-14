L.ui.view.extend({
	title: L.tr('Port adaptive'),

	get_Port: L.rpc.declare({
		object: 'luci2.advance',
		method: 'getport',
		expect: { }
	}),

	set_Port: L.rpc.declare({
		object: 'luci2.advance',
		method: 'setport',
		params: [ 'data' ],
	}),

	start_set: L.rpc.declare({
		object: 'luci2.advance',
		method: 'start',
	}),

	set_Boot: L.rpc.declare({
		object: 'luci2.advance',
		method: 'setboot',
		params: [ 'data' ],
	}),

	execute: function() {
		var self = this;
		$('#p1').click(function() {
			self.set_Port("1").then(function() {
				window.location.reload();
			});
		});
		$('#p2').click(function() {
			self.set_Port("2").then(function() {
				window.location.reload();
			});
		});
		$('#p3').click(function() {
			self.set_Port("3").then(function() {
				window.location.reload();
			});
		});
		$('#p4').click(function() {
			self.set_Port("4").then(function() {
				window.location.reload();
			});
		});
		$('#p5').click(function() {
			self.set_Port("5").then(function() {
				window.location.reload();
			});
		});
		$('#start').click(function() {
			$('#info').text('Start set, please wait...');
			self.start_set().then(function() {
				window.location.reload();
			});
		});
		$('#boot').click(function() {
			if($('#boot').text() == "ON"){
				self.set_Boot("0").then(function() {
					window.location.reload();
				});
			} else {
				self.set_Boot("1").then(function() {
					window.location.reload();
				});
			}
		});
		self.get_Port().then(function(result) {
			if(result.port > 0){
				$('#p'+ result.port).text("wan");
				$('#p'+ result.port).removeClass("btn btn-save").addClass("btn btn-primary");
			}
			if(result.enable != 0){
				$('#boot').text("ON");
				$('#boot').removeClass("btn btn-danger").addClass("btn btn-success");
			}
			if(result.name == "ac28"){
				$('#p5').css("display","none");
			}
		});
		return 0;
	}
});
