CFLAGS +=-DDDR -I $(BASE)/ddr3

#ifeq ($(ddr2)$(ddr), )
#nt5cc128m16ip=y
#CFLAGS += -D'DDR_MODEL="nt5cc128m16ip/em6gd16ewkg/em6gd16ewbh/m15t1g1664a"'
#endif
ifdef ddr3
CFLAGS += -DDDR3
$(ddr3):=y
CFLAGS += -D'DDR_MODEL="$(ddr3)"'
CFLAGS += -D$(ddr3)
endif

ifdef ddr2
CFLAGS += -DDDR2
$(ddr2):=y
CFLAGS += -D'DDR_MODEL="$(ddr2)"'
CFLAGS += -D$(ddr2)
endif

obj-y += ddr_init.c ddr_common.c

obj-$(ddr2_512mcommon) += ddr2_512mcommon.c
obj-$(ddr2_1gcommon) += ddr2_1gcommon.c
obj-$(ddr2_2gcommon) += ddr2_2gcommon.c
obj-$(em68b16cwqh) += ddr2_em68b16cwqh.c
obj-$(micron) += ddr2_micron.c
obj-$(m14d5121632a) += ddr2_m14d5121632a.c
obj-$(hy5ps1g1631c) += ddr2_hy5ps1g1631c.c
obj-$(a3r12e40dbf) += ddr2_a3r12e40dbf.c
obj-$(n2tu51216dg) += ddr2_n2tu51216dg.c
obj-$(nt5tu32m16eg) += ddr2_nt5tu32m16eg.c
obj-$(w9751g6kb) += ddr2_w9751g6kb.c
obj-$(pme809416dbr) += ddr2_pme809416dbr.c
obj-$(ct54v321601a) += ddr2_ct54v321601a.c
obj-$(prn64m16u88bo8zlf) += ddr2_prn64m16u88bo8zlf.c
obj-$(m14d1g1664a) += ddr2_m14d1g1664a.c

obj-$(ddr3_1gcommon) += ddr3_1gcommon.c
obj-$(ddr3_2gcommon) += ddr3_2gcommon.c
obj-$(nt5cc128m16ip) += ddr3_nt5cc128m16ip.c
obj-$(em6gd16ewkg) += ddr3_nt5cc128m16ip.c
obj-$(em6gd16ewbh) += ddr3_nt5cc128m16ip.c
obj-$(em6gc16egdh12j) += ddr3_em6gc16egdh12j.c
obj-$(m15t1g1664a) += ddr3_m15t1g1664a.c
obj-$(k4b1g1646e) += ddr3_k4b1g1646e.c
obj-$(p2p2gf4alf) += ddr3_nt5cc128m16ip.c
obj-$(nt5cc128m16jr) += ddr3_nt5cc128m16jr.c
obj-$(w632gu6nb) += ddr3_w632gu6nb.c
obj-$(w632gu6mb) += ddr3_w632gu6nb.c
obj-$(m15t2g16128a) += ddr3_nt5cc128m16jr.c
obj-$(is43tr16128d) += ddr3_is43tr16128d.c
