/**
 ******************************************************************************
 *
 * @file siwifi_pci.c
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */
#include <linux/pci.h>
#include <linux/module.h>

#include "siwifi_defs.h"
#include "siwifi_dini.h"
#include "siwifi_v7.h"

#define PCI_VENDOR_ID_DINIGROUP              0x17DF
#define PCI_DEVICE_ID_DINIGROUP_DNV6_F2PCIE  0x1907

#define PCI_DEVICE_ID_XILINX_SIWIFI_VIRTEX7    0x7011

static const struct pci_device_id siwifi_pci_ids[] = {
    {PCI_DEVICE(PCI_VENDOR_ID_DINIGROUP, PCI_DEVICE_ID_DINIGROUP_DNV6_F2PCIE)},
    {PCI_DEVICE(PCI_VENDOR_ID_XILINX, PCI_DEVICE_ID_XILINX_SIWIFI_VIRTEX7)},
    {0,}
};


/* Uncomment this for depmod to create module alias */
/* We don't want this on development platform */
//MODULE_DEVICE_TABLE(pci, siwifi_pci_ids);

static int siwifi_pci_probe(struct pci_dev *pci_dev,
                          const struct pci_device_id *pci_id)
{
    struct siwifi_plat *siwifi_plat = NULL;
    void *drvdata;
    int ret = -ENODEV;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if (pci_id->vendor == PCI_VENDOR_ID_DINIGROUP) {
        ret = siwifi_dini_platform_init(pci_dev, &siwifi_plat);
    } else if (pci_id->vendor == PCI_VENDOR_ID_XILINX) {
        ret = siwifi_v7_platform_init(pci_dev, &siwifi_plat);
    }

    if (ret)
        return ret;

    siwifi_plat->pci_dev = pci_dev;

    ret = siwifi_platform_init(siwifi_plat, &drvdata);
    pci_set_drvdata(pci_dev, drvdata);

    if (ret)
        siwifi_plat->deinit(siwifi_plat);

    return ret;
}

static void siwifi_pci_remove(struct pci_dev *pci_dev)
{
    struct siwifi_hw *siwifi_hw;
    struct siwifi_plat *siwifi_plat;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    siwifi_hw = pci_get_drvdata(pci_dev);
    siwifi_plat = siwifi_hw->plat;

    siwifi_platform_deinit(siwifi_hw);
    siwifi_plat->deinit(siwifi_plat);

    pci_set_drvdata(pci_dev, NULL);
}

static struct pci_driver siwifi_pci_drv = {
    .name     = KBUILD_MODNAME,
    .id_table = siwifi_pci_ids,
    .probe    = siwifi_pci_probe,
    .remove   = siwifi_pci_remove
};

int siwifi_pci_register_drv(void)
{
    return pci_register_driver(&siwifi_pci_drv);
}

void siwifi_pci_unregister_drv(void)
{
    pci_unregister_driver(&siwifi_pci_drv);
}
