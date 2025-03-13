L.ui.view.extend({

    // 定义 cmd 用于执行系统命令
    cmd: L.rpc.declare({
        object: 'web.advance', // 假设你已经有一个 'web.advance' 对象来处理命令
        method: 'cmd',
        params: ['cmd'],
    }),
    read: L.rpc.declare({
        object: 'file',
        method: 'read',
        params: ['path'],
        expect: {
            data: '',
        },
    }),

    execute: function() {
        var self = this;
        // 创建一个配置页面，名称为 'basic_setting'，标题为 'Firewall setting'，支持折叠功能
        var m = new L.cbi.Map('basic_setting', {
            caption: L.tr('Firewall Settings'),
            collabsible: true
        });

        // 创建一个虚拟section '__firewall'
        var s = m.section(L.cbi.DummySection, '__firewall', {
//            caption: L.tr('Firewallsetting'),
        });

        // 创建一个新的复选框 'fw_enable'，用于控制是否启用DMZ日志
        var fw_enable = s.option(L.cbi.CheckboxValue, 'fw_enable', {
            caption: L.tr('Enable Firewall'), // 标注为 'Enable Firewall'
            optional: true
        });

        // 修改此处以执行 iptables 命令检查防火墙状态
        fw_enable.ucivalue = function(sid) {
            var fw_value = L.uci.get("basic_setting", "firewall", "enable");
            
            if (fw_value == 1) { // 假设 '1' 表示启用防火墙
                return true;  // 防火墙启用，复选框勾选
            } else {
                return false;  // 防火墙未启用，复选框未勾选
            }
        }; 

        // 定义保存功能，将日志启用状态保存到 UCI 配置文件中
        fw_enable.save = function(sid) {
            var fw_value = fw_enable.formvalue(sid); // 获取复选框当前值
            L.uci.add('basic_setting', 'setting', 'firewall');
            L.uci.set('basic_setting', 'firewall', 'enable', fw_value); // 转换为 '1' 或 '0'
            L.uci.save(); // 保存配置

            // 根据启用状态执行相应命令
            if (fw_value === '1' || fw_value === true) {
                // 如果启用了防火墙，启动防火墙
                self.cmd("/etc/init.d/firewall start").then(function() {
                });
            } else {
                // 否则停止防火墙
                self.cmd("/etc/init.d/firewall stop").then(function() {
                });
            }
        };

        // 将生成的表单插入到ID为 'fws' 的HTML元素中，以显示在前端界面
        m.insertInto('#fws');
    }
});
