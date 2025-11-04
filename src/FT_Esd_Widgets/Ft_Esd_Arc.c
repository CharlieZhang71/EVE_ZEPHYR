#include "Ft_Esd.h"
#include "Ft_Esd_Arc.h"

ESD_METHOD(Ft_Esd_Arc_Render_Signal, Context = Ft_Esd_Arc)
ESD_PARAMETER(x, Type = int)
ESD_PARAMETER(y, Type = int)
ESD_PARAMETER(radius, Type = int)
ESD_PARAMETER(border, Type = int)
ESD_PARAMETER(color, Type = ft_argb32_t)
ESD_PARAMETER(start, Type = int)
ESD_PARAMETER(end, Type = int)
void Ft_Esd_Arc_Render_Signal(Ft_Esd_Arc *context, int x, int y, int radius, int border, ft_argb32_t color, int start, int end)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	EVE_HalContext *phost = Esd_GetHost();
	EVE_CoDl_colorRgb_ex(phost, color);
	EVE_CoCmd_arc(phost, x, y, (radius - border), radius, start, end);
#endif
}
