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


#include "rt_config.h"

/*----- 802.11H -----*/

/* Periodic Radar detection, switch channel will occur in RTMPHandleTBTTInterrupt()*/
/* Before switch channel, driver needs doing channel switch announcement.*/
VOID RadarDetectPeriodic(
	IN PRTMP_ADAPTER	pAd)
{
	/* need to check channel availability, after switch channel*/
	if (pAd->Dot11_H.RDMode != RD_SILENCE_MODE)
			return;

	/* channel availability check time is 60sec, use 65 for assurance*/
	if (pAd->Dot11_H.RDCount++ > pAd->Dot11_H.ChMovingTime)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Not found radar signal, start send beacon and radar detection in service monitor\n\n"));
		AsicEnableBssSync(pAd);
		pAd->Dot11_H.RDMode = RD_NORMAL_MODE;
		return;
	}
}

/*
	========================================================================

	Routine Description:
		Radar channel check routine

	Arguments:
		pAd 	Pointer to our adapter

	Return Value:
		TRUE	need to do radar detect
		FALSE	need not to do radar detect

	========================================================================
*/
BOOLEAN RadarChannelCheck(
	IN PRTMP_ADAPTER	pAd,
	IN UCHAR			Ch)
{
	INT 	i;
	BOOLEAN result = FALSE;

	for (i=0; i<pAd->ChannelListNum; i++)
	{
		if (Ch == pAd->ChannelList[i].Channel)
		{
			result = pAd->ChannelList[i].DfsReq;
			break;
		}
	}

	return result;
}

ULONG JapRadarType(
	IN PRTMP_ADAPTER pAd)
{
	ULONG		i;
	const UCHAR	Channel[15]={52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140};

	if (pAd->CommonCfg.RDDurRegion != JAP)
	{
		return pAd->CommonCfg.RDDurRegion;
	}

	for (i=0; i<15; i++)
	{
		if (pAd->CommonCfg.Channel == Channel[i])
		{
			break;
		}
	}

	if (i < 4)
		return JAP_W53;
	else if (i < 15)
		return JAP_W56;
	else
		return JAP; /* W52*/

}


/* 
    ==========================================================================
    Description:
        Set channel switch Period
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_CSPeriod_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	pAd->Dot11_H.CSPeriod = (USHORT) simple_strtol(arg, 0, 10);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_CSPeriod_Proc::(CSPeriod=%d)\n", pAd->Dot11_H.CSPeriod));

	return TRUE;
}

/* 
    ==========================================================================
    Description:
		change channel moving time for DFS testing.

	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage: 
               1.) iwpriv ra0 set ChMovTime=[value]
    ==========================================================================
*/
INT Set_ChMovingTime_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PSTRING arg)
{
	UINT8 Value;

	Value = (UINT8) simple_strtol(arg, 0, 10);

	pAd->Dot11_H.ChMovingTime = Value;

	DBGPRINT(RT_DEBUG_TRACE, ("%s: %d\n", __FUNCTION__,
		pAd->Dot11_H.ChMovingTime));

	return TRUE;
}


/* 
    ==========================================================================
    Description:
		Reset channel block status.
	Arguments:
	    pAd				Pointer to our adapter
	    arg				Not used

    Return Value:
        None

    Note:
        Usage: 
               1.) iwpriv ra0 set ChMovTime=[value]
    ==========================================================================
*/
INT Set_BlockChReset_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PSTRING arg)
{
	INT i;

	DBGPRINT(RT_DEBUG_TRACE, ("%s: Reset channel block status.\n", __FUNCTION__));	
	
	for (i=0; i<pAd->ChannelListNum; i++)
		pAd->ChannelList[i].RemainingTimeForUse = 0;

	return TRUE;
}


#if defined(DFS_SUPPORT) || defined(CARRIER_DETECTION_SUPPORT)

INT	Set_RadarShow_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{

#ifdef CARRIER_DETECTION_SUPPORT
	printk("pAd->CommonCfg.CarrierDetect.CD_State = %d\n", pAd->CommonCfg.CarrierDetect.CD_State);
	printk("pAd->CommonCfg.CarrierDetect.criteria = %d\n", pAd->CommonCfg.CarrierDetect.criteria);
	printk("pAd->CommonCfg.CarrierDetect.Delta = %d\n", pAd->CommonCfg.CarrierDetect.delta);
	printk("pAd->CommonCfg.CarrierDetect.DivFlag = %d\n", pAd->CommonCfg.CarrierDetect.div_flag);
	printk("pAd->CommonCfg.CarrierDetect.Threshold = %d(0x%x)\n", pAd->CommonCfg.CarrierDetect.threshold, pAd->CommonCfg.CarrierDetect.threshold);
#endif /* CARRIER_DETECTION_SUPPORT */

	return TRUE;
}

/*
       ========================================================================
       Routine Description:
               Control CCK_MRC Status
       Arguments:
               pAd     Pointer to our adapter
       Return Value:

       ========================================================================
*/
VOID CckMrcStatusCtrl(IN PRTMP_ADAPTER pAd)
{
#ifdef RT5592
       UCHAR bbp = 0, bCckMrc = 0;
       RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R95, &bbp);
       bCckMrc = (bbp >> 7);
       if (bCckMrc)
       {
			if (pAd->CommonCfg.Channel>14
#ifdef CARRIER_DETECTION_SUPPORT
				|| (pAd->CommonCfg.CarrierDetect.Enable == TRUE &&
					pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth == BW_20)
#endif /* CARRIER_DETECTION_SUPPORT */
			)
			{
				/* Disable CCK_MRC*/
				bbp &= ~(1 << 7);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R95, bbp);
			}
       }
       else
       {
			if (pAd->CommonCfg.Channel<=14)
			{
#ifdef CARRIER_DETECTION_SUPPORT
				if (pAd->CommonCfg.CarrierDetect.Enable == FALSE ||
					pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth == BW_40)
#endif /* CARRIER_DETECTION_SUPPORT */
				{
					/* Enable CCK_MRC */
					bbp |= (1 << 7);
					RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R95, bbp);
				}
			}
       }
#endif /* RT5592 */
}


/*
       ========================================================================
       Routine Description:
               Enhance DFS/CS when using GLRT.
       Arguments:
               pAd     Pointer to our adapter
       Return Value:

       ========================================================================
*/
VOID RadarGLRTCompensate(IN PRTMP_ADAPTER pAd)
{
#ifdef RT5592


	if (pAd->CommonCfg.bIEEE80211H == 1 
#ifdef CARRIER_DETECTION_SUPPORT
		|| pAd->CommonCfg.CarrierDetect.Enable == TRUE
#endif /* CARRIER_DETECTION_SUPPORT */
		)
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x91);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x24);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x95);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x2D);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x99);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x40);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9A);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x3E);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9B);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x42);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9C);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x3D);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9D);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x40);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0xA1);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x2F);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0xA5);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x2A);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0xB5);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x40);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0xCE);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x43);
	}
#endif /* RT5592 */
}
#endif /*defined(DFS_SUPPORT) || defined(CARRIER_DETECTION_SUPPORT) */

