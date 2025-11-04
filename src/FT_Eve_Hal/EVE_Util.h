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

#ifndef EVE_UTIL__H
#define EVE_UTIL__H
#include "EVE_HalDefs.h"

/***************
** PARAMETERS **
***************/

#if (defined(_WIN32) || defined(__linux__))
typedef enum EVE_BOOTUP_RESUME
{
	EVE_BOOTUP_RESUME_NORMAL = 0,
	EVE_BOOTUP_RESUME_CONTINUE,
	// EVE_BOOTUP_RESUME_ABORT,
} EVE_BOOTUP_RESUME;

typedef enum EVE_BOOTUP_STATUS
{
	EVE_BOOTUP_STATUS_FAILED = 0,
	EVE_BOOTUP_STATUS_SUCCESS,
	EVE_BOOTUP_STATUS_PENDING,
} EVE_BOOTUP_STATUS;
#endif

typedef struct EVE_BootupParameters
{
	/* Clock PLL multiplier (ft81x: 5, 60MHz, bt81x: 6, 72MHz) */
	EVE_81X_PLL_FREQ_T SystemClock;

	/* External oscillator (default: false) */
	bool ExternalOsc;

	/* SPI */
#if (EVE_SUPPORT_CHIPID >= EVE_FT810) || defined(EVE_MULTI_GRAPHICS_TARGET)
	EVE_SPI_CHANNELS_T SpiChannels; /* Variable to contain single/dual/quad channels */
	uint8_t SpiDummyBytes; /* Number of dummy bytes as 1 or 2 for SPI read */
#endif

#if (defined(_WIN32) || defined(__linux__))
	EVE_BOOTUP_STATUS *StatusResult; /**< Defer bootup sequence intermediately in case of delay or failure */
	EVE_BOOTUP_RESUME Resume;
#endif

} EVE_BootupParameters;

typedef struct EVE_ConfigParameters
{
	/* Display */
	int16_t Width; /* Line buffer width (pixels) */
	int16_t Height; /* Screen and render height (lines) */
	int16_t HCycle;
	int16_t HOffset;
	int16_t HSync0;
	int16_t HSync1;
	int16_t VCycle;
	int16_t VOffset;
	int16_t VSync0;
	int16_t VSync1;
	uint8_t PCLK; /**< Pixel clock divisor (0 = disable), REG_PCLK. On BT820 (LVDS support) this refers to TXCLKDIV+1 */
	int8_t PCLKPol;

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	/** Pixels per clock, multiplier. 1, 2, or 4 */
	uint8_t PixelsPerClock;
#endif

#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
	int8_t Swizzle;
	int8_t CSpread;
	uint8_t OutBitsR;
	uint8_t OutBitsG;
	uint8_t OutBitsB;
	bool Dither;
#endif

	uint32_t RefClkFreq; /**< Reference clock frequency (system clock 60MHz, 72MHz, etc on BT8xx, or pixel clock 572Mhz etc on BT82x) */

#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT815)
	bool AdaptiveFramerate;
#endif

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT817)
	bool PClk2X;
	uint16_t PClkFreq;
	int16_t AhHCycleMax;
#endif

#ifdef EVE_SUPPORT_HSF
	/** Physical horizontal pixels. Set to 0 to disable HSF. */
	int16_t HsfWidth; /**< Screen width (columns) */
#endif

#endif
	
#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	/** Allocate a swapchain at the back of RAM_G, ahead of any RAM_G used by the extracted coprocessor ROM */
	int16_t SwapchainFormat;
#endif

} EVE_ConfigParameters;

/* Display resolution presets.
NOTE: Also update `s_DisplayResolutions` and `s_DisplayNames` in EVE_Util.c around ln 50,
as well as `EVE_Util_configDefaults` around ln 500, when adding display presets. */
typedef enum EVE_DISPLAY_T
{
	EVE_DISPLAY_DEFAULT = 0,

	/* Landscape */
	EVE_DISPLAY_QVGA_320x240_56Hz,
	EVE_DISPLAY_WQVGA_480x272_60Hz,
	EVE_DISPLAY_WVGA_800x480_74Hz,
	EVE_DISPLAY_WSVGA_1024x600_59Hz,
	EVE_DISPLAY_HDTV_1280x720_58Hz,
	EVE_DISPLAY_WXGA_1280x800_57Hz,
	EVE_DISPLAY_FHD_1920x1080_60Hz, 
	EVE_DISPLAY_WUXGA_1920x1200_90Hz, /**< Landscape */
	EVE_DISPLAY_WUXGA_1920x1200_111Hz, /**< Landscape (maximum refresh rate) */

	/* Portrait */
	EVE_DISPLAY_HVGA_320x480_60Hz,

	/* IDM2040 */
	EVE_DISPLAY_WVGA_800x480_W860_60Hz,
	
	/* IDM2040-21R(FT800) */
	EVE_DISPLAY_WQVGA_480x480_48Hz,

	/* Riverdi */
	EVE_DISPLAY_RIVERDI_IPS35_62Hz,
	EVE_DISPLAY_RIVERDI_IPS43_58Hz,
	EVE_DISPLAY_RIVERDI_IPS50_63Hz,
	EVE_DISPLAY_RIVERDI_IPS70_59Hz,
	EVE_DISPLAY_RIVERDI_IPS101_59Hz,

	EVE_DISPLAY_NB,

} EVE_DISPLAY_T;

/**********************
** INIT AND SHUTDOWN **
**********************/

/* Get the default bootup parameters. */
EVE_HAL_EXPORT void EVE_Util_bootupDefaults(EVE_HalContext *phost, EVE_BootupParameters *bootup);

/* Boot up the device. Obtains the chip Id. Sets up clock and SPI speed. */
EVE_HAL_EXPORT bool EVE_Util_bootup(EVE_HalContext *phost, EVE_BootupParameters *bootup);

/* Get the default configuration parameters for the specified display. */
EVE_HAL_EXPORT void EVE_Util_configDefaults(EVE_HalContext *phost, EVE_ConfigParameters *config, EVE_DISPLAY_T display);

/* Get the default configuration parameters for the specified display parameters. */
EVE_HAL_EXPORT bool EVE_Util_configDefaultsEx(EVE_HalContext *phost, EVE_ConfigParameters *config, uint32_t width, uint32_t height, uint32_t refreshRate, uint32_t hsfWidth);

/* Boot up the device. Configures the display, resets or initializes coprocessor state. */
EVE_HAL_EXPORT bool EVE_Util_config(EVE_HalContext *phost, EVE_ConfigParameters *config);

/* Complementary of bootup. Does not close the HAL context. */
EVE_HAL_EXPORT void EVE_Util_shutdown(EVE_HalContext *phost);

/* Sets the display list to a blank cleared screen. */
EVE_HAL_EXPORT void EVE_Util_clearScreen(EVE_HalContext *phost);

/** Get the current touch transform matrix, unrotated and unmirrored. */
EVE_HAL_EXPORT void EVE_Util_getNormalizedTouchTransform(
    EVE_HalContext *phost,
    int32_t *outA, int32_t *outB, int32_t *outC,
    int32_t *outD, int32_t *outE, int32_t *outF);

/** Apply a touch transform in normalized (no swap, no mirror) form to the current display rotation and size. */
EVE_HAL_EXPORT void EVE_Util_setNormalizedTouchTransformFrom(
    EVE_HalContext *phost,
    int32_t inA, int32_t inB, int32_t inC,
    int32_t inD, int32_t inE, int32_t inF);

/* Resets the coprocessor.
To be used after a coprocessor fault, or to exit CMD_LOGO.
After a reset, flash will be in attached state (not in full speed).
Coprocessor will be set to the latest API level. */
EVE_HAL_EXPORT bool EVE_Util_resetCoprocessor(EVE_HalContext *phost);

/* Calls EVE_Util_bootup and EVE_Util_config using the default parameters */
EVE_HAL_EXPORT bool EVE_Util_bootupConfig(EVE_HalContext *phost);

/**********************
** INTERACTIVE SETUP **
**********************/

/* Command line device selection utility */
#if (defined(_WIN32) || defined(__linux__))
EVE_HAL_EXPORT void EVE_Util_selectDeviceInteractive(EVE_CHIPID_T *chipId, size_t *deviceIdx);
#else
static inline void EVE_Util_selectDeviceInteractive(EVE_CHIPID_T *chipId, size_t *deviceIdx)
{
	*chipId = EVE_SUPPORT_CHIPID;
	*deviceIdx = -1;
}
#endif

/* Command line display selection utility */
#if (defined(_WIN32) || defined(__linux__)) && defined(EVE_MULTI_GRAPHICS_TARGET)
EVE_HAL_EXPORT void EVE_Util_selectDisplayInteractive(EVE_DISPLAY_T *display);
#else
static inline void EVE_Util_selectDisplayInteractive(EVE_DISPLAY_T *display)
{
	*display = EVE_DISPLAY_DEFAULT;
}
#endif

#if (defined(_WIN32) || defined(__linux__)) && defined(EVE_FLASH_AVAILABLE)
EVE_HAL_EXPORT void EVE_Util_selectFlashFileInteractive(eve_tchar_t *flashPath, size_t flashPathSize, bool *updateFlash, bool *updateFlashFirmware, const EVE_HalParameters *params, const eve_tchar_t *flashFile);
#else
static inline void EVE_Util_selectFlashFileInteractive(eve_tchar_t *flashPath, size_t flashPathSize, bool *updateFlash, bool *updateFlashFirmware, const EVE_HalParameters *params, const eve_tchar_t *flashFile)
{
	flashPath[0] = '\0';
	*updateFlash = false;
	*updateFlashFirmware = false;
}
#endif

#if (defined(_WIN32) || defined(__linux__)) && defined(EVE_FLASH_AVAILABLE)
EVE_HAL_EXPORT void EVE_Util_uploadFlashFileInteractive(EVE_HalContext *phost, const eve_tchar_t *flashPath, bool updateFlashFirmware);
#else
static inline void EVE_Util_uploadFlashFileInteractive(EVE_HalContext *phost, const eve_tchar_t *flashPath, bool updateFlashFirmware)
{
	/* no-op */
}
#endif

#if defined(BT8XXEMU_PLATFORM)
EVE_HAL_EXPORT void EVE_Util_emulatorDefaults(EVE_HalParameters *params, void *emulatorParams, EVE_CHIPID_T chipId);
#if defined(EVE_FLASH_AVAILABLE)
EVE_HAL_EXPORT void EVE_Util_emulatorFlashDefaults(EVE_HalParameters *params, const void *emulatorParams, void *flashParams, const eve_tchar_t *flashPath);
#endif
#endif

/* Command line device selection utility.
Provides selection of flash file, and option to write the flash file to the device.
Parameter `flashFile` is only relevant for Windows build.
Falls back to no interactivity on FT9XX platform */
EVE_HAL_EXPORT bool EVE_Util_openDeviceInteractive(EVE_HalContext *phost, const eve_tchar_t *flashFile);

/* Calls EVE_Util_bootup and EVE_Util_config using the default parameters.
Falls back to no interactivity on FT9XX platform */
EVE_HAL_EXPORT bool EVE_Util_bootupConfigInteractive(EVE_HalContext *phost, EVE_DISPLAY_T display);

/* Forces a coprocessor fault, useful for triggering a delayed reset (TODO: Remove this) */
EVE_HAL_EXPORT void EVE_Util_forceFault(EVE_HalContext *phost, const char *err);

#endif /* #ifndef EVE_HAL_INCL__H */

/* end of file */
