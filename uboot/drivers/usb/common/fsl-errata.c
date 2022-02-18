/*
 * Freescale USB Controller
 *
 * Copyright 2013 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <fsl_errata.h>
#include<fsl_usb.h>

/* USB Erratum Checking code */
#if defined(CONFIG_PPC) || defined(CONFIG_ARM)
bool has_dual_phy(void)
{
	u32 svr = get_svr();
	u32 soc = SVR_SOC_VER(svr);

	switch (soc) {
#ifdef CONFIG_PPC
	case SVR_T1023:
	case SVR_T1024:
	case SVR_T1013:
	case SVR_T1014:
		return IS_SVR_REV(svr, 1, 0);
	case SVR_T1040:
	case SVR_T1042:
	case SVR_T1020:
	case SVR_T1022:
	case SVR_T2080:
	case SVR_T2081:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 1, 1);
	case SVR_T4240:
	case SVR_T4160:
	case SVR_T4080:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 2, 0);
#endif
	}

	return false;
}

bool has_erratum_a006261(void)
{
	u32 svr = get_svr();
	u32 soc = SVR_SOC_VER(svr);

	switch (soc) {
#ifdef CONFIG_PPC
	case SVR_P1010:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 2, 0);
	case SVR_P2041:
	case SVR_P2040:
		return IS_SVR_REV(svr, 1, 0) ||
			IS_SVR_REV(svr, 1, 1) || IS_SVR_REV(svr, 2, 1);
	case SVR_P3041:
		return IS_SVR_REV(svr, 1, 0) ||
			IS_SVR_REV(svr, 1, 1) ||
			IS_SVR_REV(svr, 2, 0) || IS_SVR_REV(svr, 2, 1);
	case SVR_P5010:
	case SVR_P5020:
	case SVR_P5021:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 2, 0);
	case SVR_T4240:
	case SVR_T4160:
	case SVR_T4080:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 2, 0);
	case SVR_T1040:
		return IS_SVR_REV(svr, 1, 0);
	case SVR_T2080:
	case SVR_T2081:
		return IS_SVR_REV(svr, 1, 0);
	case SVR_P5040:
		return IS_SVR_REV(svr, 1, 0);
#endif
	}

	return false;
}

bool has_erratum_a007075(void)
{
	u32 svr = get_svr();
	u32 soc = SVR_SOC_VER(svr);

	switch (soc) {
#ifdef CONFIG_PPC
	case SVR_B4860:
	case SVR_B4420:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 2, 0);
	case SVR_P1010:
		return IS_SVR_REV(svr, 1, 0);
	case SVR_P4080:
		return IS_SVR_REV(svr, 2, 0) || IS_SVR_REV(svr, 3, 0);
#endif
	}
	return false;
}

bool has_erratum_a007798(void)
{
#ifdef CONFIG_PPC
	return SVR_SOC_VER(get_svr()) == SVR_T4240 &&
		IS_SVR_REV(get_svr(), 2, 0);
#endif
	return false;
}

bool has_erratum_a007792(void)
{
	u32 svr = get_svr();
	u32 soc = SVR_SOC_VER(svr);

	switch (soc) {
#ifdef CONFIG_PPC
	case SVR_T4240:
	case SVR_T4160:
	case SVR_T4080:
		return IS_SVR_REV(svr, 2, 0);
	case SVR_T1024:
	case SVR_T1023:
		return IS_SVR_REV(svr, 1, 0);
	case SVR_T1040:
	case SVR_T1042:
	case SVR_T1020:
	case SVR_T1022:
	case SVR_T2080:
	case SVR_T2081:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 1, 1);
#endif
	}
	return false;
}

bool has_erratum_a005697(void)
{
	u32 svr = get_svr();
	u32 soc = SVR_SOC_VER(svr);

	switch (soc) {
#ifdef CONFIG_PPC
	case SVR_9131:
	case SVR_9132:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 1, 1);
#endif
	}
	return false;
}

bool has_erratum_a004477(void)
{
	u32 svr = get_svr();
	u32 soc = SVR_SOC_VER(svr);

	switch (soc) {
#ifdef CONFIG_PPC
	case SVR_P1010:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 2, 0);
	case SVR_P1022:
	case SVR_9131:
	case SVR_9132:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 1, 1);
	case SVR_P2020:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 2, 0) ||
			IS_SVR_REV(svr, 2, 1);
	case SVR_B4860:
	case SVR_B4420:
		return IS_SVR_REV(svr, 1, 0) || IS_SVR_REV(svr, 2, 0);
	case SVR_P4080:
		return IS_SVR_REV(svr, 2, 0) || IS_SVR_REV(svr, 3, 0);
#endif
	}

	return false;
}

bool has_erratum_a008751(void)
{
	u32 svr = get_svr();
	u32 soc = SVR_SOC_VER(svr);

	switch (soc) {
#ifdef CONFIG_ARM64
	case SVR_LS2080:
	case SVR_LS2085:
		return IS_SVR_REV(svr, 1, 0);
#endif
	}
	return false;
}

#endif
