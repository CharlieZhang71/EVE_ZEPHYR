
#include "Esd_RenderTargetState.h"

#include "Esd_Context.h"

// Persistent state in RAM_G, stored after bitmap buffer
// - Backup of the previous display list (plus CMD_NEWLIST and CMD_ENDLIST)
// - Backup of REG_RE_ROTATE
// - Readout of REG_TOUCH_TAG after render engine completes this render target
#define ESD_RT_STATE_SIZE (EVE_DL_SIZE + 16)
#define ESD_RT_STATE_OFFSET_DL (0)
#define ESD_RT_STATE_OFFSET_ROTATE (EVE_DL_SIZE + 8)
#define ESD_RT_STATE_OFFSET_TAG (EVE_DL_SIZE + 12)

/* TODO: If drawing a render target outside of RENDER ESD state, then we can bypass the state backup mechanisms */
/* Then set a special flag or set to RENDER state temporarily */

/* TODO: Add a separate render pass for processing render targets separately without requiring context backups */

ESD_CORE_EXPORT uint32_t Esd_RenderTarget_DlStart(Esd_BitmapInfo *info, Esd_RenderTargetState *state, int16_t offsetX, int16_t offsetY)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	EVE_HalContext *phost = Esd_GetHost();
	Esd_Context *ec = Esd_CurrentContext;
	Esd_GpuAlloc *ga = Esd_GAlloc;
	Esd_HandleState *handleState = &Esd_CurrentContext->HandleState;
	uint8_t handle;

	if (EVE_CHIPID < EVE_BT820)
		return GA_INVALID;

	if (!info || !state) return GA_INVALID; // Missing input fields

	if (ec->SpinnerPopped)
	{
		Esd_GpuAlloc_Get(ga, info->GpuHandle); // Keep any existing memory allocation alive
		return GA_INVALID; // Cannot use render target while spinner is active
	}
	
	EVE_CoCmd_stop(phost); // TODO: Spinner adjustments - check if this can be removed now

	// Back up state from EVE_HalContext
	state->Destination = phost->CoRenderTargetDst;
	state->Format = phost->CoRenderTargetFmt;
	state->Width = phost->CoRenderTargetW;
	state->Height = phost->CoRenderTargetH;

	// Backup current offset for touch
	state->TouchTransformC = ec->TouchTransformC;
	state->TouchTransformF = ec->TouchTransformF;

	// Backup display list state
	state->DlStateBase = phost->DlStateBase;
	state->DlStateIndex = phost->DlStateIndex;
	state->DlPrimitive = phost->DlPrimitive;

	// Advance display list cache stack base
	phost->DlStateBase = state->DlStateIndex + 1;

	// Backup bitmap handle state, and revert to match hardware state
	for (handle = 0; handle < ESD_BITMAPHANDLE_NB; ++handle)
	{
		// FIXME: InitialPage
		state->HandleFlags[handle] = handleState->Flags[handle];
		state->HandlePage[handle] = handleState->Page[handle];
		state->HandleResized[handle] = handleState->Resized[handle];
		state->HandleInitialPage[handle] = handleState->InitialPage[handle];
		handleState->Flags[handle] = 0;
		handleState->Page[handle] = handleState->HwPage[handle];
		handleState->Resized[handle] = handleState->HwResized[handle];
	}

	// Get required memory size
	uint32_t rtSize = Esd_RenderTarget_Size(info->Format, info->Width, info->Height);
	uint32_t size = rtSize + ESD_RT_STATE_SIZE;

	if (info->StorageSize != size)
	{
		if (info->StorageSize)
		{
			// Free previous allocation
			Esd_GpuAlloc_Free(ga, info->GpuHandle);
			info->StorageSize = 0;
		}

		// Allocate new memory
		info->GpuHandle = Esd_GpuAlloc_Alloc(ga, size, GA_GC_FLAG);
		info->StorageSize = size;
		info->Size = rtSize;
		info->Stride = Esd_RenderTarget_Stride(info->Format, info->Width);
	}

	// Get address of the new render target
	uint32_t dest = Esd_GpuAlloc_Get(ga, info->GpuHandle);
	if (dest == GA_INVALID)
	{
		// Allocation failed
		info->StorageSize = 0;
		return GA_INVALID;
	}

	const uint32_t stateAddr = dest + rtSize;
	const uint32_t listAddr = stateAddr + ESD_RT_STATE_OFFSET_DL;
	const uint32_t rotateAddr = stateAddr + ESD_RT_STATE_OFFSET_ROTATE;

	// Backup the current display list and REG_CMD_DL using EVE_CoCmd
	// EVE_CoCmd_memCpy(phost, dest + rtSize, RAM_DL, EVE_DL_SIZE);
	// EVE_CoCmd_memCpy(phost, dest + rtSize + EVE_DL_SIZE, REG_CMD_DL, 4);
	EVE_CoCmd_copyList(phost, listAddr);
	EVE_CoCmd_memCpy(phost, rotateAddr, REG_RE_ROTATE, 4); // Backup REG_RE_ROTATE

	// Adjust touch transform
	ec->TouchTransformC -= ((int32_t)offsetX) << 16;
	ec->TouchTransformF -= ((int32_t)offsetY) << 16;

	// EVE_Hal_wr32(phost, REG_CTOUCH_TOUCH0_XY, 0x80008000); // TEST
	// EVE_Hal_wr32(phost, REG_CTOUCH_TOUCH0_XY, 0x80008000); // TEST
	// EVE_Hal_wr32(phost, REG_CTOUCH_TOUCH0_XY, 0x80008000); // TEST
	// EVE_CoCmd_regWrite(phost, REG_CTOUCH_TOUCH0_XY, 0x80008000); // TEST

	// uint32_t ctouch0 = EVE_Hal_rd32(phost, REG_CTOUCH_TOUCH0_XY);

	// Set new render target using EVE_CoCmd
	EVE_CoCmd_graphicsFinish(phost); // VERIFY: Maybe move the actual rendertarget before the swap to pipeline things better? // VERIFY: Check if on hardware REG_RE_ are locked in on swap...
	EVE_CoCmd_regWrite(phost, REG_RE_ROTATE, 0); // No rotation for render targets, but retain touch transform rotation (bypass CMD_SETROTATE)

#if 0
	// EVE_CoCmd_regWrite(phost, REG_TOUCH_TRANSFORM_C, ec->TouchTransformC - (2048L << 16));
	// EVE_CoCmd_regWrite(phost, REG_TOUCH_TRANSFORM_F, ec->TouchTransformF - (2048L << 16));
	// EVE_CoCmd_regWrite(phost, REG_TOUCH_MODE, TOUCHMODE_ONESHOT);
	// EVE_CoCmd_waitCond(phost, REG_CTOUCH_TOUCH0_XY, EQUAL, 0x00008000, 0x00008000);
	EVE_CoCmd_regWrite(phost, REG_TOUCH_TRANSFORM_C, ec->TouchTransformC);
	EVE_CoCmd_regWrite(phost, REG_TOUCH_TRANSFORM_F, ec->TouchTransformF);
	// EVE_CoCmd_regWrite(phost, REG_TOUCH_MODE, TOUCHMODE_ONESHOT);
	// EVE_CoCmd_waitCond(phost, REG_CTOUCH_TOUCH0_XY, NOTEQUAL, 0x00004000, 0x00004000); // FIXME: This probably breaks CMD_SKETCH -- TODO: Hack to offset CMD_SKETCH?
#endif

	EVE_CoCmd_renderTarget(phost, dest, info->Format, info->Width, info->Height);
	// EVE_CoCmd_fence(phost); // TEST
	EVE_CoCmd_dlStart(phost);

	return dest;
#else
	return GA_INVALID;
#endif
}

ESD_CORE_EXPORT void Esd_RenderTarget_DisplaySwap(Esd_BitmapInfo *info, Esd_RenderTargetState *state)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	Esd_Context *ec = Esd_CurrentContext;
	EVE_HalContext *phost = Esd_GetHost();
	Esd_GpuAlloc *ga = Esd_GAlloc;
	Esd_HandleState *handleState = &Esd_CurrentContext->HandleState;
	uint8_t handle;

	if (EVE_CHIPID < EVE_BT820)
		return;

	/* Restore initial bitmap handle state, call before swap since it may write to DL */
	Esd_BitmapHandle_OnSwap(state->HandleFlags, state->HandleInitialPage);

	// Swap the display list
	EVE_CoDl_display(phost);
	EVE_CoDl_display(phost);
	EVE_CoCmd_swap(phost);

	// Start the next display list
	EVE_CoCmd_dlStart(phost);

	// Restore the previous display list
	uint32_t dest = Esd_GpuAlloc_Get(ga, info->GpuHandle);
	const uint32_t stateAddr = dest + (info->StorageSize - ESD_RT_STATE_SIZE);
	if (dest != GA_INVALID)
	{
		// EVE_CoCmd_memCpy(phost, RAM_DL, dest + info->StorageSize - EVE_DL_SIZE - 8, EVE_DL_SIZE);
		// EVE_Hal_wr32(phost, REG_CMD_DL, dest + info->StorageSize - 8);
		const uint32_t listAddr = stateAddr + ESD_RT_STATE_OFFSET_DL;
		EVE_CoCmd_callList(phost, listAddr);
	}

	/* Restore bitmap handle state at end of display list if they are actually used in this display list */
	for (handle = 0; handle < ESD_BITMAPHANDLE_NB; ++handle)
	{
		uint8_t flags = state->HandleFlags[handle];
		handleState->Flags[handle] = flags;
		if (flags & ESD_BHFLAGS_PAGE_SET)
		{
			handleState->Page[handle] = state->HandlePage[handle];
			handleState->InitialPage[handle] = state->HandleInitialPage[handle];
		}
		if (flags & ESD_BHFLAGS_SIZE_SET)
		{
			handleState->Resized[handle] = state->HandleResized[handle];
		}
	}

	// Restore the previous display list state
	phost->DlStateBase = state->DlStateBase;
	phost->DlStateIndex = state->DlStateIndex;
	phost->DlPrimitive = state->DlPrimitive;

	// Restore the previous render target, wait for ongoing render to finish
	EVE_CoCmd_graphicsFinish(phost);

	// Restore rotate and other render registers after previous render finished
	if (dest != GA_INVALID)
	{
		const uint32_t rotateAddr = stateAddr + ESD_RT_STATE_OFFSET_ROTATE;
		EVE_CoCmd_memCpy(phost, REG_RE_ROTATE, rotateAddr, 4); // Restore REG_RE_ROTATE
	}

#if 0
	// Restore touch transform
	// EVE_CoCmd_regWrite(phost, REG_TOUCH_TRANSFORM_C, state->TouchTransformC - (2048L << 16));
	// EVE_CoCmd_regWrite(phost, REG_TOUCH_TRANSFORM_F, state->TouchTransformF - (2048L << 16));
	// EVE_CoCmd_regWrite(phost, REG_TOUCH_MODE, TOUCHMODE_ONESHOT);
	// EVE_CoCmd_waitCond(phost, REG_CTOUCH_TOUCH0_XY, EQUAL, 0x00008000, 0x00008000);
	EVE_CoCmd_regWrite(phost, REG_TOUCH_TRANSFORM_C, state->TouchTransformC);
	EVE_CoCmd_regWrite(phost, REG_TOUCH_TRANSFORM_F, state->TouchTransformF);
	// EVE_CoCmd_regWrite(phost, REG_TOUCH_MODE, TOUCHMODE_ONESHOT);
	// EVE_CoCmd_waitCond(phost, REG_CTOUCH_TOUCH0_XY, NOTEQUAL, 0x00004000, 0x00004000); // FIXME: This probably breaks CMD_SKETCH -- TODO: Hack to offset CMD_SKETCH?
	ec->TouchTransformC = state->TouchTransformC;
	ec->TouchTransformF = state->TouchTransformF;
#endif

	// Readout REG_TOUCH_TAG and store it in RAM_G
	if (dest != GA_INVALID)
	{
		const uint32_t touchTagAddr = stateAddr + ESD_RT_STATE_OFFSET_TAG;
		EVE_CoCmd_memCpy(phost, touchTagAddr, REG_TOUCH_TAG, 4); // Read REG_TOUCH_TAG into RAM_G
	}

	// Resume previous render target
	EVE_CoCmd_renderTarget(phost, state->Destination, state->Format, state->Width, state->Height);
#endif
}

ESD_CORE_EXPORT eve_tag_t Esd_RenderTarget_TouchTag(Esd_BitmapInfo *info)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	Esd_GpuAlloc *ga = Esd_GAlloc;
	EVE_HalContext *phost = Esd_GetHost();

	if (EVE_CHIPID < EVE_BT820)
		return EVE_TAG_NONE;

	uint32_t dest = Esd_GpuAlloc_Get(ga, info->GpuHandle);
	if (dest != GA_INVALID)
	{
		const uint32_t stateAddr = dest + (info->StorageSize - ESD_RT_STATE_SIZE);
		const uint32_t touchTagAddr = stateAddr + ESD_RT_STATE_OFFSET_TAG;
		if (touchTagAddr < RAM_G_SIZE)
		{
			// Read directly from RAM_G since this is called in the update after the whole frame is done already
			return EVE_Hal_rd32(phost, touchTagAddr) & EVE_TAG_MASK;
		}
	}
	return EVE_TAG_NONE;
#endif
}

/* end of file */
