
#include "Esd_BitmapInfo.h"
#include "Esd_Context.h"

#include "Esd_GpuAlloc.h"

#ifndef NDEBUG
#define ESD_BITMAPINFO_DEBUG
#endif

extern ESD_CORE_EXPORT EVE_HalContext *Esd_Host;
extern ESD_CORE_EXPORT Esd_GpuAlloc *Esd_GAlloc;

#ifdef EVE_FLASH_AVAILABLE
#define ESD_BITMAPINFO_SUPPORT_DIRECT_FLASH(bitmapInfo) (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) && !bitmapInfo->Compressed && ESD_IS_FORMAT_ASTC(bitmapInfo->Format))
#endif

/*

Bitmap metadata format (little-endian):

0 int32_t width; // Width in pixels
4 int32_t height; // Height in pixels
8 int32_t stride; // Stride of the bitmap data
12 uint32_t format; // Format of the bitmap data
16 uint16_t paletteSize; // Uncompressed size of the palette file
18 uint8_t paletteFileExt[10]; // Filename suffix of the palette (Paletted)
28 uint8_t addtlResExt[12]; // Filename suffix of the additional bitmap info resource (DXT1)
40 uint16_t cells; // Number of cells usable by the user
42 uint16_t swizzle; // Swizzle, same format as BITMAP_SWIZZLE, plus one bit to enable the option

esdSignature must match "BMP"
version must be 1
size must match ESD_METADATA_END

*/

#define ESD_METADATA_WIDTH (ESD_METADATA_DATA + 0)
#define ESD_METADATA_HEIGHT (ESD_METADATA_DATA + 4)
#define ESD_METADATA_STRIDE (ESD_METADATA_DATA + 8)
#define ESD_METADATA_FORMAT (ESD_METADATA_DATA + 12)
#define ESD_METADATA_PALETTESIZE (ESD_METADATA_DATA + 16)
#define ESD_METADATA_PALETTEFILEEXT (ESD_METADATA_DATA + 18)
#define ESD_METADATA_PALETTEFILEEXT_MAX (10)
#define ESD_METADATA_ADDTLRESEXT (ESD_METADATA_DATA + 28)
#define ESD_METADATA_ADDTLRESEXT_MAX (12)
#define ESD_METADATA_CELLS (ESD_METADATA_DATA + 40)
#define ESD_METADATA_SWIZZLE (ESD_METADATA_DATA + 42)
#define ESD_METADATA_END (ESD_METADATA_DATA + 44)

static bool Esd_LoadFromFile_ex(uint32_t *imageFormat, bool deflate, uint32_t dst, const char *file, uint32_t options)
{
	return imageFormat
	    ? EVE_Util_loadImageFile_ex(Esd_Host, dst, file, imageFormat, options)
	    : (deflate
	            ? EVE_Util_loadInflateFile(Esd_Host, dst, file)
	            : EVE_Util_loadRawFile(Esd_Host, dst, file));
}

static bool Esd_LoadFromFile(uint32_t *imageFormat, bool deflate, uint32_t dst, const char *file)
{
	return Esd_LoadFromFile_ex(imageFormat, deflate, dst, file, OPT_NODL);
}

static bool Esd_LoadFromProgMem(uint32_t *imageFormat, bool deflate, uint32_t dst, eve_progmem_const uint8_t *data, uint32_t size)
{
	if (imageFormat)
		return EVE_CoCmd_loadImage_progMem(Esd_Host, dst, data, size, imageFormat);
	if (deflate)
		return EVE_CoCmd_inflate_progMem(Esd_Host, dst, data, size);
	EVE_Hal_wrProgMem(Esd_Host, dst, data, size);
	return true;
}

#ifdef EVE_FLASH_AVAILABLE

static bool Esd_LoadFromFlash(uint32_t *imageFormat, bool deflate, uint32_t dst, uint32_t src, uint32_t size)
{
	return imageFormat
	    ? EVE_CoCmd_loadImage_flash(Esd_Host, dst, src, imageFormat)
	    : (deflate
	            ? EVE_CoCmd_inflate_flash(Esd_Host, dst, src)
	            : EVE_CoCmd_flashRead_flush(Esd_Host, dst, src, size));
}

#ifdef ESD_LITTLEFS_FLASH

#if 0

static bool Esd_LoadFromFlashFS(uint32_t *imageFormat, bool deflate, uint32_t dst, const char *file)
{
	/* TODO: Load LittleFS file to RAM_G similar to SD */
	return false;
}

#endif

#ifdef ESD_LITTLEFS_FLASH
#define FA_NOTFOUND (-(LFS_ERR_NOENT))
#define FA_NOTRESERVED (-(LFS_ERR_INVAL))
#endif

int32_t Esd_LittleFs_LoadFlashAddress(int32_t *addr, const char *file, uint8_t *metadata)
{
	int err;
	lfs_file_t lfsFile;
	uint8_t buf[EVE_FLASH_WRITE_ALIGN];
	struct lfs_file_config fileConfig = { 0 };
	if (!file)
		return FA_INVALID; // No file name specified
	if (*addr >= EVE_FLASH_FIRMWARE_SIZE)
		return *addr; // Flash address already cached
	if (*addr != FA_INVALID)
		return FA_INVALID; // Flash file not found or not reserved
	if (!Esd_LittleFS_Mount())
		return FA_INVALID; // LittleFS cannot be mounted
	// eve_printf("[DIAGNOSTIC] Load flash address %s\n", file);
	fileConfig.buffer = buf;
	if (err = lfs_file_opencfg(&Esd_CurrentContext->Lfs, &lfsFile, file, LFS_O_RDONLY, &fileConfig))
	{
		if (err == LFS_ERR_NOENT)
		{
			*addr = FA_NOTFOUND;
			eve_printf_debug("Flash file doesn't exist '%s'\n", file);
		}
		else
			*addr = FA_INVALID;
		return FA_INVALID; // File doesn't exist
	}
	lfs_block_t head;
	if (err = lfs_file_reserved(&Esd_CurrentContext->Lfs, &lfsFile, &head))
	{
		if (err == LFS_ERR_INVAL)
		{
			eve_printf_debug("Flash file is not reserved, load as a normal file '%s'\n", file);
			*addr = FA_NOTRESERVED; // This is a regular file, cannot access directly
		}
		else
			*addr = FA_INVALID;
	}
	else
	{
		*addr = EVE_FLASH_FIRMWARE_SIZE + (head * Esd_CurrentContext->Lfs.cfg->block_size);
	}
	lfs_file_close(&Esd_CurrentContext->Lfs, &lfsFile);
	if (metadata)
		Esd_LittleFs_ReadMetadataFile(metadata, file);
	// if (*addr >= EVE_FLASH_FIRMWARE_SIZE)
	// 	eve_printf("[DIAGNOSTIC] Got flash address %i\n", (int)(*addr));
	return (*addr >= EVE_FLASH_FIRMWARE_SIZE) ? *addr : FA_INVALID; // Return address in flash
}

#endif

#endif

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
// Load from EVE SD card, only available on BT820 and later
static bool Esd_LoadFromEveSdCard(uint32_t *imageFormat, bool deflate, uint32_t address, char *filename, uint32_t size)
{
	EVE_HalContext *phost = Esd_GetHost();
	if (!Esd_EveSdCardReady())
	{
		eve_printf_debug("EVE_CoCmd_loadImage_sdCard %d %s: EVE SD card not ready\n", address, filename);
		return false;
	}
	if (imageFormat)
	{
		uint32_t fsSourceRes = EVE_CoCmd_fsSource(phost, filename);
		bool fsSourceSucceeds = Esd_CheckFsSourceRes(fsSourceRes, filename);
		if (!fsSourceSucceeds)
			return false;
		return EVE_CoCmd_loadImage_sdCard(Esd_Host, address, filename, imageFormat);
	}
	else if (deflate)
	{
		uint32_t fsSourceRes = EVE_CoCmd_fsSource(phost, filename);
		bool fsSourceSucceeds = Esd_CheckFsSourceRes(fsSourceRes, filename);
		if (!fsSourceSucceeds)
			return false;
		return EVE_CoCmd_inflate_sdCard(Esd_Host, address, filename);
	}
	else
	{
		uint32_t fsReadRes = EVE_CoCmd_fsRead(Esd_Host, address, filename);
		return Esd_CheckFsSourceRes(fsReadRes, filename);
	}
}
#endif //endif EVE_BT820

#ifdef EVE_SUPPORT_VIDEO

static bool Esd_LoadVideoFrameFromFile(uint32_t *imageFormat, uint32_t dst, const char *file)
{
	EVE_HalContext *phost = Esd_GetHost();
	if (phost->CmdFault)
		return false;

	if (!EVE_Hal_supportVideo(phost))
	{
		eve_assert_ex(false, "Esd_LoadVideoFrameFromFile is not available on the current graphics platform\n");
		return false;
	}

	if (!EVE_Util_sdCardReady(phost))
	{
		eve_printf_debug("SD card not ready\n");
		return false;
	}

	/* Allocate RAM_G space for FIFO and completion pointer */
	uint32_t fifoSize = 16 * 1024; /* TODO: What's an ideal FIFO size? */
	Esd_GpuHandle fifoHandle = Esd_GpuAlloc_Alloc(Esd_GAlloc, fifoSize + 4, 0);
	uint32_t fifoAddr = Esd_GpuAlloc_Get(Esd_GAlloc, fifoHandle);
	if (fifoAddr == GA_INVALID)
	{
		return false;
	}

	/* Setup media FIFO */
	if (!EVE_MediaFifo_set(phost, fifoAddr, fifoSize))
	{
		Esd_GpuAlloc_Free(Esd_GAlloc, fifoHandle);
		return false;
	}

	uint16_t bkpCmdDl;
	uint32_t bkpDl[10];

	if (EVE_CHIPID < EVE_BT815)
	{
		if (EVE_Cmd_waitFlush(phost))
		{
			/* Save */
			bkpCmdDl = EVE_Hal_rd16(phost, REG_CMD_DL);
			if (bkpCmdDl)
			{
				for (int i = 0; i < 10; ++i)
					bkpDl[i] = EVE_Hal_rd32(phost, RAM_DL + 4 * i);
			}
		}
		else
		{
			bkpCmdDl = 0;
		}
	}

	/* Load the first video frame */
	uint32_t transfered = 0;
	uint32_t ptr = fifoAddr + fifoSize;
	EVE_CoCmd_videoStart(phost);
	EVE_CoCmd_videoFrame(phost, dst, ptr);
	bool res = EVE_Util_loadMediaFile(phost, file, &transfered);
	EVE_Util_closeFile(phost);

	if (EVE_CHIPID < EVE_BT815)
	{
		if (bkpCmdDl && EVE_Cmd_waitFlush(phost))
		{
			/* Restore */
			EVE_Hal_wr16(phost, REG_CMD_DL, bkpCmdDl);
			for (int i = 0; i < 10; ++i)
				EVE_Hal_wr32(phost, RAM_DL + 4 * i, bkpDl[i]);
		}
	}

	/* Release */
	EVE_MediaFifo_close(phost);
	Esd_GpuAlloc_Free(Esd_GAlloc, fifoHandle);
	*imageFormat = RGB565;

	if (EVE_Cmd_rp(phost) != EVE_Cmd_wp(phost))
	{
		eve_printf_debug("Coprocessor did not complete command, force fault\n");
		EVE_Util_forceFault(phost, "ESD Core: CMD_VIDEOSTART and CMD_VIDEOFRAME aborted");
	}

	return res;
}

static bool Esd_LoadVideoFrameFromProgMem(uint32_t *imageFormat, uint32_t dst, eve_progmem_const uint8_t *data, uint32_t size)
{
	// TODO
	return false;
}

#ifdef EVE_FLASH_AVAILABLE

static bool Esd_LoadVideoFrameFromFlash(uint32_t *imageFormat, uint32_t dst, uint32_t src)
{
	EVE_HalContext *phost = Esd_GetHost();

	/* Allocate RAM_G space for completion pointer */
	Esd_GpuHandle ptrHandle = Esd_GpuAlloc_Alloc(Esd_GAlloc, 4, 0);
	uint32_t ptr = Esd_GpuAlloc_Get(Esd_GAlloc, ptrHandle);
	if (ptr == GA_INVALID)
	{
		return false;
	}

	/* Load the first video frame */
	EVE_CoCmd_flashSource(phost, src);
	EVE_CoCmd_videoStartF(phost);
	EVE_CoCmd_videoFrame(phost, dst, ptr);
	bool res = EVE_Cmd_waitFlush(phost);

	/* Release */
	Esd_GpuAlloc_Free(Esd_GAlloc, ptrHandle);
	*imageFormat = RGB565;
	return res;
}
#endif// End EVE_FLASH_AVAILABLE

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
static bool Esd_LoadVideoFrameFromEveSdCard(uint32_t *imageFormat, uint32_t dst, const char *file)
{
	EVE_HalContext *phost = Esd_GetHost();
	if (phost->CmdFault)
		return false;

	if (!EVE_Hal_supportVideo(phost))
	{
		eve_assert_ex(false, "Esd_LoadVideoFrameFromFile is not available on the current graphics platform\n");
		return false;
	}

	if (!Esd_EveSdCardReady())
	{
		eve_printf_debug("EVE SD card not ready\n");
		return false;
	}

	uint32_t fsSourceRes = EVE_CoCmd_fsSource(phost, file);
	bool fsSourceSucceeds = Esd_CheckFsSourceRes(fsSourceRes, file);
	if (!fsSourceSucceeds)
		return false;

	/* Allocate RAM_G space for FIFO and completion pointer */
	uint32_t fifoSize = 16 * 1024; /* TODO: What's an ideal FIFO size? */
	Esd_GpuHandle fifoHandle = Esd_GpuAlloc_Alloc(Esd_GAlloc, fifoSize + 4, 0);
	uint32_t fifoAddr = Esd_GpuAlloc_Get(Esd_GAlloc, fifoHandle);
	if (fifoAddr == GA_INVALID)
	{
		return false;
	}

	/* Setup media FIFO */
	if (!EVE_MediaFifo_set(phost, fifoAddr, fifoSize))
	{
		Esd_GpuAlloc_Free(Esd_GAlloc, fifoHandle);
		return false;
	}

	uint16_t bkpCmdDl;
	uint32_t bkpDl[10];

	if (EVE_CHIPID < EVE_BT815)
	{
		if (EVE_Cmd_waitFlush(phost))
		{
			/* Save */
			bkpCmdDl = EVE_Hal_rd16(phost, REG_CMD_DL);
			if (bkpCmdDl)
			{
				for (int i = 0; i < 10; ++i)
					bkpDl[i] = EVE_Hal_rd32(phost, RAM_DL + 4 * i);
			}
		}
		else
		{
			bkpCmdDl = 0;
		}
	}

	/* Load the first video frame */
	uint32_t transfered = 0;
	uint32_t ptr = fifoAddr + fifoSize;
	EVE_CoCmd_videoStart_ex(phost, OPT_FS | OPT_MEDIAFIFO);
	EVE_CoCmd_videoFrame(phost, dst, ptr);
	bool res = EVE_Util_loadMediaFile(phost, file, &transfered);
	EVE_Util_closeFile(phost);

	if (EVE_CHIPID < EVE_BT815)
	{
		if (bkpCmdDl && EVE_Cmd_waitFlush(phost))
		{
			/* Restore */
			EVE_Hal_wr16(phost, REG_CMD_DL, bkpCmdDl);
			for (int i = 0; i < 10; ++i)
				EVE_Hal_wr32(phost, RAM_DL + 4 * i, bkpDl[i]);
		}
	}

	/* Release */
	EVE_MediaFifo_close(phost);
	Esd_GpuAlloc_Free(Esd_GAlloc, fifoHandle);
	*imageFormat = RGB565;

	if (EVE_Cmd_rp(phost) != EVE_Cmd_wp(phost))
	{
		eve_printf_debug("Coprocessor did not complete command, force fault\n");
		EVE_Util_forceFault(phost, "ESD Core: CMD_VIDEOSTART and CMD_VIDEOFRAME aborted");
	}

	return res;
}
#endif// End EVE_SUPPORT_CHIPID >= EVE_BT820

#endif// End EVE_SUPPORT_VIDEO

ESD_CORE_EXPORT void Esd_UnloadBitmap(Esd_BitmapInfo *bitmapInfo)
{
	// Forces to drop any cached address (LFS)
}

ESD_CORE_EXPORT bool Esd_LoadBitmapMetadata(Esd_BitmapInfo *bitmapInfo, uint8_t *metadata)
{
	if (metadata && metadata[ESD_METADATA_SIGNATURE])
	{
		uint32_t esdSignature = ESD_RD32_LE(metadata, ESD_METADATA_SIGNATURE);
		if (esdSignature != ESD_RESOURCE_BMP)
		{
#ifdef ESD_BITMAPINFO_DEBUG
			eve_printf_debug("Bitmap has an invalid metadata signature (0x%x)\n", (int)esdSignature);
#endif
			return false;
		}

		// Update compression and extracted size from metadata
		uint8_t compression = metadata[ESD_METADATA_COMPRESSION];
		ESD_METADATA_SET(bool, bitmapInfo, Compressed, compression == ESD_RESOURCE_DEFLATE, bitmapInfo->File);
		ESD_METADATA_SET(int32_t, bitmapInfo, Size, ESD_RD32I_LE(metadata, ESD_METADATA_RAWSIZE), bitmapInfo->File);

		// Update bitmap specific options from metadata
		ESD_METADATA_SET(int32_t, bitmapInfo, Width, ESD_RD32I_LE(metadata, ESD_METADATA_WIDTH), bitmapInfo->File);
		ESD_METADATA_SET(int32_t, bitmapInfo, Height, ESD_RD32I_LE(metadata, ESD_METADATA_HEIGHT), bitmapInfo->File);
		ESD_METADATA_SET(int32_t, bitmapInfo, Stride, ESD_RD32I_LE(metadata, ESD_METADATA_STRIDE), bitmapInfo->File);
		ESD_METADATA_SET_EX(uint32_t, bitmapInfo, Format, ESD_RD32_LE(metadata, ESD_METADATA_FORMAT), bitmapInfo->File, bitmapInfo->CoLoad = false; bitmapInfo->Video = false);
		ESD_METADATA_SET(int16_t, bitmapInfo, Cells, ESD_RD16_LE(metadata, ESD_METADATA_CELLS), bitmapInfo->File);
#if (EVE_SUPPORT_CHIPID >= EVE_BT815)
		uint16_t swizzle = ESD_RD16_LE(metadata, ESD_METADATA_SWIZZLE);
		bool hasSwizzle = swizzle & (1 << 12);
		if (hasSwizzle)
		{
			ESD_METADATA_SET(uint16_t, bitmapInfo, SwizzleB, (swizzle >> 9) & 7, bitmapInfo->File);
			ESD_METADATA_SET(uint16_t, bitmapInfo, SwizzleG, (swizzle >> 6) & 7, bitmapInfo->File);
			ESD_METADATA_SET(uint16_t, bitmapInfo, SwizzleR, (swizzle >> 3) & 7, bitmapInfo->File);
			ESD_METADATA_SET(uint16_t, bitmapInfo, SwizzleA, swizzle & 7, bitmapInfo->File);
		}
		ESD_METADATA_SET(bool, bitmapInfo, Swizzle, hasSwizzle, bitmapInfo->File);
#endif

#ifdef ESD_BITMAPINFO_DEBUG
		// eve_printf_debug("Bitmap info metadata reloaded\n");
#endif
	}

	// Okay if not loaded or if signature didn't fail
	return true;
}

#ifdef ESD_LITTLEFS_FLASH
ESD_CORE_EXPORT void Esd_BitmapInfo_LoadFlashAddress(Esd_BitmapInfo *bitmapInfo)
{
	uint8_t metadata[ESD_METADATA_MAX];
	metadata[ESD_METADATA_SIGNATURE] = '\0';
	Esd_LittleFs_LoadFlashAddress(&bitmapInfo->FlashAddress, bitmapInfo->File, metadata);
	Esd_LoadBitmapMetadata(bitmapInfo, metadata);
	Esd_LittleFs_LoadFlashAddress(&bitmapInfo->PaletteFlashAddress, bitmapInfo->PaletteFile, NULL);
}
#endif

ESD_CORE_EXPORT uint32_t Esd_LoadBitmapEx(Esd_BitmapInfo *bitmapInfo, uint8_t *metadata)
{
	EVE_HalContext *phost = Esd_GetHost();
	int32_t flashAddr = FA_INVALID;
	uint32_t addr;
	(void)phost;

	if (metadata)
		metadata[ESD_METADATA_SIGNATURE] = '\0';

	if (!bitmapInfo)
	{
#ifdef ESD_BITMAPINFO_DEBUG
		eve_printf_debug("Bitmap info is NULL\n");
#endif
		return GA_INVALID;
	}

#ifdef ESD_COMPATIBILITY_FLASHPREFERRAM
	// Compatibility routine
	if (bitmapInfo->Flash)
	{
		if (bitmapInfo->PreferRam)
		{
			bitmapInfo->Type = ESD_RESOURCE_DIRECTFLASH;
			bitmapInfo->PreferRam = false;
		}
		else
		{
			bitmapInfo->Type = ESD_RESOURCE_FLASH;
		}
		bitmapInfo->Flash = false;
	}
#endif

	// If PreferRam is set, skip this and try to load to RAM_G first
#ifdef EVE_FLASH_AVAILABLE
	if (!ESD_RESOURCE_IS_PREFERRAM(bitmapInfo->Type) && ESD_BITMAPINFO_SUPPORT_DIRECT_FLASH(bitmapInfo))
	{
		// Just get the flash address if that's what we want

		flashAddr = Esd_LittleFs_LoadFlashAddress(&bitmapInfo->FlashAddress, bitmapInfo->File, metadata);
		if (!Esd_LoadBitmapMetadata(bitmapInfo, metadata))
			return GA_INVALID;
		// Ensure any new metadata still supports direct flash
		if (!bitmapInfo->Compressed && ESD_IS_FORMAT_ASTC(bitmapInfo->Format))
		{
			if (flashAddr != FA_INVALID)
				return ESD_DL_FLASH_ADDRESS(flashAddr);
			return GA_INVALID;
		}
	}
#else
	if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type))
	{
#ifdef ESD_BITMAPINFO_DEBUG
		eve_printf_debug("Flash storage not supported on this platform\n");
#endif
		return GA_INVALID;
	}
#endif

	// Get address of specified handle
	// eve_printf_debug("%i: %i\n", bitmapInfo->GpuHandle.Id, bitmapInfo->GpuHandle.Seq);
	addr = Esd_GpuAlloc_Get(Esd_GAlloc, bitmapInfo->GpuHandle);
	// eve_printf("Esd_LoadBitmapEx bitmapInfo->Type %d\n", bitmapInfo->Type);
	if (addr == GA_INVALID)
	{
		// Load bitmap Flash or RAM_G
		if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type))
		{
			flashAddr = Esd_LittleFs_LoadFlashAddress(&bitmapInfo->FlashAddress, bitmapInfo->File, metadata);
			if (flashAddr == FA_INVALID)
			{
				/* TODO: This could be a regular LittleFS file, load to RAM_G similar to SD */
			}
		}
		if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? (flashAddr == FA_INVALID) : !bitmapInfo->File) // File is union with ProgMem
		{
#ifdef ESD_BITMAPINFO_DEBUG
			/*
			if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type))
			    eve_printf_debug("Bitmap flash address is not found, cannot auto-load\n");
			else if (bitmapInfo->Type == ESD_RESOURCE_PROGMEM)
			    eve_printf_debug("Bitmap info progmem is NULL, cannot auto-load\n");
			else
			    eve_printf_debug("Bitmap info file name is NULL, cannot auto-load\n");
			*/
#endif
			return GA_INVALID;
		}

		// Load metadata from file on SD card
		if (metadata && (bitmapInfo->Type == ESD_RESOURCE_FILE || bitmapInfo->Type == ESD_RESOURCE_SD) && bitmapInfo->File)
		{
			// Generate metafile name
			size_t nameLen = strlen(bitmapInfo->File);
			if (nameLen + 6 > 128) // ".esdm" NUL
				return GA_INVALID;
			char metaFile[128];
			strcpy_s(metaFile, sizeof(metaFile), bitmapInfo->File);
			strcpy_s(&metaFile[nameLen], sizeof(metaFile) - nameLen, ".esdm");

			// Try to load
			EVE_Util_readFile(phost, metadata, ESD_METADATA_MAX, metaFile);
		}

#ifdef ESD_BITMAPINFO_DEBUG
#ifndef ESD_LITTLEFS_FLASH
		if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type))
			eve_printf_debug("[DEVELOPER] Loading flash addr %i\n", bitmapInfo->FlashAddress);
		else
#endif
		    if (bitmapInfo->Type == ESD_RESOURCE_PROGMEM)
			eve_printf_debug("[DEVELOPER] Loading progmem\n");
		else
			eve_printf_debug("[DEVELOPER] Loading '%s'\n", bitmapInfo->File);
#endif

		// Load bitmap metadata
		Esd_LoadBitmapMetadata(bitmapInfo, metadata);

		// Not loaded, load this bitmap
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
		// 32-byte aligned for fsread command
		uint16_t aligned = (ESD_RESOURCE_IS_EVE_SDCARD(bitmapInfo->Type) && bitmapInfo->Compressed == ESD_RESOURCE_RAW) ? 32 : 0;
		bitmapInfo->GpuHandle = Esd_GpuAlloc_Alloc_ex(Esd_GAlloc, bitmapInfo->Size,
		    (bitmapInfo->Persistent ? 0 : GA_GC_FLAG) | ((ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) && ESD_RESOURCE_IS_PREFERRAM(bitmapInfo->Type)) ? GA_LOW_FLAG : 0), aligned);
#else
		bitmapInfo->GpuHandle = Esd_GpuAlloc_Alloc(Esd_GAlloc, bitmapInfo->Size,
		    (bitmapInfo->Persistent ? 0 : GA_GC_FLAG) | ((ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) && ESD_RESOURCE_IS_PREFERRAM(bitmapInfo->Type)) ? GA_LOW_FLAG : 0));
#endif
		addr = Esd_GpuAlloc_Get(Esd_GAlloc, bitmapInfo->GpuHandle);
		if (addr != GA_INVALID)
		{
			bool coLoad = bitmapInfo->CoLoad || bitmapInfo->Format == JPEG || bitmapInfo->Format == PNG || bitmapInfo->Format == AVI;
			bitmapInfo->CoLoad = coLoad;

			bool video = bitmapInfo->Video || bitmapInfo->Format == AVI;
			bitmapInfo->Video = video;

			if (video && !EVE_Hal_supportVideo(phost))
			{
#ifdef ESD_BITMAPINFO_DEBUG
				eve_printf_debug_once("Video is not supported\n");
#endif
				/* Video not supported */
				/* TODO: We *could* do a software AVI parse, and submit it as JPG */
				Esd_GpuAlloc_Free(Esd_GAlloc, bitmapInfo->GpuHandle);
				return GA_INVALID;
			}

#ifdef ESD_BITMAPINFO_DEBUG
			eve_printf_debug("Allocated space for bitmap\n");
#endif

			// Allocation space OK
			bool loadSuccess = true;

#if defined(EVE_SUPPORT_VIDEO)
			if (video)
			{
#ifdef EVE_FLASH_AVAILABLE
				if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type))
				{
					loadSuccess = Esd_LoadVideoFrameFromFlash(&bitmapInfo->Format, addr, flashAddr);
				}
				else
#endif
				switch (bitmapInfo->Type)
				{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
				case ESD_RESOURCE_SD:
					loadSuccess = Esd_LoadVideoFrameFromEveSdCard(&bitmapInfo->Format, addr, bitmapInfo->File);
					break;
#endif
				case ESD_RESOURCE_PROGMEM:
					loadSuccess = Esd_LoadVideoFrameFromProgMem(&bitmapInfo->Format, addr, bitmapInfo->ProgMem, bitmapInfo->StorageSize << 2);
					break;
				default:
					loadSuccess = Esd_LoadVideoFrameFromFile(&bitmapInfo->Format, addr, bitmapInfo->File);
					break;
				}
			}
			else
#endif
			{
				Esd_BitmapInfo *format = coLoad ? &bitmapInfo->Format : NULL;
				bool isDeflate = (bitmapInfo->Compressed == ESD_RESOURCE_DEFLATE);

#ifdef EVE_FLASH_AVAILABLE
				if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type))
				{
					loadSuccess = Esd_LoadFromFlash(format, isDeflate, addr, flashAddr, bitmapInfo->Size);
				}
				else
#endif
				switch (bitmapInfo->Type)
				{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
				case ESD_RESOURCE_SD:
					loadSuccess = Esd_LoadFromEveSdCard(format, isDeflate, addr, bitmapInfo->File, bitmapInfo->Size);
					break;
#endif
				case ESD_RESOURCE_PROGMEM:
					loadSuccess = Esd_LoadFromProgMem(format, isDeflate, addr, bitmapInfo->ProgMem, bitmapInfo->StorageSize << 2);
					break;
				default:
					loadSuccess = Esd_LoadFromFile(format, isDeflate, addr, bitmapInfo->File);
					break;
				}
			}

			if (!loadSuccess)
			{
#ifdef ESD_BITMAPINFO_DEBUG
				eve_printf_debug(ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? "Failed to load bitmap from flash\n" : "Failed to load bitmap from file\n");
#endif
				// Failed to load
				Esd_GpuAlloc_Free(Esd_GAlloc, bitmapInfo->GpuHandle);
				addr = GA_INVALID;
			}

			if (coLoad && (bitmapInfo->Format == PALETTED565 || bitmapInfo->Format == PALETTED4444 || bitmapInfo->Format == L8))
			{
				// Truncate co loaded PNG that has paletted or L8 format
				uint32_t truncateSize = (bitmapInfo->Format != L8) ? 512 : 0;
				truncateSize += (bitmapInfo->Stride * bitmapInfo->Height);
				Esd_GpuAlloc_Truncate(Esd_GAlloc, bitmapInfo->GpuHandle, truncateSize);
			}

#ifdef ESD_COMPATIBILITY_ADDITIONALFILE
			int32_t additionalFlashAddr = ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? Esd_LittleFs_LoadFlashAddress(&bitmapInfo->AdditionalFlashAddress, bitmapInfo->AdditionalFile, NULL) : FA_INVALID;
			// Special load for DXT1, additional file loaded halfway the address space
			if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? (additionalFlashAddr != FA_INVALID) : (!!bitmapInfo->AdditionalFile))
			{
				if (
#ifdef EVE_FLASH_AVAILABLE
				    ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? !Esd_LoadFromFlash(coLoad ? &bitmapInfo->Format : NULL, bitmapInfo->Compressed == ESD_RESOURCE_DEFLATE, addr + (bitmapInfo->Size >> 1), additionalFlashAddr, bitmapInfo->Size) :
#endif
				                                            !Esd_LoadFromFile_ex(coLoad ? &bitmapInfo->Format : NULL, bitmapInfo->Compressed == ESD_RESOURCE_DEFLATE, addr + (bitmapInfo->Size >> 1), bitmapInfo->AdditionalFile, opt))
				{
#ifdef ESD_BITMAPINFO_DEBUG
					eve_printf_debug(ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? "Failed to load additional bitmap from flash\n" : "Failed to load additional bitmap from file\n");
#endif
					// Failed to load from additional file
					// Esd_GpuAlloc_Free(Esd_GAlloc, bitmapInfo->GpuHandle);
					addr = GA_INVALID;
				}
			}
#endif
		}
		else
		{
#ifdef ESD_BITMAPINFO_DEBUG
			eve_printf_debug("Unable to allocate space for bitmap\n");
#endif
		}
	}

#ifdef EVE_FLASH_AVAILABLE
	if (addr == GA_INVALID && ESD_BITMAPINFO_SUPPORT_DIRECT_FLASH(bitmapInfo))
	{
		// Just get the flash address anyway if we failed to load (in case of PreferRam)
		if (flashAddr != FA_INVALID)
			return ESD_DL_FLASH_ADDRESS(flashAddr);
		return GA_INVALID;
	}
#endif

	if (addr != GA_INVALID)
	{
		addr = ESD_DL_RAM_G_ADDRESS(addr);
	}

	return addr;
}

ESD_CORE_EXPORT uint32_t Esd_LoadBitmap(Esd_BitmapInfo *bitmapInfo)
{
	uint8_t metadata[ESD_METADATA_MAX];
	return Esd_LoadBitmapEx(bitmapInfo, metadata);
}

ESD_CORE_EXPORT uint32_t Esd_LoadPalette(Esd_BitmapInfo *bitmapInfo)
{
	// eve_printf("Esd_LoadPalette %d\n", bitmapInfo->Type);
	EVE_HalContext *phost = Esd_GetHost();
	uint32_t addr;
	(void)phost;

	if (!bitmapInfo)
	{
#ifdef ESD_BITMAPINFO_DEBUG
		eve_printf_debug("Bitmap info is NULL\n");
#endif
		return GA_INVALID;
	}

	if (!ESD_IS_FORMAT_PALETTED(bitmapInfo->Format))
	{
		// Not paletted
		return GA_INVALID;
	}

#ifndef EVE_FLASH_AVAILABLE
	if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type))
	{
#ifdef ESD_BITMAPINFO_DEBUG
		// eve_printf_debug("Flash storage not supported on this platform\n");
#endif
		return GA_INVALID;
	}
#endif

	if (EVE_CHIPID >= EVE_FT810)
	{
		// Don't load if no palette file
#ifndef ESD_LITTLEFS_FLASH
		if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type))
		{
			if (bitmapInfo->PaletteFlashAddress < EVE_FLASH_FIRMWARE_SIZE)
				return GA_INVALID;
		}
		else
#endif
		if (!bitmapInfo->PaletteFile)
		{
			return GA_INVALID;
		}

		// Get palette address of specified handle
		addr = Esd_GpuAlloc_Get(Esd_GAlloc, bitmapInfo->PaletteGpuHandle);
		if (addr == GA_INVALID)
		{
			static const uint32_t sizes[PALETTED8 - PALETTED565 + 1] = { 256 * 2, 256 * 2, 256 * 4 }; // Assume palettes are full-sized, fair assumption for now
			uint32_t size = sizes[bitmapInfo->Format - PALETTED565];
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
			if(bitmapInfo->Format == PALETTEDARGB8)
			{
				size = 256 * 4;
			}
#endif
			// Verify if there is a file name or flash address specified, or if the flash address can be found in the flash filesystem
			int32_t paletteFlashAddr = ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? Esd_LittleFs_LoadFlashAddress(&bitmapInfo->PaletteFlashAddress, bitmapInfo->PaletteFile, NULL) : FA_INVALID;
			if (ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? (paletteFlashAddr == FA_INVALID) : (!bitmapInfo->PaletteFile)) // PaletteFile is union with PaletteProgMem
			{
#ifdef ESD_BITMAPINFO_DEBUG
				// eve_printf_debug("Bitmap info palette is NULL, cannot auto-load\n");
#endif
				return GA_INVALID;
			}

			// Not loaded, load this bitmap palette
			bitmapInfo->PaletteGpuHandle = Esd_GpuAlloc_Alloc(Esd_GAlloc, size, bitmapInfo->Persistent ? 0 : GA_GC_FLAG);
			addr = Esd_GpuAlloc_Get(Esd_GAlloc, bitmapInfo->PaletteGpuHandle);
			if (addr != GA_INVALID)
			{
#ifdef ESD_BITMAPINFO_DEBUG
				eve_printf_debug("Allocated space for bitmap palette\n");
#endif
				// Allocation space OK
				if (bitmapInfo->Type == ESD_RESOURCE_PROGMEM)
				{
					EVE_Hal_wrProgMem(phost, addr, bitmapInfo->PaletteProgMem, size); // Always succeeds
				}
				else if (
#ifdef EVE_FLASH_AVAILABLE
				    ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? !EVE_CoCmd_flashRead_flush(Esd_Host, addr, paletteFlashAddr, size) :
#endif
				                                            !EVE_Util_loadRawFile(Esd_Host, addr, bitmapInfo->PaletteFile))
				{
#ifdef ESD_BITMAPINFO_DEBUG
					eve_printf_debug(ESD_RESOURCE_IS_FLASH(bitmapInfo->Type) ? "Failed to load palette from flash\n" : "Failed to load palette from file\n");
#endif
					// Failed to load from file
					Esd_GpuAlloc_Free(Esd_GAlloc, bitmapInfo->PaletteGpuHandle);
					addr = GA_INVALID;
				}
			}
			else
			{
#ifdef ESD_BITMAPINFO_DEBUG
				eve_printf_debug("Unable to allocate space for bitmap palette\n");
#endif
			}
		}
	}
	else // FT_80X_ENABLE
	{
		eve_printf_debug("Palette not yet implemented for FT80x\n");
		addr = GA_INVALID;
	}

	if (addr != GA_INVALID)
	{
		addr = ESD_DL_RAM_G_ADDRESS(addr);
	}

	return addr;
}

ESD_CORE_EXPORT void Esd_BitmapCell_Persist(Esd_BitmapCell bitmapCell)
{
	if (bitmapCell.Info)
	{
		Esd_LoadBitmap(bitmapCell.Info);
		Esd_LoadPalette(bitmapCell.Info);
	}
}

/* TODO: Review/test this */
ESD_CORE_EXPORT void Esd_BitmapInfo_FromCoCmd(Esd_BitmapInfo *bitmapInfo)
{
	EVE_HalContext *phost = Esd_GetHost();
	Esd_GpuAlloc *ga = Esd_GAlloc;

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		/* Update video properties at runtime */
		uint32_t source, fmt, w, h, palette;
		uint32_t stride, size;
		EVE_CoCmd_getImage(phost, &source, &fmt, &w, &h, &palette);
		bitmapInfo->Width = w;
		bitmapInfo->Height = h;
		bitmapInfo->Format = fmt; // RGB565, YCBCR, ARGB8 or L8
		switch (fmt)
		{
		case RGB565:
			stride = w * 2;
			size = w * h * 2;
			break;
		case YCBCR:
		case L8:
			stride = w;
			size = w * h;
			break;
		case ARGB8:
			stride = w * 4;
			size = w * h * 4;
			break;
		default:
			size = bitmapInfo->Size;
			stride = bitmapInfo->Stride;
			break;
		}
		if (size != bitmapInfo->Size && Esd_GpuAlloc_Get(ga, bitmapInfo->GpuHandle) != GA_INVALID)
		{
			if (size > bitmapInfo->Size)
				Esd_GpuAlloc_Free(ga, bitmapInfo->GpuHandle);
			else
				Esd_GpuAlloc_Truncate(ga, bitmapInfo->GpuHandle, size);
		}
		bitmapInfo->Size = size;
		bitmapInfo->Stride = stride;
	}
#endif
}
/* end of file */
