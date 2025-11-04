/**
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 * ("Bridgetek") subject to the licence terms set out
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
 * There are no warranties (or similar) in relation to the quality of the
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

#ifndef ESD_UTILITY__H
#define ESD_UTILITY__H

#include "Esd_Base.h"

#ifdef __cplusplus
extern "C" {
#endif

ESD_CORE_EXPORT void Esd_AttachFlashFast();

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
typedef enum
{
	SDCARD_INSERTED_SUCCEEDS = 0, // card inserted succeeds
	SDCARD_NOT_DETECT = 53248, // Sd card not detected
	SDCARD_NO_RESPONSE = 53249, // No response from card
	SDCARD_TIMEOUT_DURING_INIT = 53250, // Card timeout during initialization
	SDCARD_SECTOR0_NOT_FOUND = 53251, // Sector 0(MBR)not found
	SDCARD_FATFS_NOT_FOUND = 53252, // FAT filesystem not found
	SDCARD_FAIL_ENTER_HISPEED_MODE = 53253, // Card failed to enter high-speed mode
	SDCARD_COPROCESSOR_FAULT = 4294967295 // Coprocessor fault
} EVE_SDCARD_STATUS;

typedef enum
{
	FSSOURCE_SUCCEEDS = 0, // fssource succeeds
	FSSOURCE_FILE_NOT_FOUND = 2, // File not found
	FSSOURCE_FILE_IO_ERROR = 5, // File I/O Error
} EVE_FSSOURCE_STATUS;

/* Load EVE SD card */
ESD_CORE_EXPORT bool Esd_AttachEveSdCard(EVE_HalContext *phost);
ESD_CORE_EXPORT bool Esd_EveSdCardReady();
ESD_CORE_EXPORT bool Esd_CheckFsSourceRes(uint32_t res, const char *filename);
#endif

ESD_CORE_EXPORT void Esd_ShowLogo();
ESD_CORE_EXPORT void Esd_BeginLogo();
ESD_CORE_EXPORT void Esd_EndLogo();

ESD_CORE_EXPORT bool Esd_Calibrate();

ESD_CORE_EXPORT void Esd_DeferredFree(void *ptr);
ESD_CORE_EXPORT void Esd_ProcessFree();

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ESD_UTILITY__H */

/* end of file */
