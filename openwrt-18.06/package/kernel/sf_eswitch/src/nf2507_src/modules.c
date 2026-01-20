#ifdef L2SW_LINUX_KERNEL
#include <linux/init.h>  
#include <linux/module.h>  
#include "l2sw_switch.h"  

MODULE_LICENSE("GPL");  

static int l2sw_kernel_init(void)  
{  
    int index;
    for(index = 0; index < L2SW_MAX_NUM_OF_UNIT; index++ )
    {
        l2sw_switch_init(index);
    }
        
  
  printk("insmod l2sw success\n");  
  return 0;  
}  
static void l2sw_kernel_exit(void)  
{  
  printk("rmmod l2sw success\n");  
}  
   
module_init(l2sw_kernel_init);  
module_exit(l2sw_kernel_exit); 
#endif


