L.ui.view.extend({

	testUpgrade: L.rpc.declare({
		object: 'luci2.system',
		method: 'upgrade_test',
		expect: { '': { } }
	}),

	startUpgrade: L.rpc.declare({
		object: 'luci2.system',
		method: 'upgrade_start',
		params: [ 'keep' ]
	}),

	cleanUpgrade: L.rpc.declare({
		object: 'luci2.system',
		method: 'upgrade_clean'
	}),

	restoreBackup: L.rpc.declare({
		object: 'luci2.system',
		method: 'backup_restore'
	}),

	cleanBackup: L.rpc.declare({
		object: 'luci2.system',
		method: 'backup_clean'
	}),

	getBackupConfig: L.rpc.declare({
		object: 'luci2.system',
		method: 'backup_config_get',
		expect: { config: '' }
	}),

	setBackupConfig: L.rpc.declare({
		object: 'luci2.system',
		method: 'backup_config_set',
		params: [ 'data' ]
	}),

	listBackup: L.rpc.declare({
		object: 'luci2.system',
		method: 'backup_list',
		expect: { files: [ ] }
	}),

	testReset: L.rpc.declare({
		object: 'luci2.system',
		method: 'reset_test',
		expect: { supported: false }
	}),

	startReset: L.rpc.declare({
		object: 'luci2.system',
		method: 'reset_start'
	}),

	getVersion: L.rpc.declare({
		object: 'system',
		method: 'board',
	}),

	callSet: L.rpc.declare({
		object: 'uci',
		method: 'set',
		params: ['config', 'section', 'values']
	}),

	callLoad: L.rpc.declare({
		object: 'uci',
		method: 'get',
		params: ['config', 'section', 'option'],
		expect: { value: ' ' }
	}),

	callShow: L.rpc.declare({
		object: 'uci',
		method: 'get',
		params: ['config']
	}),

	callCommit: L.rpc.declare({
		object: 'uci',
		method: 'commit',
		params: ['config']
	}),

	get_server_addr: L.rpc.declare({
		object: 'web.system',
		method: 'get_server_addr',
	}),

	write: L.rpc.declare({
		object: 'file',
		method: 'write',
		params: ['path', 'data']
	}),

	log_write: L.rpc.declare({
		object: 'log',
		method: 'write',
		params: ['event']
	}),

	exec: L.rpc.declare({
		object: 'file',
		method: 'exec',
		params: ['command']
	}),

	stat: L.rpc.declare({
		object: 'file',
		mathod: 'stat',
		params: ['path']
	}),

	md5sum: L.rpc.declare({
		object: 'file',
		method: 'md5',
		params: ['path']
	}),

	startOtaUpgrade: L.rpc.declare({
		object: 'luci2.system',
		method: 'upgrade_start',
	}),

	get_img_version: L.rpc.declare({
		object: 'web.advance',
		method: 'get_img_version',
		expect: { 'img_version' : '' }
	}),

	curl_cmd: L.rpc.declare({
		object: 'web.advance',
		method: 'cmd',
		params: ['cmd'],
		timeout: 120000,
		expect: { 'result' : '' }
	}),

	cmd: L.rpc.declare({
		object: 'web.advance',
		method: 'cmd',
		params: [ 'cmd' ],
	}),

	delete_tmp_bin: L.rpc.declare({
		object: 'web.advance',
		method: 'delete_tmp_bin',
	}),

	handleFlashUpload: function() {
		var self = this;
		L.ui.upload(
			L.tr('Firmware upload'),
			L.tr('Select the sysupgrade image to flash and click "%s" to proceed.').format(L.tr('Ok')), {
				filename: '/tmp/firmware.bin',
				success: function(info) {
					self.handleFlashVerify(info);
				}
			}
		);
	},

	handleFlashVerify: function(info) {
		var self = this;
		self.testUpgrade().then(function(res) {
			if (res.code == 0) {
				var form = [
						$('<p />').text(L.tr('The firmware image was uploaded completely. Please verify the checksum and file size below, then click "%s" to start the flash procedure.').format(L.tr('Ok'))),
						$('<ul />')
							.append($('<li />')
								.append($('<strong />').text(L.tr('Checksum') + ': '))
								.append(info.checksum))
							.append($('<li />')
								.append($('<strong />').text(L.tr('Size') + ': '))
								.append('%1024mB'.format(info.size))),
						$('<label />')
							.append($('<input />')
								.attr('type', 'checkbox')
								.attr('name', 'keep')
								.prop('checked', true))
							.append(' ')
							.append(L.tr('Keep configuration when reflashing'))
					];

				L.ui.dialog(
					L.tr('Verify firmware'), form, {
						style: 'confirm',
						confirm: function() {
							var keep = form[2].find("input[name='keep']").prop("checked");
							self.startUpgrade(keep).then(function() {
								L.session.stopHeartbeat();
								L.ui.reconnect();
							});
						}
					}
				);
			} else {
				L.ui.dialog(
					L.tr('Invalid image'), [
						$('<p />').text(L.tr('The uploaded image file does not contain a supported format. Make sure that you choose the generic image format for your platform.'))
					], {
						style: 'close',
						close: function() {
							self.cleanUpgrade().then(function() {
								L.ui.dialog(false);
							});
						}
					}
				);
			}
		});
	},

	handleBackupUpload: function() {
		var self = this;
		L.ui.upload(
			L.tr('Backup restore'),
			L.tr('Select the backup archive to restore and click "%s" to proceed.').format(L.tr('Ok')), {
				filename: '/tmp/backup.tar.gz',
				success: function(info) {
					self.handleBackupVerify(info);
				}
			}
		);
	},

	handleBackupVerify: function(info) {
		var self = this;
		L.ui.dialog(
			L.tr('Backup restore'), [
				$('<p />').text(L.tr('The backup archive was uploaded completely. Please verify the checksum and file size below, then click "%s" to restore the archive.').format(L.tr('Ok'))),
				$('<ul />')
					.append($('<li />')
						.append($('<strong />').text(L.tr('Checksum') + ': '))
						.append(info.checksum))
					.append($('<li />')
						.append($('<strong />').text(L.tr('Size') + ': '))
						.append('%1024mB'.format(info.size)))
			], {
				style: 'confirm',
				confirm: function() {
					self.handleBackupRestore();
				}
			}
		);
	},
	handleBackupRestore: function() {
		var self = this;
		self.restoreBackup().then(function(res) {
			if (res.code == 0) {
				L.ui.dialog(
					L.tr('Backup restore'), [
						$('<p />').text(L.tr('The backup was successfully restored, it is advised to reboot the system now in order to apply all configuration changes.')),
						$('<input />')
							.addClass('cbi-button')
							.attr('type', 'button')
							.attr('value', L.tr('Reboot system'))
							.click(function() {
								L.system.performReboot().then(function(){
									L.ui.reconnect(L.tr('Device rebooting...'));
								});
							})
					], {
						style: 'close',
						close: function() {
							self.cleanBackup().then(function() {
								L.ui.dialog(false);
							});
						}
					}
				);
			} else {
				L.ui.dialog(
					L.tr('Backup restore'), [
						$('<span />').text(L.tr('Backup restoration failed, Make sure that you choose the file format for your platform.')),
						$('<span />').text(L.tr('Please use the .tar.gz file provided for generating backups.'))
					], {
						style: 'close',
						close: function() {
							self.cleanBackup().then(function() {
								L.ui.dialog(false);
							});
						}
					}
				);
			}
		});
	},

	handleBackupDownload: function() {
		var form = $('#btn_backup').parent();

		form.find('[name=sessionid]').val(L.globals.sid);
		form.submit();
	},

	handleFactoryUpload: function() {
		var self = this;
		L.ui.upload(
			L.tr('Factory restore'),
			L.tr('Select the factory bin archive to restore and click "%s" to proceed.').format(L.tr('Ok')), {
				filename: '/tmp/factory.bin',
				success: function(info) {
					self.handleFactoryVerify(info);
				}
			}
		);
	},

	handleFactoryVerify: function(info) {
		var self = this;
		L.ui.dialog(
			L.tr('Factory restore'), [
				$('<p />').text(L.tr('The factory archive was uploaded completely. Please verify the checksum and file size below, then click "%s" to restore the archive.').format(L.tr('Ok'))),
				$('<ul />')
					.append($('<li />')
						.append($('<strong />').text(L.tr('Checksum') + ': '))
						.append(info.checksum))
					.append($('<li />')
						.append($('<strong />').text(L.tr('Size') + ': '))
						.append('%1024mB'.format(info.size)))
			], {
				style: 'confirm',
				confirm: function() {
					self.handleFactoryRestore();
				}
			}
		);
	},

	handleFactoryRestore: function() {
		var self = this;
		self.cmd("/www/luci2/scripts/factory_backup restore_backup");
		L.ui.dialog(
			L.tr('Factory restore'), [
				$('<p />').text(L.tr('The factory was successfully restored.')),
			], {
				style: 'close',
				close: function() {
					L.ui.dialog(false);
				}
			}
		);
	},

	handleFactoryDownload: function() {
		var form = $('#btn_factory_backup').parent();

		form.find('[name=sessionid]').val(L.globals.sid);
		form.submit();
	},

	handleReset: function() {
		var self = this;
		L.ui.dialog(L.tr('Really reset all changes?'), L.tr('This will reset the system to its initial configuration, all changes made since the initial flash will be lost!'), {
			style: 'confirm',
			confirm: function() {
				self.cmd("sh /bin/sf_reset.sh");
				L.ui.reconnect();
			}
		});
	},

	need_upgrade: function(ota_ver, img_ver) {
		return new Promise(function(resolve, reject) {
			if (img_ver && ota_ver) {
				if (ota_ver > img_ver) {
					resolve(true);
				} else {
					resolve(false);
				}
			} else {
				console.log("No img_ver.");
				resolve(false);
			}
		});
	},

	ota_upgrade: function () {
		var self = this;
		self.curl_cmd("sh /usr/sbin/ota request_url").then(function(data) {
			if (data != "success") {
				self.log_write('Failed to get the reply of request, please retry.');
				L.ui.dialog(
					L.tr(''), [
					$('<p />').text(L.tr('ota version get failed.')),
				],
					{
						style: 'close',
						close: function () {
							self.cleanBackup().then(function () {
								L.ui.dialog(false);
							});
						}
					}
				);
			} else {
				L.uci.callLoad('basic_setting').then(function(data) {
					if(data == undefined || data['ota']['downloadUrl'] == undefined || data['ota']['md5Str'] == undefined || data['ota']['ota_version'] == undefined) {
						self.log_write('Failed to get the reply of request, please retry.');
						L.ui.dialog(
							L.tr(''), [
							$('<p />').text(L.tr('ota version get failed.')),
						],
							{
								style: 'close',
								close: function () {
									self.cleanBackup().then(function () {
										L.ui.dialog(false);
									});
								}
							}
						);
					} else {					
						console.log(data)
						var downloadUrl = data['ota']['downloadUrl'];
						var md5Str = data['ota']['md5Str'];
						var ota_version = data['ota']['ota_version'];
						var version = data['ota']['version'];
						L.ui.download()
						self.cleanUpgrade().then(function() {
							self.need_upgrade(ota_version, version).then(function(need_upgrade) {
								if(!need_upgrade) {
									self.log_write('latest version now');
									L.ui.dialog(
										L.tr(''), [
											$('<p />').text(L.tr('latest version now, not need to upgrade.')),
										],
										{
											style: 'close',
											close: function () {
												self.cleanBackup().then(function () {
													L.ui.dialog(false);
												});
											}
										}
									);
								} else {
									const cmd = "curl -k -m 120 -o /tmp/firmware.bin " + downloadUrl;
									self.curl_cmd(cmd)
									.then(function(data) {
										if (data != "success") {
											self.log_write('ota file download failed');
											L.ui.dialog(
												L.tr(''), [
													$('<p />').text(L.tr('ota file download failed. timeout or server busy')),
												],
												{
													style: 'close',
													close: function () {
														self.cleanBackup().then(function () {
															L.ui.dialog(false);
														});
													}
												}
											);
										} else {
											self.ota_upgrade_start(md5Str, ota_version);
										}
									})
								}
							})
						})
					}
				})
			}
		})
	},

	ota_upgrade_start: function(ota_checksum, ota_version) {
		var self = this;
		var image_tmp = "/tmp/firmware.bin";
		self.md5sum(image_tmp).then(function(data){
			L.file.exec('ls',['-l', image_tmp]).then(function(ret){
				var size_tmp =ret.stdout.split('      ');
				size_tmp = size_tmp[1].split(' ');
				var size = Number(size_tmp[1]);
				if(data != undefined && ota_checksum == data.md5)
				{
					var form = [
						$('<p />').text(L.tr('The firmware image was downloaded completely. Please verify the info below, then click "%s" to start the flash procedure.').format(L.tr('Ok'))),
						$('<ul />')
							.append($('<li />')
							.append($('<strong />').text(L.tr('Checksum') + ': '))
							.append(ota_checksum))
							.append($('<li />')
							.append($('<strong />').text(L.tr('Size') + ': '))
							.append('%1024mB'.format(size))),
					];
					L.ui.dialog(
						L.tr('Verify firmware'), form, {
							style: 'confirm',
							confirm: function () {
								var cmd = "./usr/sbin/ota upgrade_prepare %s %s".format(ota_checksum, ota_version);
								self.cmd(cmd).then(function () {
									self.startOtaUpgrade().then(function () {
										L.session.stopHeartbeat();
										L.ui.reconnect();
									});
								})
							},
							cancel: function () {
								self.cleanBackup().then(function () {
									self.delete_tmp_bin()
									L.ui.dialog(false);
								});
							}
						}
					);
				}
				else{
					L.ui.dialog(
						L.tr(''), [
						$('<p />').text(L.tr('ota file download error. error checksum')),
					],
						{
							style: 'close',
							close: function () {
								self.cleanBackup().then(function () {
									L.ui.dialog(false);
								});
							}
						}
					);
					self.log_write("the checksum is wrong");
				}
			});
		});
	},

	execute: function() {
		var self = this;

		self.testReset().then(function(reset_avail) {
			if (!reset_avail) {
				$('#btn_reset').prop('disabled', true);
			}

			if (!self.options.acls.backup) {
				$('#btn_restore, #btn_save, textarea').prop('disabled', true);
			}
			else {
				$('#btn_backup').click(function() { self.handleBackupDownload(); });
				$('#btn_restore').click(function() { self.handleBackupUpload(); });
				$('#btn_factory_backup').click(function() { self.handleFactoryDownload(); });
				$('#btn_factory_restore').click(function() { self.handleFactoryUpload(); });
			}

			if (!self.options.acls.upgrade) {
				$('#btn_flash, #btn_reset').prop('disabled', true);
			}
			else {
				$('#btn_flash').click(function() { self.handleFlashUpload(); });
				$('#btn_reset').click(function() { self.handleReset(); });
			}

			$('#btn_ota').click(function(){
				L.ui.dialog(
					L.tr(''), [
					$('<p />').text(L.tr('Are you sure you want to upgrade online? The upgrade process may take three to five minutes, please ensure that the machine will not be powered off.')),
					], {
						style: 'confirm',
						confirm: function () {
							self.cleanBackup().then(function () {
								L.ui.dialog(
									L.tr(''), [
										$('<p />').text(L.tr('ota checking software version'))
									]
								);
								self.ota_upgrade();
							});
						}
					}
				);
			});

			self.callLoad('basic_setting', 'auto_ota', 'enable').then(function (value) {
				if (value == '1') {
					$('#check_ota').prop('checked', 'true');
					$('#checkbox_ota').prop('class', 'checkbox_press').css('margin-top', '-7px');
				}
			});
			$('#check_ota').click(function () {
				if ($('#check_ota').prop('checked')) {
					$('#checkbox_ota').prop('class', 'checkbox_press').css('margin-top', '-7px');
					self.callSet('basic_setting', 'auto_ota', { 'enable': '1' });
					self.callCommit('basic_setting');
				}
				else {
					$('#checkbox_ota').prop('class', 'checkbox').css('margin-top', '20px');
					self.callSet('basic_setting', 'auto_ota', { 'enable': '0' });
					self.callCommit('basic_setting');
				}
				L.ui.dialog(
					L.tr(''), [
						$('<p />').text(L.tr('Set successfully')),
					],
					{
							style: 'close',
							close: function () {
								self.cleanBackup().then(function () {
									L.ui.dialog(false);
								});
							}
					}
				);
			});

			return self.getBackupConfig();
		})
	}
});
