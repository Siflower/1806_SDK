#ifndef SF_SWITCH_INIT_H
#define SF_SWITCH_INIT_H


#define PORT_TPID_START_POS                  0
#define PORT_FALLBACK_BDID_START_POS         16
#define PORT_SHUTDOWN_START_POS              0
#define PORT_AFT_START_POS                   4
#define PORT_BLOCKSTATE_START_POS            8
#define PORT_DEF_CFI_START_POS               12
#define PORT_DEF_PRI_START_POS               13
#define PORT_DEF_TC_START_POS                16
#define PORT_TRUSTED_START_POS               19
#define PORT_VID_PREFIX_START_POS            20
#define PORT_UNTAG_FROM_BTABLE_START_POS     21

#define PORT_TPID_MASK                       0x0000FFFF
#define PORT_FALLBACK_BDID_MASK              0x1FFF0000
#define PORT_SHUTDOWN_MASK                   0x00000001
#define PORT_AFT_MASK                        0x000000F0
#define PORT_BLOCKSTATE_MASK                 0x00000F00
#define PORT_DEF_CFI_MASK                    0x00001000
#define PORT_DEF_PRI_MASK                    0x0000E000
#define PORT_DEF_TC_MASK                     0x00070000
#define PORT_TRUSTED_MASK                    0x00080000
#define PORT_VID_PREFIX_MASK                 0x00100000
#define PORT_UNTAG_FROM_BTABLE_MASK          0x00200000

#define DEF_PORT_FALLBACK_BD_ID    0x1        /* iports fallback bd id */

void sf_local_reset(void);
int sf_init_switch_hw(void);
int sf_switch_init_timer(int init);
void sys_npu_reset(void);

int sf_port_init(void *pswitch);
int get_port_address(int port_num, int index);

#endif /* ifndef SF_SWITCH_INIT_H */
