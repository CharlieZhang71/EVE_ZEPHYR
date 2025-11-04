
#include "Esd_Utility.h"

#include "Esd_Base.h"
#include "Esd_Scissor.h"
#include "Esd_GpuAlloc.h"
#include "Esd_TouchTag.h"
#include "Esd_Context.h"

extern ESD_CORE_EXPORT Esd_GpuAlloc *Esd_GAlloc;

#if defined(EVE_FLASH_AVAILABLE)
#ifndef NDEBUG
static uint32_t s_FlashErrorLast = ~0;
#endif
#endif

#if defined(EVE_FLASH_AVAILABLE)
#ifdef ESD_SIMULATION
extern void Esd_SetFlashStatus__ESD(int status);
extern void Esd_SetFlashSize__ESD(int size);
#else
#define Esd_SetFlashStatus__ESD(status) \
	do                                  \
	{                                   \
	} while (false)
#define Esd_SetFlashSize__ESD(status) \
	do                                \
	{                                 \
	} while (false)
#endif

ESD_CORE_EXPORT void Esd_AttachFlashFast()
{
	// Wait for flash status to move on from FLASH_STATUS_INIT
	EVE_HalContext *phost = Esd_GetHost();
	if (!EVE_Hal_supportFlash(phost))
		return;

	uint32_t flashStatus;
	while (!(flashStatus = EVE_Hal_rd32(phost, REG_FLASH_STATUS)))
	{
#ifndef NDEBUG
		eve_printf_debug("Waiting for REG_FLASH_STATUS (%u)\n", (unsigned int)flashStatus);
#endif
		EVE_sleep(100);
	}

	// No need to continue if flash is okay
	if (flashStatus < FLASH_STATUS_FULL)
	{
		uint32_t error;

		Esd_SetFlashStatus__ESD(flashStatus);

		flashStatus = EVE_CoCmd_flashAttach(phost);
		Esd_SetFlashStatus__ESD(flashStatus);
		Esd_SetFlashSize__ESD(EVE_Hal_rd32(phost, REG_FLASH_SIZE));

		flashStatus = EVE_CoCmd_flashFast(phost, &error);
		Esd_SetFlashStatus__ESD(flashStatus);

#ifndef NDEBUG
		if (error != s_FlashErrorLast)
		{
			s_FlashErrorLast = error;
			switch (error)
			{
			case 0:
				eve_printf_debug("Flash OK\n");
				break;
			case FLASH_ERROR_NOTATTACHED: // 0xE001 :
				eve_printf_debug("Flash is not attached\n");
				break;
			case FLASH_ERROR_HEADERMISSING: // 0xE002:
				eve_printf_debug("No header detected in sector 0. Is flash blank?\n");
				break;
			case FLASH_ERROR_HEADERCRCFAILED: // 0xE003:
				eve_printf_debug("Sector 0 flash data failed integrity check\n");
				break;
			case FLASH_ERROR_FIRMWAREMISMATCH: // 0xE004:
				eve_printf_debug("Flash device/blob mismatch. Was correct blob loaded?\n");
				break;
			case FLASH_ERROR_FULLSPEEDFAILED: // 0xE005:
				eve_printf_debug("Failed full-speed flash test. Check board wiring\n");
				break;
			default:
				eve_printf_debug("Unknown flash error (%u)\n", (unsigned int)error);
			}
		}
#endif
	}
}
#else
#define ESD_AttachFlashFast() eve_noop()
#endif

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
static bool s_EveSdCard_Ready = false;
/**
 * @brief Mount the EVE SDcard
 *
 * @param phost  Pointer to Hal context
 * @return true True if ok
 * @return false False if error
 */
ESD_CORE_EXPORT bool Esd_AttachEveSdCard(EVE_HalContext *phost)
{
	uint32_t status = EVE_CoCmd_sdAttach(phost, OPT_4BIT | OPT_HALFSPEED | OPT_IS_SD);
	eve_printf_debug("EVE Sd card attach status 0x%x \n", status);
	switch (status)
	{
	case SDCARD_INSERTED_SUCCEEDS: {
		s_EveSdCard_Ready = true;
		eve_printf_debug("EVE Sd Card status: mounted successfully\n");
		break;
	}
	case SDCARD_NOT_DETECT: {
		s_EveSdCard_Ready = false;
		eve_printf_debug("EVE Sd card not detected\n");
		break;
	}
	case SDCARD_NO_RESPONSE: {
		s_EveSdCard_Ready = false;
		eve_printf_debug("EVE Sd Card status: No response from card\n");
		break;
	}
	case SDCARD_TIMEOUT_DURING_INIT: {
		s_EveSdCard_Ready = false;
		eve_printf_debug("EVE Sd Card status: timeout during initialization\n");
		break;
	}
	case SDCARD_SECTOR0_NOT_FOUND: {
		s_EveSdCard_Ready = false;
		eve_printf_debug("EVE Sd Card status: Sector 0(MBR)not found\n");
		break;
	}
	case SDCARD_FATFS_NOT_FOUND: {
		s_EveSdCard_Ready = false;
		eve_printf_debug("EVE Sd Card status: FAT filesystem not found\n");
		break;
	}
	case SDCARD_FAIL_ENTER_HISPEED_MODE: {
		s_EveSdCard_Ready = false;
		eve_printf_debug("EVE Sd Card status: Card failed to enter high-speed mode\n");
		break;
	}
	case SDCARD_COPROCESSOR_FAULT: {
		s_EveSdCard_Ready = false;
		eve_printf_debug("EVE Sd Card status:Coprocessor fault\n");
		break;
	}
	default:
		break;
	}
	return false;
}

ESD_CORE_EXPORT bool Esd_EveSdCardReady()
{
	return s_EveSdCard_Ready;
}

ESD_CORE_EXPORT bool Esd_CheckFsSourceRes(uint32_t res, const char *filename)
{
	if (res == FSSOURCE_SUCCEEDS)
		return true; // No error, file found
	switch (res)
	{
	case FSSOURCE_FILE_NOT_FOUND: {
		eve_printf_debug("EVE SD card: File not found %s\n", filename);
		break;
	}
	case FSSOURCE_FILE_IO_ERROR: {
		eve_printf_debug("EVE SD card: FileIO Error %s\n", filename);
		break;
	}
	default: {
		eve_printf_debug("EVE SD card: FsSource command Error %s\n", filename);
		break;
	}
	}
	return false; // Error, file not found or IO error
}
#endif // EVE_SUPPORT_CHIPID >= EVE_BT820

ESD_CORE_EXPORT void Esd_BeginLogo()
{
	/* ---- */
	/* NOTE: Bypassed CoDl optimizer on purpose due to coprocessor logo behaviour */
	/* ---- */

	EVE_HalContext *phost = Esd_GetHost();
	Esd_GpuAlloc_Reset(Esd_GAlloc);
	Esd_GpuAlloc_Alloc(Esd_GAlloc, Esd_GAlloc->RamGSize, 0); // Block allocation
	EVE_CoCmd_dlStart(phost);
	EVE_CoCmd_dl(phost, CLEAR_COLOR_RGB(255, 255, 255));
	EVE_CoCmd_dl(phost, CLEAR(1, 0, 0));
	EVE_CoCmd_dl(phost, DISPLAY());
	EVE_CoCmd_swap(phost);
	EVE_CoCmd_dlStart(phost);
	EVE_CoCmd_dl(phost, CLEAR_COLOR_RGB(255, 255, 255));
	EVE_CoCmd_dl(phost, CLEAR(1, 0, 0));
	EVE_CoCmd_dl(phost, DISPLAY());
	EVE_CoCmd_swap(phost);
	EVE_CoCmd_dlStart(phost);
	EVE_CoCmd_dl(phost, CLEAR_COLOR_RGB(255, 255, 255));
	EVE_CoCmd_dl(phost, CLEAR(1, 0, 0));
	EVE_CoCmd_dl(phost, DISPLAY());
	// EVE_CoCmd_memSet(Esd_Host, 0, 0xFF, RAM_G_SIZE);
	EVE_Cmd_waitFlush(phost);
	EVE_CoCmd_logo(phost);
	EVE_Cmd_waitLogo(phost);
	EVE_sleep(3000);
}

ESD_CORE_EXPORT void Esd_EndLogo()
{
	EVE_HalContext *phost = Esd_GetHost();
	EVE_CoCmd_dlStart(phost);
	EVE_CoCmd_dl(phost, CLEAR_COLOR_RGB(255, 255, 255));
	EVE_CoCmd_dl(phost, CLEAR(1, 0, 0));
	EVE_CoCmd_dl(phost, DISPLAY());
	EVE_CoCmd_swap(phost);
	EVE_Cmd_waitFlush(phost);
	Esd_GpuAlloc_Reset(Esd_GAlloc);
}

ESD_CORE_EXPORT void Esd_ShowLogo()
{
	Esd_CurrentContext->ShowLogo = true;
}

/// Run calibrate procedure
ESD_CORE_EXPORT bool Esd_Calibrate()
{
	Esd_Context *ec = Esd_CurrentContext;
	EVE_HalContext *phost = Esd_GetHost();
	uint32_t result;
	int32_t transMatrix[6];

#if defined(EVE_SCREEN_CAPACITIVE)
	EVE_Hal_wr8(phost, REG_CTOUCH_EXTENDED, CTOUCH_MODE_COMPATIBILITY);
#endif

	eve_printf_debug("Esd_Calibrate: Start Frame\n");

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
		EVE_CoCmd_watchdog(phost, 72000000);
#endif
	EVE_CoCmd_dlStart(phost);
	EVE_CoDl_clearColorRgb(phost, 64, 64, 64);
	EVE_CoDl_clear(phost, true, true, true);
	EVE_CoDl_colorRgb(phost, 0xff, 0xff, 0xff);

	// EVE_CoCmd_text(phost, (Esd_Host->Parameters.Display.Width / 2), (Esd_Host->Parameters.Display.Height / 2), 27, OPT_CENTER, "Please Tap on the dot");

	result = EVE_CoCmd_calibrate(phost);

	eve_printf_debug("Esd_Calibrate: End Frame\n");

	// Print the configured values
	EVE_Hal_rdMem(phost, (uint8_t *)transMatrix, REG_TOUCH_TRANSFORM_A, 4 * 6); // read all the 6 coefficients
	eve_printf_debug("Touch screen transform values are A 0x%lx,B 0x%lx,C 0x%lx,D 0x%lx,E 0x%lx, F 0x%lx\n",
	    (unsigned long)transMatrix[0], (unsigned long)transMatrix[1], (unsigned long)transMatrix[2],
	    (unsigned long)transMatrix[3], (unsigned long)transMatrix[4], (unsigned long)transMatrix[5]);

#ifdef EVE_SUPPORT_RENDERTARGET
	// Copy into ec->TouchTransform[0] etc
	// ec->TouchTransform[0] = transMatrix[0];
	// ec->TouchTransform[1] = transMatrix[1];
	// ec->TouchTransform[2] = transMatrix[2];
	// ec->TouchTransform[3] = transMatrix[3];
	// ec->TouchTransform[4] = transMatrix[4];
	// ec->TouchTransform[5] = transMatrix[5];
	ec->TouchTransformC = transMatrix[2];
	ec->TouchTransformF = transMatrix[5];
	ec->TouchTransformValid = true;
#endif

	return result != 0;
}

ESD_CORE_EXPORT void Esd_DeferredFree(void *ptr)
{
	// eve_printf_debug("Request free\n");
	(*(void **)ptr) = Esd_CurrentContext->DeferredFree;
	Esd_CurrentContext->DeferredFree = ptr;
}

ESD_CORE_EXPORT void Esd_ProcessFree()
{
	void *current = Esd_CurrentContext->DeferredFree;
	Esd_CurrentContext->DeferredFree = NULL;
	while (current)
	{
		void *next = (*(void **)current);
		// eve_printf_debug("Free deferred\n");
		esd_free(current);
		current = next;
	}
}

/* end of file */
