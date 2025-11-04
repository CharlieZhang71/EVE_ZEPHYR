#include "Ft_Esd.h"
#include "FirstPage.h"
#include "Usr_RowWidget.h"
#include "Usr_ScrollPanelVert.h"
#include "AppScreen.h" 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Forward declare so we can call it before the definition */
typedef struct RowState RowState;
static void Free_Row(FirstPage *context, RowState *state);

static RowState *g_row_list = NULL;  // Linked list of row widgets

void FirstPage_Add_New_Pushed(void *c);
void FirstPage_End_Signal(void *c);

/* Per-row state carried via the widget's Owner pointer */
struct RowState {
    Usr_RowWidget *row;
    char *Column1Text;
    char *Column2Text;
    char *Column3Text;
    char *Column4Text;
    struct RowState *next;
    FirstPage *parent;
    void *screen;
};

/* --- Callbacks that match the widget API exactly --- */
static char *Row_Column1_Input(void *owner)
{
    RowState *s = (RowState *)owner;
    return (s && s->Column1Text) ? s->Column1Text : "";
}

static char *Row_Column2_Input(void *owner)
{
    RowState *s = (RowState *)owner;
    return (s && s->Column2Text) ? s->Column2Text : "";
}

static char *Row_Column3_Input(void *owner)
{
    RowState *s = (RowState *)owner;
    return (s && s->Column3Text) ? s->Column3Text : "";
}

static char *Row_Column4_Input(void *owner)
{
    RowState *s = (RowState *)owner;
    return (s && s->Column4Text) ? s->Column4Text : "";
}

/* Common callback for all four buttons in the row widget */
static void Row_Button_Pushed(void *owner)
{
    // owner is the row widget (Usr_RowWidget) - button.Owner points to row
    // The row's Owner field points to our RowState
    Usr_RowWidget *row = (Usr_RowWidget *)owner;
    RowState *s = (RowState *)row->Owner;
    
    if (s && s->parent) {
        FirstPage *page = (FirstPage *)s->parent;
        
        printf("Row button clicked, triggering Activate_Window signal\n");
        
        // Trigger the Activate_Window signal
        if (page->Activate_Window) {
            page->Activate_Window(page->Owner);
            printf("Activate_Window signal triggered\n");
        } else {
            printf("ERROR: Activate_Window is NULL\n");
        }
    }
}


ESD_METHOD(FirstPage_Update_Signal, Context = FirstPage)
void FirstPage_Update_Signal(void *c)
{
    FirstPage *context = (FirstPage *)c;
    
    printf("\n*** FirstPage_Update_Signal CALLED ***\n");
    
    // Restore row widgets from saved state
    RowState *state = g_row_list;
    
    // If there are no rows in the list, nothing to restore
    if (!state) {
        printf("No rows in saved state, nothing to restore\n");
        printf("*** FirstPage_Update_Signal DONE ***\n\n");
        return;
    }
    
    // Check if rows are already attached (avoid duplicates)
    if (state->row && state->row->Widget.Parent != NULL) {
        printf("Rows already attached, skipping restoration\n");
        printf("*** FirstPage_Update_Signal DONE ***\n\n");
        return;
    }
    
    printf("Restoring rows from saved state...\n");
    int rowNum = 1;
    
    // Get or create the linear container first
    // NOTE: Changed to use Usr_ScrollPanelVert instead of ESD_Scroll_Panel
    Ft_Esd_Widget *scrollWidget = (Ft_Esd_Widget *)&context->Vertical_Scroll_Panel.Scroll_Layout;
    Ft_Esd_Layout_Linear *linearContainer = NULL;
    
    Ft_Esd_Widget *child = scrollWidget->First;
    while (child) {
        if (child->ClassId == Ft_Esd_Layout_Linear_CLASSID) {
            linearContainer = (Ft_Esd_Layout_Linear *)child;
            printf("Found existing Linear Layout container\n");
            break;
        }
        child = child->Next;
    }
    
    if (!linearContainer) {
        printf("Creating new Linear Layout container for restoration\n");
        linearContainer = (Ft_Esd_Layout_Linear *)calloc(1, sizeof(Ft_Esd_Layout_Linear));
        if (!linearContainer) {
            printf("ERROR: Failed to allocate Linear Layout\n");
            return;
        }
        
        Ft_Esd_Layout_Linear__Initializer(linearContainer);
        linearContainer->Widget.LocalX = 0;
        linearContainer->Widget.LocalY = 0;
        linearContainer->Widget.LocalWidth = 1121;
        linearContainer->Widget.LocalHeight = 76;
        linearContainer->Orientation = ESD_ORIENTATION_VERTICAL;
        linearContainer->Spacing = 5;
        linearContainer->Align = ESD_ALIGN_TOPFILL;
        linearContainer->AutoResize = ESD_AUTORESIZE_HEIGHT;
        linearContainer->Overflow = ESD_OVERFLOW_ALLOW;
        
        Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)linearContainer, scrollWidget);
        linearContainer->Widget.Active = 1;
        Ft_Esd_Widget_Start((Ft_Esd_Widget *)linearContainer);
    }
    
    while (state) {
        printf("Restoring row %d...\n", rowNum);
        state->parent = context;
        
        // Recreate the row widget
        Usr_RowWidget *row = (Usr_RowWidget *)calloc(1, sizeof(Usr_RowWidget));
        if (!row) {
            printf("ERROR: Failed to allocate row widget\n");
            break;
        }
        
        state->row = row;
        
        Usr_RowWidget__Initializer(row);
        row->Owner = state;
        row->Column_1_Input = Row_Column1_Input;
        row->Column_2_Input = Row_Column2_Input;
        row->Column_3_Input = Row_Column3_Input;
        row->Column_4_Input = Row_Column4_Input;
        
        // Fix the internal Linear_Layout dimensions
        row->Linear_Layout.Widget.LocalX = 0;
        row->Linear_Layout.Widget.LocalY = 0;
        row->Linear_Layout.Widget.LocalWidth = 1121;
        row->Linear_Layout.Widget.LocalHeight = 76;
        
        // CRITICAL: The button Owners are already set correctly by the initializer
        // They point to the row widget, which is what the Text property functions expect
        
        // Add row widget to layout
        row->Widget.LocalWidth = 1121;
        row->Widget.LocalHeight = 76;
        Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)row, (Ft_Esd_Widget *)linearContainer);
        row->Widget.Active = 1;
        
        // Set up button Pushed callbacks (but don't change Owner)
        row->ESD_Push_Button_1.Pushed = Row_Button_Pushed;
        row->ESD_Push_Button_2.Pushed = Row_Button_Pushed;
        row->ESD_Push_Button_3.Pushed = Row_Button_Pushed;
        row->ESD_Push_Button_4.Pushed = Row_Button_Pushed;
        
        Usr_RowWidget_Start(row);
        printf("Row %d restored\n", rowNum);
        
        state = state->next;
        rowNum++;
    }
    
    printf("*** FirstPage_Update_Signal DONE ***\n\n");
}

ESD_METHOD(FirstPage_End_Signal, Context = FirstPage)
void FirstPage_End_Signal(void *c)
{
    FirstPage *context = (FirstPage *)c;
    
    printf("\n*** FirstPage_End_Signal CALLED ***\n");
    
    // Detach and free row widgets
    RowState *state = g_row_list;
    int rowNum = 1;
    while (state) {
        if (state->row) {
            printf("Detaching and freeing row %d...\n", rowNum);
            Ft_Esd_Widget_Detach((Ft_Esd_Widget *)state->row);
            Usr_RowWidget_End(state->row);
            free(state->row);
            state->row = NULL;
        }
        state = state->next;
        rowNum++;
    }
    
    // Also clean up the Linear Layout container
    printf("Cleaning up Linear Layout container...\n");
    // NOTE: Changed to use Usr_ScrollPanelVert instead of ESD_Scroll_Panel
    Ft_Esd_Widget *scrollWidget = (Ft_Esd_Widget *)&context->Vertical_Scroll_Panel.Scroll_Layout;
    Ft_Esd_Widget *child = scrollWidget->First;
    
    while (child) {
        Ft_Esd_Widget *next = child->Next;  // Save next before detaching
        if (child->ClassId == Ft_Esd_Layout_Linear_CLASSID) {
            printf("Found Linear Layout, detaching...\n");
            Ft_Esd_Widget_Detach(child);
            Ft_Esd_Widget_End(child);
            free(child);
            printf("Linear Layout cleaned up\n");
            break;  // Only one Linear Layout expected
        }
        child = next;
    }
    
    printf("*** FirstPage_End_Signal DONE ***\n\n");
}


static void Free_Row(FirstPage *context, RowState *state)
{
    (void)context;
    if (!state) return;

    if (state->row) {
        Ft_Esd_Widget_Detach((Ft_Esd_Widget *)state->row);
        Usr_RowWidget_End(state->row);
        free(state->row);
    }
    free(state->Column1Text);
    free(state->Column2Text);
    free(state->Column3Text);
    free(state->Column4Text);
    free(state);
}


ESD_METHOD(FirstPage_Add_New_Pushed, Context = FirstPage)
void FirstPage_Add_New_Pushed(void *c)        
{
    FirstPage *context = (FirstPage *)c;
    
    printf("\n========================================\n");
    printf("ADD NEW BUTTON CLICKED\n");
    printf("========================================\n");
    
    static int row_counter = 1;
    int current_row = row_counter;  // Capture the value before incrementing
    
    printf("Creating row #%d\n", current_row);
    
    RowState *state = (RowState *)calloc(1, sizeof(RowState));
    if (!state) {
        printf("ERROR: Failed to allocate RowState\n");
        return;
    }
    printf("RowState allocated at %p\n", (void*)state);
    
    Usr_RowWidget *row = (Usr_RowWidget *)calloc(1, sizeof(Usr_RowWidget));
    if (!row) { 
        printf("ERROR: Failed to allocate Usr_RowWidget\n");
        free(state); 
        return; 
    }
    printf("Usr_RowWidget allocated at %p\n", (void*)row);
    
    printf("Initializing row widget...\n");
    Usr_RowWidget__Initializer(row);
    row->Owner = state;
    row->Column_1_Input = Row_Column1_Input;
    row->Column_2_Input = Row_Column2_Input;
    row->Column_3_Input = Row_Column3_Input;
    row->Column_4_Input = Row_Column4_Input;
    
    // Fix the internal Linear_Layout dimensions (they're set too small by default)
    row->Linear_Layout.Widget.LocalX = 0;
    row->Linear_Layout.Widget.LocalY = 0;
    row->Linear_Layout.Widget.LocalWidth = 1121;
    row->Linear_Layout.Widget.LocalHeight = 76;
    
    printf("Fixed internal Linear_Layout dimensions to 1121x76\n");
    
    printf("Row widget initialized\n");
    
    state->row = row;
    state->parent = context;
    
    // Allocate text for each column
    printf("Allocating column text buffers...\n");
    state->Column1Text = (char *)malloc(64);
    state->Column2Text = (char *)malloc(64);
    state->Column3Text = (char *)malloc(64);
    state->Column4Text = (char *)malloc(64);
    
    if (state->Column1Text) snprintf(state->Column1Text, 64, "Col1-%d", current_row);
    if (state->Column2Text) snprintf(state->Column2Text, 64, "Col2-%d", current_row);
    if (state->Column3Text) snprintf(state->Column3Text, 64, "Col3-%d", current_row);
    if (state->Column4Text) snprintf(state->Column4Text, 64, "Col4-%d", current_row);
    
    printf("Column texts set: %s, %s, %s, %s\n", 
           state->Column1Text, state->Column2Text, 
           state->Column3Text, state->Column4Text);
    
    row_counter++;  // Increment for next row
    
    // Append to END of list instead of beginning
    printf("\nAdding to linked list...\n");
    if (g_row_list == NULL) {
        g_row_list = state;
        printf("First row in list\n");
    } else {
        RowState *last = g_row_list;
        int count = 1;
        while (last->next != NULL) {
            last = last->next;
            count++;
        }
        last->next = state;
        printf("Added as row #%d in list\n", count + 1);
    }
    
    // Check if there's already a Linear Layout child in Scroll_Layout
    printf("\nLooking for scroll widget...\n");
    // NOTE: Changed to use Usr_ScrollPanelVert instead of ESD_Scroll_Panel
    Ft_Esd_Widget *scrollWidget = (Ft_Esd_Widget *)&context->Vertical_Scroll_Panel.Scroll_Layout;
    printf("Scroll widget at %p\n", (void*)scrollWidget);
    
    Ft_Esd_Layout_Linear *linearContainer = NULL;
    
    // Look for existing Linear Layout
    printf("Searching for existing Linear Layout...\n");
    Ft_Esd_Widget *child = scrollWidget->First;
    int childCount = 0;
    while (child) {
        childCount++;
        printf("  Child %d: ClassId = 0x%08X\n", childCount, child->ClassId);
        if (child->ClassId == Ft_Esd_Layout_Linear_CLASSID) {
            linearContainer = (Ft_Esd_Layout_Linear *)child;
            printf("  Found existing Linear Layout at %p\n", (void*)linearContainer);
            break;
        }
        child = child->Next;
    }
    
    // If no Linear Layout exists, create one
    if (!linearContainer) {
        printf("No Linear Layout found, creating new one...\n");
        linearContainer = (Ft_Esd_Layout_Linear *)calloc(1, sizeof(Ft_Esd_Layout_Linear));
        if (!linearContainer) {
            printf("ERROR: Failed to allocate Linear Layout\n");
            free(row);
            free(state->Column1Text);
            free(state->Column2Text);
            free(state->Column3Text);
            free(state->Column4Text);
            free(state);
            return;
        }
        printf("Linear Layout allocated at %p\n", (void*)linearContainer);
        
        Ft_Esd_Layout_Linear__Initializer(linearContainer);
        linearContainer->Widget.LocalX = 0;
        linearContainer->Widget.LocalY = 0;
        linearContainer->Widget.LocalWidth = 1121;
        linearContainer->Widget.LocalHeight = 76;  // Will grow with AutoResize
        linearContainer->Orientation = ESD_ORIENTATION_VERTICAL;
        linearContainer->Spacing = 5;
        linearContainer->Align = ESD_ALIGN_TOPFILL;
        linearContainer->AutoResize = ESD_AUTORESIZE_HEIGHT;  // Auto-grow vertically
        linearContainer->Overflow = ESD_OVERFLOW_ALLOW;
        printf("Linear Layout initialized\n");
        
        printf("Inserting Linear Layout into scroll widget...\n");
        Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)linearContainer, scrollWidget);
        linearContainer->Widget.Active = 1;
        printf("Linear Layout inserted and activated\n");
        
        printf("Starting Linear Layout widget...\n");
        Ft_Esd_Widget_Start((Ft_Esd_Widget *)linearContainer);
        printf("Linear Layout started\n");
    } else {
        printf("Using existing Linear Layout at %p\n", (void*)linearContainer);
    }
    
    // Now add row widget to the LINEAR layout (not scroll layout)
    printf("\nSetting row widget dimensions...\n");
    row->Widget.LocalWidth = 1121;
    row->Widget.LocalHeight = 76;
    printf("Row widget size: %dx%d\n", row->Widget.LocalWidth, row->Widget.LocalHeight);
    
    printf("Inserting row widget into Linear Layout...\n");
    Ft_Esd_Widget_InsertBottom((Ft_Esd_Widget *)row, (Ft_Esd_Widget *)linearContainer);
    printf("Row widget inserted\n");
    
    row->Widget.Active = 1;
    printf("Row widget activated\n");
    
    // Set up button Pushed callbacks (Owner stays as row widget for Text callbacks)
    printf("\nSetting up button callbacks...\n");
    row->ESD_Push_Button_1.Pushed = Row_Button_Pushed;
    row->ESD_Push_Button_2.Pushed = Row_Button_Pushed;
    row->ESD_Push_Button_3.Pushed = Row_Button_Pushed;
    row->ESD_Push_Button_4.Pushed = Row_Button_Pushed;
    printf("All button callbacks set\n");
    
    printf("\n=== Row %d Summary ===\n", current_row);
    printf("Button texts: %s, %s, %s, %s\n", 
           state->Column1Text, state->Column2Text, 
           state->Column3Text, state->Column4Text);
    printf("Row widget at: %p\n", (void*)row);
    printf("Parent container: %p\n", (void*)linearContainer);
    
    printf("\nStarting row widget...\n");
    Usr_RowWidget_Start(row);
    printf("Row widget started\n");
    
    // Debug: Check the widget hierarchy
    printf("\n=== Widget Hierarchy Debug ===\n");
    printf("Row widget at: %p\n", (void*)row);
    printf("Row widget Owner (state): %p\n", (void*)row->Owner);
    printf("Row widget Active: %d\n", row->Widget.Active);
    printf("Row widget Parent: %p\n", (void*)row->Widget.Parent);
    printf("Internal Linear_Layout Active: %d\n", row->Linear_Layout.Widget.Active);
    printf("Internal Linear_Layout size: %dx%d at (%d,%d)\n",
           row->Linear_Layout.Widget.LocalWidth,
           row->Linear_Layout.Widget.LocalHeight,
           row->Linear_Layout.Widget.LocalX,
           row->Linear_Layout.Widget.LocalY);
    printf("Button 1 at: %p\n", (void*)&row->ESD_Push_Button_1);
    printf("Button 1 Owner: %p\n", (void*)row->ESD_Push_Button_1.Owner);
    printf("Button 1 Active: %d, size: %dx%d\n", 
           row->ESD_Push_Button_1.Widget.Active,
           row->ESD_Push_Button_1.Widget.LocalWidth,
           row->ESD_Push_Button_1.Widget.LocalHeight);
    printf("Button 1 Text callback: %p\n", (void*)row->ESD_Push_Button_1.Text);
    printf("Row Column_1_Input callback: %p\n", (void*)row->Column_1_Input);
    printf("Calling Button 1 Text callback directly...\n");
    if (row->ESD_Push_Button_1.Text) {
        // The Text callback receives button.Owner as parameter
        const char *text = row->ESD_Push_Button_1.Text(row->ESD_Push_Button_1.Owner);
        printf("Button 1 Text result: %s\n", text ? text : "NULL");
    }
    printf("Calling Column_1_Input directly with state...\n");
    if (row->Column_1_Input) {
        const char *text2 = row->Column_1_Input(state);
        printf("Column_1_Input result: %s\n", text2 ? text2 : "NULL");
    }
    printf("==============================\n");
    
    // Force recalculation
    printf("\nForcing widget recalculation...\n");
    linearContainer->Widget.Recalculate = FT_TRUE;
    scrollWidget->Recalculate = FT_TRUE;
    printf("Recalculation flags set\n");
    
    printf("\n========================================\n");
    printf("ADD NEW COMPLETED - Row %d added\n", current_row);
    printf("========================================\n\n");
}