#include "Ft_Esd.h"
#include "Ft_Esd_Gradient.h"
#include "Ft_Esd_TouchTag.h"

extern ESD_CORE_EXPORT EVE_HalContext *Esd_Host;

ESD_METHOD(GradientManual_PeriodicUpdate, Context = Ft_Esd_Gradient)
ESD_PARAMETER(color1, Type = ft_argb32_t)
ESD_PARAMETER(color2, Type = ft_argb32_t)
void GradientManual_PeriodicUpdate(Ft_Esd_Gradient *context, ft_argb32_t color1, ft_argb32_t color2)
{
	EVE_HalContext *phost = Esd_GetHost();
	void *owner = context->Owner;
	if (!context->Manual(owner))
		return;

	Esd_Rect16 state = Esd_Scissor_Set(context->Widget.GlobalRect);
	EVE_CoCmd_gradient(phost, context->Point_1_X, context->Point_1_Y, color1, context->Point_2_X, context->Point_2_Y, color2);
	Esd_Scissor_Reset(state);

#ifdef ESD_SIMULATION
	if (context->HideCursor)
		return;

	Ft_Esd_UpdateAnchorPointPosition(context);
#endif
}

ESD_METHOD(Ft_Esd_GradientManual_TouchTracker, Context = Ft_Esd_Gradient)
void Ft_Esd_GradientManual_TouchTracker(Ft_Esd_Gradient *context)
{
#ifdef ESD_SIMULATION
	EVE_HalContext *phost = Esd_GetHost();

	// ...
	void *owner = context->Owner;
	{
		if (Ft_Esd_TouchTag_Touching(&context->TouchTag))
		{
			int DeltaX = Ft_Esd_TouchTag_TouchXDelta(&context->TouchTag);
			int DeltaY = Ft_Esd_TouchTag_TouchYDelta(&context->TouchTag);

			context->Point_1_X += DeltaX;
			context->Point_1_Y += DeltaY;
		}
		if (Ft_Esd_TouchTag_Touching(&context->TouchTag_2))
		{
			int DeltaX = Ft_Esd_TouchTag_TouchXDelta(&context->TouchTag);
			int DeltaY = Ft_Esd_TouchTag_TouchYDelta(&context->TouchTag);

			context->Point_2_X += DeltaX;
			context->Point_2_Y += DeltaY;
		}
	}
#endif
}
#ifdef ESD_SIMULATION
void Ft_Esd_DrawCoordinatesLabel(int x, int y, int value)
{
	EVE_HalContext *phost = Esd_GetHost();
	char Number[5];
	eve_sprintf(Number, "%d", value);

	Ft_Gpu_CoCmd_Text_Ex(phost, x, y, 27, 0, 0, 0, 0, 0, Number);
}

void Ft_Esd_DrawAnchorPoint(int x, int y, int radius, int tagID)
{
	EVE_HalContext *phost = Esd_GetHost();

	EVE_CoDl_tag(phost, tagID);

	Ft_Esd_CircleLine_Draw_Point(x, y, radius);

	EVE_CoDl_tag(phost, 255);
}

void Ft_Esd_UpdateAnchorPointPosition(Ft_Esd_Gradient *context)
{
	Ft_Esd_DrawAnchorPoint(context->Point_1_X, context->Point_1_Y, 14, context->TouchTag.Tag);
	Ft_Esd_DrawCoordinatesLabel(context->Point_1_X + 20, context->Point_1_Y, context->Point_1_X);
	Ft_Esd_DrawCoordinatesLabel(context->Point_1_X + 70, context->Point_1_Y, context->Point_1_Y);

	Ft_Esd_DrawAnchorPoint(context->Point_2_X, context->Point_2_Y, 14, context->TouchTag_2.Tag);
	Ft_Esd_DrawCoordinatesLabel(context->Point_2_X - 110, context->Point_2_Y, context->Point_2_X);
	Ft_Esd_DrawCoordinatesLabel(context->Point_2_X - 60, context->Point_2_Y, context->Point_2_Y);
}
#endif
