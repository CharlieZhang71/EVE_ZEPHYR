
#ifndef Ft_Esd_RenderTarget_H
#define Ft_Esd_RenderTarget_H

#include "Ft_Esd_Widget.h"

#include "Esd_BitmapInfo.h"
#include "Esd_TouchTag.h"

#define Ft_Esd_RenderTarget_CLASSID 0xDA7291C1
ESD_SYMBOL(Ft_Esd_RenderTarget_CLASSID, Type = esd_classid_t)

// Layout which positions all it's child widgets to the full size of the layout itself
ESD_WIDGET(Ft_Esd_RenderTarget, DisplayName = "Render Target", Include = "Ft_Esd_RenderTarget.h", Icon = ":/icons/image-empty.png", Category = EsdLayouts, Width = 50, Height = 50, Callback, Layout, BackToFront)
typedef struct
{
	union
	{
		void *Owner;
		Ft_Esd_Widget Widget;
	};

	Esd_BitmapInfo RenderTargetInfo;
	Esd_TouchTag TouchTag;

} Ft_Esd_RenderTarget;

void Ft_Esd_RenderTarget__Initializer(Ft_Esd_RenderTarget *context);

ESD_SLOT(Start)
void Ft_Esd_RenderTarget_Start(Ft_Esd_RenderTarget *context);

ESD_SLOT(Update)
void Ft_Esd_RenderTarget_Update(Ft_Esd_RenderTarget *context);

ESD_SLOT(Render)
void Ft_Esd_RenderTarget_Render(Ft_Esd_RenderTarget *context);

ESD_SLOT(End)
void Ft_Esd_RenderTarget_End(Ft_Esd_RenderTarget *context);

ESD_OUTPUT(BitmapCell, Type = Esd_BitmapCell)
Esd_BitmapCell Ft_Esd_RenderTarget_BitmapCell(Ft_Esd_RenderTarget *context);

#endif /* Ft_Esd_RenderTarget_H */

/* end of file */
