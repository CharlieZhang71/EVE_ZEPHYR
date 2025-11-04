/*
ESD Framework
Copyright (C) 2018  Bridgetek Pte Lte
Author: Jan Boon <jan.boon@kaetemi.be>
*/

#include "Esd_BitmapHandle.h"
#include "Esd_Context.h"

// Number of bitmap handles to use (and also the scratch handle)
// List which bitmap info is using a handle
// void *Esd_BitmapHandleInfo[ESD_BITMAPHANDLE_NB] = { 0 };
// Esd_GpuHandle Esd_BitmapHandleGpuHandle[ESD_BITMAPHANDLE_NB] = { 0 };
// uint8_t Esd_BitmapHandleUse[ESD_BITMAPHANDLE_NB] = { 0 };
// uint8_t Esd_ScratchHandle = 15;
#define ESD_SCRATCHHANDLE ESD_CO_SCRATCH_HANDLE

// bool Esd_BitmapHandleResized[ESD_BITMAPHANDLE_NB] = { 0 };
// uint8_t Esd_BitmapHandlePage[ESD_BITMAPHANDLE_NB] = { 0 };

// #define ESD_FONTHANDLE_NB 16UL // First font handle
// #define ESD_ROMFONT_NB 35UL
// uint8_t Esd_RomFontHandles[ESD_ROMFONT_NB - ESD_FONTHANDLE_NB] = { 0 };

#ifdef EVE_SUPPORT_LARGEFONT
#define ESD_ROMFONT_CAP 35UL // Max, rom font handle, exclusive
#else
#define ESD_ROMFONT_CAP 32UL // Max, rom font handle, exclusive
#endif
#define ESD_ROMFONT_MAX (phost ? (EVE_Hal_supportLargeFont(phost) ? 35UL : 32UL) : ESD_ROMFONT_CAP)
#define ESD_ROMFONT_MIN 16UL // Min, rom font handle, inclusive
#define ESD_ROMFONT_NBCAP (ESD_ROMFONT_CAP - ESD_ROMFONT_MIN)
#define ESD_ROMFONT_NB (ESD_ROMFONT_MAX - ESD_ROMFONT_MIN)
#if defined(_MSC_VER) && (_MSC_VER < 1800)
/* Designated initializers not supported in older Visual Studio versions */
static Esd_RomFontInfo s_RomFonts[ESD_ROMFONT_NBCAP] = {
	{ 0, ESD_FONT_ROM, 16UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 17UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 18UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 19UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 20UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 21UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 22UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 23UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 24UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 25UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 26UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 27UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 28UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 29UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 30UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 31UL, 0, 0, 0 },
#ifdef EVE_SUPPORT_LARGEFONT
	{ 0, ESD_FONT_ROM, 32UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 33UL, 0, 0, 0 },
	{ 0, ESD_FONT_ROM, 34UL, 0, 0, 0 },
#endif
};
#else
static Esd_RomFontInfo s_RomFonts[ESD_ROMFONT_NBCAP] = {
	{ .Type = ESD_FONT_ROM, .RomFont = 16UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 17UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 18UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 19UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 20UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 21UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 22UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 23UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 24UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 25UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 26UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 27UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 28UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 29UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 30UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 31UL },
#ifdef EVE_SUPPORT_LARGEFONT
	{ .Type = ESD_FONT_ROM, .RomFont = 32UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 33UL },
	{ .Type = ESD_FONT_ROM, .RomFont = 34UL },
#endif
};
#endif

static void Esd_InitRomFontHeight()
{
	int i;
	EVE_HalContext *phost = Esd_GetHost();

	for (i = 0; i < (int)ESD_ROMFONT_NB; ++i)
	{
		const uint32_t romFontAddr = EVE_Hal_romFontAddress(phost, i + 16);
		const uint32_t signature = EVE_Hal_rd32(phost, romFontAddr); // 0x0100AAFF or 0x0200AAFF on BT820
		const uint32_t heightOffset = signature == 0x0100AAFF ? 28 : (signature == 0x0200AAFF ? 28 : 140);
		const uint32_t heightAddr = romFontAddr + heightOffset;
		s_RomFonts[i].FontHeight = EVE_Hal_rd16(Esd_Host, heightAddr);
		s_RomFonts[i].BaseLine = s_RomFonts[i].FontHeight;
		s_RomFonts[i].CapsHeight = s_RomFonts[i].FontHeight;

		// BT820
		// NOTE: Extended Format 2 Font Metrics Block has midline and baseline, J1 firmware now handles centering of these fonts correctly
		// TODO: Disable software handling of baseline and capsheight with Extended Format 2 fonts (or set baseline and capsheight to font height)
	}
}

ESD_CORE_EXPORT uint16_t Esd_GetFontHeight(Esd_FontInfo *fontInfo)
{
	if (fontInfo)
		return fontInfo->FontHeight;
	return 0;
}

ESD_CORE_EXPORT uint16_t Esd_GetFontBaseLine(Esd_FontInfo *fontInfo)
{
	if (fontInfo)
		return fontInfo->BaseLine;
	return 0;
}

ESD_CORE_EXPORT uint16_t Esd_GetFontCapsHeight(Esd_FontInfo *fontInfo)
{
	if (fontInfo)
		return fontInfo->CapsHeight;
	return 0;
}

ESD_CORE_EXPORT uint16_t Esd_GetFontXOffset(Esd_FontInfo *fontInfo)
{
	if (fontInfo)
		return fontInfo->XOffset;
	return 0;
}

void Esd_BitmapHandle_Initialize()
{
	// memset(Esd_BitmapHandleGpuHandle, 0, sizeof(Esd_BitmapHandleGpuHandle));
	Esd_InitRomFontHeight();
}

ESD_CORE_EXPORT void Esd_BitmapHandle_FrameStart(Esd_HandleState *handleState)
{
	EVE_HalContext *phost = Esd_GetHost();
	uint32_t i;
	(void)phost;

	for (i = 0; i < ESD_BITMAPHANDLE_NB; ++i)
	{
		// 2: In use last frame
		// 1: Not in use anymore
		// 0: Not in use
		if (handleState->Use[i] < 2)
			handleState->Use[i] = 0;
		else
			--handleState->Use[i];
		// 2: In use this frame
		// 1: In use last frame
		// 0: Not in use
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
		handleState->Pending[i] = ESD_BITMAPHANDLE_RENDERED;
		handleState->Flags[i] = 0;
#endif
	}
}

/// Reset the bitmap handle state
ESD_CORE_EXPORT void Esd_BitmapHandle_Reset(Esd_HandleState *handleState)
{
	EVE_HalContext *phost = Esd_GetHost();
	uint32_t i;
	(void)phost;

	memset(handleState, 0, sizeof(Esd_HandleState));

	for (i = 0; i < ESD_BITMAPHANDLE_NB; ++i)
	{
		handleState->Address[i] = GA_INVALID;
		handleState->GpuHandle[i] = GA_HANDLE_INVALID;
	}
}

/// Flag pending bitmap info writes for this display list as rendered
/// Restore expected initial state before restoring a saved display list based on flags
ESD_CORE_EXPORT void Esd_BitmapHandle_OnSwap(uint8_t *handleFlags, uint8_t *handleInitialPage)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	EVE_HalContext *phost = Esd_GetHost();
	Esd_HandleState *handleState = &Esd_CurrentContext->HandleState;
	uint32_t handle;

	for (handle = 0; handle < ESD_BITMAPHANDLE_NB; ++handle)
	{
		/* Flag pending bitmap info writes for this display list as rendered */
		if (handleState->Pending[handle] == phost->DlStateBase)
		{
			handleState->Pending[handle] = ESD_BITMAPHANDLE_RENDERED;
		}

		/* Restore expected initial state for the previous display list context */
		if (handleFlags)
		{
			uint8_t flags = handleFlags[handle];
			if ((flags & ESD_BHFLAGS_EXPECTS_DEFAULT_SIZE) && handleState->Resized[handle])
			{
				/* Expect default size, but hardware state is no longer default size */
				Esd_CoDl_BitmapSizeReset(handle); /* Reset handle */
			}
			if ((flags & ESD_BHFLAGS_EXPECTS_INITIAL_PAGE) && handleInitialPage[handle] != handleState->Page[handle])
			{
				/* Expects hardware state to match a specific page, but hardware page changed */
				Esd_CoDl_PagedBitmapSource(handle, handleInitialPage[handle]); /* Set to expected page */
			}
		}

		/* Track final hardware state */
		handleState->HwResized[handle] = handleState->Resized[handle];
		handleState->HwPage[handle] = handleState->Page[handle];
	}
#endif
}

ESD_CORE_EXPORT uint32_t Esd_BitmapHandle_GetTotalUsed()
{
	EVE_HalContext *phost = Esd_GetHost();
	uint32_t total = 0;
	uint32_t i;
	(void)phost;

	for (i = 0; i < ESD_BITMAPHANDLE_NB; ++i)
	{
		if ((i != ESD_SCRATCHHANDLE) && (Esd_CurrentContext->HandleState.Use[i] > 0))
		{
			++total;
		}
	}
	return total;
}

ESD_CORE_EXPORT uint32_t Esd_BitmapHandle_GetTotal()
{
	EVE_HalContext *phost = Esd_GetHost();
	(void)phost;
	return ESD_BITMAPHANDLE_NB - ESD_BITMAPHANDLE_RESERVED;
}

ESD_CORE_EXPORT Esd_FontInfo *Esd_GetRomFont(uint8_t font)
{
	EVE_HalContext *phost = Esd_GetHost();
	(void)phost;

	if (font >= ESD_ROMFONT_MIN && font < ESD_ROMFONT_MAX)
		return (Esd_FontInfo *)(void *)&s_RomFonts[font - ESD_ROMFONT_MIN];
	return NULL;
}

static eve_progmem_const uint8_t c_AstcBlockHeight[] = {
	4, 4, 5, 5, 6, 5, 6, 8, 5, 6, 8, 10, 10, 12, 0xFF, 0xFF
};

ESD_CORE_EXPORT void Esd_CoDl_PagedBitmapSource(uint8_t handle, uint8_t page)
{
	EVE_HalContext *phost = Esd_GetHost();
	Esd_HandleState *handleState = &Esd_CurrentContext->HandleState;
	if (ESD_BITMAPHANDLE_VALID(handle))
	{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
		uint8_t flags = handleState->Flags[handle];
#endif
		if (handleState->Page[handle] != page)
		{
			uint32_t pageAddr;
			uint32_t pageOffset;
			Esd_BitmapInfo *info = handleState->Info[handle];
			uint32_t addr = Esd_GpuAlloc_Get(Esd_GAlloc, handleState->GpuHandle[handle]);
			EVE_CoDl_bitmapHandle(phost, handle);
			pageOffset = ((((uint32_t)page) << 7) * info->Stride * info->Height);
			if (EVE_CHIPID >= EVE_BT815 && ESD_IS_FORMAT_ASTC(info->Format))
				pageOffset /= c_AstcBlockHeight[info->Format & 0xF]; // Stride under ASTC is by block row
			pageAddr = addr + pageOffset;
			EVE_CoDl_bitmapSource(phost, pageAddr);
			handleState->Page[handle] = page;
		}
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
		else
		{
			flags |= ESD_BHFLAGS_EXPECTS_INITIAL_PAGE;
		}
		/* Track initial frame cell page state for render target support */
		if (!(flags & ESD_BHFLAGS_PAGE_SET))
		{
			flags |= ESD_BHFLAGS_PAGE_SET;
			handleState->InitialPage[handle] = page;
			handleState->Flags[handle] = flags;
		}
#endif
	}
}

ESD_CORE_EXPORT void Esd_CoDl_PagedCell(uint8_t handle, uint16_t cell)
{
	EVE_HalContext *phost = Esd_GetHost();
	EVE_CoDl_bitmapHandle(phost, handle);
	Esd_CoDl_PagedBitmapSource(handle, cell >> 7);
	EVE_CoDl_cell(phost, cell & 0x7F);
}

ESD_CORE_EXPORT uint8_t Esd_CoDl_SetupBitmap(Esd_BitmapInfo *bitmapInfo)
{
	// Get bitmap address
	EVE_HalContext *phost = Esd_GetHost();
	Esd_HandleState *handleState = &Esd_CurrentContext->HandleState;
	uint32_t addr = Esd_LoadBitmap(bitmapInfo);
	uint32_t handle;
	bool handleValid;

	if (addr == GA_INVALID)
		return ESD_BITMAPHANDLE_INVALID; // Bitmap not loaded (out of memory or file not found)

	handle = bitmapInfo->BitmapHandle;
	handleValid = ESD_BITMAPHANDLE_VALID(handle)
	    && (handle != ESD_SCRATCHHANDLE)
	    && (handleState->Info[handle] == bitmapInfo)
	    && (handleState->Address[handle] == addr)
	    && (handleState->GpuHandle[handle].Id == bitmapInfo->GpuHandle.Id)
	    && (handleState->GpuHandle[handle].Seq == bitmapInfo->GpuHandle.Seq);
	if (!handleValid
#if (EVE_SUPPORT_CHIPID >= EVE_BT820) // If the bitmap was loaded in another display list that wasn't rendered yet, rewrite it in this display list
	    || (handleState->Pending[handle] != ESD_BITMAPHANDLE_RENDERED && handleState->Pending[handle] != phost->DlStateBase)
#endif
	)
	{
		uint32_t i;
		uint32_t format;

		// Bitmap is loaded but no handle is setup, create a new handle for this bitmap
		// eve_printf_debug("Find free bitmap handle for addr %i\n", (int)addr);

		if (Esd_CurrentContext->LoopState != ESD_LOOPSTATE_RENDER)
		{
			eve_printf_debug("Warning: Can only setup bitmap during render pass\n");
			return ESD_BITMAPHANDLE_INVALID;
		}

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
		if (!handleValid)
#else
		esd_scope()
#endif
		{
			// Find a free handle
			handle = ESD_SCRATCHHANDLE; // Fallback to scratch handle
			for (i = 0; i < ESD_BITMAPHANDLE_NB; ++i)
			{
				if ((i != ESD_SCRATCHHANDLE) && (!Esd_CurrentContext->HandleState.Use[i]))
				{
					// Attach this handle to the bitmap info
					handle = i;
					Esd_CurrentContext->HandleState.Info[i] = bitmapInfo;
					Esd_CurrentContext->HandleState.Address[i] = addr;
					Esd_CurrentContext->HandleState.GpuHandle[i] = bitmapInfo->GpuHandle;
					break;
				}
			}
		}

#ifdef ESD_LITTLEFS_FLASH
		eve_printf_debug("Use handle %i, addr %i%s, gpu alloc %i, %i, file %s%s\n",
		    (int)handle, (int)addr, ESD_DL_IS_FLASH_ADDRESS(addr) ? " (flash)" : "",
		    (int)bitmapInfo->GpuHandle.Id, (int)bitmapInfo->GpuHandle.Seq,
		    bitmapInfo->Type == ESD_RESOURCE_PROGMEM ? "<progmem>" : (bitmapInfo->File ? bitmapInfo->File : "<no file>"),
		    ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? " (flash)" : "");
#elif defined(EVE_FLASH_AVAILABLE)
		eve_printf_debug("Use handle %i, addr %i%s, gpu alloc %i, %i, file %s\n",
		    (int)handle, (int)addr,
		    ESD_DL_IS_FLASH_ADDRESS(addr) ? " (flash)" : "",
		    (int)bitmapInfo->GpuHandle.Id, (int)bitmapInfo->GpuHandle.Seq,
		    bitmapInfo->Type == ESD_RESOURCE_PROGMEM ? "<progmem>" : ((!ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) && bitmapInfo->File) ? bitmapInfo->File : "<no file>"));
#else
		eve_printf_debug("Use handle %i, addr %i, gpu alloc %i, %i, file %s\n",
		    (int)handle, (int)addr,
		    (int)bitmapInfo->GpuHandle.Id, (int)bitmapInfo->GpuHandle.Seq,
		    bitmapInfo->Type == ESD_RESOURCE_PROGMEM ? "<progmem>" : ((!ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) && bitmapInfo->File) ? bitmapInfo->File : "<no file>"));
#endif

		bitmapInfo->BitmapHandle = handle;

		// Setup the handle
		EVE_CoDl_bitmapHandle(phost, handle);
		format = bitmapInfo->Format;
		if (format == DXT1)
			format = L1;
		else if (format == DXT1L2)
			format = L2;
		else if (format == JPEG) // This case only occurs if the format wasn't loaded by the coprocessor
			format = RGB565; // TODO: Support for grayscale
		else if (format == PNG) // This case only occurs if the format wasn't loaded by the coprocessor
			format = RGB565; // TODO: Support for other PNG formats

		if (ESD_IS_FORMAT_PALETTED(bitmapInfo->Format))
		{
			bool hasPalette = true;
#ifndef ESD_LITTLEFS_FLASH
			if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type))
			{
				if (bitmapInfo->PaletteFlashAddress == FA_INVALID)
					hasPalette = false;
			}
			else
#endif
			if (!bitmapInfo->PaletteFile)
			{
				hasPalette = false;
			}

			if (!hasPalette)
				addr += 512;
		}

#if (EVE_SUPPORT_CHIPID >= EVE_FT810)
		if (EVE_CHIPID >= EVE_FT810)
		{
			EVE_CoCmd_setBitmap(phost, addr, format, bitmapInfo->Width, bitmapInfo->Height); // TODO: What with stride?
		}
		else
#endif
		{
			// FT80X compatibility
			EVE_CoDl_bitmapSource(phost, addr);
			EVE_CoDl_bitmapLayout(phost, format, bitmapInfo->Stride, bitmapInfo->Height);
			EVE_CoDl_bitmapSize(phost, NEAREST, BORDER, BORDER, bitmapInfo->Width, bitmapInfo->Height);
		}

#if (EVE_SUPPORT_CHIPID >= EVE_BT815)
		if (EVE_CHIPID >= EVE_BT815)
		{
			// Important. Bitmap swizzle not reset by SETBITMAP
			if (bitmapInfo->Swizzle)
				EVE_CoDl_bitmapSwizzle(phost, bitmapInfo->SwizzleR & 0xFF, bitmapInfo->SwizzleG & 0xFF, bitmapInfo->SwizzleB & 0xFF, bitmapInfo->SwizzleA & 0xFF);
			else
				EVE_CoDl_bitmapSwizzle(phost, RED, GREEN, BLUE, ALPHA);
		}
#endif

		Esd_CurrentContext->HandleState.Resized[handle] = 0;
		Esd_CurrentContext->HandleState.Page[handle] = 0;
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
		Esd_CurrentContext->HandleState.Flags[handle] = (ESD_BHFLAGS_SIZE_SET | ESD_BHFLAGS_PAGE_SET);
		Esd_CurrentContext->HandleState.Pending[handle] = phost->DlStateBase;
#endif
	}

	// TEMPORARY WORKAROUND: SetBitmap not correctly being applied some frames... Need to check!
	// EVE_CoDl_bitmapHandle(handle);
	// uint32_t format = bitmapInfo->Format;
	// if (format == DXT1) format = L1;
	// else if (format == JPEG) format = RGB565; // TODO: Support for grayscale
	// else if (format == PNG) format = RGB565; // TODO: Support for other PNG formats
	// EVE_CoCmd_setBitmap(Esd_Host, addr, format, bitmapInfo->Width, bitmapInfo->Height); // TODO: What with stride?
	// Esd_BitmapHandleResized[handle] = 0;

	if (ESD_BITMAPHANDLE_VALID(handle) && (handle != ESD_SCRATCHHANDLE)) // When valid and not using scratch handle
	{
		Esd_CurrentContext->HandleState.Use[handle] = 2; // In use
	}

#if (EVE_SUPPORT_CHIPID >= EVE_FT810)
	if (EVE_CHIPID >= EVE_FT810)
	{
		// Use palette if available
		uint32_t paletteAddr = Esd_LoadPalette(bitmapInfo);
		if (bitmapInfo->Format != PALETTED8) // PALETTED8 uses custom palette setup
		{
			if (paletteAddr != GA_INVALID)
			{
				EVE_CoDl_paletteSource(phost, paletteAddr);
			}
			else if (ESD_IS_FORMAT_PALETTED(bitmapInfo->Format))
			{
				EVE_CoDl_paletteSource(phost, addr);
			}
		}
	}
	else
#endif
	{
		// Load PALETTED for FT80X
#if EVE_SUPPORT_CHIPID_TO(EVE_FT810)
		if (bitmapInfo->Type == ESD_RESOURCE_PROGMEM && ESD_IS_FORMAT_PALETTED(bitmapInfo->Format))
		{
			EVE_Hal_wrProgMem(phost, RAM_PAL, bitmapInfo->PaletteProgMem, 1024);
		}
#endif
	}

	return handle;
}

ESD_CORE_EXPORT uint8_t Esd_CoDl_SetupRomFont(uint8_t font)
{
	return Esd_CoDl_SetupFont(Esd_GetRomFont(font));
}

ESD_CORE_EXPORT uint8_t Esd_CoDl_SetupFont(Esd_FontInfo *fontInfo)
{
	EVE_HalContext *phost = Esd_GetHost();
	uint32_t handle;
	bool handleValid;
	eve_assert(phost);

	if (!fontInfo)
		return ESD_BITMAPHANDLE_INVALID;

	handle = fontInfo->BitmapHandle;
	if (fontInfo->Type == ESD_FONT_ROM)
	{
		// Get rom font
		Esd_RomFontInfo *romFontInfo = (Esd_RomFontInfo *)(void *)fontInfo;
		uint8_t font = romFontInfo->RomFont;
#if (EVE_SUPPORT_CHIPID >= EVE_FT810)
		if (EVE_CHIPID >= EVE_FT810)
		{
			if (!(font >= ESD_ROMFONT_MIN && font < ESD_ROMFONT_MAX))
			{
				return ESD_BITMAPHANDLE_INVALID; // Not a valid rom font
			}

			handleValid = ESD_BITMAPHANDLE_VALID(handle)
			    && (handle != ESD_SCRATCHHANDLE)
			    && (Esd_CurrentContext->HandleState.Info[handle] == romFontInfo)
			    && (Esd_CurrentContext->HandleState.GpuHandle[handle].Id == MAX_NUM_ALLOCATIONS)
			    && (Esd_CurrentContext->HandleState.GpuHandle[handle].Seq == font);
			if (!handleValid
#if (EVE_SUPPORT_CHIPID >= EVE_BT820) // If the font was loaded in another display list that wasn't rendered yet, rewrite it in this display list
			    || (Esd_CurrentContext->HandleState.Pending[handle] != ESD_BITMAPHANDLE_RENDERED && Esd_CurrentContext->HandleState.Pending[handle] != phost->DlStateBase)
#endif
			)
			{
				uint32_t i;
				// The handle is no longer valid, make a new one

				if (Esd_CurrentContext->LoopState != ESD_LOOPSTATE_RENDER)
				{
					eve_printf_debug("Warning: Can only setup rom font during render pass\n");
					return ESD_BITMAPHANDLE_INVALID;
				}

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
				if (!handleValid)
#else
				esd_scope()
#endif
				{
					// Find a free handle
					handle = font;
					if (handle < ESD_BITMAPHANDLE_NB && (handle != ESD_SCRATCHHANDLE) && (!Esd_CurrentContext->HandleState.Use[handle]))
					{
						Esd_CurrentContext->HandleState.Info[handle] = romFontInfo;
						Esd_CurrentContext->HandleState.GpuHandle[handle].Id = MAX_NUM_ALLOCATIONS;
						Esd_CurrentContext->HandleState.GpuHandle[handle].Seq = font;
					}
					else
					{
						handle = ESD_SCRATCHHANDLE; // Fallback to scratch handle
						for (i = 0; i < ESD_BITMAPHANDLE_NB; ++i)
						{
							if ((i != ESD_SCRATCHHANDLE) && (!Esd_CurrentContext->HandleState.Use[i]))
							{
								// Attach this handle to the bitmap info
								handle = i;
								Esd_CurrentContext->HandleState.Info[i] = romFontInfo;
								Esd_CurrentContext->HandleState.GpuHandle[i].Id = MAX_NUM_ALLOCATIONS;
								Esd_CurrentContext->HandleState.GpuHandle[i].Seq = font;
								break;
							}
						}
					}
				}

				eve_printf_debug("Use handle %i, font %i\n",
				    (int)handle, (int)font);

				// Set the font
				romFontInfo->BitmapHandle = handle;
				EVE_CoCmd_romFont(Esd_Host, handle, font);

				eve_assert(handle < ESD_BITMAPHANDLE_CAP); // FIXME: Remove this, C6386 false positive
				Esd_CurrentContext->HandleState.Resized[handle] = 0;
				Esd_CurrentContext->HandleState.Page[handle] = 0;
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
				Esd_CurrentContext->HandleState.Flags[handle] = (ESD_BHFLAGS_SIZE_SET | ESD_BHFLAGS_PAGE_SET);
				Esd_CurrentContext->HandleState.Pending[handle] = phost->DlStateBase;
#endif
			}
		}
		else
#endif
		{
			romFontInfo->BitmapHandle = font;
			return font;
		}
	}
	else
	{
		// Get font address
		uint32_t addr = Esd_LoadFont(fontInfo);
		if (addr == GA_INVALID)
			return ESD_BITMAPHANDLE_INVALID; // Font not loaded (out of memory or file not found)

		handleValid = ESD_BITMAPHANDLE_VALID(handle)
		    && (handle != ESD_SCRATCHHANDLE)
		    && (Esd_CurrentContext->HandleState.Info[handle] == fontInfo)
		    && (Esd_CurrentContext->HandleState.Address[handle] == addr)
		    && (Esd_CurrentContext->HandleState.GpuHandle[handle].Id == fontInfo->FontResource.GpuHandle.Id)
		    && (Esd_CurrentContext->HandleState.GpuHandle[handle].Seq == fontInfo->FontResource.GpuHandle.Seq);
		if (!handleValid
#if (EVE_SUPPORT_CHIPID >= EVE_BT820) // If the font was loaded in another display list that wasn't rendered yet, rewrite it in this display list
		    || (Esd_CurrentContext->HandleState.Pending[handle] != ESD_BITMAPHANDLE_RENDERED && Esd_CurrentContext->HandleState.Pending[handle] != phost->DlStateBase)
#endif
		)
		{
			uint32_t i;

			// The handle is no longer valid, make a new one

			if (Esd_CurrentContext->LoopState != ESD_LOOPSTATE_RENDER)
			{
				eve_printf_debug("Warning: Can only setup font during render pass\n");
				return ESD_BITMAPHANDLE_INVALID;
			}

			// Find a free handle
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
			if (!handleValid)
#else
			esd_scope()
#endif
			{
				handle = ESD_SCRATCHHANDLE; // Fallback to scratch handle
				for (i = 0; i < ESD_BITMAPHANDLE_NB; ++i)
				{
					if ((i != ESD_SCRATCHHANDLE) && (!Esd_CurrentContext->HandleState.Use[i]))
					{
						// Attach this handle to the font info
						handle = i;
						Esd_CurrentContext->HandleState.Info[i] = fontInfo;
						Esd_CurrentContext->HandleState.Address[i] = addr;
						Esd_CurrentContext->HandleState.GpuHandle[i] = fontInfo->FontResource.GpuHandle;
						break;
					}
				}
			}
#if (EVE_SUPPORT_CHIPID < EVE_BT820)

#ifdef ESD_LITTLEFS_FLASH
			eve_printf_debug("Use handle %i, addr %i, %i, gpu alloc %i, %i, %i, %i, file %s%s, %s%s\n",
			    (int)handle, (int)addr, (int)Esd_GpuAlloc_Get(Esd_GAlloc, fontInfo->GlyphResource.GpuHandle),
			    (int)fontInfo->FontResource.GpuHandle.Id, (int)fontInfo->FontResource.GpuHandle.Seq,
			    (int)fontInfo->GlyphResource.GpuHandle.Id, (int)fontInfo->GlyphResource.GpuHandle.Seq,
			    (fontInfo->FontResource.Type == ESD_RESOURCE_FILE) ? fontInfo->FontResource.File : "<no file>",
			    (ESD_RESOURCE_IS_FLASH(fontInfo->FontResource.Type) ? " (flash)" : ""),
			    (fontInfo->GlyphResource.Type == ESD_RESOURCE_FILE) ? fontInfo->GlyphResource.File : "<no file>",
			    (ESD_RESOURCE_IS_FLASH(fontInfo->GlyphResource.Type) ? " (flash)" : ""));
#else
			eve_printf_debug("Use handle %i, addr %i, %i, gpu alloc %i, %i, %i, %i, file %s, %s, flash %i, %i\n",
			    (int)handle, (int)addr, (int)Esd_GpuAlloc_Get(Esd_GAlloc, fontInfo->GlyphResource.GpuHandle),
			    (int)fontInfo->FontResource.GpuHandle.Id, (int)fontInfo->FontResource.GpuHandle.Seq,
			    (int)fontInfo->GlyphResource.GpuHandle.Id, (int)fontInfo->GlyphResource.GpuHandle.Seq,
			    (fontInfo->FontResource.Type == ESD_RESOURCE_FILE) ? fontInfo->FontResource.File : "<no file>",
			    (fontInfo->GlyphResource.Type == ESD_RESOURCE_FILE) ? fontInfo->GlyphResource.File : "<no file>",
			    (ESD_RESOURCE_IS_FLASH(fontInfo->FontResource.Type) ? (int)fontInfo->FontResource.FlashAddress : 0),
			    (ESD_RESOURCE_IS_FLASH(fontInfo->GlyphResource.Type) ? (int)fontInfo->GlyphResource.FlashAddress : 0));
#endif
#endif

			// Set the font
			fontInfo->BitmapHandle = handle;
#if (EVE_SUPPORT_CHIPID >= EVE_FT810)
			if (EVE_CHIPID >= EVE_FT810)
				EVE_CoCmd_setFont2(Esd_Host, handle, addr, fontInfo->FirstChar);
			else
#endif
				eve_assert_ex(false, "No support yet in ESD for custom fonts for FT800 target");

			eve_assert(handle < ESD_BITMAPHANDLE_CAP); // FIXME: Remove this, C6386 false positive
			Esd_CurrentContext->HandleState.Resized[handle] = 0;
			Esd_CurrentContext->HandleState.Page[handle] = 0;
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
			Esd_CurrentContext->HandleState.Flags[handle] = (ESD_BHFLAGS_SIZE_SET | ESD_BHFLAGS_PAGE_SET);
			Esd_CurrentContext->HandleState.Pending[handle] = phost->DlStateBase;
#endif
		}
	}

	if (ESD_BITMAPHANDLE_VALID(handle) && (handle != ESD_SCRATCHHANDLE)) // When valid and not using scratch handle
	{
		Esd_CurrentContext->HandleState.Use[handle] = 2; // In use
	}

	return handle;
}

ESD_CORE_EXPORT void Esd_CoDl_BitmapWidthHeight(uint8_t handle, uint16_t width, uint16_t height)
{
	EVE_HalContext *phost = Esd_GetHost();
	if (ESD_BITMAPHANDLE_VALID(handle))
	{
		EVE_CoDl_bitmapHandle(phost, handle);
		EVE_CoDl_bitmapSize(phost, NEAREST, BORDER, BORDER, width, height);
		Esd_CurrentContext->HandleState.Resized[handle] = true;
	}
}

ESD_CORE_EXPORT void Esd_CoDl_BitmapSize(uint8_t handle, uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height)
{
	EVE_HalContext *phost = Esd_GetHost();
	Esd_HandleState *handleState = &Esd_CurrentContext->HandleState;
	uint8_t flags;
	if (ESD_BITMAPHANDLE_VALID(handle))
	{
		EVE_CoDl_bitmapHandle(phost, handle);
		EVE_CoDl_bitmapSize(phost, filter, wrapx, wrapy, width, height);
		handleState->Resized[handle] = true;

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
		/* Track initial frame resize state for render target support */
		flags = handleState->Flags[handle];
		if (!(flags & ESD_BHFLAGS_SIZE_SET))
		{
			flags |= ESD_BHFLAGS_SIZE_SET;
			handleState->Flags[handle] = flags;
		}
#endif
	}
}

ESD_CORE_EXPORT void Esd_CoDl_BitmapSizeReset(uint8_t handle)
{
	uint8_t flags;
	Esd_HandleState *handleState = &Esd_CurrentContext->HandleState;
	if (handleState->Resized[handle])
	{
		Esd_BitmapInfo *bitmapInfo = (Esd_BitmapInfo *)handleState->Info[handle];
		Esd_CoDl_BitmapWidthHeight(handle, bitmapInfo->Width, bitmapInfo->Height);
		handleState->Resized[handle] = false;
	}
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	/* Track initial frame resize state for render target support */
	flags = handleState->Flags[handle];
	if (!(flags & ESD_BHFLAGS_SIZE_SET))
	{
		flags |= (ESD_BHFLAGS_SIZE_SET | ESD_BHFLAGS_EXPECTS_DEFAULT_SIZE);
		handleState->Flags[handle] = flags;
	}
#endif
}

/* end of file */
