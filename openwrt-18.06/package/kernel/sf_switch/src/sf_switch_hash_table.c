#include "sf_switch_hash_table.h"
#include "sf_mdio.h"
#include "sf_debug.h"
#include "sf_switch_init.h"
#include <linux/if_vlan.h>

spinlock_t mac_hash_lock;                     /* Spin lock flag */
spinlock_t vlan_hash_lock;                     /* Spin lock flag */
static unsigned char add_fail_count = 0;
static unsigned char add_count = 0;
//when lots of mac come in a short time, trigger a host ageing
unsigned char start_ageing= 0;

int  hash_table_config_done(int type) {
	uint status_reg = 0;
	int cmd_done = 0;
	int i = 0;
	if(type == MAC_TABLE)
	  status_reg  = REQ_STATUS_REG;
	else if(type == VLAN_TABLE)
	  status_reg  = REQ_VLAN_STATUS_REG;
	else
	  return 0;

	/*Read command status rteg*/
	cmd_done = sf_readl(status_reg);
	/* Poll the Status Register for CMD DONE */
	while (!(cmd_done & 0x1)) {
		if(i >= 1000){
			printk("time out for wait hash table config done\n");
			return 0;
		}
		cmd_done = sf_readl(status_reg);
		udelay(10);
		i++;
	} /*End of while loop*/
	/* Clear Status Reg */
	sf_writel( CLR_STATUS_REG,status_reg);
	return cmd_done;
}

void set_vlan_id_addr_reg(int *reg_value, int size, int type){
	int i, data[5] = {0};

	memcpy(data, reg_value, size);
	/* initialize mac/vlan addresses  Link List formation 0 for mac, 1 for vlan*/
	for (i = 0; i < 5; i++){
	  if (type == 0)
		sf_writel(data[i], (REQ_MAC1_ADDR_REG + i*4));
	  else
		sf_writel(data[i], (REQ_VLAN_MAC1_ADDR_REG + i*4));
	}
}

void mac_hash_table_init(void) {
	int  i = 0, reg_value[5] = {0};
	unsigned int index = 0, mac_table_addr = 0, col_ptr = 0;

	spin_lock_init(&mac_hash_lock);
	/* 2-field Mac table format: Table Depth - 1024(hash 512 + collison 512)
	 *============================================================================*
	 * valid bits | col_ptr | portNO | field_valids | action entry |vlanid |MAC *
	 *============================================================================*
	 * 4-bit      | 16-bit  | 4-bit  | 8-bit        | 31-bit       |13-bit |48-bit *
	 *============================================================================*/
	/* INIT CMD: Init total Hash Space with zeros */
	sf_writel( CMD_INIT ,REQ_CMD_REG);

	hash_table_config_done(MAC_TABLE);

	set_vlan_id_addr_reg(reg_value, sizeof(reg_value), 0);
	sf_writel( 0x0, REQ_ENTRY_REG );
	/* chain the collision space */
	for (i = 0; i < MAC_COLL_ENTRIES; i++) {
		/* 32bit: 4res + 4validbits + 16colptr + 4portnum +
		   (4)part of fieldvalids */
		//{8’d4, col_ptr[15:0], 8’d0}
		col_ptr = (0x40201 + i) << 8;
		sf_writel( col_ptr, REQ_MAC4_ADDR_REG);
		index = MAC_HASH_ENTRIES + i;
		mac_table_addr = (index << 16) + CMD_MEM_WRITE;
		sf_writel( mac_table_addr ,REQ_CMD_REG);
		hash_table_config_done(MAC_TABLE);
	} /*End of for loop collision space*/
	/* Intialize Free List Head Ptr */
	sf_writel( MAC_INIT_HEAD_PTR, FREE_LIST_HEAD_PTR);

	/* Initialize Free List Ptr */
	sf_writel( MAC_INIT_TAIL_PTR, FREE_LIST_TAIL_PTR);

	/* Initialize No of entries */
	sf_writel( MAC_COLL_ENTRIES, FREE_LIST_ENTRIES);
} /*end of hw_hash_table_init Function*/

void vlan_hash_table_init(void) {
	int  i = 0, reg_value[5] = {0};
	unsigned int index = 0, mac_table_addr = 0, col_ptr = 0;

	/* Avoid Multiple PE's Initialization */

	spin_lock_init(&vlan_hash_lock);
	/* vlan group table format *
	 *=====================================================================*
	 * valid bits | col_ptr | portNO | field_valids | action entry | vlan  *
	 *=====================================================================*
	 * 4-bit      | 16-bit  | 4-bit  | 8-bit        | 32-bit       | 12-bit*
	 *=====================================================================*/
	/* VLAN HASH MEMORY INTIALIZATION */

	/* Initialize CMD register for Hash Table Intializes with zeros */
	sf_writel( CMD_INIT, REQ_VLAN_CMD_REG);

	hash_table_config_done(VLAN_TABLE);

	set_vlan_id_addr_reg(reg_value, sizeof(reg_value), 1);

	/* clear mac1 & mac2 registers */
	/* chain the vlan collision space */
	for (i = 0; i < 64; i++) {
		col_ptr = (0x40041 + i);
		sf_writel( ((col_ptr & 0xFF) << 24), REQ_VLAN_MAC2_ADDR_REG);
		sf_writel( (col_ptr >> 8) & 0xFFF, REQ_VLAN_MAC3_ADDR_REG );
		index = VLAN_HASH_ENTRIES + i;
		mac_table_addr = (index << 16);
		sf_writel( CMD_MEM_WRITE | mac_table_addr, REQ_VLAN_CMD_REG);

		hash_table_config_done(VLAN_TABLE);
	} /*End of vlan collision spce for loop*/

	/* Intialize Free List Head Ptr */
	sf_writel( VLAN_INIT_HEAD_PTR, FREE_VLAN_LIST_HEAD_PTR);

	/* Initialize Free List Ptr */
	sf_writel( VLAN_INIT_TAIL_PTR, FREE_VLAN_LIST_TAIL_PTR);

	/* Initialize No of entries */
	sf_writel( VLAN_FREE_LIST_ENTRIES, FREE_VLAN_LIST_ENTRIES);


} /*end of hw_vlan_hash_table_init Function*/

int vlan_hash_table_insert(uint field_entries, uint portNo, uint action_entry) {
	int cmd_done = 0, vlanNo = 0, reg_value[5] = {0};
	uint field_valids =  BIT_SET_VLAN_1F_VLANTBL;

	spin_lock_bh(&vlan_hash_lock);
	/* print_msg(SWITCH_INIT_DEBUG,NULL,"TEST:REQ_VLAN_CMD_REG:%x\n",
	 *                             REQ_VLAN_CMD_REG); */
	/* vlan group table format *
	 **======================================================================*
	 ** valid bits | col_ptr | portNo | field_valids | action entry | vlan   *
	 **======================================================================*
	 ** 4-bit      | 16-bit  | 4-bit  | 8-bit        | 32-bit       | 12-bit *
	 **=====================================================================*/

	/* VLAN (BD) Hash table action entry map:*
	 **=======================================*
	 ** [07:00]         - forward port list
	 ** [15:08]         - untag list
	 ** [18:16]         - ucast_hit_action
	 ** [21:19]         - mcast_hit_action
	 ** [24:22]         - ucast_miss_action
	 ** [27:25]         - mcast_miss_action
	 ** [30:28]         - mstp
	 ** [31:31]         - Not used yet
	 **=======================================*/

	/* fill vlan reg */
	vlanNo = field_entries;
	/*      print_msg(SWITCH_INIT_DEBUG,NULL,"ADD VID:%x\r\n", vlanNo); */
	reg_value[0] = vlanNo & VLANID_MASK;
	set_vlan_id_addr_reg(reg_value, sizeof(reg_value), 1);

	/* fill ENTRY reg */
	/*  print_msg(SWITCH_INIT_DEBUG,NULL,"VLAN ENTRY %x \r\n",
	 *                  action_entry & BRENTRY_ACTION_ENTRY_MASK); */
	sf_writel( action_entry & BRENTRY_ACTION_ENTRY_MASK, REQ_VLAN_ENTRY_REG);

	/* Description of ADD CMD REG:
	 ** 3:0   - CMD
	 ** 7:4   - Reserved
	 ** 8     - Vlan Field Valid
	 ** 15:9 - Reserved
	 ** 31:16 - Mem Index / portno[16:19]
	 **/

	/* Issue ADD cmd */
	sf_writel( CMD_ADD |(field_valids & CMD_ENTRY_FIELD_VALID_MASK), REQ_VLAN_CMD_REG);
	/* Description of STAUTS CMD REG:
	 ** 0 - req1_cmd_done
	 ** 1 - req1_sig_entry_not_found
	 ** 2 - req1_sig_init_done
	 ** 3 - req1_sig_entry_added
	 ** 4 - req1_match
	 ** 31:5 - reserved.
	 **/

	cmd_done = hash_table_config_done(VLAN_TABLE);

	spin_unlock_bh(&vlan_hash_lock);
	if ((cmd_done & STATUS_CMD_DONE) &&(cmd_done & STATUS_ENTRY_ADDED)) {
		return 0;
	} else {
		return -1;
	}
}

int vlan_hash_table_flush(void) {
	int cmd_done = 0, reg_value[5] = {0};

	spin_lock_bh(&vlan_hash_lock);
	reg_value[0] = 0x1;
	set_vlan_id_addr_reg(reg_value, sizeof(reg_value), 1);


	/* Issue UPDATE cmd */
	sf_writel(  CMD_FLUSH | 0x4000, REQ_VLAN_CMD_REG);

	cmd_done = hash_table_config_done(VLAN_TABLE);

	spin_unlock_bh(&vlan_hash_lock);
	if (cmd_done) {

		/* Entry Added/Updated Successfully */
		return 0;
	} else {
		/*  print_msg(SWITCH_INIT_DEBUG,NULL,
			"VLAN UPDATE CMD FAILED\n"); */
		/* Entry Not Added */
		return -1;
	}

	return 0;
}

int add_vlan_entry_args(int vlanid, int port, int port_list, int untag_list) {

	uint action_entry = 0;

	/*Adding vlan id 1, port list*/
	action_entry =  ((port_list << BRENTRY_FWD_PORT_LIST_START_POS) & BRENTRY_FWD_PORT_LIST_MASK) | ((untag_list << BRENTRY_UNTAG_LIST_START_POS) & BRENTRY_UNTAG_LIST_MASK);

	return vlan_hash_table_insert(vlanid, port, action_entry);
}

void mac_init_mac_reg(uint field_entries[5])
{
	/*Writing mac address*/
	sf_writel(SF_INT_SWAP(field_entries[0]), REQ_MAC1_ADDR_REG);
	sf_writel((field_entries[1] & 0xFFFF0000) | SF_SHORT_SWAP(field_entries[1]), REQ_MAC2_ADDR_REG);
	sf_writel(SF_INT_SWAP(field_entries[2]), REQ_MAC3_ADDR_REG);
	sf_writel(SF_INT_SWAP(field_entries[3]), REQ_MAC4_ADDR_REG);
}

int mac_hash_table_search(uint field_entries[5], uint field_valids)
{
	int cmd_done = 0, entry = 0;

	spin_lock_bh(&mac_hash_lock);
	/* initialize MAC regs */
	mac_init_mac_reg(field_entries);

	sf_writel(CMD_SEARCH | ((field_valids & CMD_ENTRY_FIELD_VALID_MASK)), REQ_CMD_REG);

	cmd_done = hash_table_config_done(MAC_TABLE);

	/*Bridge entry match done check*/
	if (cmd_done & STATUS_ENTRY_MATCH) {
		/* SEARCH FOUND */
		cmd_done = sf_readl(REQ_ENTRY_REG);
		entry = cmd_done & BRENTRY_ACTION_ENTRY_MASK;
		/*  print_msg(SWITCH_INIT_DEBUG,NULL,"%s: entry = %x\n",
		 *                              __func__, entry); */
		/* Clear req entry reg */
		sf_writel(0x0, REQ_ENTRY_REG);
		spin_unlock_bh(&mac_hash_lock);
		return entry;
	}
	/* SEARCH NOT FOUND */
	spin_unlock_bh(&mac_hash_lock);
	return -1;

} /*End of hw_hash_table_search Function*/

int vlan_hash_table_search(uint vlanid) {
	int  cmd_done = 0, entry = 0, reg_value[5] = {0},
		 field_valids = BIT_SET_VLAN_1F_VLANTBL;

	spin_lock_bh(&vlan_hash_lock);
	reg_value[0] = vlanid & VLANID_MASK;
	set_vlan_id_addr_reg(reg_value, sizeof(reg_value), 1);

	/* write the search command */
	sf_writel(CMD_SEARCH | field_valids, REQ_VLAN_CMD_REG);

	cmd_done = hash_table_config_done(VLAN_TABLE);

	/*Bridge entry match done check*/
	if (cmd_done & STATUS_ENTRY_MATCH) {

		/* SEARCH FOUND */
		cmd_done = sf_readl(REQ_VLAN_ENTRY_REG);
		entry = (cmd_done & BRENTRY_ACTION_ENTRY_MASK);
		/* Clear req entry reg */
		sf_writel(0x0, REQ_VLAN_ENTRY_REG);
		spin_unlock_bh(&vlan_hash_lock);
		return entry;
	} else {
			/* SEARCH NOT FOUND */

		spin_unlock_bh(&vlan_hash_lock);
		return -1;
	} /*End of bridge entry match done -IF condition*/

} /*End of wsp_vlan_entry_search Function*/
/**********************************************************************
 * * Function Name  : wsp_vlan_entry_update
 * * Description    : Update given BRENTRY PTR into HASH MAC TABLE
 * * Inputs
 * * Parameters   : - Mac Address and brentry
 * * Globals      : -
 * * Outputs        :
 * *  Parameters  : -
 * * Returns      :   Returns Entry Update Status
 * * Changes        :
 * **********************************************************************/
int vlan_hash_entry_update( uint vlanid, uint action_entry) {

	int  cmd_done = 0, reg_value[5] = {0};
	uint field_valids =  BIT_SET_VLAN_1F_VLANTBL;
	spin_lock_bh(&vlan_hash_lock);

	reg_value[0] = vlanid & VLANID_MASK;
	set_vlan_id_addr_reg(reg_value, sizeof(reg_value), 1);


	/* fill ENTRY reg if valid */
	sf_writel(action_entry & BRENTRY_ACTION_ENTRY_MASK, REQ_VLAN_ENTRY_REG);

	/* Issue UPDATE cmd */
	sf_writel(CMD_UPDATE | field_valids, REQ_VLAN_CMD_REG);

	cmd_done = hash_table_config_done(VLAN_TABLE);
	spin_unlock_bh(&vlan_hash_lock);

	if (cmd_done & STATUS_ENTRY_ADDED) {
		/* Entry Added/Updated Successfully */
		return 0;
	} else {
		/*  print_msg(SWITCH_INIT_DEBUG,NULL,
			"VLAN UPDATE CMD FAILED\n"); */
		/* Entry Not Added */
		return -1;
	}

	return 0;
} /*End of wsp_vlan_entry_update() Function*/

#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
/**********************************************************************
 * * Function Name  : vlan_hash_table_refresh
 * * Description    : if we check phy link up or down,we will refresh the vlan hash table
 * *                  to adapt the energy detect mode
 * **********************************************************************/
int vlan_hash_table_refresh(struct switch_dev *dev) {
	struct sfax8_switch_t *sfax8_switch = container_of(dev, struct sfax8_switch_t, swdev);
	struct sf_priv *priv = (struct sf_priv *)netdev_priv(sfax8_switch->netdev);
	int i = 0, vid = 0, member = 0, member_insert = 0, action_entry = 0;
	int member_current = 0;
	/*vlan1 has 4 port,if port link up,set port_change to 2;if port link down,set port_change to 1*/
	unsigned char port_change[3] = {0};

	vid = sfax8_switch->vlan_entries[1].vid;
	member = sfax8_switch->vlan_entries[1].member;
	member_current = ((vlan_hash_table_search(vid) & BRENTRY_FWD_PORT_LIST_MASK) >> BRENTRY_FWD_PORT_LIST_START_POS) & ((vlan_hash_table_search(vid) & BRENTRY_UNTAG_LIST_MASK) >> BRENTRY_UNTAG_LIST_START_POS);
	member_insert = 0;
	for (i = 0; i < SF_POWERSAVE_PORTS; i++){
		if(check_phy_linkup(i, priv)){
			member_insert |= 1 << i;
			if (!(member_current & (1 << i)))
			  port_change[i] = 2;
		}else{
			if ((member_current & (1 << i)))
			  port_change[i] = 1;
		}
	}

	member_insert |= 1 << SF_HOST_PORT;
	action_entry = ((member_insert << BRENTRY_FWD_PORT_LIST_START_POS) & BRENTRY_FWD_PORT_LIST_MASK) | ((member_insert << BRENTRY_UNTAG_LIST_START_POS) & BRENTRY_UNTAG_LIST_MASK);
	print_msg(SWITCH_INIT_DEBUG,NULL,"update vlan entry member 0x%x, vid %d\n",member_insert, vid);
	vlan_hash_entry_update(vid, action_entry);

	for (i = 0; i < SF_POWERSAVE_PORTS; i++){
		/*port_change value 2 means port i link from down to up, should disable energy detect mode*/
		if (port_change[i] == 2)
		  sf_phy_energy_detect_disable(i, priv->phy_dev);
		/*port_change value 1 means port i link from up to down, should enable energy detect mode*/
		else if (port_change[i] == 1)
		  sf_phy_energy_detect_enable(i, priv->phy_dev);
	}

	return 0;
}
#endif

int mac_hash_table_update(uint field_entries[5], uint brentry, uint field_valids) {
	int cmd_done = 0;
	spin_lock_bh(&mac_hash_lock);
	/* initialize mac regs */
	mac_init_mac_reg(field_entries);

	/* fill ENTRY reg if valid */
	sf_writel( brentry, REQ_ENTRY_REG);

	/* Issue UPDATE cmd */
	sf_writel(CMD_UPDATE | ((field_valids & CMD_ENTRY_FIELD_VALID_MASK)), REQ_CMD_REG);

	cmd_done = hash_table_config_done(MAC_TABLE);
	sf_writel( 0x0, REQ_ENTRY_REG);
	spin_unlock_bh(&mac_hash_lock);
	if (cmd_done & STATUS_ENTRY_ADDED) {
		/* Entry Added/Updated Successfully */
		return 0;
	}
	return -1;
} /*End of hw_hash_table_update Function*/

int mac_hash_table_add(uint field_entries[5],  uint iport, uint brentry, uint field_valids) {
	int cmd_done = 0;
	spin_lock_bh(&mac_hash_lock);

	/* initialize MAC regs */
	mac_init_mac_reg(field_entries);

	/* fill ENTRY reg */
	sf_writel( brentry, REQ_ENTRY_REG);

	/* Description of ADD CMD REG:
	 ** 3:0   - CMD
	 ** 7:4   - Reserved
	 ** 8     - DMAC Field Valid
	 ** 9     - Vlan Field Valid
	 ** 15:10 - Reserved
	 ** 31:16 - Mem Index
	 **/

	/* Issue ADD cmd */
	sf_writel(CMD_ADD | field_valids | (iport << CMD_ENTRY_PORTS_START_POS), REQ_CMD_REG);
	/* print_msg(SWITCH_INIT_DEBUG,NULL,"ADD %x \r\n",CMD_ADD |
	 *         field_valids | (iport << CMD_ENTRY_PORTS_START_POS)); */

	/* Description of STAUTS CMD REG:
	 ** 0 - req1_cmd_done
	 ** 1 - req1_sig_entry_not_found
	 ** 2 - req1_sig_init_done
	 ** 3 - req1_sig_entry_added
	 ** 4 - req1_match
	 ** 31:5 - reserved.
	 **/

	cmd_done = hash_table_config_done(MAC_TABLE);
	sf_writel( 0, REQ_ENTRY_REG);
	spin_unlock_bh(&mac_hash_lock);

	if ((cmd_done & STATUS_CMD_DONE) &&( cmd_done & STATUS_ENTRY_ADDED)) {

		/* print_msg(SWITCH_INIT_DEBUG,NULL,"ADD CMD DONE\n");
		 *         Entry Added Successfully */
		/* update hash entry count for cli */
		add_count++;
		return 0;
	} else {

		/*      print_msg(SWITCH_INIT_DEBUG,NULL,"ADD CMD FAILED\n"); */
		/* Entry Not Added */
		return -1;
	}
} /*End of hw_hash_table_add Function*/

void host_ageing(void) {
	uint hashidx = 0, vlan_valid = 0;
	uint coll_ptr = 0, mac3 = 0, mac4 = 0;
	static uint hashBase;

	/* 2-field Mac table format: Table Depth - 1024(hash 512 + collison 512)
	 * *============================================================================*
     * * valid bits | col_ptr | portNO | field_valids | action entry | vlanid|  MAC *
     * *============================================================================*
     * * 4-bit      | 16-bit  | 4-bit  | 8-bit(mac4)  | 31-bit (mac3)| 13-bit|48-bit*
     * *============================================================================*/

    /*=============================================================================*
     * * MAC1:MAC[31:0]                                                               *
     * *==============================================================================*
     * * MAC2:ACT_ENTRY[31:29] + VLANID[28:16] + MAC[15:0]                            *
     * *==============================================================================*
     * * MAC3:FIELD_VALIDS[31:28] + ACT_ENTRY[27:0]                                   *
     * *==============================================================================*
     * *MAC4:RSVD[31:28]+VALIDBITS[27:24]+COLPTR[23:8]+ PORTNO[7:4] + FIELDVALIDS[3:0]*
     * *=============================================================================*/
	if(add_count < 500 ) return;

    for (hashidx = 0; hashidx <  HASH_CNT_PER_ITERATION; hashidx++) {
        coll_ptr = mac3 = mac4 = vlan_valid = 0;

		spin_lock_bh(&mac_hash_lock);
        sf_writel(0, REQ_MAC1_ADDR_REG );
        sf_writel(0, REQ_MAC2_ADDR_REG );
        sf_writel(0, REQ_MAC3_ADDR_REG );
        sf_writel(0, REQ_MAC4_ADDR_REG );

        /* Read hash index entry */
		sf_writel(((hashBase + hashidx)<<16) | CMD_MEM_READ,REQ_CMD_REG);
		hash_table_config_done(MAC_TABLE);

		/* read field_entries */
		mac4 = sf_readl(REQ_MAC4_ADDR_REG);

        if (HASH_ENTRY_VALID_FLAG & mac4) {

            if (mac4 & HASH_ENTRY_COLL_FLAG)
              coll_ptr = (mac4 >> 8) & 0xFFFF;

            mac3 = sf_readl(REQ_MAC3_ADDR_REG);
            if (HASH_ENTRY_FRESH_FLAG & mac3) {
                /* delete */
                /*  vlan_valid =
                 *    (mac3 & HASH_ENTRY_VLAN_SET_FLAG) ?
                 *    BIT_SET_VLAN_2F_MACTBL : 0; */
                sf_writel( CMD_DEL | BIT_SET_MAC_2F_MACTBL |
                            BIT_SET_VLAN_2F_MACTBL, REQ_CMD_REG);
				hash_table_config_done(MAC_TABLE);
				add_count--;
                if (coll_ptr) {
					hashidx--;
					spin_unlock_bh(&mac_hash_lock);
                    continue;
                }
            } else {
                /* update */
                if (!(mac3 & HASH_ENTRY_STATIC_FLAG)) {

                    mac3 |= HASH_ENTRY_FRESH_FLAG;
                    sf_writel(mac3, REQ_MAC3_ADDR_REG );
                    sf_writel(CMD_MEM_WRITE | ((hashBase + hashidx) << 16), REQ_CMD_REG);
					hash_table_config_done(MAC_TABLE);
                } else {
					spin_unlock_bh(&mac_hash_lock);
                    continue;
                }
            }
        } /* End if - HASH_ENTRY_VALID_FLAG */
        /* collision space entries */
		spin_unlock_bh(&mac_hash_lock);

        while (coll_ptr) {

			spin_lock_bh(&mac_hash_lock);
            sf_writel(0, REQ_MAC1_ADDR_REG );
            sf_writel(0, REQ_MAC2_ADDR_REG );
            sf_writel(0, REQ_MAC3_ADDR_REG );
            sf_writel(0, REQ_MAC4_ADDR_REG );
            /* Read hash index entry */
            sf_writel( (coll_ptr<<16) | CMD_MEM_READ, REQ_CMD_REG);
            hash_table_config_done(MAC_TABLE);
                /* read field_entries */
            mac3 = mac4 = vlan_valid = 0;
            mac4 = sf_readl(REQ_MAC4_ADDR_REG);
            /* check entry's validity */
            if (HASH_ENTRY_VALID_FLAG & mac4) {

                mac3 = sf_readl(REQ_MAC3_ADDR_REG);
                if (HASH_ENTRY_FRESH_FLAG & mac3) {
                    /* delete */
                    /* vlan_valid = (mac3 &
                     *                         HASH_ENTRY_VLAN_SET_FLAG) ?
                     *                                                 BIT_SET_VLAN_2F_MACTBL : 0;*/
                    sf_writel( CMD_DEL | BIT_SET_MAC_2F_MACTBL | BIT_SET_VLAN_2F_MACTBL, REQ_CMD_REG);
					hash_table_config_done(MAC_TABLE);
					add_count--;
                } else {

                    /* update */
                    mac3 |= HASH_ENTRY_FRESH_FLAG;
                    sf_writel(mac3, REQ_MAC3_ADDR_REG );
                    /* Issue the MEM write cmd */
                    sf_writel(CMD_MEM_WRITE | (coll_ptr << 16),REQ_CMD_REG);
					hash_table_config_done(MAC_TABLE);
                }
                coll_ptr = 0;
                if (mac4 & HASH_ENTRY_COLL_FLAG)
                  coll_ptr = (mac4 >> 8) & 0xFFFF;
            } /* End if - HASH_ENTRY_VALID_FLAG */
			spin_unlock_bh(&mac_hash_lock);
        } /* End While - coll_ptr */

    } /* end of for loop- hashidx */
    /*update hashBase */
    hashBase += hashidx;
    /* updating entries in blocks */
    if (hashBase >= 511)
      hashBase = 0;
}

int sf_switch_hash_learn(pktHdrs_t *pktHdr, struct sf_priv *priv) {

    uchar *mac;
    int ret = -1;
    uint fwd_action = 0;
    uint vlanid = 0, action_entry = 0;
	uint field_entries[5], field_valids = BIT_SET_MAC_2F_MACTBL;
    uint action_portno = 0, phy_port = 0;
    struct ethhdr *ethHdr = (struct ethhdr *)pktHdr->data;
	if(add_count > FAST_AGEING_THRES){
		start_ageing = 1;
		host_ageing();
		start_ageing = 0;
	}
	print_msg(SWITCH_DEBUG,priv->netdev,"%s vlan_id %d \n",__FUNCTION__, pktHdr->vlanid);
    mac = ethHdr->h_source;
	if(pktHdr->vlanid == 0 )
	  vlanid = DEF_PORT_FALLBACK_BD_ID;
	else
	  vlanid = pktHdr->vlanid;


#ifdef CONFIG_SFAX8_SWITCH_VLAN
    if (pktHdr->vlanid == 0) {
        vlanid = priv->sfax8_switch->port[pktHdr->phyno].fallback_bd_id;
    } else vlanid = pktHdr->vlanid;
#endif

    /* initialize required filed valids for mac table learning/searching */

	/* need to updated field entries valid filed basis */
	field_entries[0] = ((*mac << 0)&0x000000ff) |((*(mac + 1) << 8)&0x0000ff00) | ((*(mac + 2) << 16)&0x00ff0000) |((*(mac + 3) << 24)&0xff000000);

	field_entries[1] = ((*(mac + 4) << 0)&0x000000ff) |((*(mac + 5) << 8)&0x0000ff00);

	field_entries[1] |= (vlanid << 16);
	field_entries[2] = field_entries[3] = field_entries[4] = 0;

    /* update vlan field valid for mac table */
    field_valids |= BIT_SET_VLAN_2F_MACTBL;

    phy_port = pktHdr->phyno;

    /* check for for already entry existence */
    ret = mac_hash_table_search(field_entries, field_valids);
	print_msg(SWITCH_DEBUG,priv->netdev,"search ret is 0x%x\n",ret);
    if (-1 == ret) {
		if(pktHdr->punt_reason != SF_PUNT_SA_MISS) netdev_err(priv->netdev, "error mac %pM not find entry with punt reason %d phy_port=%d\n ", mac,pktHdr->punt_reason,phy_port);
		print_msg(SWITCH_DEBUG,priv->netdev,"learn info is mac %pM, vid=%d phy_port=%d\n",mac,vlanid,phy_port);
		/* fill action_entry's fresh bit */
		action_entry = FRESH_FLAG_SET;
		/* Add SMAC entry with action entry & valid fields */
		/* update action entry with port info
		 *                     on which it arrived */
		action_entry |= ((0x1 << phy_port) &
					FWD_PORT_LIST_MASK);

		/* Get bridge action entry */
		ret = vlan_hash_table_search(vlanid);
		if (-1 == ret) {
			/* Un handeled bridge entry,
			 *             take fwd action of fp_global br entry */
			fwd_action = ACT_FORWARD;
		} else {
            /* Get bridge action entry's
             *                     unicast hit fwd action */
            fwd_action =
                (ret & BRENTRY_UCAST_HIT_ACT_MASK) >>
                BRENTRY_UCAST_HIT_ACT_START_POS;
        }
		/* Update mac action entry's fwd action */
		action_entry = (action_entry & ~MAC_2F_ENTRY_FWD_ACT_MASK) |
			(fwd_action << MAC_2F_ENTRY_FWD_ACT_START_POS);

		if(memcmp(ethHdr->h_source, priv->netdev->dev_addr, sizeof(ethHdr->h_source)) == 0){
			action_entry |= (0x1 << MAC_2F_ENTRY_STATIC_BIT_POS);
		}
		print_msg(SWITCH_DEBUG,priv->netdev,"add action entry is 0x%x field valids is 0x%x\n",action_entry,field_valids);
			/* add mac entry */
		ret = mac_hash_table_add(field_entries, phy_port,
					action_entry, field_valids);
		if (ret == -1) {
			netdev_err(priv->netdev,"add mac entry failed mac address is %pM fail count %d add count %d\n",ethHdr->h_source, add_fail_count, add_count);
			add_fail_count++;
			// BUG_ON(add_fail_count > 10);
			return -1;
		}
		else{
			add_fail_count = 0;
		}

	} else {
		if(pktHdr->punt_reason == SF_PUNT_SA_IS_ACTIVE || pktHdr->punt_reason == SF_PUNT_SA_RELEARN){
			print_msg(SWITCH_DEBUG,priv->netdev,"relearn or active is mac %pM, vid=%d phy_port=%d\n", mac,vlanid,phy_port);
			action_entry = ret;
			/* mac hash entry found */
			switch (pktHdr->punt_reason) {
				case SF_PUNT_SA_IS_ACTIVE:
					//clean fresh bit
					if (action_entry & FRESH_FLAG_MASK)
						action_entry = action_entry & ~FRESH_FLAG_MASK;
					break;
				case SF_PUNT_SA_RELEARN:
					/* validate relearn:*
					 * * check for ingress port match
					 * with action entry port num */
					action_portno = (ret & (FWD_PORT_LIST_MASK &
									(1 << phy_port)));
					if (!((1 << phy_port) & action_portno)) {
						action_entry = action_entry &
							(~FWD_PORT_LIST_MASK);
						/* reset old phyno and update new phyno into action entry */
						action_entry |= ((0x1 << phy_port) &
									FWD_PORT_LIST_MASK);
					}
					break;
				default:
					break;
			} /* End of switch - (pktHdr->punt_reason) */
			/* update new phyno into mac hash table */
			ret = mac_hash_table_update(field_entries,
						action_entry, field_valids);
		}
		else{
			netdev_err(priv->netdev,"error mac address %pM is in table but punt with %d\n", mac ,pktHdr->punt_reason);
			// error sa miss
			return 1;
		}
	}
	return 0;
}

int sf_do_packet_parsing(uchar *data, pktHdrs_t *pktHdr) {

	ushort eth_type = 0, length = ETH_SMAC_DMAC_SIZE;
	struct ethhdr *eth_hdr = NULL;
	vlanEthhdr_t *vlanEthHdr;
	uchar *pLayer2Hdr;
#ifndef CONFIG_SF_KERNEL_LITE
	uchar *pLayer3Hdr;
#endif
	/* initilize starting layer of data pointer to packet header data */
    pktHdr->data = data;

    /* do packet parsing & initilize corresponig header pointer fields */
    pLayer2Hdr = data;
    eth_hdr = (struct ethhdr *)pLayer2Hdr;
    eth_type = SF_SHORT_SWAP(eth_hdr->h_proto);

    /*Update layer-3 header pointers by looping the ether type*/
    while (1) {

#ifndef CONFIG_SF_KERNEL_LITE
        /*Ether net type ipv4*/
        if (eth_type == ETH_P_IP) {
            /*
             *			* set packet type as IP in the pktHdr and get the next
             *						* protocol and return
             *									*/

            /* adjust length for IP header */
            length = length + ETH_PROTO_SIZE;

            /* extract the layer-3 header pointer
             *						from pktbuf for ipv4 */
            pLayer3Hdr = pLayer2Hdr + length;

            return 0;
        } else if (eth_type == ETH_P_IPV6) {

            /*
             *			* set packet type as IPV6 in the pktbuf and get the next
             *						* protocol and return
             *									*/

            /* adjust length for IP header */
            length = length + ETH_PROTO_SIZE;
            /* extract the layer-3 header pointer
             *						from pktbuf for ipv6 */
            pLayer3Hdr = pLayer2Hdr + length;
            return 0;
        } else if (eth_type == ETH_P_ARP) {

            /*
             *			* set packet type as ARP in the pktbuf and get the next
             *						* protocol and return
             *									*/
            return 0;
        } else if (eth_type == ETH_P_8021Q) {
#else
        if (eth_type == ETH_P_8021Q) {
#endif

            /*set packet type as VLAN in the pktbuf and get the next
             *			* protocol and continue*/
            vlanEthHdr = (vlanEthhdr_t *)(pLayer2Hdr);
            pktHdr->vlanid =
                SF_SHORT_SWAP(vlanEthHdr->h_vlan_TCI) & 0xFFF;

            /* adjust VLAN header length */
            length = length + VLAN_HLEN;
            eth_type = *(ushort *)(pLayer2Hdr + length);
            eth_type = SF_SHORT_SWAP(eth_type);
        } else {
            /* need handle remaining protocols like
             *						PPPoE, PTP, etc */
            break;
        }
    }
    return -1;
}

int sf_process_packet(uchar *data, uint data_len, struct rx_header *rxhdr,struct sf_priv *priv) {

    pktHdrs_t pktHdr;
    int ret = 0;
	print_msg(ETH_RX_DEBUG,priv->netdev,"%s rxhdr ctrl 0x%x punt_reason 0x%x phy_no %d\n da mac %pM sa mac %pM\n", __FUNCTION__, rxhdr->ctrl, rxhdr->punt_reason, rxhdr->rxport_num, data,data + 6);
	print_msg(ETH_RX_DEBUG,priv->netdev,"%s rxhdr ctrl 0x%x punt_reason 0x%x phy_no %d\n",__FUNCTION__, rxhdr->ctrl, rxhdr->punt_reason, rxhdr->rxport_num);
    memset(&pktHdr, 0, sizeof(pktHdrs_t));
    /* update rx port num */
    pktHdr.phyno = rxhdr->rxport_num;
    pktHdr.punt_reason = rxhdr->punt_reason;
    /* do packet parsing */
    ret = sf_do_packet_parsing(data, &pktHdr);
    /* do reason code handling based on punt reason code */
    switch (rxhdr->punt_reason) {
		case SF_PUNT_SA_RELEARN:
			if (!memcmp(data + 6, priv->netdev->dev_addr, 5))
				// if (*(data + 11) == priv->netdev->dev_addr[5] || *(data + 11) == (priv->netdev->dev_addr[5]+1))
					break;
        case SF_PUNT_SA_MISS:
        case SF_PUNT_SA_IS_ACTIVE:
            return sf_switch_hash_learn(&pktHdr,priv);
        default:
            break;
    }
    return 0;
}

#ifdef CONFIG_SFAX8_SWITCH_VLAN
int sf_switch_init_broadcast_mac(int vlanid, int port_list) {

	uchar mac[6] = {0xff, 0xff, 0xff, 0xff,0xff,0xff};
	int ret = -1;
    uint fwd_action = 0;
	uint  action_entry = 0;
	uint field_entries[5], field_valids = BIT_SET_MAC_2F_MACTBL;
	uint  phy_port = 0;


	// printk("%s vlan id %d portlist  0x%x mac %pM\n",__FUNCTION__, vlanid, port_list, mac);
	/* need to updated field entries valid filed basis */
	field_entries[0] = ((*mac << 0)&0x000000ff) |((*(mac + 1) << 8)&0x0000ff00) | ((*(mac + 2) << 16)&0x00ff0000) |((*(mac + 3) << 24)&0xff000000);
	field_entries[1] = ((*(mac + 4) << 0)&0x000000ff) |((*(mac + 5) << 8)&0x0000ff00);
	field_entries[1] |= (vlanid << 16);
	field_entries[2] = field_entries[3] = field_entries[4] = 0;

	/* update vlan field valid for mac table */
	field_valids |= BIT_SET_VLAN_2F_MACTBL;

	/* fill action_entry's fresh bit */
	action_entry = FRESH_FLAG_SET;
	action_entry |= (0x1 << MAC_2F_ENTRY_STATIC_BIT_POS);
	/* Add SMAC entry with action entry & valid fields */
	/* update action entry with port info
	 *                     on which it arrived */
	action_entry |= (port_list & FWD_PORT_LIST_MASK);

	fwd_action = ACT_FORWARD;

	/* Update mac action entry's fwd action */
	action_entry = (action_entry & ~MAC_2F_ENTRY_FWD_ACT_MASK) |
		(fwd_action << MAC_2F_ENTRY_FWD_ACT_START_POS);

	/* add mac entry */
	ret = mac_hash_table_add(field_entries, phy_port,
				action_entry, field_valids);
	if (ret == -1) {
		add_fail_count++;
		// BUG_ON(add_fail_count > 10);
		return -1;
	}
	else{
		add_fail_count = 0;
	}

	return 0;
}
#endif
