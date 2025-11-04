
#include "Ft_Esd_RenderTarget.h"

#include "Esd_RenderTargetState.h"
#include "Esd_BitmapHandle.h"
#include "Esd_Render.h"
#include "Esd_Context.h"
#include "Esd_TouchTag.h"

#include "EVE_CoDl.h"

static Ft_Esd_WidgetSlots s_Ft_Esd_RenderTarget__Slots = {
	(void (*)(void *))Ft_Esd_Widget_Initialize,
	(void (*)(void *))Ft_Esd_Widget_Start,
	(void (*)(void *))Ft_Esd_Widget_Enable,
	(void (*)(void *))Ft_Esd_RenderTarget_Update,
	(void (*)(void *))Ft_Esd_RenderTarget_Render,
	(void (*)(void *))Ft_Esd_Widget_Idle,
	(void (*)(void *))Ft_Esd_Widget_Disable,
	(void (*)(void *))Ft_Esd_Widget_End,
};

void Ft_Esd_RenderTarget__Initializer(Ft_Esd_RenderTarget *context)
{
	Ft_Esd_Widget__Initializer((Ft_Esd_Widget *)context);
	context->Widget.ClassId = Ft_Esd_RenderTarget_CLASSID;
	context->Widget.Slots = &s_Ft_Esd_RenderTarget__Slots;
	context->Widget.LocalWidth = 50;
	context->Widget.LocalHeight = 50;
	context->RenderTargetInfo = (Esd_BitmapInfo) { .Format = RGB565, .Width = 64, .Height = 64, ESD_BITMAP_DEFAULTS };

	// Special case for Esd_TouchTag, if Tap equals Esd_RenderTarget_TouchTag, the render target's touch tag will be used (recursively)
	Esd_TouchTag__Initializer(&context->TouchTag);
	context->TouchTag.Owner = context;
	context->TouchTag.Tap = (void *)Esd_RenderTarget_TouchTag;
	context->TouchTag.Down = (void *)&context->RenderTargetInfo;
}

void Ft_Esd_RenderTarget_Start(Ft_Esd_RenderTarget *context)
{
	Esd_TouchTag_Start(&context->TouchTag);
}

void Ft_Esd_RenderTarget_Update(Ft_Esd_RenderTarget *context)
{
	Esd_TouchTag_Update(&context->TouchTag);
	bool recalculate = context->Widget.Recalculate;
	if (recalculate)
	{
		context->Widget.Recalculate = 0;
		context->RenderTargetInfo.Width = (context->Widget.GlobalWidth + 15) & ~0xF;
		context->RenderTargetInfo.Height = (context->Widget.GlobalHeight + 15) & ~0xF;
		Ft_Esd_Widget *child = context->Widget.First;
		while (child)
		{
			Ft_Esd_Widget *const next = child->Next;
			if (child->Active)
			{
				Ft_Esd_Widget_SetGlobalRect(child,
				    0,
				    0,
				    context->Widget.GlobalWidth,
				    context->Widget.GlobalHeight);
			}
			child = next;
		}
	}
	Ft_Esd_Widget_IterateChildActiveSlotReverse((Ft_Esd_Widget *)context, FT_ESD_WIDGET_UPDATE);
	if (recalculate)
		Ft_Esd_Widget_CheckDefaultIdle(&context->Widget);
}

void Ft_Esd_RenderTarget_Render(Ft_Esd_RenderTarget *context)
{
	EVE_HalContext *phost = Esd_Host;
	(void)phost;
	
	// Checks
	if (context->Widget.GlobalWidth < 0 || context->Widget.GlobalWidth > 4096
		|| context->Widget.GlobalHeight < 0 || context->Widget.GlobalHeight > 4096) {
		return; // Invalid size
	}
	
	// Start render target
	Esd_RenderTargetState rtState; // State used to backup previous display list
	uint32_t dest = Esd_RenderTarget_DlStart(&context->RenderTargetInfo, &rtState, context->Widget.GlobalX, context->Widget.GlobalY); // This sets render target and calls CMD_DLSTART
	if (dest != GA_INVALID) // Only process if we allocated the memory successfully
	{
		EVE_CoDl_clear(phost, 1, 1, 1);

		Ft_Esd_Widget_IterateChildActiveValidSlotReverse((Ft_Esd_Widget *)context, FT_ESD_WIDGET_RENDER);

		// End render target. This calls DISPLAY and CMD_SWAP
		Esd_RenderTarget_DisplaySwap(&context->RenderTargetInfo, &rtState);
	}

	// Set the touch tag for this render target
	eve_tag_t rtTag = Esd_TouchTag_Tag(&context->TouchTag);
	EVE_CoDl_tag(phost, rtTag);

	// Draw the render target. It's just a normal bitmap
	Esd_BitmapCell cell = { .Cell = 0, .Info = &context->RenderTargetInfo };
	Esd_Render_Bitmap(context->Widget.GlobalX, context->Widget.GlobalY, cell, 0xFFFFFFFF);

	// Reset the touch tag
	EVE_CoDl_tag(phost, EVE_TAG_NONE);
}

void Ft_Esd_RenderTarget_End(Ft_Esd_RenderTarget *context)
{
	Esd_TouchTag_End(&context->TouchTag);
}

Esd_BitmapCell Ft_Esd_RenderTarget_BitmapCell(Ft_Esd_RenderTarget *context)
{
	return (Esd_BitmapCell) { .Cell = 0, .Info = &context->RenderTargetInfo };
}

#ifdef ESD_SIMULATION
#include <stdlib.h>

typedef struct
{
	Ft_Esd_RenderTarget Instance;
} Ft_Esd_RenderTarget__ESD;

void *Ft_Esd_RenderTarget__Create__ESD()
{
	Ft_Esd_RenderTarget__ESD *context = (Ft_Esd_RenderTarget__ESD *)esd_malloc(sizeof(Ft_Esd_RenderTarget__ESD));
	Ft_Esd_RenderTarget__Initializer(&context->Instance);
	context->Instance.Owner = context;
	return context;
}

void Ft_Esd_RenderTarget__Destroy__ESD(void *context)
{
	esd_free(context);
}

#endif /* ESD_SIMULATION */

/* end of file */
