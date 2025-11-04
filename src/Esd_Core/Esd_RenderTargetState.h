/*
EVE Screen Designer Core Library
Copyright (C) 2025  Bridgetek Pte Lte
Author: Jan Boon <jan.boon@kaetemi.be>
*/

#ifndef ESD_RENDERTARGETSTATE__H
#define ESD_RENDERTARGETSTATE__H

#include "Esd_Base.h"
#include "Esd_GpuAlloc.h"
#include "Esd_BitmapInfo.h"
#include "Esd_Math.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file Esd_RenderTargetState.h
 * @brief Render target management for EVE Screen Designer
 *
 * This module provides functionality to render content to off-screen bitmaps
 * (render targets) on BT820 EVE chips. Render targets allow you to:
 * - Pre-render complex content for better performance
 * - Create reusable graphical elements
 * - Apply effects to groups of widgets
 * - Exceed display list size limits
 *
 * @note Requires EVE BT820 chip (EVE_CHIPID >= EVE_BT820)
 *
 * Usage Guide:
 * 1. Create an Esd_BitmapInfo structure with desired format and dimensions
 * 2. Declare an Esd_RenderTargetState to store backup information
 * 3. Call Esd_RenderTarget_DlStart() to begin rendering to the target
 * 4. Issue drawing commands as normal
 * 5. Call Esd_RenderTarget_DisplaySwap() to complete the render target
 * 6. Use the resulting bitmap with standard bitmap drawing functions
 *
 * Example:
 * ```c
 * // Persistent state
 * Esd_BitmapInfo rtInfo = {
 *     .Format = RGB565,
 *     .Width = 256,
 *     .Height = 128,
 *     ESD_BITMAP_DEFAULTS
 * };
 *
 * // Temporary state
 * Esd_RenderTargetState rtState;
 *
 * // Start rendering to target
 * uint32_t addr = Esd_RenderTarget_DlStart(&rtInfo, &rtState, 0, 0);
 * if (addr != GA_INVALID)
 * {
 *     // Clear and draw content
 *     EVE_CoDl_clear(phost, 1, 1, 1);
 *     EVE_CoCmd_button(phost, 10, 10, 100, 50, 27, 0, "Hello");
 *
 *     // Complete render target
 *     Esd_RenderTarget_DisplaySwap(&rtInfo, &rtState);
 * }
 *
 * // Use the render target as a bitmap
 * Esd_BitmapCell cell = { .Cell = 0, .Info = &rtInfo };
 * Esd_Render_Bitmap(x, y, cell, 0xFFFFFFFF);
 * ```
 */

/**
 * @brief Temporary state for backing up and restoring the rendering context
 *
 * This structure backs up the rendering state before switching to a
 * render target, allowing the system to resume the previous state after
 * render target operations are complete.
 */
ESD_TYPE(Esd_RenderTargetState, Native = Struct)
typedef struct Esd_RenderTargetState
{
	/* Previous render target configuration */
	uint32_t Destination; /**< Previous render target destination address */
	uint16_t Format; /**< Previous render target pixel format */
	uint16_t Width; /**< Previous render target width in pixels */
	uint16_t Height; /**< Previous render target height in pixels */

	/* Display list state */
	uint8_t DlStateBase; /**< Display list state stack base index */
	uint8_t DlStateIndex; /**< Current display list state index */
	uint8_t DlPrimitive; /**< Current primitive being drawn */

	/* Bitmap handle states for all 64 possible handles */
	bool HandleResized[ESD_BITMAPHANDLE_CAP]; /**< Bitmap handle resize flags */
	uint8_t HandlePage[ESD_BITMAPHANDLE_CAP]; /**< Current page for each handle */
	uint8_t HandleFlags[ESD_BITMAPHANDLE_CAP]; /**< State flags for each handle */
	uint8_t HandleInitialPage[ESD_BITMAPHANDLE_CAP]; /**< Initial page for each handle */

	/* Touch transform state */
	int32_t TouchTransformC; /**< Touch transform C coefficient (X offset) */
	int32_t TouchTransformF; /**< Touch transform F coefficient (Y offset) */

} Esd_RenderTargetState;
ESD_TYPE(Esd_RenderTargetState *, Native = Pointer, Edit = Library)

/**
 * @brief Calculate the total memory size required for a render target
 *
 * @param format Pixel format (L8, RGB565, ARGB8, etc.)
 * @param width Width in pixels
 * @param height Height in pixels
 * @return Total size in bytes, or 0 for unknown formats
 *
 * @note The actual allocation will be larger to include state backup
 */
static inline uint32_t Esd_RenderTarget_Size(uint16_t format, uint32_t width, uint32_t height)
{
	switch (format)
	{
	case L8:
		// 1 byte per pixel
		return width * height;

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	case LA8:
#endif
	case RGB565:
	case ARGB4:
	case ARGB1555:
		// 2 bytes per pixel
		return width * height * 2;

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	case RGB8:
	case ARGB6:
		// 3 bytes per pixel
		return width * height * 3;

	case ARGB8:
		// 4 bytes per pixel
		return width * height * 4;

	case RGB6: {
		// 9 bytes per 4 pixels horizontally
		uint32_t blocksPerLine = (width + 3) / 4; // round up to cover all pixels
		uint32_t byteStride = blocksPerLine * 9;
		return byteStride * height;
	}

	case YCBCR: {
		// 4 pixels in a 2x2 block → 4 bytes per block
		// So width * height / 4 bytes
		// But only one write per 2 lines, so stride = width, lines = height / 2
		uint32_t lines = (height + 1) / 2;
		return width * lines;
	}
#endif

	default:
		// Unknown format
		return 0;
	}
}

/**
 * @brief Calculate the stride (bytes per line) for a render target
 *
 * @param format Pixel format
 * @param width Width in pixels
 * @return Stride in bytes, or 0 for unknown formats
 */
static inline uint32_t Esd_RenderTarget_Stride(uint16_t format, uint32_t width)
{
	switch (format)
	{
	case L8:
		// 1 byte per pixel
		return width;

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	case LA8:
#endif
	case RGB565:
	case ARGB4:
	case ARGB1555:
		// 2 bytes per pixel
		return width * 2;

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	case RGB8:
	case ARGB6:
		// 3 bytes per pixel
		return width * 3;

	case ARGB8:
		// 4 bytes per pixel
		return width * 4;

	case RGB6: {
		// 9 bytes per 4 pixels, rounded up
		uint32_t blocksPerLine = (width + 3) / 4;
		return blocksPerLine * 9;
	}

	case YCBCR:
		// 1 byte per pixel in width, but only written every 2 lines
		return width;
#endif

	default:
		// Unknown format
		return 0;
	}
}

/**
 * @brief Begin new render target. Returns address of bitmap, or GA_INVALID
 *
 * This function sets up a render target for off-screen rendering. It backs up
 * the current display list and rendering state, allocates memory for the render
 * target if necessary, and switches the rendering context to draw into the new target.
 *
 * This function does not wait for the coprocessor to finish executing all tasks.
 *
 * @param info Bitmap info structure that will be configured for the render target.
 *             The GpuHandle, StorageSize, and Size fields will be updated.
 * @param state State structure to store the backed-up rendering context
 * @param offsetX X offset for touch transform adjustment (default: 0)
 * @param offsetY Y offset for touch transform adjustment (default: 0)
 * @return GPU memory address of the render target, or GA_INVALID if memory allocation failed
 *
 * @note Requires BT820 chip
 * @note Must be paired with Esd_RenderTarget_DisplaySwap()
 * @note Touch coordinates are automatically adjusted by the offset values
 */
ESD_FUNCTION(Esd_RenderTarget_DlStart, Type = bool, Attributes = ESD_CORE_EXPORT, Category = EveRenderFunctions, Buffered, Include = "Esd_RenderTargetState.h")
ESD_PARAMETER(info, Type = Esd_BitmapInfo *)
ESD_PARAMETER(state, Type = Esd_RenderTargetState *)
ESD_PARAMETER(offsetX, Type = int16_t, Default = 0)
ESD_PARAMETER(offsetY, Type = int16_t, Default = 0)
ESD_CORE_EXPORT uint32_t Esd_RenderTarget_DlStart(Esd_BitmapInfo *info, Esd_RenderTargetState *state, int16_t offsetX, int16_t offsetY);

/**
 * @brief Calls DISPLAY and CMD_SWAP, and restores the previous display list
 *
 * This function finalizes the render target by:
 * 1. Restoring bitmap handle states
 * 2. Swapping the display list (calls DISPLAY and CMD_SWAP)
 * 3. Starting a new display list
 * 4. Restoring the previous display list content
 * 5. Restoring the previous render target and touch transforms
 * 6. Reading the touch tag for the render target
 *
 * This function does not wait for the coprocessor to finish executing all tasks.
 *
 * @param info The same bitmap info used in Esd_RenderTarget_DlStart()
 * @param state The same state structure used in Esd_RenderTarget_DlStart()
 *
 * @note This function must be called after Esd_RenderTarget_DlStart()
 * @note The render target can be used as a normal bitmap after this call
 */
ESD_FUNCTION(Esd_RenderTarget_DisplaySwap, Attributes = ESD_CORE_EXPORT, Category = EveRenderFunctions, Include = "Esd_RenderTargetState.h")
ESD_PARAMETER(info, Type = Esd_BitmapInfo *)
ESD_PARAMETER(state, Type = Esd_RenderTargetState *)
ESD_CORE_EXPORT void Esd_RenderTarget_DisplaySwap(Esd_BitmapInfo *info, Esd_RenderTargetState *state);

/**
 * @brief Read the touch TAG of the render target from RAM_G
 *
 * Retrieves the touch tag that was active within the render target area.
 * This allows touch detection to work correctly for content rendered to
 * off-screen targets.
 *
 * The tag is retrieved from the RAM_G working area of the render target.
 *
 * @param info The bitmap info for the render target
 * @return Touch tag value (24-bit), or EVE_TAG_NONE if no touch
 *
 * @note Call this after the frame is complete (during the update cycle) to get accurate touch data
 * @note Only available on BT820 chips
 */
ESD_FUNCTION(Esd_RenderTarget_TouchTag, Type = eve_tag_t, Attributes = ESD_CORE_EXPORT, Category = EveRenderFunctions, Include = "Esd_RenderTargetState.h")
ESD_PARAMETER(info, Type = Esd_BitmapInfo *)
ESD_CORE_EXPORT eve_tag_t Esd_RenderTarget_TouchTag(Esd_BitmapInfo *info);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ESD_RENDERTARGETSTATE__H */

/* end of file */
