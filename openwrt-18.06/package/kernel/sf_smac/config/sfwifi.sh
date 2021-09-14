#!/bin/sh

. /lib/functions.sh

INSMOD="/sbin/insmod"
RMMOD="/sbin/rmmod"

load_firmware()
{
    cmd="/sbin/insmod startcore"
    eval $cmd
}

unload_firmware()
{
    cmd="/sbin/rmmod startcore"
    eval $cmd
}

insmod_rf() {
#   umount /sys/kernel/debug
#   mount -t debugfs none /sys/kernel/debug
    modprobe sf16a18_rf
    sleep 1
}

unload_rf() {
    cmd="/sbin/rmmod sf16a18_rf"
    eval $cmd
    sleep 1
}

insmod_umac(){
    all_modparams=
    extra_user_args=
    modparams="
    ht_on=${ht_on-1}
    vht_on=${vht_on-1}
    he_on=${he_on-1}
    ldpc_on=${ldpc_on-1}
    stbc_on=${stbc_on-1}
    phycfg=${phycfg-0}
    uapsd_timeout=${uapsd_timeout-0}
    ap_uapsd_on=${ap_uapsd_on-0}
    sgi=${sgi-1}
    sgi80=${sgi80-1}
    use_2040=${use_2040-1}
    nss=${nss-1}
    amsdu_rx_max=${amsdu_rx_max-1}
    bfmee=${bfmee-1}
    bfmer=${bfmer-0}
    mesh=${mesh-0}
    murx=${murx-0}
    mutx=${mutx-0}
    mutx_on=${mutx_on-1}
    use_80=${use_80-1}
    custregd=${custregd-0}
    lp_clk_ppm=${lp_clk_ppm-1000}
    addr_maskall=${addr_maskall-0}
	not_send_null=${not_send_null-0}
    ps_on=${ps_on-1}
    tx_lft=${tx_lft-100}
    tdls=${tdls-1}
    txpower_lvl=${txpower_lvl-1}
    uf=${uf-0}
    "
    smac_modparams="
    mfp_on=${mfp_on-0}
    gf_on=${gf_on-0}
    bwsig_on=${bwsig_on-0}
    dynbw_on=${dynbw_on-0}
    agg_tx=${agg_tx-1}
    amsdu_force=${amsdu_force-2}
    rc_probes_on=${rc_probes_on-0}
    cmon=${cmon-1}
    hwscan=${hwscan-1}
    autobcn=${autobcn-1}
    dpsm=${dpsm-1}
    tweak_bw=${tweak_bw-0}
    led_status=${led_status-1}
    "
    fmac_modparams="
    ant_div=${ant_div-1}
    tx_queue_num=${tx_queue_num-5}
    "
    if [ sf16a18_smac == "$1" ]; then
        if [ lb == $2 ];then
            all_modparams="is_hb=0"$modparams$smac_modparams
            extra_user_args="force_mod_name=sf16a18_lb_smac"
        else
            all_modparams="is_hb=1"$modparams$smac_modparams
            extra_user_args="force_mod_name=sf16a18_hb_smac"
        fi
    else
        if [ lb == $2 ];then
            all_modparams="is_hb=${is_hb-0}"$modparams$fmac_modparams
            extra_user_args="force_mod_name=sf16a18_lb_fmac"
        else
            all_modparams="is_hb=${is_hb-1}"$modparams$fmac_modparams
            extra_user_args="force_mod_name=sf16a18_hb_fmac"
        fi
    fi

    cmd="/sbin/insmod $1 $extra_user_args $all_modparams"
    eval $cmd
}

insmod_mac80211(){
    modprobe mac80211
}

insmod_cfg80211(){
    modprobe cfg80211
}

insmod_smac_lb(){
    insmod_umac sf16a18_smac lb
}

insmod_smac_hb(){
    insmod_umac sf16a18_smac hb
}

insmod_smac() {
    insmod_smac_lb
    insmod_smac_hb
}

unload_smac_lb(){
    cmd="/sbin/rmmod sf16a18_lb_smac"
    eval $cmd
}

unload_smac_hb(){
    cmd="/sbin/rmmod sf16a18_hb_smac"
    eval $cmd
}

unload_smac(){
    unload_smac_lb
    unload_smac_hb
}

insmod_fmac_lb(){
    insmod_umac sf16a18_fmac lb
}

insmod_fmac_hb(){
    insmod_umac sf16a18_fmac hb
}

insmod_fmac() {
    insmod_fmac_lb
    insmod_fmac_hb
}

unload_fmac_lb(){
    cmd="/sbin/rmmod sf16a18_lb_fmac"
    eval $cmd
}

unload_fmac_hb(){
    cmd="/sbin/rmmod sf16a18_hb_fmac"
    eval $cmd
}

unload_fmac(){
    unload_fmac_lb
    unload_fmac_hb
}
