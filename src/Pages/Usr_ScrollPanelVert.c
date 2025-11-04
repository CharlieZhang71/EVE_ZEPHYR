/*
Usr_ScrollPanelVert
C Source
Vertical-only Scroll Panel Widget Implementation
*/

/*
Comment:
Introduction:
This widget draws a panel with vertical scrolling only.
It supports scrolling with a vertical scroll bar but no horizontal scrolling.
*/

#include "Usr_ScrollPanelVert.h"

#include "Esd_Context.h"

ESD_CORE_EXPORT void Esd_Noop(void *context);
ft_bool_t Usr_ScrollPanelVert_ShowPanel__Default(void *context) { return 1; }
ft_bool_t Usr_ScrollPanelVert_Active_Scroll__Default(void *context) { return 0; }
ft_bool_t Usr_ScrollPanelVert_RaiseBorder__Default(void *context) { return 1; }
Ft_Esd_Theme * Usr_ScrollPanelVert_Theme__Default(void *context) { return Ft_Esd_Theme_GetCurrent(); }
int Usr_ScrollPanelVert_Radius__Default(void *context) { return 4L; }
void Usr_ScrollPanelVert_ScrollYChanged__Noop(void *context, ft_int16_t value) { }

static ft_argb32_t Usr_ScrollPanelVert_TouchPanel_Color__Property(void *context);
static int Usr_ScrollPanelVert_TouchPanel_Radius__Property(void *context);
static ft_bool_t Usr_ScrollPanelVert_TouchPanel_Raised__Property(void *context);
static ft_bool_t Usr_ScrollPanelVert_TouchPanel_ShowBorder__Property(void *context);
static ft_bool_t Usr_ScrollPanelVert_TouchPanel_IncludeDefaultTag__Property(void *context);
static Ft_Esd_Theme * Usr_ScrollPanelVert_ESD_Scroll_Bar_Theme__Property(void *context);
static ft_int16_t Usr_ScrollPanelVert_ESD_Scroll_Bar_Size__Property(void *context);
static ft_int16_t Usr_ScrollPanelVert_ESD_Scroll_Bar_Max__Property(void *context);
static ft_int16_t Usr_ScrollPanelVert_ESD_Scroll_Bar_Min__Property(void *context);

static Ft_Esd_WidgetSlots s_Usr_ScrollPanelVert__Slots = {
	(void(*)(void *))Ft_Esd_Widget_Initialize,
	(void(*)(void *))Ft_Esd_Widget_Start,
	(void(*)(void *))Ft_Esd_Widget_Enable,
	(void(*)(void *))Usr_ScrollPanelVert_Update,
	(void(*)(void *))Ft_Esd_Widget_Render,
	(void(*)(void *))Ft_Esd_Widget_Idle,
	(void(*)(void *))Ft_Esd_Widget_Disable,
	(void(*)(void *))Usr_ScrollPanelVert_End,
};

static void Usr_ScrollPanelVert_Scroll_Layout_ScrollYChanged__Writer(void *context, ft_int16_t value);
static void Usr_ScrollPanelVert_TouchPanel_Up__Signal(void *context);
static void Usr_ScrollPanelVert_TouchPanel_Down__Signal(void *context);
static void Usr_ScrollPanelVert_TouchPanel_Tap__Signal(void *context);
static void Usr_ScrollPanelVert_ESD_Scroll_Bar_Changed__Writer(void *context, int value);

Ft_Esd_Widget *Usr_ScrollPanelVert_Widget_Interface__Container(void *context);

void Usr_ScrollPanelVert__Linear_Layout__Initializer(Usr_ScrollPanelVert *context)
{
	Ft_Esd_Layout_Linear *object = (Ft_Esd_Layout_Linear *)&context->Linear_Layout;
	Ft_Esd_Layout_Linear__Initializer(object);
	object->Owner = (void *)context;
	object->Widget.Active = 1;
	object->Widget.LocalX = 0;
	object->Widget.LocalY = 0;
	object->Widget.LocalWidth = 50;
	object->Widget.LocalHeight = 50;
	object->Margin = 0;
	object->Orientation = ESD_ORIENTATION_HORIZONTAL;
	object->Align = ESD_ALIGN_FILLLEFT;
	Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)object, (Ft_Esd_Widget *)context);
}

void Usr_ScrollPanelVert__Stretch__Initializer(Usr_ScrollPanelVert *context)
{
	Ft_Esd_Layout_Stretch *object = (Ft_Esd_Layout_Stretch *)&context->Stretch;
	Ft_Esd_Layout_Stretch__Initializer(object);
	object->Owner = (void *)context;
	object->Widget.Active = 1;
	object->Widget.LocalX = 0;
	object->Widget.LocalY = 0;
	object->Widget.LocalWidth = 12;
	object->Widget.LocalHeight = 53;
	Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)object, (Ft_Esd_Widget *)&context->Linear_Layout);
}

void Usr_ScrollPanelVert__Scroll_Layout__Initializer(Usr_ScrollPanelVert *context)
{
	Ft_Esd_Layout_Scroll *object = (Ft_Esd_Layout_Scroll *)&context->Scroll_Layout;
	Ft_Esd_Layout_Scroll__Initializer(object);
	object->Owner = (void *)context;
	object->Widget.Active = 1;
	object->Widget.LocalX = 0;
	object->Widget.LocalY = 0;
	object->Widget.LocalWidth = 50;
	object->Widget.LocalHeight = 50;
	object->Align = ESD_ALIGN_TOPLEFT;
	object->Scissor = 1;
	object->ScrollYChanged = Usr_ScrollPanelVert_Scroll_Layout_ScrollYChanged__Writer;
	Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)object, (Ft_Esd_Widget *)&context->Stretch);
}

void Usr_ScrollPanelVert__TouchPanel__Initializer(Usr_ScrollPanelVert *context)
{
	Ft_Esd_TouchPanel *object = (Ft_Esd_TouchPanel *)&context->TouchPanel;
	Ft_Esd_TouchPanel__Initializer(object);
	object->Owner = (void *)context;
	object->Widget.Active = 1;
	object->Widget.LocalX = 0;
	object->Widget.LocalY = 0;
	object->Widget.LocalWidth = 259;
	object->Widget.LocalHeight = 284;
	object->Color = Usr_ScrollPanelVert_TouchPanel_Color__Property;
	object->Radius = Usr_ScrollPanelVert_TouchPanel_Radius__Property;
	object->Raised = Usr_ScrollPanelVert_TouchPanel_Raised__Property;
	object->ShowBorder = Usr_ScrollPanelVert_TouchPanel_ShowBorder__Property;
	object->IncludeDefaultTag = Usr_ScrollPanelVert_TouchPanel_IncludeDefaultTag__Property;
	object->Up = Usr_ScrollPanelVert_TouchPanel_Up__Signal;
	object->Down = Usr_ScrollPanelVert_TouchPanel_Down__Signal;
	object->Tap = Usr_ScrollPanelVert_TouchPanel_Tap__Signal;
	Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)object, (Ft_Esd_Widget *)&context->Stretch);
}

void Usr_ScrollPanelVert__Linear_Layout_3__Initializer(Usr_ScrollPanelVert *context)
{
	Ft_Esd_Layout_Linear *object = (Ft_Esd_Layout_Linear *)&context->Linear_Layout_3;
	Ft_Esd_Layout_Linear__Initializer(object);
	object->Owner = (void *)context;
	object->Widget.Active = 1;
	object->Widget.LocalX = 0;
	object->Widget.LocalY = 0;
	object->Widget.LocalWidth = 24;
	object->Widget.LocalHeight = 50;
	object->ChildClipping = 0;
	Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)object, (Ft_Esd_Widget *)&context->Linear_Layout);
}

void Usr_ScrollPanelVert__Stretch_2__Initializer(Usr_ScrollPanelVert *context)
{
	Ft_Esd_Layout_Stretch *object = (Ft_Esd_Layout_Stretch *)&context->Stretch_2;
	Ft_Esd_Layout_Stretch__Initializer(object);
	object->Owner = (void *)context;
	object->Widget.Active = 1;
	object->Widget.LocalX = 0;
	object->Widget.LocalY = 0;
	object->Widget.LocalWidth = 50;
	object->Widget.LocalHeight = 50;
	Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)object, (Ft_Esd_Widget *)&context->Linear_Layout_3);
}

void Usr_ScrollPanelVert__ESD_Scroll_Bar__Initializer(Usr_ScrollPanelVert *context)
{
	Ft_Esd_ScrollBar *object = (Ft_Esd_ScrollBar *)&context->ESD_Scroll_Bar;
	Ft_Esd_ScrollBar__Initializer(object);
	object->Owner = (void *)context;
	object->Widget.Active = 1;
	object->Widget.LocalX = 0;
	object->Widget.LocalY = 0;
	object->Widget.LocalWidth = 24;
	object->Widget.LocalHeight = 24;
	object->Theme = Usr_ScrollPanelVert_ESD_Scroll_Bar_Theme__Property;
	object->Size = Usr_ScrollPanelVert_ESD_Scroll_Bar_Size__Property;
	object->Max = Usr_ScrollPanelVert_ESD_Scroll_Bar_Max__Property;
	object->Min = Usr_ScrollPanelVert_ESD_Scroll_Bar_Min__Property;
	object->Changed = Usr_ScrollPanelVert_ESD_Scroll_Bar_Changed__Writer;
	Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)object, (Ft_Esd_Widget *)&context->Stretch_2);
}

void Usr_ScrollPanelVert__Initializer(Usr_ScrollPanelVert *context)
{
	Ft_Esd_Widget__Initializer((Ft_Esd_Widget *)context);
	context->Widget.ClassId = Usr_ScrollPanelVert_CLASSID;
	context->Widget.Slots = &s_Usr_ScrollPanelVert__Slots;
	context->Widget.LocalX = 0;
	context->Widget.LocalY = 0;
	context->Widget.LocalWidth = 400;
	context->Widget.LocalHeight = 300;
	context->Up = Esd_Noop;
	context->Down = Esd_Noop;
	context->Tap = Esd_Noop;
	context->Touching = 0;
	context->Inside = 0;
	context->ShowPanel = Usr_ScrollPanelVert_ShowPanel__Default;
	context->Active_Scroll = Usr_ScrollPanelVert_Active_Scroll__Default;
	context->TouchX = 0L;
	context->RaiseBorder = Usr_ScrollPanelVert_RaiseBorder__Default;
	context->TouchY = 0L;
	context->Theme = Usr_ScrollPanelVert_Theme__Default;
	context->ScrollY = 0;
	context->Align = ESD_ALIGN_TOPFILL;
	context->ScrollBars = ESD_VISIBLE_NEVER;
	context->TouchScroll = 1;
	context->PreviousTouchY = 0;
	context->Radius = Usr_ScrollPanelVert_Radius__Default;
	context->PreviousScrollY = 0;
	context->ScrollBarSize = 24;
	context->ScrollYChanged = Usr_ScrollPanelVert_ScrollYChanged__Noop;
	Usr_ScrollPanelVert__Linear_Layout__Initializer(context);
	Usr_ScrollPanelVert__Stretch__Initializer(context);
	Usr_ScrollPanelVert__Scroll_Layout__Initializer(context);
	Usr_ScrollPanelVert__TouchPanel__Initializer(context);
	Usr_ScrollPanelVert__Linear_Layout_3__Initializer(context);
	Usr_ScrollPanelVert__Stretch_2__Initializer(context);
	Usr_ScrollPanelVert__ESD_Scroll_Bar__Initializer(context);
	context->Widget.Container = Usr_ScrollPanelVert_Widget_Interface__Container(context);
}

void Usr_ScrollPanelVert_Update_Scroll(Usr_ScrollPanelVert *context);

void Usr_ScrollPanelVert_Update__Builtin(Usr_ScrollPanelVert *context)
{
	void *owner = context->Owner;
	ft_bool_t update_variable = Ft_Esd_TouchPanel_Touching(&context->TouchPanel);
	context->Touching = update_variable;
	ft_bool_t update_variable_1 = Ft_Esd_TouchPanel_Inside(&context->TouchPanel);
	context->Inside = update_variable_1;
	int update_variable_2 = Ft_Esd_TouchPanel_TouchX(&context->TouchPanel);
	context->TouchX = update_variable_2;
	int update_variable_3 = Ft_Esd_TouchPanel_TouchY(&context->TouchPanel);
	context->TouchY = update_variable_3;
	ft_int16_t update_variable_5 = context->ScrollY;
	context->Scroll_Layout.ScrollY = update_variable_5;
	uint8_t update_variable_6 = context->Align;
	context->Scroll_Layout.Align = update_variable_6;
	ft_bool_t update_variable_7 = context->TouchScroll;
	Ft_Esd_Widget_SetActive((Ft_Esd_Widget *)&context->TouchPanel, update_variable_7);
	
	// Calculate vertical scrollbar visibility
	ft_uint16_t left_5 = Ft_Esd_Layout_Scroll_RangeY(&context->Scroll_Layout);
	ft_uint8_t left_6 = context->ScrollBars;
	ft_uint8_t right_5 = ESD_VISIBLE_WHENNEEDED;
	int right_4 = left_6 == right_5;
	int left_4 = left_5 && right_4;
	ft_uint8_t left_7 = context->ScrollBars;
	ft_uint8_t right_7 = ESD_VISIBLE_ALWAYS;
	int right_6 = left_7 == right_7;
	int update_variable_11 = left_4 || right_6;
	Ft_Esd_Widget_SetActive((Ft_Esd_Widget *)&context->Linear_Layout_3, update_variable_11);
	ft_int16_t update_variable_12 = context->ScrollBarSize;
	Ft_Esd_Widget_SetWidth((Ft_Esd_Widget *)&context->Linear_Layout_3, update_variable_12);
	ft_int16_t update_variable_13 = context->ScrollY;
	context->ESD_Scroll_Bar.Value = update_variable_13;
	
	Ft_Esd_Widget_Update((Ft_Esd_Widget *)context);
}

void Usr_ScrollPanelVert_End(Usr_ScrollPanelVert *context)
{
	void *owner = context->Owner;
	Ft_Esd_Widget_End((Ft_Esd_Widget *)context);
	Ft_Esd_Widget_Detach((Ft_Esd_Widget *)&context->Linear_Layout); 
	Ft_Esd_Widget_Detach((Ft_Esd_Widget *)&context->Stretch); 
	Ft_Esd_Widget_Detach((Ft_Esd_Widget *)&context->Scroll_Layout); 
	Ft_Esd_Widget_Detach((Ft_Esd_Widget *)&context->TouchPanel); 
	Ft_Esd_Widget_Detach((Ft_Esd_Widget *)&context->Linear_Layout_3); 
	Ft_Esd_Widget_Detach((Ft_Esd_Widget *)&context->Stretch_2); 
	Ft_Esd_Widget_Detach((Ft_Esd_Widget *)&context->ESD_Scroll_Bar); 
}

void Usr_ScrollPanelVert_Update(Usr_ScrollPanelVert *context)
{
	void *owner = context->Owner;
	Usr_ScrollPanelVert_Update_Scroll(context);
	Usr_ScrollPanelVert_Update__Builtin(context);
}

Ft_Esd_Widget * Usr_ScrollPanelVert_Widget_Interface__Container(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	return (Ft_Esd_Widget *)&context->Scroll_Layout;
}

ft_int16_t Usr_ScrollPanelVert_MinY(Usr_ScrollPanelVert *context)
{
	void *owner = context->Owner;
	return context->Scroll_Layout.MinY;
}

ft_int16_t Usr_ScrollPanelVert_MaxY(Usr_ScrollPanelVert *context)
{
	void *owner = context->Owner;
	return context->Scroll_Layout.MaxY;
}

ft_int16_t Usr_ScrollPanelVert_RangeY(Usr_ScrollPanelVert *context)
{
	void *owner = context->Owner;
	return Ft_Esd_Layout_Scroll_RangeY(&context->Scroll_Layout);
}

ft_int16_t Usr_ScrollPanelVert_Width(Usr_ScrollPanelVert *context)
{
	void *owner = context->Owner;
	return Ft_Esd_Layout_Scroll_Width(&context->Scroll_Layout);
}

ft_int16_t Usr_ScrollPanelVert_Height(Usr_ScrollPanelVert *context)
{
	void *owner = context->Owner;
	return Ft_Esd_Layout_Scroll_Height(&context->Scroll_Layout);
}

void Usr_ScrollPanelVert_Scroll_Layout_ScrollYChanged__Writer(void *c, ft_int16_t value)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	context->ScrollY = value;
	context->ScrollYChanged(owner, value);
	context->ESD_Scroll_Bar.Value = value;
}

ft_argb32_t Usr_ScrollPanelVert_TouchPanel_Color__Property(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	Ft_Esd_Theme * theme = context->Theme(owner);
	return Ft_Esd_Theme_GetBackColor(theme);
}

int Usr_ScrollPanelVert_TouchPanel_Radius__Property(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	return context->Radius(owner);
}

ft_bool_t Usr_ScrollPanelVert_TouchPanel_Raised__Property(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	return context->RaiseBorder(owner);
}

ft_bool_t Usr_ScrollPanelVert_TouchPanel_ShowBorder__Property(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	return context->ShowPanel(owner);
}

ft_bool_t Usr_ScrollPanelVert_TouchPanel_IncludeDefaultTag__Property(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	return context->Active_Scroll(owner);
}

void Usr_ScrollPanelVert_TouchPanel_Up__Signal(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	context->Up(owner);
}

void Usr_ScrollPanelVert_TouchPanel_Down__Signal(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	context->Down(owner);
	int set_variable = context->TouchY;
	context->PreviousTouchY = set_variable;
	ft_int16_t set_variable_1 = context->ScrollY;
	context->PreviousScrollY = set_variable_1;
}

void Usr_ScrollPanelVert_TouchPanel_Tap__Signal(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	context->Tap(owner);
}

Ft_Esd_Theme * Usr_ScrollPanelVert_ESD_Scroll_Bar_Theme__Property(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	return context->Theme(owner);
}

ft_int16_t Usr_ScrollPanelVert_ESD_Scroll_Bar_Size__Property(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	return Ft_Esd_Layout_Scroll_Height(&context->Scroll_Layout);
}

ft_int16_t Usr_ScrollPanelVert_ESD_Scroll_Bar_Max__Property(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	return context->Scroll_Layout.MaxY;
}

ft_int16_t Usr_ScrollPanelVert_ESD_Scroll_Bar_Min__Property(void *c)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	return context->Scroll_Layout.MinY;
}

void Usr_ScrollPanelVert_ESD_Scroll_Bar_Changed__Writer(void *c, int value)
{
	Usr_ScrollPanelVert *context = (Usr_ScrollPanelVert *)c;
	void *owner = context->Owner;
	context->ScrollY = (int)value;
	context->Scroll_Layout.ScrollY = (int)value;
	context->ScrollYChanged(owner, (int)value);
}

void Usr_ScrollPanelVert_Update_Scroll(Usr_ScrollPanelVert *context)
{
	void *owner = context->Owner;
	ft_bool_t touching = context->Touching;
	ft_bool_t touchScroll = context->TouchScroll;
	
	if (touching && touchScroll)
	{
		int touchY = context->TouchY;
		int previousTouchY = context->PreviousTouchY;
		ft_int16_t previousScrollY = context->PreviousScrollY;
		
		int deltaY = touchY - previousTouchY;
		ft_int16_t newScrollY = previousScrollY - deltaY;
		
		// Clamp the scroll value
		ft_int16_t minY = context->Scroll_Layout.MinY;
		ft_int16_t maxY = context->Scroll_Layout.MaxY;
		
		if (newScrollY < minY)
			newScrollY = minY;
		if (newScrollY > maxY)
			newScrollY = maxY;
		
		context->ScrollY = newScrollY;
		context->Scroll_Layout.ScrollY = newScrollY;
		context->ScrollYChanged(owner, newScrollY);
	}
}

#ifdef ESD_SIMULATION
#include <stdlib.h>

typedef struct
{
	Usr_ScrollPanelVert Instance;
	ft_bool_t ShowPanel;
	ft_bool_t Active_Scroll;
	ft_bool_t RaiseBorder;
	Ft_Esd_Theme * Theme;
	int Radius;
} Usr_ScrollPanelVert__ESD;

ft_bool_t Usr_ScrollPanelVert__Get_ShowPanel__ESD(void *context) { return ((Usr_ScrollPanelVert__ESD *)context)->ShowPanel; }
void Usr_ScrollPanelVert__Set_ShowPanel__ESD(void *context, ft_bool_t value) { ((Usr_ScrollPanelVert__ESD *)context)->ShowPanel = value; }
ft_bool_t Usr_ScrollPanelVert__Get_Active_Scroll__ESD(void *context) { return ((Usr_ScrollPanelVert__ESD *)context)->Active_Scroll; }
void Usr_ScrollPanelVert__Set_Active_Scroll__ESD(void *context, ft_bool_t value) { ((Usr_ScrollPanelVert__ESD *)context)->Active_Scroll = value; }
ft_bool_t Usr_ScrollPanelVert__Get_RaiseBorder__ESD(void *context) { return ((Usr_ScrollPanelVert__ESD *)context)->RaiseBorder; }
void Usr_ScrollPanelVert__Set_RaiseBorder__ESD(void *context, ft_bool_t value) { ((Usr_ScrollPanelVert__ESD *)context)->RaiseBorder = value; }
Ft_Esd_Theme * Usr_ScrollPanelVert__Get_Theme__ESD(void *context) { return ((Usr_ScrollPanelVert__ESD *)context)->Theme; }
void Usr_ScrollPanelVert__Set_Theme__ESD(void *context, Ft_Esd_Theme * value) { ((Usr_ScrollPanelVert__ESD *)context)->Theme = value; }
int Usr_ScrollPanelVert__Get_Radius__ESD(void *context) { return ((Usr_ScrollPanelVert__ESD *)context)->Radius; }
void Usr_ScrollPanelVert__Set_Radius__ESD(void *context, int value) { ((Usr_ScrollPanelVert__ESD *)context)->Radius = value; }

void *Usr_ScrollPanelVert__Create__ESD()
{
	Usr_ScrollPanelVert__ESD *context = (Usr_ScrollPanelVert__ESD *)esd_malloc(sizeof(Usr_ScrollPanelVert__ESD));
	if (context)
	{
		Usr_ScrollPanelVert__Initializer(&context->Instance);
		context->Instance.Owner = context;
		context->ShowPanel = 1;
		context->Instance.ShowPanel = Usr_ScrollPanelVert__Get_ShowPanel__ESD;
		context->Active_Scroll = 0;
		context->Instance.Active_Scroll = Usr_ScrollPanelVert__Get_Active_Scroll__ESD;
		context->RaiseBorder = 1;
		context->Instance.RaiseBorder = Usr_ScrollPanelVert__Get_RaiseBorder__ESD;
		context->Theme = Ft_Esd_Theme_GetCurrent();
		context->Instance.Theme = Usr_ScrollPanelVert__Get_Theme__ESD;
		context->Radius = 4L;
		context->Instance.Radius = Usr_ScrollPanelVert__Get_Radius__ESD;
	}
	else
	{
		eve_printf_debug("Out of memory (Usr_ScrollPanelVert__Create__ESD)");
	}
	return context;
}

void Usr_ScrollPanelVert__Destroy__ESD(void *context)
{
	esd_free(context);
}

void Usr_ScrollPanelVert__Set_Touching__ESD(void *context, ft_bool_t value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.Touching = value; }
void Usr_ScrollPanelVert__Set_Inside__ESD(void *context, ft_bool_t value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.Inside = value; }
void Usr_ScrollPanelVert__Set_TouchX__ESD(void *context, int value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.TouchX = value; }
void Usr_ScrollPanelVert__Set_TouchY__ESD(void *context, int value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.TouchY = value; }
void Usr_ScrollPanelVert__Set_ScrollY__ESD(void *context, ft_int16_t value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.ScrollY = value; }
void Usr_ScrollPanelVert__Set_Align__ESD(void *context, uint8_t value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.Align = value; }
void Usr_ScrollPanelVert__Set_ScrollBars__ESD(void *context, ft_uint8_t value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.ScrollBars = value; }
void Usr_ScrollPanelVert__Set_TouchScroll__ESD(void *context, ft_bool_t value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.TouchScroll = value; }
void Usr_ScrollPanelVert__Set_PreviousTouchY__ESD(void *context, ft_int16_t value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.PreviousTouchY = value; }
void Usr_ScrollPanelVert__Set_PreviousScrollY__ESD(void *context, ft_int16_t value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.PreviousScrollY = value; }
void Usr_ScrollPanelVert__Set_ScrollBarSize__ESD(void *context, ft_int16_t value) { ((Usr_ScrollPanelVert__ESD *)context)->Instance.ScrollBarSize = value; }

#endif /* ESD_SIMULATION */

/* end of file */