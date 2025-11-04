#include "Ft_Esd.h"
#include "Ft_Esd_Glow.h"

ESD_METHOD(Ft_Esd_Glow_Render_Signal, Context = Ft_Esd_Glow)
ESD_PARAMETER(x, Type = int)
ESD_PARAMETER(y, Type = int)
ESD_PARAMETER(w, Type = int)
ESD_PARAMETER(h, Type = int)
ESD_PARAMETER(color, Type = ft_argb32_t)
void Ft_Esd_Glow_Render_Signal(Ft_Esd_Glow *context, int x, int y, int w, int h, ft_argb32_t color)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	EVE_HalContext *phost = Esd_GetHost();
	EVE_CoDl_colorRgb_ex(phost, color);
	EVE_CoCmd_glow(phost, x, y, w, h);
#endif
}
