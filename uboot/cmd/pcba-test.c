#include <common.h>
#include <command.h>
#include <mach/pcba_test.h>

static int run_pcba_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_SFA18_PCBA_TEST
	return sf_pcba_test();
#else
	return 0;
#endif
}

U_BOOT_CMD(
		sf_pcba_test,	CONFIG_SYS_MAXARGS,	1,	run_pcba_test,
	"check to run siflower pcba-test",
	""
);
