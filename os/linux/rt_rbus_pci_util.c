/*
 *************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36, Taiyuan St., Jhubei City,
 * Hsinchu County 302,
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2010, Ralink Technology, Inc.
 *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program; if not, write to the                         *
 * Free Software Foundation, Inc.,                                       *
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                       *
 *************************************************************************/


#define RTMP_MODULE_OS
#define RTMP_MODULE_OS_UTIL

/*#include "rt_config.h" */
#include "rtmp_comm.h"
#include "rtmp_osabl.h"
#include "rt_os_util.h"


#ifdef RTMP_MAC_PCI

/* Function for TxDesc Memory allocation. */
void RTMP_AllocateTxDescMemory(
	IN	PPCI_DEV				pPciDev,
	IN	UINT					Index,
	IN	ULONG					Length,
	IN	BOOLEAN					Cached,
	OUT	PVOID					*VirtualAddress,
	OUT	PNDIS_PHYSICAL_ADDRESS	PhysicalAddress)
{
/*	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie; */
	dma_addr_t DmaAddr = (dma_addr_t)(*PhysicalAddress);

    *VirtualAddress = (PVOID)dma_alloc_coherent(pPciDev == NULL ? NULL : &pPciDev->dev,sizeof(char)*Length, &DmaAddr, GFP_ATOMIC);
	*PhysicalAddress = (NDIS_PHYSICAL_ADDRESS)DmaAddr;
}


/* Function for MgmtDesc Memory allocation. */
void RTMP_AllocateMgmtDescMemory(
	IN	PPCI_DEV				pPciDev,
	IN	ULONG					Length,
	IN	BOOLEAN					Cached,
	OUT	PVOID					*VirtualAddress,
	OUT	PNDIS_PHYSICAL_ADDRESS	PhysicalAddress)
{
/*	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie; */
	dma_addr_t DmaAddr = (dma_addr_t)(*PhysicalAddress);

    *VirtualAddress = (PVOID)dma_alloc_coherent(pPciDev == NULL ? NULL : &pPciDev->dev,sizeof(char)*Length, &DmaAddr, GFP_ATOMIC);
	*PhysicalAddress = (NDIS_PHYSICAL_ADDRESS)DmaAddr;
}


/* Function for RxDesc Memory allocation. */
void RTMP_AllocateRxDescMemory(
	IN	PPCI_DEV				pPciDev,
	IN	ULONG					Length,
	IN	BOOLEAN					Cached,
	OUT	PVOID					*VirtualAddress,
	OUT	PNDIS_PHYSICAL_ADDRESS	PhysicalAddress)
{
/*	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie; */
	dma_addr_t DmaAddr = (dma_addr_t)(*PhysicalAddress);

    *VirtualAddress = (PVOID)dma_alloc_coherent(pPciDev == NULL ? NULL : &pPciDev->dev,sizeof(char)*Length, &DmaAddr, GFP_ATOMIC);
	*PhysicalAddress = (NDIS_PHYSICAL_ADDRESS)DmaAddr;
}


/* Function for free allocated Desc Memory. */
void RTMP_FreeDescMemory(
	IN	PPCI_DEV				pPciDev,
	IN	ULONG					Length,
	IN	PVOID					VirtualAddress,
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress)
{
/*	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie; */
	dma_addr_t DmaAddr = (dma_addr_t)(PhysicalAddress);

    dma_free_coherent(pPciDev == NULL ? NULL : &pPciDev->dev, Length, VirtualAddress, DmaAddr);
}


/* Function for TxData DMA Memory allocation. */
void RTMP_AllocateFirstTxBuffer(
	IN	PPCI_DEV				pPciDev,
	IN	UINT					Index,
	IN	ULONG					Length,
	IN	BOOLEAN					Cached,
	OUT	PVOID					*VirtualAddress,
	OUT	PNDIS_PHYSICAL_ADDRESS	PhysicalAddress)
{
/*	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie; */
	dma_addr_t DmaAddr = (dma_addr_t)(*PhysicalAddress);

    *VirtualAddress = (PVOID)dma_alloc_coherent(pPciDev == NULL ? NULL : &pPciDev->dev,sizeof(char)*Length, &DmaAddr, GFP_ATOMIC);
	*PhysicalAddress = (NDIS_PHYSICAL_ADDRESS)DmaAddr;
}


void RTMP_FreeFirstTxBuffer(
	IN	PPCI_DEV				pPciDev,
	IN	ULONG					Length,
	IN	BOOLEAN					Cached,
	IN	PVOID					VirtualAddress,
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress)
{
/*	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie; */
	dma_addr_t DmaAddr = (dma_addr_t)(PhysicalAddress);
    dma_free_coherent(pPciDev == NULL ? NULL : &pPciDev->dev, Length, VirtualAddress, DmaAddr);
}


/*
 * FUNCTION: Allocate a packet buffer for DMA
 * ARGUMENTS:
 *     AdapterHandle:  AdapterHandle
 *     Length:  Number of bytes to allocate
 *     Cached:  Whether or not the memory can be cached
 *     VirtualAddress:  Pointer to memory is returned here
 *     PhysicalAddress:  Physical address corresponding to virtual address
 * Notes:
 *     Cached is ignored: always cached memory
 */
PNDIS_PACKET RTMP_AllocateRxPacketBuffer(
	IN	VOID					*pReserved,
	IN	VOID					*pPciDev,
	IN	ULONG					Length,
	IN	BOOLEAN					Cached,
	OUT	PVOID					*VirtualAddress,
	OUT	PNDIS_PHYSICAL_ADDRESS	PhysicalAddress)
{
	struct sk_buff *pkt;

/*	pkt = dev_alloc_skb(Length); */
	DEV_ALLOC_SKB(pReserved, pkt, Length);

	if (pkt == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("can't allocate rx %ld size packet\n",Length));
	}

	if (pkt) {
		RTMP_SET_PACKET_SOURCE(OSPKT_TO_RTPKT(pkt), PKTSRC_NDIS);
		*VirtualAddress = (PVOID) pkt->data;	
/*#ifdef CONFIG_5VT_ENHANCE */
/*		*PhysicalAddress = PCI_MAP_SINGLE(pAd, *VirtualAddress, 1600, RTMP_PCI_DMA_FROMDEVICE); */
/*#else */
		*PhysicalAddress = PCI_MAP_SINGLE_DEV(pPciDev, *VirtualAddress, Length,  -1, RTMP_PCI_DMA_FROMDEVICE);
/*#endif */
	} else {
		*VirtualAddress = (PVOID) NULL;
		*PhysicalAddress = (NDIS_PHYSICAL_ADDRESS) 0;
	}	

	return (PNDIS_PACKET) pkt;
}

/*
 * invaild or writeback cache 
 * and convert virtual address to physical address 
 */
ra_dma_addr_t linux_pci_map_single(void *pPciDev, void *ptr, size_t size, int sd_idx, int direction)
{
/*	PRTMP_ADAPTER pAd; */
/*	POS_COOKIE pObj; */


	if (direction == RTMP_PCI_DMA_TODEVICE)
		direction = DMA_TO_DEVICE;

	if (direction == RTMP_PCI_DMA_FROMDEVICE)
		direction = DMA_FROM_DEVICE;

	/* 
		------ Porting Information ------
		> For Tx Alloc:
			mgmt packets => sd_idx = 0
			SwIdx: pAd->MgmtRing.TxCpuIdx
			pTxD : pAd->MgmtRing.Cell[SwIdx].AllocVa;
	 
			data packets => sd_idx = 1
	 		TxIdx : pAd->TxRing[pTxBlk->QueIdx].TxCpuIdx 
	 		QueIdx: pTxBlk->QueIdx 
	 		pTxD  : pAd->TxRing[pTxBlk->QueIdx].Cell[TxIdx].AllocVa;

	 	> For Rx Alloc:
	 		sd_idx = -1
	*/

/*	pAd = (PRTMP_ADAPTER)handle; */
/*	pObj = (POS_COOKIE)pAd->OS_Cookie; */
	
	{
        PPCI_DEV dev = (PPCI_DEV)pPciDev;
		return (ra_dma_addr_t)dma_map_single(&(dev->dev), ptr, size, direction);
	}

}

void linux_pci_unmap_single(void *pPciDev, ra_dma_addr_t radma_addr, size_t size, int direction)
{
/*	PRTMP_ADAPTER pAd; */
/*	POS_COOKIE pObj; */
	dma_addr_t DmaAddr = (dma_addr_t)radma_addr;


	if (direction == RTMP_PCI_DMA_TODEVICE)
		direction = DMA_TO_DEVICE;

	if (direction == RTMP_PCI_DMA_FROMDEVICE)
		direction = DMA_FROM_DEVICE;

/*	pAd=(PRTMP_ADAPTER)handle; */
/*	pObj = (POS_COOKIE)pAd->OS_Cookie; */
	
	if (size > 0) {
        PPCI_DEV dev = (PPCI_DEV)pPciDev;
		dma_unmap_single(&(dev->dev), DmaAddr, size, direction);
    }
	
}



#ifdef RTMP_PCI_SUPPORT
VOID *RTMPFindHostPCIDev(
    IN	VOID					*pPciDevSrc)
{
	struct pci_dev *pci_dev = (struct pci_dev *)pPciDevSrc;
	struct pci_dev *parent_pci_dev;
    USHORT  reg16;
    UCHAR   reg8;
	UINT	DevFn;
    PPCI_DEV    pPci_dev;
/*	POS_COOKIE 	pObj; */

/*	pObj = (POS_COOKIE) pAd->OS_Cookie; */

/*	if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE)) */
/*		return; */
    
    DBGPRINT(RT_DEBUG_TRACE, ("%s.===>\n", __FUNCTION__));

    parent_pci_dev = NULL;
    if (pci_dev->bus->parent)
    {
        for (DevFn = 0; DevFn < 255; DevFn++)
        {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
            pPci_dev = pci_get_slot(pci_dev->bus->parent, DevFn);
#else
            pPci_dev = pci_find_slot(pci_dev->bus->parent->number, DevFn);
#endif
            if (pPci_dev)
            {                
                pci_read_config_word(pPci_dev, PCI_CLASS_DEVICE, &reg16);
                reg16 = le2cpu16(reg16);
                pci_read_config_byte(pPci_dev, PCI_CB_CARD_BUS, &reg8);
                if ((reg16 == PCI_CLASS_BRIDGE_PCI) && 
                    (reg8 == pci_dev->bus->number))
                {
                    return pPci_dev;
                }
            }
        }
    }
	return NULL;
}
#endif /* RTMP_PCI_SUPPORT */


/*
========================================================================
Routine Description:
	Write 32-bit to a register.

Arguments:
	pAd				- WLAN control block pointer
	Offset			- Register offset
	Value			- 32-bit value

Return Value:
	None

Note:
========================================================================
*/
VOID RTMP_PCI_Writel(
	IN	ULONG					Value,
	IN	VOID					*pAddr)
{
	writel(Value, pAddr);
}


/*
========================================================================
Routine Description:
	Write 16-bit to a register.

Arguments:
	pAd				- WLAN control block pointer
	Offset			- Register offset
	Value			- 32-bit value

Return Value:
	None

Note:
========================================================================
*/
VOID RTMP_PCI_Writew(
	IN	ULONG					Value,
	IN	VOID					*pAddr)
{
	writew(Value, pAddr);
}


/*
========================================================================
Routine Description:
	Write 8-bit to a register.

Arguments:
	pAd				- WLAN control block pointer
	Offset			- Register offset
	Value			- 32-bit value

Return Value:
	None

Note:
========================================================================
*/
VOID RTMP_PCI_Writeb(
	IN	ULONG					Value,
	IN	VOID					*pAddr)
{
	writeb(Value, pAddr);
}


/*
========================================================================
Routine Description:
	Read 32-bit from a register.

Arguments:
	pAd				- WLAN control block pointer
	Offset			- Register offset
	Value			- 32-bit value

Return Value:
	None

Note:
========================================================================
*/
ULONG RTMP_PCI_Readl(
	IN	VOID					*pAddr)
{
	return readl(pAddr);
}


/*
========================================================================
Routine Description:
	Read 16-bit from a register.

Arguments:
	pAd				- WLAN control block pointer
	Offset			- Register offset
	Value			- 16-bit value

Return Value:
	None

Note:
	No use.
========================================================================
*/
ULONG RTMP_PCI_Readw(
	IN	VOID					*pAddr)
{
	return readw(pAddr);
}


/*
========================================================================
Routine Description:
	Read 8-bit from a register.

Arguments:
	pAd				- WLAN control block pointer
	Offset			- Register offset
	Value			- 16-bit value

Return Value:
	None

Note:
========================================================================
*/
ULONG RTMP_PCI_Readb(
	IN	VOID					*pAddr)
{
	return readb(pAddr);
}


/*
========================================================================
Routine Description:
	Read 16-bit from the PCI config space.

Arguments:
	pDev			- PCI device
	Offset			- Register offset
	Value			- 16-bit value

Return Value:
	None

Note:
========================================================================
*/
int RtmpOsPciConfigReadWord(
	IN	VOID					*pDev,
	IN	UINT32					Offset,
	OUT UINT16					*pValue)
{
	return pci_read_config_word((struct pci_dev *)pDev, Offset, pValue);
}


/*
========================================================================
Routine Description:
	Write 16-bit to the PCI config space.

Arguments:
	pDev			- PCI device
	Offset			- Register offset
	Value			- 16-bit value

Return Value:
	None

Note:
========================================================================
*/
int RtmpOsPciConfigWriteWord(
	IN	VOID					*pDev,
	IN	UINT32					Offset,
	IN	UINT16					Value)
{
	return pci_write_config_word((struct pci_dev *)pDev, Offset, Value);
}


/*
========================================================================
Routine Description:
	Read 32-bit from the PCI config space.

Arguments:
	pDev			- PCI device
	Offset			- Register offset
	Value			- 32-bit value

Return Value:
	None

Note:
========================================================================
*/
int RtmpOsPciConfigReadDWord(
	IN	VOID					*pDev,
	IN	UINT32					Offset,
	OUT UINT32					*pValue)
{
	return pci_read_config_dword((struct pci_dev *)pDev, Offset, pValue);
}


/*
========================================================================
Routine Description:
	Write 32-bit to the PCI config space.

Arguments:
	pDev			- PCI device
	Offset			- Register offset
	Value			- 32-bit value

Return Value:
	None

Note:
========================================================================
*/
int RtmpOsPciConfigWriteDWord(
	IN	VOID					*pDev,
	IN	UINT32					Offset,
	IN	UINT32					Value)
{
	return pci_write_config_dword((struct pci_dev *)pDev, Offset, Value);
}


/*
========================================================================
Routine Description:
	Query for devices' capabilities.

Arguments:
	pDev			- PCI device
	Cap				- Capability code

Return Value:
	None

Note:
========================================================================
*/
int RtmpOsPciFindCapability(
	IN	VOID					*pDev,
	IN	int						Cap)
{
	return pci_find_capability(pDev, Cap);
}


#ifdef RTMP_PCI_SUPPORT
/*
========================================================================
Routine Description:
	Enable MSI function for PCI.

Arguments:
	pDev			- PCI device

Return Value:
	None

Note:
========================================================================
*/
int RtmpOsPciMsiEnable(
	IN	VOID					*pDev)
{
#ifdef PCI_MSI_SUPPORT
	return pci_enable_msi(pDev);
#else
	return 0;
#endif
}


/*
========================================================================
Routine Description:
	Disable MSI function for PCI.

Arguments:
	pDev			- PCI device

Return Value:
	None

Note:
========================================================================
*/
VOID RtmpOsPciMsiDisable(
	IN	VOID					*pDev)
{
#ifdef PCI_MSI_SUPPORT
	pci_disable_msi(pDev);
#endif
}
#endif /* RTMP_PCI_SUPPORT */

#endif /* RTMP_MAC_PCI */

/* End of rt_rbus_pci_util.c */
