/*
ESD Framework
Copyright (C) 2018  Bridgetek Pte Lte
Author: Jan Boon <jan.boon@kaetemi.be>
*/

#include "Esd_FontInfo.h"

#ifndef NDEBUG
#define ESD_FONTINFO_DEBUG
#endif

#ifdef ESD_FONTINFO_DEBUG
#define esd_resourceinfo_printf(fmt, ...) eve_printf(fmt, ##__VA_ARGS__)
#else
#define esd_resourceinfo_printf(fmt, ...) eve_noop()
#endif

extern ESD_CORE_EXPORT EVE_HalContext *Esd_Host;
extern ESD_CORE_EXPORT Esd_GpuAlloc *Esd_GAlloc;

static void rewriteGlyphAddr(EVE_HalContext *phost, Esd_FontInfo *fontInfo, uint32_t fontAddr, uint32_t glyphAddr)
{
	switch (fontInfo->Type)
	{
	case ESD_FONT_LEGACY: {
		fontInfo->FontHeight = EVE_Hal_rd32(phost, fontAddr + 140);
		EVE_Hal_wr32(phost, fontAddr + 144, glyphAddr);
		fontInfo->BitmapHandle = ESD_BITMAPHANDLE_INVALID;
		break;
	}
	case ESD_FONT_EXTENDED: {
		fontInfo->FontHeight = EVE_Hal_rd32(phost, fontAddr + 28);
		EVE_Hal_wr32(phost, fontAddr + 32, glyphAddr);
		fontInfo->BitmapHandle = ESD_BITMAPHANDLE_INVALID;
		break;
	}
	}
	if (!fontInfo->BaseLine)
		fontInfo->BaseLine = fontInfo->FontHeight;
	if (!fontInfo->CapsHeight)
		fontInfo->CapsHeight = fontInfo->FontHeight;
	fontInfo->GlyphAddress = glyphAddr;
}

ESD_CORE_EXPORT uint32_t Esd_LoadFont(Esd_FontInfo *fontInfo)
{
	if (fontInfo->Type != ESD_FONT_LEGACY && fontInfo->Type != ESD_FONT_EXTENDED)
	{
		return GA_INVALID;
	}
#if (EVE_SUPPORT_CHIPID >= EVE_BT820) // Load Font for EVE_BT82x
	
	uint32_t fontAddr = Esd_GpuAlloc_Get(Esd_GAlloc, fontInfo->FontResource.GpuHandle);
	bool isGlyphAddrValid = true;

	if (fontAddr == GA_INVALID)
	{
		fontAddr = Esd_LoadResource(&fontInfo->FontResource, NULL);
		isGlyphAddrValid = false;
	}

	if (fontAddr == GA_INVALID)
		return GA_INVALID;

	uint32_t fontBlockSize = (fontInfo->Type == ESD_FONT_LEGACY) ? 148 : EVE_Hal_rd32(Esd_Host, fontAddr + 4);
	uint32_t glyphAddr = fontAddr + fontBlockSize;
	if (isGlyphAddrValid)
		isGlyphAddrValid = (glyphAddr == fontInfo->GlyphAddress);

	if (!isGlyphAddrValid)
	{
		rewriteGlyphAddr(Esd_Host, fontInfo, fontAddr, glyphAddr);
	}

	return fontAddr;
#else // EVE_BT81x and earlier
	uint32_t glyphAddr;
	// Load glyphs
	glyphAddr = Esd_LoadResource(&fontInfo->GlyphResource, NULL);
	if (glyphAddr != GA_INVALID)
	{
		// Load map
		uint32_t fontAddr;
		bool isGlyphAddrValid = true;

#ifdef EVE_FLASH_AVAILABLE
		if (fontInfo->FontResource.Type == ESD_RESOURCE_DIRECTFLASH)
			fontInfo->FontResource.Type = ESD_RESOURCE_FLASH;
#endif
		fontAddr = Esd_GpuAlloc_Get(Esd_GAlloc, fontInfo->FontResource.GpuHandle);
		if (fontAddr == GA_INVALID)
		{
			fontAddr = Esd_LoadResource(&fontInfo->FontResource, NULL);
			isGlyphAddrValid = false;
		}
		else
		{
			isGlyphAddrValid = (glyphAddr == fontInfo->GlyphAddress);
		}

		if (fontAddr == GA_INVALID)
		{
			// Failed to load font block, unload glyphs
			esd_resourceinfo_printf("Failed to load font block, free glyphs\n");
			Esd_FreeResource(&fontInfo->GlyphResource);
			return GA_INVALID;
		}

		if (!isGlyphAddrValid)
		{
			// Rewrite glyph address in the font block
			rewriteGlyphAddr(Esd_Host, fontInfo, fontAddr, glyphAddr);
			uint32_t format;
			(void)format;
			format = EVE_Hal_rd32(Esd_Host, fontAddr + (fontInfo->Type == ESD_FONT_LEGACY) ? 128 : 8);
		
#ifdef EVE_FLASH_AVAILABLE
			// Special case for user convenience, handling user error
			if (fontInfo->GlyphResource.Type == ESD_RESOURCE_DIRECTFLASH && !ESD_IS_FORMAT_ASTC(format))
			{
				// In case the glyph resource was set to direct flash but is not in ASTC format, reload it
				esd_resourceinfo_printf("Glyph resource set to Direct Flash but is not in ASTC format, reload\n");
				fontInfo->GlyphResource.Type = ESD_RESOURCE_FLASH;
				glyphAddr = Esd_LoadResource(&fontInfo->GlyphResource, NULL);
				if (glyphAddr == GA_INVALID)
				{
					// Failed to reload glyph, free font block resource
					esd_resourceinfo_printf("Failed to reload glyph, free font block resource\n");
					Esd_FreeResource(&fontInfo->FontResource);
					return GA_INVALID;
				}
			}
#endif
		}

		return fontAddr;
	}
#endif // EVE_SUPPORT_CHIPID >= EVE_BT820
	return GA_INVALID;
}

ESD_CORE_EXPORT void Esd_FontPersist(Esd_FontInfo *fontInfo)
{
	Esd_LoadFont(fontInfo);
}

/* end of file */
