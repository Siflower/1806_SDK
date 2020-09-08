// common methods

function hideParts(arr) {
    for (var i=0; i<arr.length; i++) {
        document.getElementById(arr[i]).style.display = 'none';
        if (arr[i]=='ap-infos'||arr[i]=='batch-edit-group')
            document.getElementById('group-info').style.display = '';
    }
}

function showParts(arr) {
    for (var i=0; i<arr.length; i++) {
        document.getElementById(arr[i]).style.display = '';
    }
}

function changeOption() {
    var wanSelect = document.getElementById("wan-connect-type");
    var index = wanSelect.selectedIndex;
    var closeTableId = wanSelect[selected].value;
    var openTableId = wanSelect[index].value;
    selected = index;
    document.getElementById(closeTableId).style.display = "none";
    document.getElementById(openTableId).style.display = "";
}
function getRadioValue(name) {
    var typeRadio = document.getElementsByName(name);
    var radioSelect;
    for (var i=0 ; i<typeRadio.length; i++){
        if (typeRadio[i].checked){
            radioSelect = typeRadio[i].value;
        }
    }
    return radioSelect;
}
function getRadioChange(name) {
    var wanSelect = getRadioValue(name);
    document.getElementById('pppoe').style.display = "none";
    document.getElementById('static').style.display = "none";
    document.getElementById('dhcp').style.display = "none";
    document.getElementById(wanSelect).style.display = "";
}


function clickHelp(i) {
    if (i==0){
        document.getElementById("Help").style.visibility="visible";
    } else {
        document.getElementById("Help").style.visibility="hidden";
    }
}

function coverClose() {
    document.getElementById("waiting").style.display="none";
}

function closeTip() {
    document.getElementById("Error").style.visibility = 'hidden';
    if(errTipDoc != undefined){
        errTipDoc.focus();
        errTipDoc.select();
        errTipDoc = null;
    }
}

function dragFunc(Drag) {
    Drag.onmousedown = function(event) {
        var ev = event || window.event;
        //event.stopPropagation();
        var disX = ev.clientX - Drag.offsetLeft;
        var disY = ev.clientY - Drag.offsetTop;
        document.onmousemove = function(event) {
            var ev = event || window.event;
            var dwidth = Drag.clientWidth;
            var dHeight = Drag.clientHeight;
            var screenW = window.screen.width;
            var screenH = window.screen.height;
            var dleft = ev.clientX - disX;
            var dtop = ev.clientY - disY
            if (dleft<0){
                dleft = 0;
            } else if (dleft>(screenW-dwidth)){
                dleft = screenW-dwidth
            }
            if (dtop<0){
                dtop = 0;
            } else if (dtop>(screenH-dHeight-200)){
                dtop = screenH-dHeight-200;
            }
            Drag.style.left = dleft + "px";
            Drag.style.top = dtop + "px";
            Drag.style.cursor = "move";
        };
    };
    Drag.onmouseup = function() {
        document.onmousemove = null;
        this.style.cursor = "default";
    };
}

function isValidIP(ip) {
    var reg = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/
    if (ip=="0.0.0.0"||ip=="255.255.255.255")
        return false;
    return reg.test(ip);
}

function debugVaildIp(ip) {
    var reg = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/
    return reg.test(ip);
}

function isValidDomain(domain) {
    var reg = /^(?=^.{3,255}$)[a-zA-Z0-9][-a-zA-Z0-9]{0,62}(\.[a-zA-Z0-9][-a-zA-Z0-9]{0,62})+$/;
    return reg.test(domain);

}

function isInvalidIPV6(ip) {
    var reg = /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$|^(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z]|[A-Za-z][A-Za-z0-9\-]*[A-Za-z0-9])$|^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/;
    return reg.test(ip);
}

function isValidMac(mac) {
    //var reg_mac = /[A-F\d]{2}:[A-F\d]{2}:[A-F\d]{2}:[A-F\d]{2}:[A-F\d]{2}:[A-F\d]{2}/;
    var reg_mac = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
    return reg_mac.test(mac);
}

function isValidNetmask(mask) {
    if (!isValidIP(mask)){
        return false;
    }
    obj=mask;
    var exp=/^(254|252|248|240|224|192|128|0)\.0\.0\.0|255\.(254|252|248|240|224|192|128|0)\.0\.0|255\.255\.(254|252|248|240|224|192|128|0)\.0|255\.255\.255\.(254|252|248|240|224|192|128|0)$/;
    var reg = obj.match(exp);
    if (reg==null) {
        return false; //"非法"
    } else {
        return true; //"合法"
    }
}

function isEncoded(encodeArray) {
    var open = document.getElementById(encodeArray[0]);
    var key = document.getElementById(encodeArray[1]);
    if (open.checked) {
        key.value = '';
        key.disabled = 'disabled';
    } else {
        key.disabled = '';
        key.focus();
    }
}

function checkStatus() {
    var status = document.getElementById("switchSpan").innerText;
    if (status == '已开启') {
        return 1;
    } else {
        return 0;
    }
}

function switchChecked(flag) {
    if (flag) {
        document.getElementById("switchSpan").innerText = "已开启";
        document.getElementById("switchSpan").style.color = "rgb(223, 0, 7)";
        document.getElementById("switchOn").style.display = "";
        document.getElementById("switchOff").style.display = "none"
    } else {
        document.getElementById("switchSpan").innerText = "未开启";
        document.getElementById("switchSpan").style.color = "rgb(175, 175, 175)";
        document.getElementById("switchOn").style.display = "none";
        document.getElementById("switchOff").style.display = "";
    }
}

function timestampToTime(timestamp) {
    var date = new Date(timestamp * 1000);//时间戳为10位需*1000，时间戳为13位的话不需乘1000
    var Y = date.getUTCFullYear() + '-';
    var M = (date.getUTCMonth()+1 < 10 ? '0'+(date.getMonth()+1) : date.getMonth()+1) + '-';
    var D = date.getUTCDate();
    D = D < 10 ? '0' + D : D;
    var h = date.getUTCHours();
    var m = date.getUTCMinutes();
    var s = date.getUTCSeconds();
    var dateAndTime = {'date':Y+M+D, 'time':this.zero(h)+":"+this.zero(m)+":"+this.zero(s)};
    return dateAndTime;
}

function zero(s) {
    return s < 10 ? '0' + s: s;
}
//page redirect
function pageRedirect(url) {
    $.ajax({
        type:'get',
        url:url,
        dataType:'JSONP',
        success:function(res,heads,code){
            window.location.href=url;
        },
        error:function (res,heads,code) {
            console.log("res:"+res);
            console.log("heads:"+heads);
            console.log("code:"+code);
            if (code!=null&&code!=""){
                setTimeout(function () {
                    window.location.href=url;
                },1000)
            }
        }
    });
}
//String len include chinese
function strlen(str){
    var len = 0;
    for (var i=0; i<str.length; i++) {
        var c = str.charCodeAt(i);
        //单字节加1
        if ((c >= 0x0001 && c <= 0x007e) || (0xff60<=c && c<=0xff9f)) {
            len++;
        }
        else {
            len+=3;
        }
    }
    return len;
}

//input limit --number
function numLimit(e) {
    if(e.value.length==1){
        e.value=e.value.replace(/[^0-9]/g,'')
    }else{
        e.value=e.value.replace(/\D/g,'');
        if (e.value.length>=1){
            e.value = parseInt(e.value,10);
        }
    }
}
//input kick weak signal
function minusNumlimt(e) {
    e.style.color = "black";
    if (e.value.startsWith("-")){
        if (e.value.length==2) {
            e.value="-" + e.value.replace(/[^0-9]/g,'')
        }else if (e.value.length>2) {
            e.value=e.value.replace(/\D/g,'');
            if (e.value.length>=2){
                e.value = "-" +parseInt(e.value);
            }
        }
    } else {
        if(e.value.length==1){
            e.value=e.value.replace(/[^0-9]/g,'')
        }else{
            e.value=e.value.replace(/\D/g,'');
            if (e.value.length>=1){
                e.value = parseInt(e.value);
            }
        }
    }
}
//input limit point+num
function pNumLimit(ob) {
    if (!ob.value.match(/^[\+\-]?\d*?\.?\d*?$/))
        ob.value = ob.t_value;
    else
        ob.t_value = ob.value;
    if (ob.value.match(/^(?:[\+\-]?\d+(?:\.\d+)?)?$/))
        ob.o_value = ob.value;
    ob.value=this.value.replace(/\s+/g,'');
}

//input limit --num+str
function nStrLimit(e) {
    e.value=check(e.value);
    e.value=e.value.replace(/\s+/g,'');
}

function noSpaceWord(e) {
    e.value=e.value.replace(/\s+/g,'');
}

//hostNameLimit
function hostNameLimit(str) {
    var reg = new RegExp(/^[a-zA-Z0-9-]{1,}$/);
    return reg.test(str)
}
// 0-9 A-Z _
function numStrUnderLine(e) {
    e.value=e.value.replace(/[^\w\.\/]/ig,'');
}

//only num and letter
function numLetterLimit(e) {
	e.value=e.value.replace(/[^a-zA-Z0-9]/g,'');
}


//check chinese str
function check(str){
    var temp="";
    for(var i=0;i<str.length;i++)
        if(str.charCodeAt(i)>0&&str.charCodeAt(i)<255)
            temp+=str.charAt(i);
    return temp
}

//check lan of static
function checkLan(ip,lan) {
    var flag = false;
    if (lan!=undefined){
        var lanArray = lan.split(".");
        var ipArray = ip.split(".");
        for (var i=0;i<4;i++){
            if (i<3){
                if (lanArray[i]!=ipArray[i]){
                    flag = true;
                }
            }
        }
        return flag;
    }else {
        return undefined;
    }
}
//check port range
function checkPortRange(port) {
    var reg = new RegExp(/(^[1-9]\d{0,3}$)|(^[1-5]\d{4}$)|(^6[0-4]\d{3}$)|(^65[0-4]\d{2}$)|(^655[0-2]\d$)|(^6553[0-5]$)/);
    return reg.test(port)
}
function checkVirtulServerExternalPort(port) {
    var portArrray = [53, 21, 70, 80, 119, 110, 1723, 25, 1080, 23];
    for (var i=0; i<portArrray.length; i++) {
        if (port == portArrray[i]) {
            return true;
        }
    }
    return false;
}
// router set netMask limit
function routerMaskLim(ip) {
    if (ip == '255.255.255.255') {
        return true;
    }else {
        return lannetMaskLimit(ip);
    }
}

// lan net mask limit
function lannetMaskLimit(ip) {
    var netmaskArray = new  Array();
    netmaskArray[0] = "128.0.0.0";
    netmaskArray[1] = "128.0.0.0";
    netmaskArray[2] = "129.0.0.0";
    netmaskArray[3] = "224.0.0.0";
    netmaskArray[4] = "240.0.0.0";
    netmaskArray[5] = "248.0.0.0";
    netmaskArray[6] = "252.0.0.0";
    netmaskArray[7] = "254.0.0.0";
    netmaskArray[8] = "255.0.0.0";
    netmaskArray[9] = "255.128.0.0";
    netmaskArray[10] = "255.129.0.0";
    netmaskArray[11] = "255.224.0.0";
    netmaskArray[12] = "255.240..0.0";
    netmaskArray[13] = "255.248.0.0";
    netmaskArray[14] = "255.252.0.0";
    netmaskArray[15] = "255.254.0.0";
    netmaskArray[16] = "255.255.0.0";
    netmaskArray[17] = "255.255.128.0";
    netmaskArray[18] = "255.255.192.0";
    netmaskArray[19] = "255.255.224.0";
    netmaskArray[20] = "255.255.240.0";
    netmaskArray[21] = "255.255.248.0";
    netmaskArray[22] = "255.255.252.0";
    netmaskArray[23] = "255.255.254.0";
    netmaskArray[24] = "255.255.255.0";
    netmaskArray[25] = "255.255.255.128";
    netmaskArray[26] = "255.255.255.192";
    netmaskArray[27] = "255.255.255.224";
    netmaskArray[28] = "255.255.255.240";
    netmaskArray[29] = "255.255.255.248";
    netmaskArray[30] = "255.255.255.252";
    netmaskArray[31] = "255.255.255.254";

    for (var i =0 ;i< 32 ;i++){
        if (ip==netmaskArray[i]){
            return true;
        }
    }
    return false;
}

//get chainese sum number
function getChanLen(str) {
    var re = /[\u4E00-\u9FA5]/g;
    if (str){
        if (re.test(str)) {
          return str.match(re).length;
        }
    }
}

//reWrite startsWith and endsWith compatible IE
$(document).ready(function() {
    String.prototype.startsWith = function(str) {
        var reg = new RegExp("^" + str);
        return reg.test(this);
    }
//测试ok，直接使用str.endsWith("abc")方式调用即可
    String.prototype.endsWith = function(str) {
        var reg = new RegExp(str + "$");
        return reg.test(this);
    }
});

function addCSS(obj,css){
    var selector;
    var isIE678 = !-[1,];
    var cssSheetTail = document.styleSheets.length - 1;
    for(selector in obj){
        if(!isIE678 || selector.search('disabled') < 0){
            if(document.styleSheets[cssSheetTail].insertRule && !isIE678){
                //for firefox
                var index = document.styleSheets[cssSheetTail].cssRules.length;
                document.styleSheets[cssSheetTail].insertRule(selector+' {'+css+': '+obj[selector]+'}' ,index);
            }else{
                if(isIE678){
                    var array = selector.split(',');
                    for(each in array){
                        document.styleSheets[cssSheetTail].addRule(array[each],css+': '+obj[selector]);
                    }
                }else{
                    document.styleSheets[cssSheetTail].addRule(selector,css+': '+obj[selector]);
                }
            }
        }
    }
}

function setTableInnerHTML(table, html) {
  if (navigator && navigator.userAgent.match(/msie/i)) {
    var temp = table.ownerDocument.createElement('div');
    temp.innerHTML = '<table><tbody>' + html + '</tbody></table>';
    if (table.tBodies.length == 0) {
      var tbody = document.createElement("tbody");
      table.appendChild(tbody);
    }
    table.replaceChild(temp.firstChild.firstChild, table.tBodies[0]);
  } else {
    table.innerHTML = html;
  }
}

function setTrInnerHTML(tr, html) {
  if (navigator && navigator.userAgent.match(/msie/i)) {
    var temp = tr.ownerDocument.createElement('div');
    temp.innerHTML = '<table><tbody>' + html + '</tbody></table>';
    var trParent = tr.parentNode;
    trParent.removeChild(tr);
    trParent.appendChild(temp.firstChild.firstChild.firstChild);
  } else {
    tr.innerHTML = html;
  }
}
window.console = window.console || (function () {
    var c = {}; c.log = c.warn = c.debug = c.info = c.error = c.time = c.dir = c.profile
    = c.clear = c.exception = c.trace = c.assert = function () { };
    return c;
})();

function supportQuerySelector(){
    if(!document.querySelector){
        (function () {
            var style = document.createStyleSheet(), select = function (selector, maxCount) {
                var all = document.all, l = all.length, i, resultSet = [];
                console.log('selector is ',selector);
                style.addRule(selector, "foo:bar");
                for (i = 0; i < l; i += 1) {
                    if (all[i].currentStyle.foo === "bar") {
                        resultSet.push(all[i]);
                        if (resultSet.length > maxCount) {
                            break;
                        }
                    }
                }
                style.removeRule(0);
                return resultSet;
            };

            // be rly sure not to destroy a thing!

            if (document.querySelectorAll || document.querySelector) {
                return;
            }

            document.querySelectorAll = function (selector) {
                return select(selector, Infinity);
            };

            document.querySelector = function (selector) {
                return select(selector, 1)[0] || null;
            };
            }());
        var all = document.all ,length = all.length;
        for (var i = 0; i < length; i += 1) {
            if(!all[i].querySelector && all[i].id){
                all[i].querySelector = function (selector){
                    return document.querySelector('#' + this.id + ' ' + selector);
                }
            }
        }
    }
}
