/**
 * This source code ("the Software")is provided by Bridgetek Pte Ltd
 * ("Bridgetek")subject to the licence terms set out
 *   http://brtchip.com/BRTSourceCodeLicenseAgreement/ ("the Licence Terms").
 * You must read the Licence Terms before downloading or using the Software.
 * By installing or using the Software you agree to the Licence Terms. If you
 * do not agree to the Licence Terms then do not download or use the Software.
 *
 * Without prejudice to the Licence Terms, here is a summary of some of the key
 * terms of the Licence Terms (and in the event of any conflict between this
 * summary and the Licence Terms then the text of the Licence Terms will
 * prevail).
 *
 * The Software is provided "as is".
 * There are no warranties (or similar)in relation to the quality of the
 * Software. You use it at your own risk.
 * The Software should not be used in, or for, any medical device, system or
 * appliance. There are exclusions of Bridgetek liability for certain types of loss
 * such as: special loss or damage; incidental loss or damage; indirect or
 * consequential loss or damage; loss of income; loss of business; loss of
 * profits; loss of revenue; loss of contracts; business interruption; loss of
 * the use of money or anticipated savings; loss of information; loss of
 * opportunity; loss of goodwill or reputation; and/or loss of, damage to or
 * corruption of data.
 * There is a monetary cap on Bridgetek's liability.
 * The Software may have subsequently been amended by another user and then
 * distributed by that other user ("Adapted Software").  If so that user may
 * have additional licence terms that apply to those amendments. However, Bridgetek
 * has no liability in relation to those amendments.
 */

/*

This header file provides inline convenience functions to provide an easy mechanism
to port between different EVE generations.

*/

#ifndef EVE_COMPAT__H
#define EVE_COMPAT__H
#include "EVE_HalDefs.h"
#include "EVE_GpuDefs.h"

/**
 * @brief Read the touch co-ordinates for touch 4
 *
 * Reads from REG_CTOUCH_TOUCH4_XY on BT820, or REG_CTOUCH_TOUCH4_X and REG_CTOUCH_TOUCH4_Y on earlier EVE generations.
 *
 * @param phost Pointer to Hal context
 *
 * @return 32-bit value containing the touch co-ordinates
 */
static inline uint32_t EVE_Compat_rdRegCTouchTouch4XY(EVE_HalContext *phost)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		return EVE_Hal_rd32(phost, REG_CTOUCH_TOUCH4_XY);
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		return ((uint32_t)EVE_Hal_rd16(phost, REG_CTOUCH_TOUCH4_X) << 16)
		    | (EVE_Hal_rd16(phost, REG_CTOUCH_TOUCH4_Y));
#endif
	}
}

/**
 * @brief Read the volume for the playback channel
 *
 * Reads from REG_VOL_L_PB on BT820, or REG_VOL_PB on earlier EVE generations.
 *
 * @param phost Pointer to Hal context
 *
 * @return 8-bit value containing the volume
 */
static inline int32_t EVE_Compat_rdRegVolPB(EVE_HalContext *phost)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		return EVE_Hal_rd32(phost, REG_VOL_L_PB);
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		return EVE_Hal_rd8(phost, REG_VOL_PB);
#endif
	}
}

/**
 * @brief Write the volume for the playback channel
 *
 * Writes to REG_VOL_L_PB and REG_VOL_R_PB on BT820, or REG_VOL_PB on earlier EVE generations.
 *
 * @param phost Pointer to Hal context
 * @param vol 8-bit value containing the volume
 */
static inline void EVE_Compat_wrRegVolPB(EVE_HalContext *phost, uint8_t vol)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		EVE_Hal_wr32(phost, REG_VOL_L_PB, vol);
		EVE_Hal_wr32(phost, REG_VOL_R_PB, vol);
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		EVE_Hal_wr8(phost, REG_VOL_PB, vol);
#endif
	}
}

#endif /* #ifndef EVE_HAL_INCL__H */

/* end of file */
