local  require = require
module "luci.sys.zoneinfo.tzdatanew"
local uci = require "luci.model.uci"
local _uci_real  = cursor or _uci_real or uci.cursor()
local value = _uci_real:get("luci", "main", "lang")
if ( value == "zh_cn" ) then

TZ_NEW = {
	{ '(GMT-12:00)埃尼威托克，夸贾林岛', 'MHT-12' },
	{ '(GMT-11:00)中途岛，萨摩亚群岛', 'SST11' },
	{ '(GMT-10:00)夏威夷', 'CKT10' },
	{ '(GMT-09:00)阿拉斯加', 'AKST9AKDT,M3.2.0,M11.1.0' },
	{ '(GMT-08:00)太平洋时间(美国和加拿大)；蒂华','PST-8' },
	{ '(GMT-07:00)山地时间(美国和加拿大)；亚利桑那', 'MST7' },
	{ '(GMT-06:00)中部时间(美国和加拿大)；中美洲，墨西哥城', 'CST6CDT,M4.1.0,M10.5.0' },
	{ '(GMT-05:00)东部时间(美国和加拿大)；波哥达，利马', 'PET5' },
	{ '(GMT-04:00)大西洋时间(加拿大)；加拉加斯，圣地亚哥', 'AST4' },
	{ '(GMT-03:30)纽芬兰', 'NST3:30NDT,M3.2.0,M11.1.0' },
	{ '(GMT-03:00)巴西利亚，布宜诺斯艾利斯，乔治敦，格陵兰', 'ART3' },
	{ '(GMT-02:00)中大西洋', 'FNT2' },
	{ '(GMT-01:00)佛得角群岛，亚速尔群岛', 'EGT1EGST,M3.5.0/0,M10.5.0/1'},
	{ '(GMT+00:00)格林威治平时；都柏林，爱丁堡，伦敦，里斯本', 'GMT0' },
	{ '(GMT+01:00)阿姆斯特丹，柏林，罗马，斯得哥尔摩，巴黎', 'CET-1CEST,M3.5.0,M10.5.0/3' },
	{ '(GMT+02:00)开罗，雅典，伊斯坦布尔，明斯克，耶路撒冷', 'IST-2IDT,M3.4.4/26,M10.5.0' },
	{ '(GMT+03:00)巴格达，科威特，利雅得，莫斯科，圣彼得堡', 'MSK-3'},
	{ '(GMT+03:30)德黑兰', 'IRST-3:30IRDT,J80/0,J264/0'},
	{ '(GMT+04:00)阿布扎比，马斯喀特，巴库，第比利斯，埃里温', '<+04>-4' },
	{ '(GMT+04:30)喀布尔', 'AFT-4:30' },
	{ '(GMT+05:00)叶卡捷林堡，伊斯兰堡，卡拉奇，塔什干', 'PKT-5' },
	{ '(GMT+05:30)马德拉斯，加尔各答，孟买，新德里', 'XDLT5:30'},
	{ '(GMT+05:45)加德满都', 'NPT-5:45'},
	{ '(GMT+06:00)阿拉木图，新西伯利亚，阿斯塔纳，达卡', 'BDT-6' },
	{ '(GMT+06:30)仰光', 'MMT-6:30'},
	{ '(GMT+07:00)曼谷，雅加达，河内', 'ICT-7' },
	{ '(GMT+08:00)北京，重庆，乌鲁木齐，香港特别行政区，台北', 'CST-8' },
	{ '(GMT+09:00)东京，大坂，札幌，汉城，雅库茨克', 'JST-9' },
	{ '(GMT+09:30)阿德莱德', 'ACST-9:30ACDT,M10.1.0,M4.1.0/3' },
	{ '(GMT+10:00)布里斯班，关岛，堪培拉，墨尔本，悉尼', 'AEST-10AEDT,M10.1.0,M4.1.0/3' },
	{ '(GMT+11:00)马加丹，索罗门群岛，新喀里多尼亚', '<+11>-11' },
	{ '(GMT+12:00)富士，勘察加半岛，马绍尔群岛，惠灵顿', 'TVT-12' },
	{ '(GMT+13:00)努库阿洛法', 'PHOT-13' }
}
else
	   TZ_NEW = {
	{ '(GMT-12:00)Enewetak，Kwajalein Island', 'MHT-12' },
	{ '(GMT-11:00)Midway Islands，Samoa Islands', 'SST11' },
	{ '(GMT-10:00)Hawaii', 'CKT10' },
    { '(GMT-09:00)Alaska', 'AKST9AKDT,M3.2.0,M11.1.0' },
    { '(GMT-08:00)Pacific time (United States and Canada):Tijuana','PST-8' },
    { '(GMT-07:00)Mountain time (United States and Canada): Arizona', 'MST7' },
	{ '(GMT-06:00)Central time (United States and Canada): Central America, Mexico City', 'CST6CDT,M4.1.0,M10.5.0' },
	{ '(GMT-05:00)Eastern time (United States and Canada): Bogota, Lima', 'PET5' },
	{ '(GMT-04:00)Atlantic time (Canada): Santiago, Caracas', 'AST4' },
	{ '(GMT-03:30)Newfoundland', 'NST3:30NDT,M3.2.0,M11.1.0' },
	{ '(GMT-03:00)Brasilia, Buenos Aires, Georgetown, Greenland', 'ART3' },
	{ '(GMT-02:00)Midlantic', 'FNT2' },
	{ '(GMT-01:00)Cape Verde, Azores', 'EGT1EGST,M3.5.0/0,M10.5.0/1'},
	{ '(GMT+00:00)Greenwich mean time: Dublin, Edinburgh, London, Lisbon', 'GMT0' },
	{ '(GMT+01:00)Amsterdam, Berlin, Rome, Stockholm, Paris', 'CET-1CEST,M3.5.0,M10.5.0/3' },
	{ '(GMT+02:00)Cairo, Athens, Istanbul, Minsk, Jerusalem', 'IST-2IDT,M3.4.4/26,M10.5.0' },
	{ '(GMT+03:00)Baghdad, Kuwait, Riyadh, Moscow, st. Petersburg', 'MSK-3'},
	{ '(GMT+03:30)Tehran', 'IRST-3:30IRDT,J80/0,J264/0'},
	{ '(GMT+04:00)ABU dhabi, Muscat, Baku, Tbilisi, Erevan', '<+04>-4' },
	{ '(GMT+04:30)Kabul', 'AFT-4:30' },
	{ '(GMT+05:00)Ekaterinburg, Islamabad, Karachi, Tashkent', 'PKT-5' },
	{ '(GMT+05:30)Madras, kolkata, mumbai, New Delhi', 'XDLT5:30'},
	{ '(GMT+05:45)Katmandu', 'NPT-5:45'},
	{ '(GMT+06:00)Almaty, Novosibirsk, Astana, Dhaka', 'BDT-6' },
	{ '(GMT+06:30)Rangoon', 'MMT-6:30'},
	{ '(GMT+07:00)Bangkok, Jakarta, Hanoi', 'ICT-7' },
	{ '(GMT+08:00)Beijing, Chongqing, Urumqi, Hong Kong special administrative region, Taipei', 'CST-8' },
	{ '(GMT+09:00)Tokyo, Osaka, Sapporo, Seoul, Yakutsk', 'JST-9' },
	{ '(GMT+09:30)Adelaide', 'ACST-9:30ACDT,M10.1.0,M4.1.0/3' },
	{ '(GMT+10:00)Brisbane, Guam, Canberra, Melbourne, Sydney', 'AEST-10AEDT,M10.1.0,M4.1.0/3' },
	{ '(GMT+11:00)Magadan, Solomon Islands, New Caledonia', '<+11>-11' },
	{ '(GMT+12:00)Fuji, Kamchatka peninsula, Marshall Islands, Wellington', 'TVT-12' },
	{ '(GMT+13:00)Nukualofa', 'PHOT-13' }
}
end

