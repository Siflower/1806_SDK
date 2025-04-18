/*
 * =====================================================================================
 *
 *       Filename:  siwifi_lmac_glue_linux.c
 *
 *    Description:  glue implement for lmac built in kernel
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower
 *
 * =====================================================================================
 */


#include "siwifi_lmac_glue.h"
#include <linux/kthread.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include "siwifi_utils.h"
#include "reg_access.h"
#include "ipc_user_event.h"

#ifdef CONFIG_SMP
#define BOUND_CPU_ID  (CONFIG_NR_CPUS -1)
#endif

#define LMAC_IRQ_NAME (siwifi_mod_params.is_hb ? "lmac-hb" : "lmac-lb")

/*external function declare */
extern void siwifi_pre_alloc(void);
extern int rw_start(void);
extern void sf_wifi_init_context(struct lmac_pl_context *ctx);
extern void rw_intc_fiq(void);
extern void *sf_wifi_shmem_malloc(int size);
extern void sf_wifi_shmem_init(int band, void *base);
extern void sf_wifi_shmem_free(void *ptr);
extern void sf_wifi_shmem_reset(void);
extern unsigned int sf_wifi_shmem_free_size(void);

//standard API for LMAC, any OS must implement this interface when porting lmac code
//make the g_lmac_pl_ctx_impl depart from platform private is for the clean include header files when compile lmac code
static struct lmac_pl_context g_lmac_pl_ctx_impl;

u8 *get_share_env(struct v1_plat_data *priv)
{
    u8 *ipc_shenv;
#ifdef CFG_DYNAMIC_MGM_SHAREMEM
    ipc_shenv = priv->lmac_pl_ctx->share_mem_base + priv->lmac_pl_ctx->share_mem_offset + SHARED_RAM_START_ADDR;
#else
    ipc_shenv =(priv->lmac_pl_ctx->share_mem_base + SHARED_RAM_START_ADDR;
#endif
    return ipc_shenv;
}

static void sf_wifi_lmac_prepare_complete(void)
{
    struct v1_plat_data *priv = (struct v1_plat_data *)g_lmac_pl_ctx_impl.plat_priv;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    priv->lmac_prepared = 1;
    wake_up_all(&priv->lmac_wq);
}

static irqreturn_t sf_wifi_lmac_irq_handle(int irq, void *dev_id)
{
    struct v1_plat_data *priv;
    priv = (struct v1_plat_data *)dev_id;
    priv->lmac_irq_running = 1;
    //wake up the wq
    rw_intc_fiq();
    wake_up_all(&priv->lmac_wq);
    priv->lmac_irq_running = 0;
    return IRQ_HANDLED;
}

static int sf_wifi_lmac_global_irq_request(void)
{
    int ret;
#ifdef CONFIG_SMP
    struct cpumask lmac_irq_affi;
#endif

    struct v1_plat_data *priv = (struct v1_plat_data *)g_lmac_pl_ctx_impl.plat_priv;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if(priv->lmac_irq_requested){
        printk("irq has been already requested!\n");
        return 0;
    }
#ifdef CONFIG_SMP
    ret = request_irq(priv->lmac_irq, sf_wifi_lmac_irq_handle, IRQF_TRIGGER_LOW | IRQF_NOBALANCING, LMAC_IRQ_NAME, priv);
#else
    ret = request_irq(priv->lmac_irq, sf_wifi_lmac_irq_handle, IRQF_TRIGGER_LOW, LMAC_IRQ_NAME, priv);
#endif
    if(ret){
        printk("can not request irq %d\n", priv->lmac_irq);
        return ret;
    }
#ifdef CONFIG_SMP
    memset(&lmac_irq_affi, 0, sizeof(struct cpumask));
    cpumask_set_cpu(BOUND_CPU_ID, &lmac_irq_affi);
    //FIXME:now the irq must work on the same cpu with the lmac main task
    //now I changed the source code of linux irq/manager.c
    //because I found that if I want a irq to be no-balancing and also bounded to a cpu
    //the most standard way to change a irq's affinity is :
    //echo xx > /proc/irq/xxx/smp_affinity & /proc/irq/xxx/affinity_list
    //here we also can use userhelper to do this things , but by this method we can not set the IRQF_NOBALANCING in the request_irq function

    //perhaps, we should implemention another irq_chip in driver/irq_chip to do what we want
    ret = irq_set_affinity(priv->lmac_irq, &lmac_irq_affi);
    if(ret){
        printk("can not set the affinity for irq : %d\n", priv->lmac_irq);
        free_irq(priv->lmac_irq, priv);
        return ret;
    }
#endif
    priv->lmac_irq_requested = 1;
    priv->lmac_irq_disable_dep = 0;
    return 0;
}

static int sf_wifi_lmac_global_irq_free(void)
{
    struct v1_plat_data *priv = (struct v1_plat_data *)g_lmac_pl_ctx_impl.plat_priv;
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    if(!priv->lmac_irq_requested){
        printk("irq has been already freeed!\n");
        return 0;
    }
    free_irq(priv->lmac_irq, priv);
    priv->lmac_irq_requested = 0;
    priv->lmac_irq_disable_dep = 0;
    return 0;
}

static int sf_wifi_lmac_global_irq_enable(void)
{
    struct v1_plat_data *priv = (struct v1_plat_data *)g_lmac_pl_ctx_impl.plat_priv;
    if(!priv->lmac_irq_requested){
        printk("the irq is not requested , can not enable!\n");
        goto DONE;
    }

    if(priv->lmac_irq_running){
        //printk("the irq is going, do not need to disable or enable the irq\n");
        goto DONE;
    }
    WARN_ON(priv->lmac_irq_disable_dep < 1);

    if(priv->lmac_irq_disable_dep == 1){
        enable_irq(priv->lmac_irq);
    }
    priv->lmac_irq_disable_dep--;
DONE:
    return 0;
}


static int sf_wifi_lmac_global_irq_disable(int sync)
{
    struct v1_plat_data *priv = (struct v1_plat_data *)g_lmac_pl_ctx_impl.plat_priv;

    if(!priv->lmac_irq_requested){
        printk("the irq is not requested , can not disable!\n");
        goto DONE;
    }
    if(priv->lmac_irq_running){
        //printk("the irq is going, do not need to disable or enable the irq\n");
        goto DONE;
    }
    if(!priv->lmac_irq_disable_dep){
        if(sync){
            //FIXME :now can not use the sync
            disable_irq(priv->lmac_irq);
        }else{
            disable_irq_nosync(priv->lmac_irq);
        }
    }
    priv->lmac_irq_disable_dep++;
DONE:
    return 0;
}

static void sf_wifi_lmac_res_barrier(int type)
{
    if(type & BARRIER_WMB)
        wmb();
    if(type & BARRIER_RMB)
        rmb();
    if(type & BARRIER_MB)
        mb();
    if(type & BARRIER_IO)
        iob();
}

#ifdef CONFIG_SF16A18_WIFI_LED
#include "lmac_led.h"

//the led lighting interval, the unit is millisecond
#define LMAC_LED_MODULE_ON_LIGHTING_TIMEOUT 5000
#define LMAC_LED_CPU_SLEEPING_TIMEOUT   2000
#define LMAC_LED_HW_WORKING_TIMEOUT    1000
#define LMAC_LED_RX_PACKET_TIMEOUT      500
#define LMAC_LED_TURN_ON_LAST           50
#define LMAC_LED_BLIGHTING_INTERVAL 50

static int sf_wifi_lmac_led_turn_on(int led)
{
    uint32_t timeout = 0;
    struct v1_plat_data *priv = (struct v1_plat_data *)g_lmac_pl_ctx_impl.plat_priv;

    //FIXME:here we should add a spin lock to protect the led_status & led_time
    //because this function(trun on/off) could be called both by software thread(cpu0) and by hardware interrupt(cpu1)
    //but considering this code will be removed & the price to disable the global IRQ,
    //I do not want to implement
    if(priv->led_status & led){
        return 0;
    }

    if(led & LED_HW_RUNNING)
        timeout = 1000;
    if(led & LED_RX_PACKET)
        timeout = 500;
    if(led & LED_CPU_SLEEP){
        timeout = 3000;
    }

    if(!timeout){
        printk("invalid led type, 0x%x\n", led);
        return -1;
    }
    priv->led_timeout = timeout;
    priv->led_status |= led;

    return 0;
}

static int sf_wifi_lmac_led_turn_off(int led)
{
    uint32_t timeout = 0;
    struct v1_plat_data *priv = (struct v1_plat_data *)g_lmac_pl_ctx_impl.plat_priv;
    if(!(priv->led_status & led)){
        return 0;
    }
    if(led & LED_CPU_SLEEP){
        timeout = 2000;
    }
    if(led & LED_HW_RUNNING)
        timeout = 4000;
    if(led & LED_RX_PACKET)
        timeout = 1000;

    if(!timeout){
        printk("invalid led type, 0x%x\n", led);
        return -1;
    }

    priv->led_timeout = timeout;
    priv->led_status &= ~led;

    return 0;
}

static void sf_wifi_lmac_led_expires(unsigned long data)
{
    struct v1_plat_data *priv;
    priv = (struct v1_plat_data *)data;
    if(!priv->led_on){
        led_onoff(priv->band, 1);
        mod_timer(&priv->led_timer, jiffies + HZ * LMAC_LED_BLIGHTING_INTERVAL / 1000);
        priv->led_on = 1;
    }else{
        led_onoff(priv->band, 0);
        mod_timer(&priv->led_timer, jiffies + HZ * priv->led_timeout / 1000);
        priv->led_on = 0;
    }
}

static int sf_wifi_lmac_led_init(struct v1_plat_data *priv)
{
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    //init & led timer to timerlist
    led_gpio_init(priv->band);

    //default used the module on lighting timeout
    init_timer(&priv->led_timer);
    priv->led_timer.expires = jiffies + (HZ * LMAC_LED_MODULE_ON_LIGHTING_TIMEOUT / 1000);
    priv->led_timer.data = (unsigned long) priv;
    priv->led_timer.function = sf_wifi_lmac_led_expires;
    priv->led_timeout = 5000;

    add_timer(&priv->led_timer);

    SIWIFI_DBG(SIWIFI_FN_EXIT_STR);
    return 0;
}

static int sf_wifi_lmac_led_release(struct v1_plat_data *priv)
{
    del_timer_sync(&priv->led_timer);
    return 0;
}
#endif


static void sf_wifi_lmac_task_sleep(struct v1_plat_data *priv)
{
    //SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    if (priv->lmac_pl_ctx->led_on)
        priv->lmac_pl_ctx->led_on(LED_CPU_SLEEP);
}

static void sf_wifi_lmac_task_wake_up(struct v1_plat_data *priv)
{
    //SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);
    if (priv->lmac_pl_ctx->led_on)
        priv->lmac_pl_ctx->led_off(LED_CPU_SLEEP);
}

static int sf_wifi_lmac_sleep_request(uint32_t (*hw_condition)(void))
{
    struct v1_plat_data *priv = (struct v1_plat_data *)g_lmac_pl_ctx_impl.plat_priv;

    wait_event_cmd(priv->lmac_wq,
            (priv->request_lmac_exit || (hw_condition())),
            sf_wifi_lmac_task_sleep(priv),
            sf_wifi_lmac_task_wake_up(priv));
    if(priv->request_lmac_exit){
        printk("user request the main task exit!\n");
        return 1;
    }
    return 0;
}

static int sf_wifi_lmac_deep_debug_enable(int type, int enable)
{
    struct v1_plat_data *priv = (struct v1_plat_data *)g_lmac_pl_ctx_impl.plat_priv;
    uint8_t data;
    //check if conflicts happen for different type, now :
    //LA/IQDUMP/NORMAL FUNCTION can not be worked together, so if one is enable, must denied the others's request
    if(enable &&
            priv->deep_debug_type != 0 &&
            priv->deep_debug_type != type){
        printk("deep_debug %d has been enabled, can not enable another type : %d\n", priv->deep_debug_type, type);
        return 0;
    }

    if(type == DEEP_DEBUG_LA){
#ifndef CONFIG_SF16A18_WIFI_LA_ENABLE
        printk("la not support \n");
        return 0;
#endif
        //share memory selection
        data = readb(REG_SYSM_SHARE_RAM_SEL);
        if(enable)
            data |= ((priv->band & LB_MODULE) ? ( 1 << 2 ) : ( 1 << 3 ));
        else
            data &= ~((priv->band & LB_MODULE) ? ( 1 << 2 ) : ( 1 << 3 ));

        writeb(data, REG_SYSM_SHARE_RAM_SEL);

        //clk selection
        if(enable)
            writeb(1, (priv->band & LB_MODULE) ? REG_SYSM_WIFI1_LA_CLK_SEL : REG_SYSM_WIFI2_LA_CLK_SEL);
        else
            writeb(0, (priv->band & LB_MODULE) ? REG_SYSM_WIFI1_LA_CLK_SEL : REG_SYSM_WIFI2_LA_CLK_SEL);
    }else if(type == DEEP_DEBUG_IQDUMP){
        //TODO

    }else{
        printk("%s invalid type : %d\n", __func__, type);
        return 0;
    }

    if(enable)
        priv->deep_debug_type |= type;
    else
        priv->deep_debug_type &= ~type;

    return 1;
}


static void* sf_wifi_lmac_share_mem_alloc(int size)
{
#ifdef CFG_DYNAMIC_MGM_SHAREMEM
    return sf_wifi_shmem_malloc(size);
#else
    return NULL;
#endif
}

static void sf_wifi_lmac_share_mem_free(void *ptr)
{
#ifdef CFG_DYNAMIC_MGM_SHAREMEM
    sf_wifi_shmem_free(ptr);
#else
    return;
#endif
}

static uint32_t sf_wifi_lmac_share_mem_get_free_size(void)
{
#ifdef CFG_DYNAMIC_MGM_SHAREMEM
    return sf_wifi_shmem_free_size();
#else
    return 0;
#endif
}

static int sf_wifi_lmac_main_loop(void *params)
{
    struct sched_param param = { .sched_priority = MAX_RT_PRIO - 1};
    sched_setscheduler(current, SCHED_FIFO, &param);
    //TODO:how to check the task's state in
    //drivers/net/xen-netback/interface.c
    //rw_start, rw_start should have some callbacks outside
    rw_start();
    return 0;
}

static int sf_wifi_lmac_tasks_create(struct v1_plat_data *priv)
{

    priv->lmac_kthread = kthread_create(sf_wifi_lmac_main_loop, NULL, "lmac_work_thread");
    if (IS_ERR(priv->lmac_kthread)) {
        printk("Could not allocate kthread for lmac_work_thread\n");
        return -1;
    }
#ifdef CONFIG_SMP
    kthread_bind(priv->lmac_kthread, BOUND_CPU_ID);
#endif
    wake_up_process(priv->lmac_kthread);

    //wait the init complete
    wait_event_interruptible(priv->lmac_wq, (priv->lmac_prepared == 1));
    return 0;
}


static void sf_wifi_lmac_tasks_destroy(struct v1_plat_data *priv)
{
    if(priv->lmac_kthread){
        //first wake up the thread
        priv->request_lmac_exit = 1;
        wake_up_all(&priv->lmac_wq);
        kthread_stop(priv->lmac_kthread);
    }
    priv->lmac_kthread = NULL;
}

int lmac_glue_init(struct v1_plat_data *priv)
{
    //init lmac context
    sf_wifi_init_context(&g_lmac_pl_ctx_impl);

    g_lmac_pl_ctx_impl.prepare_complete = sf_wifi_lmac_prepare_complete;
    g_lmac_pl_ctx_impl.request_global_irq = sf_wifi_lmac_global_irq_request;
    g_lmac_pl_ctx_impl.free_global_irq = sf_wifi_lmac_global_irq_free;
    g_lmac_pl_ctx_impl.enable_global_irq = sf_wifi_lmac_global_irq_enable;
    g_lmac_pl_ctx_impl.disable_global_irq = sf_wifi_lmac_global_irq_disable;
    g_lmac_pl_ctx_impl.do_barrier = sf_wifi_lmac_res_barrier;
#ifdef CONFIG_SF16A18_WIFI_LED
    g_lmac_pl_ctx_impl.led_on = sf_wifi_lmac_led_turn_on;
    g_lmac_pl_ctx_impl.led_off = sf_wifi_lmac_led_turn_off;
#else
    g_lmac_pl_ctx_impl.led_on = NULL;
    g_lmac_pl_ctx_impl.led_off = NULL;
#endif
    g_lmac_pl_ctx_impl.request_sleep = sf_wifi_lmac_sleep_request;
#if defined(CFG_DYNAMIC_MGM_SHAREMEM)
    g_lmac_pl_ctx_impl.allocate_share_mem = sf_wifi_lmac_share_mem_alloc;
    g_lmac_pl_ctx_impl.free_share_mem = sf_wifi_lmac_share_mem_free;
    g_lmac_pl_ctx_impl.get_free_size_share_mem = sf_wifi_lmac_share_mem_get_free_size;
#endif
    g_lmac_pl_ctx_impl.deep_debug_enable = sf_wifi_lmac_deep_debug_enable;

    g_lmac_pl_ctx_impl.share_mem_base = NULL;
    g_lmac_pl_ctx_impl.share_mem_offset = 0;
    g_lmac_pl_ctx_impl.share_mem_total_size = 0;

    g_lmac_pl_ctx_impl.plat_priv = (void *)priv;
    priv->lmac_pl_ctx = &g_lmac_pl_ctx_impl;
    priv->lmac_irq_disable_dep = 0;
    priv->lmac_kthread = NULL;
    priv->lmac_irq_running = 0;

    init_waitqueue_head(&priv->lmac_wq);
#ifdef CONFIG_SF16A18_WIFI_LED
    priv->led_timeout = 0;
    priv->led_status = 0;
    priv->led_on = 0;
#endif

#ifdef CFG_DYNAMIC_MGM_SHAREMEM
    sf_wifi_shmem_init(priv->band, priv->base);
#else
    g_lmac_pl_ctx_impl.share_mem_base = priv->base;
    g_lmac_pl_ctx_impl.share_mem_offset = 0;
    //FIXME:
    //if we do not use the dynamic method to deal with the share memory
    //the share ram total size is meaningless
    g_lmac_pl_ctx_impl.share_mem_total_size = 0;
#endif
#ifdef CONFIG_SF16A18_WIFI_LED
    sf_wifi_lmac_led_init(priv);
#endif

    printk("lmac init with linux lmac_irq : %d, umac_irq : %d\n", priv->lmac_irq, priv->umac_irq);
    return 0;
}

u8 *lmac_glue_share_mem_init(struct v1_plat_data *priv)
{
#ifdef CFG_DYNAMIC_MGM_SHAREMEM
    sf_wifi_shmem_reset();
    siwifi_pre_alloc();
#endif
    return get_share_env(priv);
}

int lmac_glue_start(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{
    int ret = 0;
    if((ret = sf_wifi_lmac_tasks_create(priv))){
        printk("can not create lmac tasks, return %d\n", ret);
        return ret;
    }
    return ret;
}

void lmac_glue_stop(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{
    sf_wifi_lmac_tasks_destroy(priv);
}

void lmac_glue_deinit(struct v1_plat_data *priv)
{
#ifdef CONFIG_SF16A18_WIFI_LED
    /* reelease the wifi led resources*/
    sf_wifi_lmac_led_release(priv);
#endif
}

void notify_lmac_complete_ipc(struct siwifi_hw *siwifi_hw)
{
    sf_wifi_lmac_prepare_complete();
}

void notify_lmac_la_init_ipc(struct siwifi_hw *siwifi_hw, int8_t type,int8_t enable)
{
    int ret = sf_wifi_lmac_deep_debug_enable(type, enable);
    struct v1_plat_data *priv = (struct v1_plat_data *)&siwifi_hw->plat->priv;
    set_ipc_event_user_reply(get_share_env(priv), ret);
    clear_ipc_event_user_bit(get_share_env(priv), IPC_USER_EVENT_DEEP_DEBUG_SET);
}
