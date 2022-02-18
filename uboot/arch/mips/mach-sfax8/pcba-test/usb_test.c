#include <common.h>
#include <asm/io.h>
#include <mach/pcba_test.h>
#include <usb.h>

int usb_test(void *d)
{
#ifdef CONFIG_USB
	int usb_stor_curr_dev = -1;

#ifdef CONFIG_TARGET_SFA18_MPW0
	writel(0x1, (void*)0xB9E0C88C);
#endif
	printf("enter usb_test!\n");

	if(usb_init() < 0){
		printf("usb init failed!\n");
		return CMD_FAIL;
	}

	usb_stor_curr_dev = usb_stor_scan(1);

	printf("end usb_test!\n");

	if(usb_stor_curr_dev < 0)
		return CMD_FAIL;
	else
		return CMD_SUCCESS;
#else
	printf("Do not have usb experted!\n");
	return CMD_FAIL;
#endif

}
