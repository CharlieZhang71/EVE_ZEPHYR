/*
Usr_ScrollPanelVert
Header
Vertical-only Scroll Panel Widget
*/

#ifndef Usr_ScrollPanelVert__H
#define Usr_ScrollPanelVert__H

#include "Esd_Base.h"
#include "Esd_Core.h"
#include "Ft_Esd.h"
#include "Ft_Esd_Layout_Linear.h"
#include "Ft_Esd_Layout_Scroll.h"
#include "Ft_Esd_Layout_Stretch.h"
#include "Ft_Esd_Math.h"
#include "Ft_Esd_ScrollBar.h"
#include "Ft_Esd_Theme.h"
#include "Ft_Esd_TouchPanel.h"
#include "Ft_Esd_Widget.h"

#ifndef ESD_LOGIC
#define ESD_LOGIC(name, ...)
#define ESD_ACTOR(name, ...)
#define ESD_WIDGET(name, ...)
#define ESD_PAGE(name, ...)
#define ESD_APPLICATION(name, ...)
#define ESD_INPUT(name, ...)
#define ESD_ARRAY_INPUT(name, ...)
#define ESD_OUTPUT(name, ...)
#define ESD_SLOT(name, ...)
#define ESD_SIGNAL(name, ...)
#define ESD_VARIABLE(name, ...)
#define ESD_WRITER(name, ...)
#endif

#define Usr_ScrollPanelVert_CLASSID 0x30702CDE
ESD_SYMBOL(Usr_ScrollPanelVert_CLASSID, Type = esd_classid_t)

/* Vertical-only scroll panel with no horizontal scrolling */
ESD_WIDGET(Usr_ScrollPanelVert, Include = "Usr_ScrollPanelVert.h", Callback, DisplayName = "Vertical Scroll Panel", Category = EsdWidgets, Icon = ":/icons/ui-scroll-pane.png", X = 0, Y = 0, Width = 400, Height = 300, Layout)
typedef struct
{
	union
	{
		void *Owner;
		Ft_Esd_Widget Widget;
	};
	ESD_SIGNAL(Up)
	void(* Up)(void *context);
	ESD_SIGNAL(Down)
	void(* Down)(void *context);
	ESD_SIGNAL(Tap)
	void(* Tap)(void *context);
	ESD_VARIABLE(Touching, Type = ft_bool_t, ReadOnly)
	ft_bool_t Touching;
	ESD_VARIABLE(Inside, Type = ft_bool_t, ReadOnly)
	ft_bool_t Inside;
	ESD_INPUT(ShowPanel, Type = ft_bool_t, Default = true)
	ft_bool_t(* ShowPanel)(void *context);
	/* Active Scroll overrides on child widget tag is 255 */
	ESD_INPUT(Active_Scroll, DisplayName = "Active Scroll", Type = ft_bool_t)
	ft_bool_t(* Active_Scroll)(void *context);
	ESD_VARIABLE(TouchX, Type = int, Private)
	int TouchX;
	ESD_INPUT(RaiseBorder, DisplayName = "Raise Border", Type = ft_bool_t, Default = true)
	ft_bool_t(* RaiseBorder)(void *context);
	ESD_VARIABLE(TouchY, Type = int, Private)
	int TouchY;
	ESD_INPUT(Theme, Type = Ft_Esd_Theme *, Default = Ft_Esd_Theme_GetCurrent)
	Ft_Esd_Theme *(* Theme)(void *context);
	ESD_VARIABLE(ScrollY, Type = ft_int16_t, Public)
	ft_int16_t ScrollY;
	ESD_VARIABLE(Align, Type = Esd_Align, Default = ESD_ALIGN_TOPFILL, Public)
	uint8_t Align;
	ESD_VARIABLE(ScrollBars, Type = Esd_Visible, Default = ESD_VISIBLE_NEVER, Public)
	ft_uint8_t ScrollBars;
	ESD_VARIABLE(TouchScroll, Type = ft_bool_t, Default = true, Public)
	ft_bool_t TouchScroll;
	ESD_VARIABLE(PreviousTouchY, Type = ft_int16_t, Private)
	ft_int16_t PreviousTouchY;
	ESD_INPUT(Radius, Type = int, Min = 0, Default = 4)
	int(* Radius)(void *context);
	ESD_VARIABLE(PreviousScrollY, Type = ft_int16_t, Private)
	ft_int16_t PreviousScrollY;
	ESD_VARIABLE(ScrollBarSize, DisplayName = "Scroll Bar Size", Type = ft_int16_t, Default = 24, Public)
	ft_int16_t ScrollBarSize;
	ESD_WRITER(ScrollYChanged, Type = ft_int16_t)
	void(* ScrollYChanged)(void *context, ft_int16_t value);
	Ft_Esd_Layout_Linear Linear_Layout;
	Ft_Esd_Layout_Stretch Stretch;
	Ft_Esd_Layout_Scroll Scroll_Layout;
	Ft_Esd_TouchPanel TouchPanel;
	Ft_Esd_Layout_Linear Linear_Layout_3;
	Ft_Esd_Layout_Stretch Stretch_2;
	Ft_Esd_ScrollBar ESD_Scroll_Bar;
} Usr_ScrollPanelVert;

void Usr_ScrollPanelVert__Initializer(Usr_ScrollPanelVert *context);

ESD_SLOT(End)
void Usr_ScrollPanelVert_End(Usr_ScrollPanelVert *context);

ESD_SLOT(Update)
void Usr_ScrollPanelVert_Update(Usr_ScrollPanelVert *context);

ESD_OUTPUT(MinY, Type = ft_int16_t)
ft_int16_t Usr_ScrollPanelVert_MinY(Usr_ScrollPanelVert *context);

ESD_OUTPUT(MaxY, Type = ft_int16_t)
ft_int16_t Usr_ScrollPanelVert_MaxY(Usr_ScrollPanelVert *context);

ESD_OUTPUT(RangeY, Type = ft_int16_t)
ft_int16_t Usr_ScrollPanelVert_RangeY(Usr_ScrollPanelVert *context);

ESD_OUTPUT(Width, Type = ft_int16_t)
ft_int16_t Usr_ScrollPanelVert_Width(Usr_ScrollPanelVert *context);

ESD_OUTPUT(Height, Type = ft_int16_t)
ft_int16_t Usr_ScrollPanelVert_Height(Usr_ScrollPanelVert *context);

#endif /* Usr_ScrollPanelVert__H */

/* end of file */