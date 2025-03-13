L.ui.view.extend({
	cmd: L.rpc.declare({
		object: 'web.advance',
		method: 'cmd',
		params: ['cmd'],
	}),

	execute: function () {
		var self = this;
		var loop1;
		var ifoncap = false;
		var ifondownload = 0;
		$('#capture').val(L.tr('Start Capture'));
		function loadbox(msg) {
			var load = $('<div />')
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
								.append($('<div />').addClass('loading_word').css('height', '61px').append($('<p />').text(L.tr(msg)))),
						),
				);
			return load;
		}

		window.addEventListener('beforeunload', function () {
			if (ifondownload == 2) {
				ifondownload = 1;
				setTimeout(function () { $('.loading').remove(); }, 5000);
			}
			else if (ifondownload == 1) { 
				stop_cap(0);
				$('.loading').remove();
			}
		});

		$(window).one('hashchange', function () {
			if (ifondownload == 1) {
				stop_cap(0);
				$('.loading').remove();
			}
		});

		$('#timeset').on('click', function () {
			if ($('#timeset').val() == 'Custom') $('#timecustom').show();
			else $('#timecustom').hide();
		});

		function start_cap() {
			if ($('#timeset').val() == '15mins') stop_cap(900000);
			else if ($('#timeset').val() == 'Custom') {
				if ($('#ctime').val().length < 1) {
					$('#ctime').parent().addClass('has-error');
					$('#error').text(L.tr('Field must not be empty')).show();
					return;
				}
				stop_cap(parseInt($('#ctime').val()) * 60 * 1000);
			}
			$('body').append(loadbox('Waiting for the download to start, and do nothing during this time.'));
			var forchrome = setTimeout(function () {
				clearTimeout(forchrome);
				$('.loading').remove();
				if (ifondownload == 2) ifondownload = 1;
			}, 6000);
			$('#capture').val(L.tr('Stop Capture'));
			var form = $('#capture').parent();
			form.find('[name=sessionid]').val(L.globals.sid);
			form.submit();
			ifondownload = 2;
			ifoncap = true;
		}

		function stop_cap(stop) {
			if (loop1 != undefined) {
				clearTimeout(loop1);
				loop1 = undefined;
			}
			var stopcap = setTimeout(function () {
				clearTimeout(stopcap);
				self.cmd('killall -9 tcpdump');
				ifondownload = 0;
				$('#capture').val(L.tr('Start Capture'));
				$('body').append(loadbox('Waiting for the download to stop, and do nothing during this time.'));
				var show = setTimeout(function () {
					clearTimeout(show);
					$('.loading').remove();
				}, 5000);
				ifoncap = false;
			}, stop);
			loop1 = stopcap;
		}

		$('#ctime').on('keyup', function () {
			if ($('#ctime').val().match(/^-?[0-9]+$/) != null && parseInt($('#ctime').val()) > 0 && parseInt($('#ctime').val()) < 60) {
				$('#ctime').parent().removeClass('has-error');
				$('#error').hide();
			} else if ($('#ctime').val().length < 1) {
				$('#ctime').parent().addClass('has-error');
				$('#error').text(L.tr('Field must not be empty')).show();
			}else{
				$('#ctime').parent().addClass('has-error');
				$('#error').text(L.tr('Must be a number between %d and %d').format(1,60)).show();
			}
		});

		$('#capture').click(function () {
			if (!ifoncap) start_cap();
			else stop_cap(0);
		});
	},
});

