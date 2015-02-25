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


#ifndef __RT5592_H__
#define __RT5592_H__

#include "chip/rt30xx.h"

#ifndef RTMP_RF_RW_SUPPORT
#error "For RT5592, you should define the compile flag -DRTMP_RF_RW_SUPPORT"
#endif

#ifndef RT30xx
#error "For RT5592, you should define the compile flag -DRT30xx"
#endif

#define BBP_REG_BF			BBP_R163 // TxBf control
#define BBP_REG_BF			BBP_R163 // TxBf control

#ifdef CONFIG_STA_SUPPORT
#ifdef RTMP_MAC_PCI
#define PCIE_PS_SUPPORT
#endif /* RTMP_MAC_PCI */
#endif /* CONFIG_STA_SUPPORT */

#ifdef RTMP_FLASH_SUPPORT
#ifdef RTMP_MAC_PCI
#define EEPROM_DEFAULT_FILE_PATH                     "/etc_ro/Wireless/RT2860AP/RT5592_PCIe_LNA_2T2R_ALC_V1_2.bin"
#endif /* RTMP_MAC_PCI */


#define RF_OFFSET					0x48000
#endif // RTMP_FLASH_SUPPORT //


/*
 * If MAC 0x5E8 bit[31] = 0, Xtal is 20M
 * If MAC 0x5E8 bit[31] = 1, Xtal is 40M
 */ 
enum XTAL{
	XTAL20M,
	XTAL40M
};

/* 
 * Frequency plan item  for RT5592 
 * N: R9[4], R8[7:0]
 * K: R9[3:0]
 * mod: R9[7], R11[3:2] (eg. mod=8 => 0x0, mod=10 => 0x2)
 * R: R11[1:0] (eg. R=1 => 0x0, R=3 => 0x2)
 */
typedef struct _RT5592_FREQUENCY_ITEM {
	UCHAR Channel;
	UINT16 N;
	UCHAR K;
	UCHAR mod;
	UCHAR R;
} RT5592_FREQUENCY_ITEM, *PRT5592_FREQUENCY_ITEM;

/* Frequency plan table */
typedef struct _RT5592_FREQUENCY_PLAN {
	const struct _RT5592_FREQUENCY_ITEM *pFrequencyPlan;
	UCHAR totalFreqItem;
} RT5592_FREQUENCY_PLAN, *PRT5592_FREQUENCY_PLAN;

VOID RT5592_Init(
	IN struct _RTMP_ADAPTER			*pAd);

#endif /* __RT5592_H__ */
