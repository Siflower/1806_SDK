#! /bin/sh
. /lib/functions.sh

button_test_init() {
    btn_codes=$(uci show btn_ctrl_cfg | grep 'btn_code' | cut -d '=' -f 2 | sort | uniq)
    btn_count=$(echo "$btn_codes" | wc -l)
    uci set btn_ctrl_cfg.test_result="test_result"
    uci set btn_ctrl_cfg.test_result.test_result="fail"
    uci set btn_ctrl_cfg.test_result.button1_test_result="fail"
if [ "$btn_count" -eq 2 ]; then
    uci set btn_ctrl_cfg.test_result.button2_test_result="fail"
fi

    btn_configs=$(uci show btn_ctrl_cfg | awk -F'[.=]' '/@btn_config/ && !seen[$2]++ {print $2}')

    for btn_config in $btn_configs; do
        local btn_cmd=$(uci get btn_ctrl_cfg.$btn_config.btn_cmd)
        if [ -z "$btn_cmd" ]; then
            exit
        fi
        uci set btn_ctrl_cfg.$btn_config.btn_cmd_old="$btn_cmd"
        uci delete btn_ctrl_cfg.$btn_config.btn_cmd
    done

    uci set btn_ctrl_cfg.button1_test1="btn_config"
    uci set btn_ctrl_cfg.button1_test1.btn_code="0x198"
    uci set btn_ctrl_cfg.button1_test1.btn_action="4"
    uci set btn_ctrl_cfg.button1_test1.test_result='0'
    uci set btn_ctrl_cfg.button1_test1.btn_cmd="sh usr/share/led-button/button_test.sh test button1_test1"

    uci set btn_ctrl_cfg.button1_test2="btn_config"
    uci set btn_ctrl_cfg.button1_test2.btn_code="0x198"
    uci set btn_ctrl_cfg.button1_test2.btn_action="5"
    uci set btn_ctrl_cfg.button1_test2.test_result='0'
    uci set btn_ctrl_cfg.button1_test2.btn_cmd="sh usr/share/led-button/button_test.sh test button1_test2"

if [ "$btn_count" -eq 2 ]; then
    uci set btn_ctrl_cfg.button2_test1="btn_config"
    uci set btn_ctrl_cfg.button2_test1.btn_code="0x199"
    uci set btn_ctrl_cfg.button2_test1.btn_action="4"
    uci set btn_ctrl_cfg.button2_test1.test_result='0'
    uci set btn_ctrl_cfg.button2_test1.btn_cmd="sh usr/share/led-button/button_test.sh test button2_test1"

    uci set btn_ctrl_cfg.button2_test2="btn_config"
    uci set btn_ctrl_cfg.button2_test2.btn_code="0x199"
    uci set btn_ctrl_cfg.button2_test2.btn_action="5"
    uci set btn_ctrl_cfg.button2_test2.test_result='0'
    uci set btn_ctrl_cfg.button2_test2.btn_cmd="sh usr/share/led-button/button_test.sh test button2_test2"
fi
    uci commit

    local result_11=0
if [ "$btn_count" -eq 2 ]; then
    local result_21=0
fi
    local reset_check_finish=0
    local wps_check_finish=0
    timeout=180
    start_time=$(date +%s)
    echo "Enable button test"

    while true; do
        result_11=$(uci get btn_ctrl_cfg.button1_test1.test_result 2>/dev/null)
if [ "$btn_count" -eq 2 ]; then
        result_21=$(uci get btn_ctrl_cfg.button2_test1.test_result 2>/dev/null)
fi
        if [ "$result_11" -eq 1 ] && [ "$reset_check_finish" -eq 0 ]; then
            reset_check_finish=1
            echo "Reset Button success!"
        fi

if [ "$btn_count" -eq 2 ]; then
        if [ "$result_21" -eq 1 ] && [ "$wps_check_finish" -eq 0 ]; then
            wps_check_finish=1
            echo "WPS Button success!"
        fi

        if [ "$reset_check_finish" -eq 1 ] && [ "$wps_check_finish" -eq 1 ]; then
            exit 0
        fi
else
        if [ "$reset_check_finish" -eq 1 ]; then
            exit 0
        fi
fi
        current_time=$(date +%s)
        elapsed_time=$((current_time - start_time))
        if [ $elapsed_time -gt $timeout ]; then
            echo "Button test fail!"
            button_test_finish
            exit 0
        fi
        sleep 1
    done
}

set_test_config() {
    local cfg="$1"
    config_get btn_cmd "$cfg" btn_cmd
    config_set btn_cmd_old "$cfg" btn_cmd
    config_set btn_cmd "$cfg" ""
}

recover_config() {
    btn_configs=$(uci show btn_ctrl_cfg | awk -F'[.=]' '/@btn_config/ && !seen[$2]++ {print $2}')
    for btn_config in $btn_configs; do
        local btn_cmd_old=$(uci get btn_ctrl_cfg.$btn_config.btn_cmd_old)
        if [ -z "$btn_cmd_old" ]; then
            exit
        fi
        uci set btn_ctrl_cfg.$btn_config.btn_cmd="$btn_cmd_old"
        uci delete btn_ctrl_cfg.$btn_config.btn_cmd_old
    done
}

button_test_finish() {
    recover_config
    local result_11=$(uci get btn_ctrl_cfg.button1_test1.test_result)
    local result_12=$(uci get btn_ctrl_cfg.button1_test2.test_result)
if [ "$btn_count" -eq 2 ]; then
    local result_21=$(uci get btn_ctrl_cfg.button2_test1.test_result)
    local result_22=$(uci get btn_ctrl_cfg.button2_test2.test_result)
fi
    local result_1=0
if [ "$btn_count" -eq 2 ]; then
    local result_2=0
fi
    if [ "$result_11" -eq 1 ] && [ "$result_12" -eq 1 ]; then
        uci set btn_ctrl_cfg.test_result.button1_test_result="success"
        result_1=1
    else
        uci set btn_ctrl_cfg.test_result.button1_test_result="fail"
    fi
if [ "$btn_count" -eq 2 ]; then
    if [ "$result_21" -eq 1 ] && [ "$result_22" -eq 1 ]; then
        uci set btn_ctrl_cfg.test_result.button2_test_result="success"
        result_2=1
    else
        uci set btn_ctrl_cfg.test_result.button2_test_result="fail"
    fi

    if [ "$result_1" -eq 1 ] && [ "$result_2" -eq 1 ]; then
        uci set btn_ctrl_cfg.test_result.test_result="success"
    else
        uci set btn_ctrl_cfg.test_result.test_result="fail"
    fi
else
    if [ "$result_1" -eq 1 ]; then
        uci set btn_ctrl_cfg.test_result.test_result="success"
    else
        uci set btn_ctrl_cfg.test_result.test_result="fail"
    fi
fi
    uci delete btn_ctrl_cfg.button1_test1
    uci delete btn_ctrl_cfg.button1_test2
if [ "$btn_count" -eq 2 ]; then
    uci delete btn_ctrl_cfg.button2_test1
    uci delete btn_ctrl_cfg.button2_test2
fi
    uci commit
    echo "Disable button test"
}

_button_test() {
    uci set btn_ctrl_cfg.$1.test_result='1'
    uci commit
}

case "$1" in
        init) button_test_init ;;
        exit) button_test_finish ;;
        test) _button_test $2;;
esac