#include "Ft_Esd.h"
#include "Ft_Esd_Cgradient.h"

ESD_METHOD(Ft_Esd_Cgradient_Render_Signal, Context = Ft_Esd_Cgradient)
ESD_PARAMETER(shape, Type = int)
ESD_PARAMETER(x, Type = int)
ESD_PARAMETER(y, Type = int)
ESD_PARAMETER(w, Type = int)
ESD_PARAMETER(h, Type = int)
ESD_PARAMETER(color1, Type = ft_argb32_t)
ESD_PARAMETER(color2, Type = ft_argb32_t)
void Ft_Esd_Cgradient_Render_Signal(Ft_Esd_Cgradient *context, int shape, int x, int y, int w, int h, ft_argb32_t color1, ft_argb32_t color2)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	EVE_HalContext *phost = Esd_GetHost();
	EVE_CoCmd_cGradient(phost, shape, x, y, w, h, color1, color2);
#endif
}
