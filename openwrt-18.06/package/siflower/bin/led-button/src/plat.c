#include "action.h"

/* This table is used to match board & boardname. */
struct board_name bd_name[] = {
	{ BOARD_0, "sf16a18-p10-gmac" },
	{ BOARD_1, "sf16a18-86v" },
	{ BOARD_2, "sf16a18-ac" },
	{ BOARD_3, "sf16a18-rep" },
	{ BOARD_4, "sf16a18-evb" },
	{ BOARD_5, "sf16a18-p10h" },
	{ BOARD_6, "sf16a18-p10" },
	{ BOARD_7, "sf16a18-86v-c2" },
	{ BOARD_8, "sf16a18-rep-nopa" },
	{ BOARD_9, "sf16a18-ac20" },
	{ BOARD_10, "sf16a18-evb_v5" },
	{ BOARD_11, "sf19a28-evb" },
	{ BOARD_12, "sf19a28-ac28" },
	{ BOARD_13, "sf19a28-rep" },
	{ BOARD_14, "sf19a28-ac28s" },
	{ BOARD_15, "sf19a28-bpi" },
	{ BOARD_16, "sf19a28-rep-sw" },
	{ BOARD_17, "sf19a28-rep-phy" },
	{ BOARD_18, "sf19a28-bpi-luci2" },
	{ BOARD_19, "sf19a28-bpi-luci2-neutral" },
	{ BOARD_20, "sf19a28-bpi-luci2-siflower" },
	{ BOARD_MAX, "none" }, /* Ending flags. */
};

int _sfax8_grep_version(char *str) {
	int len = 0;
	char *ptr = str;
	int flag = 1;
	if (*ptr == 'v' || *ptr == 'V') {
		flag = 0;
		ptr += 1;
		len++;
	}
	while (*ptr != '\0' && *ptr != '-') {
		if (/**ptr != '\r' && *ptr != '\n' && */(*ptr < '0' || *ptr > '9')) {
			flag = 1;
		}
		ptr++;
		len++;
	}
	return flag ? len : 0;

}

int sfax8_grep_version(char *model, char *board)
{
	char *p_str = board;
	char *p_tmp = model;
	int len = 0;
	int count = 0;
	if (NULL == model || NULL == board){
		return 0;
	}
	len = _sfax8_grep_version(model);
	if (len) {
		memcpy(p_str, model, len);
		p_str += len;
		count += len;
	}

	while (p_tmp && (p_tmp = strstr(p_tmp, "-"))) {
		if (p_tmp) {
			len = _sfax8_grep_version(p_tmp + 1);
			if (len) {
				memcpy(p_str, p_tmp, len + 1);
				p_str += len + 1;
				count += len + 1;
			}
			p_tmp += len + 1;
		}
	}
	*(board + count) = '\0';
	return count;
}

struct board_name *plat_get_board(void)
{
	FILE *file;
	int i, len;
	char model_str[MAX_NAME_LEN] = { 0 };
	char boardname[MAX_NAME_LEN] = { 0 };

	file = fopen("/tmp/sysinfo/model", "r");
	if (file) {
		fread(model_str, sizeof(char), MAX_NAME_LEN, file);
		/* ignore the last char '\n' */
		len = strlen(model_str);
		model_str[len - 1] = '\0';
		len = sfax8_grep_version(model_str, boardname);
		debug(LOG_DEBUG, "get board: %s, len = %d\n", boardname, len);

		/* board name match */
		for (i = BOARD_0; i < BOARD_MAX; i++) {
			if (len == strlen(bd_name[i].name)) {
				if (strncmp(boardname, bd_name[i].name, len) == 0)
					break;
			}
		}
		fclose(file);

		if (i == BOARD_MAX)
			return NULL;
		else
			return &bd_name[i];
	}

	return NULL;
}
