L.ui.view.extend({
    execute: function() {
        $(".clickSkip").off('click').on('click',function(){
            L.ui.callSet('basic_setting', 'web_guide', { 'inited': '1' });
            L.ui.callCommit('basic_setting');
            history.replaceState(null, '', window.location.origin + window.location.pathname + window.location.search);
            L.ui.login();
            location.reload(true)
        })

        $(".clickOk").off('click').on('click',function(){
            L.ui.loadHtml("guide","body");
        })

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

        $('.login_langinfo').off('click').on("click",function(event) {
            event.stopPropagation();
            $('.login_lang ul').toggle();
        });

        $(document).click(function() {
            $('.login_lang ul').hide();
        });

        $(".login_lang").off('click').on("click","ul li",function(){
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
    }
})