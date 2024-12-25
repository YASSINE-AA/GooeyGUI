/*
 Copyright (c) 2024 Yassine Ahmed Ali

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gooey.h"
#include <ctype.h>

GooeyTheme default_theme;

GooeyTheme *active_theme = NULL;
GooeyBackend *active_backend = NULL;
GooeyBackends ACTIVE_BACKEND = -1;

void (*MessageBoxCallback[100])(int option);
GooeyWindow *CurrentMessageBox = NULL;

int window_width = 400, window_height = 700;
GOOEY_CURSOR currently_set_cursor = GOOEY_CURSOR_ARROW;
int called = 0;

int Gooey_Init(GooeyBackends backend)
{
    unsigned long primaryColor = (unsigned long)strtol("0x2196F3", NULL, 0);
    unsigned long baseColor = (unsigned long)strtol("0xFFFFFF", NULL, 0);
    unsigned long neutralColor = (unsigned long)strtol("0x000000", NULL, 0);
    unsigned long widgetBaseColor = (unsigned long)strtol("0xD3D3D3", NULL, 0);
    unsigned long dangerColor = (unsigned long)strtol("0xE91E63", NULL, 0);
    unsigned long infoColor = (unsigned long)strtol("0x2196F3", NULL, 0);
    unsigned long successColor = (unsigned long)strtol("0x00A152", NULL, 0);

    default_theme = (GooeyTheme){.base = baseColor, .neutral = neutralColor, .primary = primaryColor, .widget_base = widgetBaseColor, .danger = dangerColor, .info = infoColor, .success = successColor};

    active_theme = &default_theme;

    switch (backend)
    {

    case GLFW:
        LOG_INFO("using GLFW backend.");
        active_backend = &glfw_backend;

        break;
    default:
        break;
    }
    if (active_backend->Init() < 0)
    {
        LOG_CRITICAL("Backend initialization failed.");
        return -1;
    }

    ACTIVE_BACKEND = backend;
    LOG_INFO("Gooey initialized successfully.");

    return 0;
}

GooeyWindow GooeyWindow_Create(const char *title, int width, int height, bool visibilty)
{

    GooeyWindow win = active_backend->CreateWindow(title, width, height);
    active_backend->GetWinDim(&window_width, &window_height, win.creation_id);

    win.type = WINDOW_REGULAR;
    win.buttons = malloc(sizeof(GooeyButton) * MAX_WIDGETS);
    win.labels = malloc(sizeof(GooeyLabel) * MAX_WIDGETS);
    win.checkboxes = malloc(sizeof(GooeyCheckbox) * MAX_WIDGETS);
    win.radio_buttons = malloc(sizeof(GooeyRadioButton) * MAX_WIDGETS);
    win.radio_button_groups = malloc(sizeof(GooeyRadioButtonGroup) * MAX_WIDGETS);

    win.sliders = malloc(sizeof(GooeySlider) * MAX_WIDGETS);
    win.dropdowns = malloc(sizeof(GooeyDropdown) * MAX_WIDGETS);
    win.textboxes = malloc(sizeof(GooeyTextbox) * MAX_WIDGETS);
    win.layouts = malloc(sizeof(GooeyLayout) * MAX_WIDGETS);
    win.lists = malloc(sizeof(GooeyList) * MAX_WIDGETS);
    win.canvas = malloc(sizeof(GooeyCanvas) * MAX_WIDGETS);
    win.menu = NULL;
    win.widgets = malloc(sizeof(GooeyWidget *) * MAX_WIDGETS);

    win.visibility = visibilty;
    win.canvas_count = 0;
    win.button_count = 0;
    win.label_count = 0;
    win.checkbox_count = 0;
    win.radio_button_count = 0;
    win.radio_button_group_count = 0;
    win.slider_count = 0;
    win.dropdown_count = 0;
    win.textboxes_count = 0;
    win.layout_count = 0;
    win.list_count = 0;
    win.widget_count = 0;
    LOG_INFO("Window created with dimensions (%d, %d).", width, height);
    return win;
}

GooeyWindow GooeyWindow_CreateChild(const char *title, int width, int height, bool visibility)
{
    GooeyWindow win = active_backend->SpawnWindow(title, width, height, visibility);

    win.type = WINDOW_REGULAR;
    win.buttons = malloc(sizeof(GooeyButton) * MAX_WIDGETS);
    win.labels = malloc(sizeof(GooeyLabel) * MAX_WIDGETS);
    win.checkboxes = malloc(sizeof(GooeyCheckbox) * MAX_WIDGETS);
    win.radio_buttons = malloc(sizeof(GooeyRadioButton) * MAX_WIDGETS);
    win.radio_button_groups = malloc(sizeof(GooeyRadioButtonGroup) * MAX_WIDGETS);

    win.sliders = malloc(sizeof(GooeySlider) * MAX_WIDGETS);
    win.dropdowns = malloc(sizeof(GooeyDropdown) * MAX_WIDGETS);
    win.textboxes = malloc(sizeof(GooeyTextbox) * MAX_WIDGETS);
    win.layouts = malloc(sizeof(GooeyLayout) * MAX_WIDGETS);
    win.lists = malloc(sizeof(GooeyList) * MAX_WIDGETS);
    win.canvas = malloc(sizeof(GooeyCanvas) * MAX_WIDGETS);

    win.menu = NULL;
    win.widgets = malloc(sizeof(GooeyWidget *) * MAX_WIDGETS);
    win.visibility = visibility;
    win.canvas_count = 0;
    win.button_count = 0;
    win.label_count = 0;
    win.checkbox_count = 0;
    win.radio_button_count = 0;
    win.radio_button_group_count = 0;
    win.slider_count = 0;
    win.dropdown_count = 0;
    win.textboxes_count = 0;
    win.layout_count = 0;
    win.list_count = 0;
    win.widget_count = 0;
    LOG_INFO("Window created with dimensions (%d, %d).", width, height);
    return win;
}

void GooeyWindow_RegisterWidget(GooeyWindow *win, GooeyWidget *widget)
{
    if (win && win->widgets)
        win->widgets[win->widget_count++] = widget;
}

bool GooeyWindow_HandleCursorChange(GooeyWindow *win, GOOEY_CURSOR *cursor, int x, int y)
{
    for (int i = 0; i < win->widget_count; ++i)
    {
        if (x >= win->widgets[i]->x && x <= win->widgets[i]->x + win->widgets[i]->width &&
            y >= win->widgets[i]->y && y <= win->widgets[i]->y + win->widgets[i]->height)
        {
            switch (win->widgets[i]->type)
            {
            case WIDGET_TEXTBOX:
                *cursor = GOOEY_CURSOR_TEXT;
                break;

            case WIDGET_LABEL:
                *cursor = GOOEY_CURSOR_ARROW;
                break;
            default:

                *cursor = GOOEY_CURSOR_HAND;
                break;
            }

            return true;
        }
    }

    return false;
}

void GooeyWindow_setTheme(const char *fontPath)
{

    if (active_backend)
    {
        *active_theme = parser_load_theme_from_file(fontPath);
        active_backend->UpdateBackground();
    }
}

void GooeyRadioButtonGroup_Draw(GooeyWindow *win)
{
    for (int i = 0; i < win->radio_button_group_count; ++i)
    {
        GooeyRadioButtonGroup *group = &win->radio_button_groups[i];
        for (int j = 0; j < group->button_count; ++j)
        {
            GooeyRadioButton *button = &group->buttons[j];
            int button_center_x = button->core.x + RADIO_BUTTON_RADIUS;
            int button_center_y = button->core.y + RADIO_BUTTON_RADIUS;

            int label_width = active_backend->GetTextWidth(button->label, strlen(button->label));

            int label_x = ACTIVE_BACKEND == X11 ? button->core.x + RADIO_BUTTON_RADIUS * 2 + 10 : button->core.x + RADIO_BUTTON_RADIUS * 2;
            int label_y = ACTIVE_BACKEND == X11 ? button->core.y + RADIO_BUTTON_RADIUS + 5 : button->core.y + RADIO_BUTTON_RADIUS / 2;
            active_backend->DrawText(label_x, label_y, button->label, active_theme->neutral, 0.25f, win->creation_id);
            active_backend->SetForeground(active_theme->neutral);
            active_backend->FillArc(button->core.x, button->core.y, RADIO_BUTTON_RADIUS * 2, RADIO_BUTTON_RADIUS * 2, 0, 360 * 64, win->creation_id);
            if (button->selected)
            {
                active_backend->SetForeground(active_theme->primary);
                active_backend->FillArc(ACTIVE_BACKEND == X11 ? button->core.x + 2 : button->core.x, ACTIVE_BACKEND == X11 ? button->core.y + 2 : button->core.y, RADIO_BUTTON_RADIUS * 1.5, RADIO_BUTTON_RADIUS * 1.5, 0, 360 * 64, win->creation_id);
            }
            else
            {
                active_backend->SetForeground(active_theme->base);

                active_backend->FillArc(ACTIVE_BACKEND == X11 ? button->core.x + 2 : button->core.x, ACTIVE_BACKEND == X11 ? button->core.y + 2 : button->core.y, RADIO_BUTTON_RADIUS * 1.5, RADIO_BUTTON_RADIUS * 1.5, 0, 360 * 64, win->creation_id);
            }
        }
    }
}
GooeyRadioButtonGroup *GooeyRadioButtonGroup_Create(GooeyWindow *win)
{
    if (win->radio_button_group_count >= MAX_WIDGETS)
    {
        LOG_ERROR("Cannot create more radio button groups. Maximum limit reached.\n");
        return NULL;
    }
    GooeyRadioButtonGroup *group = &win->radio_button_groups[win->radio_button_group_count++];
    group->button_count = 0;
    LOG_INFO("Radio button group created and added to window.");

    return group;
}

GooeyRadioButton *GooeyRadioButtonGroup_AddChild(GooeyWindow *win, GooeyRadioButtonGroup *group, int x, int y, const char *label, void (*callback)(bool))
{
    if (group->button_count >= MAX_RADIO_BUTTONS)
    {
        LOG_ERROR("Cannot add more radio buttons to the group. Maximum limit reached.\n");
        return NULL;
    }
    GooeyRadioButton *button = &group->buttons[group->button_count++];
    button->core.x = x;
    button->core.y = y;

    button->core.type = WIDGET_RADIOBUTTON;
    button->selected = false;
    if (label)
        strncpy(button->label, label, sizeof(button->label) - 1);
    else
        sprintf(button->label, "Radio button %d", group->button_count);

    LOG_INFO("Added child to radio button group at x=%d, y=%d.", x, y);

    return button;
}

bool GooeyRadioButtonGroup_HandleClick(GooeyWindow *win, int x, int y)
{
    for (int i = 0; i < win->radio_button_group_count; ++i)
    {
        GooeyRadioButtonGroup *group = &win->radio_button_groups[i];
        for (int j = 0; j < group->button_count; ++j)
        {
            GooeyRadioButton *button = &group->buttons[j];
            int dx = x - (button->core.x + RADIO_BUTTON_RADIUS);
            int dy = y - (button->core.y + RADIO_BUTTON_RADIUS);

            if (dx * dx + dy * dy <= (RADIO_BUTTON_RADIUS + 10) * (RADIO_BUTTON_RADIUS + 10))
            {
                for (int k = 0; k < group->button_count; ++k)
                {
                    group->buttons[k].selected = false;
                }

                button->selected = true;

                if (button->callback)
                {
                    button->callback(true);
                }
                return true;
            }
        }
    }
    return false;
}

void GooeyWindow_Redraw(GooeyWindow *win)

{

    active_backend->Clear(win->creation_id);

    GooeyList_Draw(win);

    for (int i = 0; i < win->canvas_count; ++i)
    {
        for (int j = 0; j < win->canvas[i].element_count; ++j)
        {
            CanvaElement *element = &win->canvas[i].elements[j];
            switch (element->operation)
            {
            case CANVA_DRAW_RECT:
                CanvasDrawRectangleArgs *args = (CanvasDrawRectangleArgs *)element->args;
                if (args->is_filled)
                    active_backend->FillRectangle(args->x, args->y, args->width, args->height, args->color, win->creation_id);
                else
                    active_backend->DrawRectangle(args->x, args->y, args->width, args->height, args->color, win->creation_id);
                break;

            case CANVA_DRAW_LINE:
                CanvasDrawLineArgs *args_line = (CanvasDrawLineArgs *)element->args;
                active_backend->DrawLine(args_line->x1, args_line->y1, args_line->x2, args_line->y2, args_line->color, win->creation_id);
                break;
            case CANVA_DRAW_ARC:
                CanvasDrawArcArgs *args_arc = (CanvasDrawArcArgs *)element->args;
                active_backend->FillArc(args_arc->x_center, args_arc->y_center, args_arc->width, args_arc->height, args_arc->angle1, args_arc->angle2, win->creation_id);
                break;
            case CANVA_DRAW_SET_FG:
                CanvasSetFGArgs *args_fg = (CanvasSetFGArgs *)element->args;
                active_backend->SetForeground(args_fg->color);
                break;
            default:
                break;
            }
        }
    }

    for (int i = 0; i < win->button_count; ++i)
    {
        GooeyButton_Draw(win, &win->buttons[i]);
    }
    for (int i = 0; i < win->textboxes_count; ++i)
    {
        GooeyTextbox_Draw(win, i);
    }

    for (int i = 0; i < win->checkbox_count; ++i)
    {
        GooeyCheckbox *checkbox = &win->checkboxes[i];

        int label_width = active_backend->GetTextWidth(checkbox->label, strlen(checkbox->label));
        int label_x = checkbox->core.x + CHECKBOX_SIZE + 10;
        int label_y = checkbox->core.y + (CHECKBOX_SIZE / 2) + 5;
        active_backend->DrawText(label_x, label_y, checkbox->label, active_theme->neutral, 0.25f, win->creation_id);

        active_backend->DrawRectangle(checkbox->core.x, checkbox->core.y,
                                      checkbox->core.width, checkbox->core.height, active_theme->neutral, win->creation_id);
        active_backend->FillRectangle(checkbox->core.x + 1, checkbox->core.y + 1,
                                      checkbox->core.width - 2, checkbox->core.height - 2, active_theme->base, win->creation_id);

        if (checkbox->checked)
        {
            active_backend->FillRectangle(checkbox->core.x + 5, checkbox->core.y + 5,
                                          checkbox->core.width - 10, checkbox->core.height - 10, active_theme->primary, win->creation_id);
        }
    }
    GooeyLabel_Draw(win);
    GooeyRadioButtonGroup_Draw(win);

    for (int i = 0; i < win->slider_count; ++i)
    {
        GooeySlider *slider = &win->sliders[i];

        active_backend->FillRectangle(slider->core.x,
                                      slider->core.y, slider->core.width, slider->core.height, active_theme->widget_base, win->creation_id);

        int thumb_x = slider->core.x + (slider->value - slider->min_value) *
                                           slider->core.width /
                                           (slider->max_value - slider->min_value);

        active_backend->FillRectangle(thumb_x - 5,
                                      slider->core.y - 5, 10, slider->core.height + 10, active_theme->primary, win->creation_id);

        if (slider->show_hints)
        {

            char min_value[20];
            char max_value[20];
            char value[20];
            sprintf(min_value, "%ld", slider->min_value);
            sprintf(max_value, "%ld", slider->max_value);
            sprintf(value, "%ld", slider->value);
            int min_value_width = active_backend->GetTextWidth(min_value, strlen(min_value));
            int max_value_width = active_backend->GetTextWidth(max_value, strlen(max_value));
            int value_width = active_backend->GetTextWidth(value, strlen(value));

            active_backend->DrawText(
                slider->core.x - min_value_width - 5, slider->core.y + 5,
                min_value, active_theme->neutral, 0.25f, win->creation_id);
            active_backend->DrawText(
                slider->core.x + slider->core.width + 5, slider->core.y + 5,
                max_value, active_theme->neutral, 0.25f, win->creation_id);
            if (slider->value != 0)
                active_backend->DrawText(thumb_x - 5,
                                         slider->core.y + 25, value, active_theme->neutral, 0.25f, win->creation_id);
        }
        active_backend->SetForeground(active_theme->neutral);
    }

    GooeyDropdown_Draw(win);
    GooeyMenu_Draw(win);

    active_backend->Render(win->creation_id);
    usleep(16667);
}

void GooeyWindow_MakeVisible(GooeyWindow *win, bool visibility)
{
    active_backend->MakeWindowVisible(win->creation_id, visibility);
    GooeyWindow_Redraw(win);
}

void __msgbox_ok(void)
{
    void (*callback)(int option) = MessageBoxCallback[active_backend->GetCurrentClickedWindow() - 1];
    if (callback)
    {
        callback(0);
    }
    active_backend->HideCurrentChild();
}

void __msgbox_cancel(void)
{

    void (*callback)(int option) = MessageBoxCallback[active_backend->GetCurrentClickedWindow() - 1];
    if (callback)
    {
        callback(1);
    }
    active_backend->HideCurrentChild();
}

void GooeyWindow_MakeResizable(GooeyWindow *msgBoxWindow, bool is_resizable)
{
    active_backend->MakeWindowResizable(0, msgBoxWindow->creation_id);
}

void GooeyButton_SetHighlight(GooeyButton *button, bool is_highlighted)
{
    button->is_highlighted = true;
}

GooeyWindow GooeyMessageBox_Create(const char *title, const char *message, MSGBOX_TYPE type, void (*callback)(int option))
{

    GooeyWindow window = GooeyWindow_CreateChild(title, 500, 195, 0);
    GooeyWindow_MakeResizable(&window, 0);
    MessageBoxCallback[window.creation_id - 1] = callback;
    window.type = WINDOW_MSGBOX;
    unsigned long msgbox_title_color;

    switch (type)
    {
    case MSGBOX_INFO:
        msgbox_title_color = active_theme->info;
        break;
    case MSGBOX_FAIL:
        msgbox_title_color = active_theme->danger;
        break;
    case MSGBOX_SUCCES:
        msgbox_title_color = active_theme->success;
        break;
    default:
        break;
    }
    GooeyLayout *layout = GooeyLayout_Create(&window, LAYOUT_VERTICAL, 20, 40, 400, 200);
    GooeyLayout *button_layout = GooeyLayout_Create(&window, LAYOUT_HORIZONTAL, 0, 0, 200, 110);
    char color_buffer[20];
    snprintf(color_buffer, sizeof(color_buffer), "0x%lx", msgbox_title_color);
    GooeyLabel *title_label = GooeyLabel_Add(&window, title, 0.5f, 50, 50);
    GooeyLabel_SetColor(title_label, color_buffer);
    GooeyLayout_AddChild(layout, title_label);

    GooeyLayout_AddChild(layout, GooeyLabel_Add(&window, message, 0.3f, 0, 0));
    GooeyLayout_AddChild(layout, button_layout);
    GooeyButton *ok_button = GooeyButton_Add(&window, "Ok", 0, 0, 20, 40, __msgbox_ok);
    GooeyButton_SetHighlight(ok_button, 1);
    GooeyLayout_AddChild(button_layout, ok_button);
    GooeyLayout_AddChild(button_layout, GooeyButton_Add(&window, "Cancel", 0, 0, 20, 40, __msgbox_cancel));

    GooeyLayout_Build(layout);
    return window;
}

void GooeyMessageBox_Show(GooeyWindow *msgBoxWindow)
{
    CurrentMessageBox = msgBoxWindow;
    GooeyWindow_MakeVisible(msgBoxWindow, 1);
}

void GooeyButton_Draw(GooeyWindow *win, GooeyButton *button)
{

    active_backend->FillRectangle(button->core.x,
                                  button->core.y, button->core.width, button->core.height, button->clicked ? active_theme->primary : active_theme->widget_base, win->creation_id);
    float text_width = active_backend->GetTextWidth(button->label, strlen(button->label));
    float text_height = active_backend->GetTextHeight(button->label, strlen(button->label));

    float text_x = button->core.x + (button->core.width - text_width) / 2;
    float text_y = button->core.y + (button->core.height + text_height) / 2;

    active_backend->DrawText(text_x,
                             text_y, button->label, button->clicked ? active_theme->base : active_theme->neutral, 0.25f, win->creation_id);
    active_backend->SetForeground(active_theme->neutral);

    if (button->is_highlighted)
    {

        active_backend->DrawRectangle(button->core.x,
                                      button->core.y, button->core.width, button->core.height, active_theme->primary, win->creation_id);
    }
}

GooeyButton *GooeyButton_Add(GooeyWindow *win, const char *label, int x, int y,
                             int width, int height, void (*callback)())
{
    GooeyButton *button = &win->buttons[win->button_count++];
    button->core.type = WIDGET_BUTTON;
    button->core.x = x;
    button->core.y = y;
    button->core.width = active_backend->GetTextWidth(label, strlen(label)) + 30;
    button->core.height = height;
    strcpy(button->label, label);
    button->callback = callback;
    button->hover = false;
    button->clicked = false;
    button->is_highlighted = false;
    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&button->core);
    LOG_INFO("Button added with dimensions x=%d, y=%d, w=%d, h=%d.", x, y, width, height);

    return button;
}
bool GooeyButton_HandleClick(GooeyWindow *win, int x, int y)
{
    bool clicked_any_button = false;

    for (int i = 0; i < win->button_count; ++i)
    {
        GooeyButton *button = &win->buttons[i];
        bool is_within_bounds = (x >= button->core.x && x <= button->core.x + button->core.width) &&
                                (y >= button->core.y && y <= button->core.y + button->core.height);

        if (is_within_bounds)
        {
            button->clicked = !button->clicked;
            clicked_any_button = true;
            if (button->callback)
            {
                button->callback();
            }
        }
        else
        {
            button->clicked = false;
        }
    }

    return clicked_any_button;
}

void GooeyButton_setText(GooeyButton *button, const char *text)
{
    if (!button)
    {
        LOG_ERROR("Widget<Button> Cannot be null \n");
        return;
    }

    strcpy(button->label, text);
}

GooeyLayout *GooeyLayout_Create(GooeyWindow *win, GooeyLayoutType layout_type,
                                int x, int y, int width, int height)
{
    if (!win || win->layout_count >= MAX_WIDGETS)
    {
        fprintf(stderr,
                "Window not initialized or unable to add more layouts (full).\n");
        return NULL;
    }
    GooeyLayout *layout = &win->layouts[win->layout_count++];

    layout->core.type = WIDGET_LAYOUT;
    layout->core.x = x;
    layout->core.y = y;
    layout->core.width = width;
    layout->core.height = height;
    layout->layout_type = layout_type;
    layout->widget_count = 0;

    return layout;
}
void GooeyLayout_AddChild(GooeyLayout *layout, void *widget)
{
    if (!layout)
    {
        LOG_ERROR("Error: Invalid layout pointer.\n");
        return;
    }

    if (!widget)
    {
        LOG_ERROR("Error: Invalid widget pointer.\n");
        return;
    }

    if (layout->widget_count >= MAX_WIDGETS)
    {
        LOG_ERROR("Error: Maximum widgets reached for the layout.\n");
        return;
    }
    layout->widgets[layout->widget_count++] = widget;
}
void GooeyLayout_Build(GooeyLayout *layout)
{
    if (!layout)
    {
        LOG_ERROR("Error: Invalid layout pointer.\n");
        return;
    }

    if (layout->widget_count == 0)
    {
        LOG_ERROR("Error: Layout has no widgets to arrange.\n");
        return;
    }

    int spacing = 30;
    int current_x = layout->core.x;
    int current_y = layout->core.y;

    float max_widget_width = layout->layout_type == LAYOUT_HORIZONTAL ? layout->core.width / layout->widget_count : layout->core.width;

    switch (layout->layout_type)
    {
    case LAYOUT_VERTICAL:
        for (int i = 0; i < layout->widget_count; i++)
        {
            GooeyWidget *widget = (GooeyWidget *)layout->widgets[i];
            if (!widget)
            {
                LOG_ERROR("Null widget encountered at index %d.\n", i);
                continue;
            }

            if (widget->type != WIDGET_CHECKBOX)
                widget->width = max_widget_width;
            widget->x = layout->core.x + (layout->core.width - widget->width) / 2;
            widget->y = current_y;
            if (widget->type == WIDGET_LAYOUT)
            {
                GooeyLayout_Build((GooeyLayout *)widget);
            }

            current_y += widget->height + spacing;
        }
        break;

    case LAYOUT_HORIZONTAL:
        for (int i = 0; i < layout->widget_count; i++)
        {
            GooeyWidget *widget = layout->widgets[i];
            widget->width = max_widget_width;
            widget->x = current_x;
            widget->y = layout->core.y + (layout->core.height - widget->height) / 2;
            if (widget->type == WIDGET_LAYOUT)
            {
                GooeyLayout_Build((GooeyLayout *)widget);
            }

            current_x += widget->width + spacing;
        }
        break;

    case LAYOUT_GRID:
    {
        break;
    }

    default:
        LOG_ERROR("Error: Unsupported layout type.\n");
        break;
    }
}

GooeyMenu *GooeyMenu_Set(GooeyWindow *win)
{
    win->menu = (GooeyMenu *)malloc(sizeof(GooeyMenu));
    win->menu->children_count = 0;
    win->menu->is_busy = 0;
    LOG_INFO("Set menu for window.");

    return win->menu;
}

void GooeyMenu_Draw(GooeyWindow *win)
{
    int window_width, window_height;

    if (win->menu)
    {
        active_backend->GetWinDim(&window_width, &window_height, win->creation_id);
        active_backend->FillRectangle(0, 0, window_width, 20, active_theme->widget_base, win->creation_id);

        int x_offset = 10;
        for (int i = 0; i < win->menu->children_count; i++)
        {
            GooeyMenuChild *child = &win->menu->children[i];
            int text_width = active_backend->GetTextWidth(child->title, strlen(child->title));
            active_backend->DrawText(x_offset, 15,
                                     child->title, active_theme->neutral, 0.25f, win->creation_id);

            if (child->is_open && child->menu_elements_count > 0)
            {
                int submenu_x = x_offset - 10;
                int submenu_y = 20;
                int submenu_width = 150;
                int submenu_height = 25 * child->menu_elements_count;

                active_backend->FillRectangle(submenu_x, submenu_y,
                                              submenu_width, submenu_height, active_theme->widget_base, win->creation_id);

                for (int j = 0; j < child->menu_elements_count; j++)
                {
                    int element_y = submenu_y + (j * 25);
                    active_backend->DrawText(submenu_x + 5,
                                             element_y + 18, child->menu_elements[j], active_theme->neutral, 0.25f, win->creation_id);
                    if (j < child->menu_elements_count - 1)
                    {

                        active_backend->DrawLine(submenu_x,
                                                 element_y + 25 - 1, submenu_x + submenu_width,
                                                 element_y + 25 - 1, active_theme->neutral, win->creation_id);
                    }
                }
            }

            x_offset += text_width + 20;
        }
    }
}

GooeyMenuChild *GooeyMenu_AddChild(GooeyWindow *win, char *title)
{
    if (!win->menu || win->menu->children_count >= MAX_MENU_CHILDREN)
    {
        LOG_ERROR("Unable to add child: Menu is full or not initialized.\n");
        return NULL;
    }

    GooeyMenuChild *child = &win->menu->children[win->menu->children_count++];
    child->title = strdup(title);
    child->menu_elements_count = 0;
    child->is_open = false;
    LOG_INFO("Child added to menu with title=\"%s\"", title);

    return child;
}

void GooeyMenuChild_AddElement(GooeyMenuChild *child, char *title,
                               void (*callback)())
{
    child->menu_elements[child->menu_elements_count] = title;
    child->callbacks[child->menu_elements_count] = callback;
    child->menu_elements_count++;
    LOG_INFO("Element added to menu child with title=\"%s\"", title);
}

void GooeyMenu_HandleClick(GooeyWindow *win, int x, int y)
{

    if (!win->menu)
        return;

    int x_offset = 10;
    for (int i = 0; i < win->menu->children_count; i++)
    {
        GooeyMenuChild *child = &win->menu->children[i];
        int text_width = active_backend->GetTextWidth(child->title, strlen(child->title));
        if (y <= 20 && x >= x_offset && x <= x_offset + text_width)
        {

            for (int k = 0; k < win->menu->children_count; k++)
            {
                win->menu->children[k].is_open = 0;
            }

            child->is_open = !child->is_open;
            win->menu->is_busy = !win->menu->is_busy;

            GooeyWindow_Redraw(win);
            return;
        }

        if (child->is_open)
        {

            int submenu_x = x_offset - 10;
            int submenu_y = 20;
            int submenu_width = 150;

            for (int j = 0; j < child->menu_elements_count; j++)
            {
                int element_y = submenu_y + (j * 25);
                if (x >= submenu_x && x <= submenu_x + submenu_width &&
                    y >= element_y && y <= element_y + 25)
                {

                    if (child->callbacks[j])
                        child->callbacks[j]();

                    for (int k = 0; k < win->menu->children_count; k++)
                    {
                        win->menu->children[k].is_open = 0;
                    }
                    win->menu->is_busy = 0;

                    GooeyWindow_Redraw(win);
                    return;
                }
            }
        }

        x_offset += text_width + 20;
    }
}

GooeyLabel *GooeyLabel_Add(GooeyWindow *win, const char *text, float font_size, int x, int y)
{
    GooeyLabel *label = &win->labels[win->label_count++];
    label->core.type = WIDGET_LABEL;
    label->core.x = x;
    label->core.y = y;
    label->font_size = font_size;
    label->color = -1; // DEFAULTS TO NEUTRAL
    strcpy(label->text, text);
    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&label->core);
    LOG_INFO("Label added with dimensions x=%d, y=%d", x, y);

    return label;
}

void *GooeyLabel_SetColor(GooeyLabel *label, const char *color)
{
    unsigned long color_long = (unsigned long)strtol(color, NULL, 0);
    label->color = color_long;
}

void GooeyLabel_SetText(GooeyLabel *label, const char *text)
{
    if (label)
        strcpy(label->text, text);
}

void GooeyLabel_Draw(GooeyWindow *win)
{
    for (int i = 0; i < win->label_count; ++i)
    {

        active_backend->DrawText(win->labels[i].core.x, win->labels[i].core.y, win->labels[i].text, win->labels[i].color != -1 ? win->labels[i].color : active_theme->neutral, win->labels[i].font_size, win->creation_id);
    }
}

GooeyTextbox *GooeyTextBox_Add(GooeyWindow *win, int x, int y, int width,
                               int height, char *placeholder, void (*onTextChanged)(char *text))
{
    win->textboxes[win->textboxes_count].core.type = WIDGET_TEXTBOX;
    win->textboxes[win->textboxes_count].core.x = x;
    win->textboxes[win->textboxes_count].core.y = y;
    win->textboxes[win->textboxes_count].core.width = width;
    win->textboxes[win->textboxes_count].core.height = height;
    win->textboxes[win->textboxes_count].focused = false;
    win->textboxes[win->textboxes_count].callback = onTextChanged;
    win->textboxes[win->textboxes_count].scroll_offset = 0;
    win->textboxes[win->textboxes_count].text[0] = '\0';
    strcpy(win->textboxes[win->textboxes_count].placeholder, placeholder);

    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&win->textboxes[win->textboxes_count].core);
    LOG_INFO("Textbox added with dimensions x=%d, y=%d, w=%d, h=%d", x, y, width, height);

    win->textboxes_count++;
    return &win->textboxes[win->textboxes_count - 1];
}

const char *GooeyTextbox_getText(GooeyTextbox *textbox)
{
    if (!textbox)
    {
        LOG_ERROR("Widget<Textbox> cannot be null.");
        return NULL;
    }

    return (const char *)textbox->text;
}

void GooeyTextbox_setText(GooeyTextbox *textbox, const char *text)
{
    if (!textbox)
    {
        LOG_ERROR("Widget<Textbox> cannot be null.");
        return;
    }
    strcpy(textbox->text, text);
}
void GooeyTextbox_Draw(GooeyWindow *win, int index)
{

    active_backend->FillRectangle(win->textboxes[index].core.x, win->textboxes[index].core.y,
                                  win->textboxes[index].core.width, win->textboxes[index].core.height, active_theme->base, win->creation_id);

    active_backend->DrawRectangle(win->textboxes[index].core.x, win->textboxes[index].core.y,
                                  win->textboxes[index].core.width, win->textboxes[index].core.height,
                                  win->textboxes[index].focused ? active_theme->primary : active_theme->neutral, win->creation_id);

    int text_x = win->textboxes[index].core.x + 5;
    int text_y = win->textboxes[index].core.y + (win->textboxes[index].core.height / 2) + 5;

    int max_text_width = win->textboxes[index].core.width - 10;
    size_t len = strlen(win->textboxes[index].text);
    size_t start_index = win->textboxes[index].scroll_offset;

    while (start_index < len &&
           active_backend->GetTextWidth(win->textboxes[index].text + start_index, len - start_index) > max_text_width)
    {
        start_index++;
    }

    char display_text[256];
    strncpy(display_text, win->textboxes[index].text + start_index, sizeof(display_text) - 1);
    display_text[sizeof(display_text) - 1] = '\0';

    active_backend->DrawText(text_x, text_y, display_text, active_theme->neutral, 0.25f, win->creation_id);

    if (win->textboxes[index].focused)
    {
        int cursor_x = text_x + active_backend->GetTextWidth(display_text, strlen(display_text));
        active_backend->DrawLine(cursor_x, win->textboxes[index].core.y + 5,
                                 cursor_x, win->textboxes[index].core.y + win->textboxes[index].core.height - 5, active_theme->neutral, win->creation_id);
    }
    else
    {

        if (win->textboxes[index].placeholder && strlen(win->textboxes[index].text) == 0)
            active_backend->DrawText(text_x, text_y, win->textboxes[index].placeholder, active_theme->neutral, 0.25f, win->creation_id);
    }
}
void GooeyTextbox_HandleKeyPress(GooeyWindow *win, GooeyEvent *key_event)
{
    printf("hey pressed \n");

    char *buf = active_backend->GetKeyFromCode(key_event);
    if (buf == NULL)
    {
        return;
    }

    for (int i = 0; i < win->textboxes_count; i++)
    {
        if (!win->textboxes[i].focused)
            continue;

        size_t len = strlen(win->textboxes[i].text);

        if (strcmp(buf, "Backspace") == 0)
        {

            if (len > 0)
            {
                win->textboxes[i].text[len - 1] = '\0';

                if (win->textboxes[i].scroll_offset > 0)
                {
                    win->textboxes[i].scroll_offset--;
                }

                if (win->textboxes[i].callback)
                {
                    win->textboxes[i].callback(win->textboxes[i].text);
                }
            }
        }
        else if (strcmp(buf, "Return") == 0)
        {

            win->textboxes[i].focused = false;
        }
        else if (strcmp(buf, "Tab") == 0)
        {
        }
        else if (isprint(buf[0]) && len < sizeof(win->textboxes[i].text) - 1)
        {

            strncat(win->textboxes[i].text, buf, 1);

            if (win->textboxes[i].callback)
            {
                win->textboxes[i].callback(win->textboxes[i].text);
            }

            int text_width = active_backend->GetTextWidth(win->textboxes[i].text, len + 1);
            int max_text_width = win->textboxes[i].core.width - 10;

            if (text_width > max_text_width)
            {
                win->textboxes[i].scroll_offset++;
            }
        }
    }
    GooeyWindow_Redraw(win);

    if (!(strcmp(buf, "Backspace") == 0 || strcmp(buf, "Return") == 0 || strcmp(buf, "Tab") == 0))
    {
        free(buf);
    }
}
bool GooeyTextbox_HandleClick(GooeyWindow *win, int x, int y)
{

    for (int i = 0; i < win->textboxes_count; i++)
    {
        GooeyTextbox *textbox = &win->textboxes[i];
        if (x >= win->textboxes[i].core.x &&
            x <= win->textboxes[i].core.x + win->textboxes[i].core.width &&
            y >= win->textboxes[i].core.y &&
            y <= win->textboxes[i].core.y + win->textboxes[i].core.height)
        {
            win->textboxes[i].focused = true;
            for (int j = 0; j < win->textboxes_count; j++)
            {
                if (j != i)
                    win->textboxes[j].focused = false;
            }
            return true;
        }
    }
    return false;
}

GooeyCheckbox *GooeyCheckbox_Add(GooeyWindow *win, int x, int y, char *label,
                                 void (*callback)(bool checked))
{
    GooeyCheckbox *checkbox = &win->checkboxes[win->checkbox_count++];
    checkbox->core.type = WIDGET_CHECKBOX, checkbox->core.x = x;
    checkbox->core.y = y;
    checkbox->core.width = CHECKBOX_SIZE;
    checkbox->core.height = CHECKBOX_SIZE;
    if (label)
    {
        strcpy(checkbox->label, label);
    }
    else
    {
        sprintf(checkbox->label, "Checkbox %d", win->checkbox_count);
    }
    checkbox->checked = false;
    checkbox->callback = callback;
    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&checkbox->core);
    LOG_INFO("Checkbox added with dimensions x=%d, y=%d", x, y);

    return checkbox;
}

bool GooeyCheckbox_HandleClick(GooeyWindow *win, int x, int y)
{
    for (int i = 0; i < win->checkbox_count; ++i)
    {
        GooeyCheckbox *checkbox = &win->checkboxes[i];
        if (x >= checkbox->core.x && x <= checkbox->core.x + checkbox->core.width &&
            y >= checkbox->core.y &&
            y <= checkbox->core.y + checkbox->core.height)
        {
            checkbox->checked = !checkbox->checked;
            if (checkbox->callback)
                checkbox->callback(checkbox->checked);
            return true;
        }
    }
    return false;
}

GooeyRadioButton *GooeyRadioButton_Add(GooeyWindow *win, int x, int y,
                                       char *label,
                                       void (*callback)(bool selected))
{
    GooeyRadioButton *radio_button =
        &win->radio_buttons[win->radio_button_count++];

    radio_button->core.type = WIDGET_RADIOBUTTON;
    radio_button->core.x = x;
    radio_button->core.y = y;
    if (label)
        strcpy(radio_button->label, label);
    else
    {
        sprintf(radio_button->label, "Radio button %d", win->radio_button_count);
    }

    radio_button->radius = RADIO_BUTTON_RADIUS;
    radio_button->selected = false;
    radio_button->callback = callback;
    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&radio_button->core);

    return radio_button;
}

bool GooeyRadioButton_HandleClick(GooeyWindow *win, int x, int y)
{
    int state = false;
    for (int i = 0; i < win->radio_button_count; ++i)
    {
        GooeyRadioButton *radio_button = &win->radio_buttons[i];
        int dx = x - radio_button->core.x;
        int dy = y - radio_button->core.y;
        if (dx * dx + dy * dy <= radio_button->radius * radio_button->radius)
        {
            radio_button->selected = !radio_button->selected;
            if (radio_button->callback)
                radio_button->callback(radio_button->selected);
            state = true;
        }
        else
        {
            if (radio_button->selected)
            {
                radio_button->selected = !radio_button->selected;
            }
        }
    }
    return state;
}

GooeyList *GooeyList_Add(GooeyWindow *win, int x, int y, int width, int height)
{
    GooeyList *list = &win->lists[win->list_count++];
    list->core.x = x;
    list->core.y = y;
    list->core.width = width;
    list->core.height = height;
    list->items = malloc(sizeof(GooeyListItem) * 100);
    list->item_count = 0;
    list->scroll_offset = 1;
    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&list->core);

    return list;
}

void GooeyList_AddItem(GooeyList *list, const char *title, const char *description, void (*callback)(void))
{
    GooeyListItem item;
    strcpy(item.title, title);
    strcpy(item.description, description);
    list->items[list->item_count++] = item;
}

bool GooeyList_HandleScroll(GooeyWindow *window, GooeyEvent *scroll_event)
{
    for (int i = 0; i < window->list_count; ++i)
    {
        GooeyList *list = &window->lists[i];

        int mouse_x = scroll_event->mouse_move.x;
        int mouse_y = scroll_event->mouse_move.y;
        if (mouse_x >= list->core.x && mouse_x <= list->core.x + list->core.width && mouse_y >= list->core.y && mouse_y <= list->core.y + list->core.height)
        {
            int scroll_offset_amount = scroll_event->mouse_scroll.y * 8;
            list->scroll_offset += scroll_offset_amount;
            return true;
        }
    }
    return false;
}

void GooeyList_Draw(GooeyWindow *win)
{
    const int item_spacing = 40;
    const int title_description_spacing = 15;

    for (int i = 0; i < win->list_count; ++i)
    {
        GooeyList *list = &win->lists[i];

        active_backend->FillRectangle(
            list->core.x,
            list->core.y,
            list->core.width,
            list->core.height,
            active_theme->widget_base,
            win->creation_id);

        active_backend->DrawRectangle(
            list->core.x,
            list->core.y,
            list->core.width,
            list->core.height,
            active_theme->neutral,
            win->creation_id);

        active_backend->FillRectangle(
            list->core.x + list->core.width,
            list->core.y,
            10,
            list->core.height,
            active_theme->widget_base,
            win->creation_id);

        active_backend->DrawRectangle(
            list->core.x + list->core.width,
            list->core.y,
            10,
            list->core.height,
            active_theme->neutral,
            win->creation_id);

        int total_content_height = list->item_count * item_spacing;
        int visible_height = list->core.height;
        int max_scroll_offset = total_content_height > visible_height
                                    ? total_content_height - visible_height
                                    : 0;

        if (list->scroll_offset < -max_scroll_offset)
            list->scroll_offset = -max_scroll_offset;
        if (list->scroll_offset > 0)
            list->scroll_offset = 0;
        int current_y_offset = list->core.y + list->scroll_offset + 10;

        active_backend->FillRectangle(
            list->core.x + list->core.width,
            list->core.y - (int)(float)((list->scroll_offset) * visible_height) / (total_content_height),
            10,
            total_content_height <= visible_height ? list->core.height : (int)visible_height * ((float)visible_height / (total_content_height)),
            active_theme->primary,
            win->creation_id);

        for (int j = 0; j < list->item_count; ++j)
        {
            GooeyListItem item = list->items[j];
            int title_y = current_y_offset + active_backend->GetTextHeight(item.title, strlen(item.title));
            int description_y = title_y + title_description_spacing;

            if (title_y < list->core.y + list->core.height && title_y > list->core.y + 5)
                active_backend->DrawText(
                    list->core.x + 10,
                    title_y,
                    item.title,
                    active_theme->neutral,
                    0.25f,
                    win->creation_id);

            if (description_y < list->core.y + list->core.height && description_y > list->core.y + 5)
                active_backend->DrawText(
                    list->core.x + 10,
                    description_y,
                    item.description,
                    active_theme->neutral,
                    0.25f,
                    win->creation_id);

            int line_seperator_y = current_y_offset + item_spacing - 10;

            if (j < list->item_count - 1)
            {
                if (line_seperator_y < list->core.y + list->core.height - 10 && line_seperator_y > list->core.y + 5)
                    active_backend->DrawLine(
                        list->core.x,
                        line_seperator_y,
                        list->core.x + list->core.width,
                        line_seperator_y,
                        active_theme->neutral,
                        win->creation_id);
            }
            current_y_offset += item_spacing;
        }
    }
}

GooeySlider *GooeySlider_Add(GooeyWindow *win, int x, int y, int width,
                             long min_value, long max_value, bool show_hints,
                             void (*callback)(long value))
{
    if (!win)
    {
        LOG_ERROR("Window cannot be NULL. \n");
        return NULL;
    }

    if (max_value <= min_value)
    {
        LOG_ERROR("max_value should be greater than min_value. \n");
        return NULL;
    }

    GooeySlider *slider = &win->sliders[win->slider_count++];
    slider->core.type = WIDGET_SLIDER;
    slider->core.x = x;
    slider->core.y = y;
    slider->core.width = width;
    slider->core.height = SLIDER_HEIGHT;
    slider->min_value = min_value;
    slider->max_value = max_value;
    slider->value = min_value;
    slider->show_hints = show_hints;
    slider->callback = callback;
    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&slider->core);

    return slider;
}

bool GooeySlider_HandleDrag(GooeyWindow *win, int x, int y, GooeyEventType type)
{
    static GooeySlider *active_slider = NULL;
    int comfort_margin = 20;

    for (int i = 0; i < win->slider_count; ++i)
    {
        GooeySlider *slider = &win->sliders[i];

        bool within_bounds =
            (y >= slider->core.y - comfort_margin && y <= slider->core.y + slider->core.height + comfort_margin) &&
            (x >= slider->core.x && x <= slider->core.x + slider->core.width);

        if (type == GOOEY_EVENT_CLICK_PRESS && within_bounds)
        {
            active_backend->InhibitResetEvents(1);
            active_slider = slider;

            slider->value =
                slider->min_value +
                ((x - slider->core.x) * (slider->max_value - slider->min_value)) /
                    slider->core.width;

            if (slider->value < slider->min_value)
                slider->value = slider->min_value;
            if (slider->value > slider->max_value)
                slider->value = slider->max_value;

            return true;
        }
        else if (type == GOOEY_EVENT_MOUSE_MOVE && slider == active_slider)
        {
            slider->value =
                slider->min_value +
                ((x - slider->core.x) * (slider->max_value - slider->min_value)) /
                    slider->core.width;

            if (slider->value < slider->min_value)
                slider->value = slider->min_value;
            if (slider->value > slider->max_value)
                slider->value = slider->max_value;

            if (slider->callback)
            {
                slider->callback(slider->value);
            }

            return true;
        }
    }

    // Handle release outside of slider bounds
    if (type == GOOEY_EVENT_CLICK_RELEASE && active_slider)
    {
        active_backend->InhibitResetEvents(0);

        if (active_slider->callback)
        {
            active_slider->callback(active_slider->value);
        }

        active_slider = NULL;
        return true;
    }

    active_backend->InhibitResetEvents(0);
    return false;
}

long GooeySlider_getValue(GooeySlider *slider)
{
    if (!slider)
    {
        LOG_ERROR("Widget<Slider> cannot be NULL. \n");
        return -1;
    }

    return slider->value;
}

void GooeySlider_setValue(GooeySlider *slider, long value)
{
    if (!slider)
    {
        LOG_ERROR("Widget<Slider> cannot be NULL. \n");
        return;
    }

    slider->value = value;
}

GooeyDropdown *GooeyDropdown_Add(GooeyWindow *win, int x, int y, int width,
                                 int height, const char **options,
                                 int num_options,
                                 void (*callback)(int selected_index))
{
    GooeyDropdown *dropdown = &win->dropdowns[win->dropdown_count++];
    dropdown->core.type = WIDGET_DROPDOWN;
    dropdown->core.x = x;
    dropdown->core.y = y;
    dropdown->core.width = width;
    dropdown->core.height = height;
    dropdown->options = options;
    dropdown->num_options = num_options;
    dropdown->selected_index = 0;
    dropdown->callback = callback;
    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&dropdown->core);
    LOG_INFO("Dropdown added with dimensions x=%d, y=%d, w=%d, h=%d", x, y, width, height);

    dropdown->is_open = false;
    return dropdown;
}
void GooeyDropdown_Draw(GooeyWindow *win)
{
    for (int i = 0; i < win->dropdown_count; i++)
    {
        int x_offset = win->dropdowns[i].core.x;

        GooeyDropdown *dropdown = &win->dropdowns[i];

        active_backend->FillRectangle(dropdown->core.x,
                                      dropdown->core.y, dropdown->core.width,
                                      dropdown->core.height, active_theme->widget_base, win->creation_id);
        active_backend->DrawText(dropdown->core.x + 5,
                                 dropdown->core.y + 20,
                                 dropdown->options[dropdown->selected_index],
                                 active_theme->neutral, 0.25f, win->creation_id);
        int text_width = active_backend->GetTextWidth(dropdown->options[dropdown->selected_index], strlen(dropdown->options[dropdown->selected_index]));
        active_backend->DrawText(x_offset, 15,
                                 dropdown->options[dropdown->selected_index], active_theme->neutral, 0.25f, win->creation_id);
        if (dropdown->is_open && dropdown->num_options > 0)
        {
            int submenu_x = x_offset;
            int submenu_y = win->dropdowns[i].core.y + win->dropdowns[i].core.height;
            int submenu_width = win->dropdowns[i].core.width;
            int submenu_height = 25 * dropdown->num_options;
            active_backend->FillRectangle(submenu_x, submenu_y,
                                          submenu_width, submenu_height, active_theme->widget_base, win->creation_id);
            for (int j = 0; j < dropdown->num_options; j++)
            {
                int element_y = submenu_y + (j * 25);
                active_backend->DrawText(submenu_x + 5,
                                         element_y + 18, dropdown->options[j], active_theme->neutral, 0.25f, win->creation_id);
                if (j < dropdown->num_options - 1)
                {
                    active_backend->DrawLine(submenu_x,
                                             element_y + 25 - 1, submenu_x + submenu_width,
                                             element_y + 25 - 1, active_theme->neutral, win->creation_id);
                }
            }
        }
        x_offset += text_width + 20;
    }
}

bool GooeyDropdown_HandleClick(GooeyWindow *win, int x, int y)
{

    bool _btn_st = false;
    for (int i = 0; i < win->dropdown_count; i++)
    {
        int x_offset = win->dropdowns[i].core.x;
        GooeyDropdown *dropdown = &win->dropdowns[i];
        int text_width = 10;
        if (x >= dropdown->core.x && x <= dropdown->core.x + dropdown->core.width && y >= dropdown->core.y && y <= dropdown->core.y + dropdown->core.height)
        {
            dropdown->is_open = !dropdown->is_open;
            GooeyWindow_Redraw(win);
            _btn_st = true;
        }

        if (dropdown->is_open)
        {
            int submenu_x = x_offset;
            int submenu_y = win->dropdowns[i].core.y + win->dropdowns[i].core.height;
            int submenu_width = win->dropdowns[i].core.width;
            for (int j = 0; j < dropdown->num_options; j++)
            {
                int element_y = submenu_y + (j * 25);
                if (x >= submenu_x && x <= submenu_x + submenu_width &&
                    y >= element_y && y <= element_y + 25)
                {
                    printf("test %d \n", j);

                    dropdown->selected_index = j;

                    if (win->dropdowns[i].callback)
                        win->dropdowns[i].callback(j);

                    dropdown->is_open = 0;
                    GooeyWindow_Redraw(win);
                }
            }
        }

        x_offset += text_width + 20;
    }
    if (_btn_st)
    {
        return true;
    }
    else
    {
        return false;
    }
}

GooeyCanvas *GooeyCanvas_Add(GooeyWindow *win, int x, int y, int width,
                             int height)
{
    GooeyCanvas *canvas = &win->canvas[win->canvas_count++];
    canvas->core.type = WIDGET_CANVAS;
    canvas->core.x = x;
    canvas->core.y = y;
    canvas->core.width = width;
    canvas->core.height = height;
    canvas->elements = malloc(sizeof(CanvaElement) * 100);
    canvas->element_count = 0;
    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&canvas->core);
    LOG_INFO("Canvas added to window with dimensions x=%d, y=%d, w=%d, h=%d.", x, y, width, height);

    return canvas;
}

void GooeyCanvas_DrawRectangle(GooeyCanvas *canvas, int x, int y, int width, int height, const char *color_hex, bool is_filled)
{

    unsigned long color = (unsigned long)strtol(color_hex, NULL, 0);

    int x_win = x + canvas->core.x;
    int y_win = y + canvas->core.y;

    if (x_win >= canvas->core.x && x_win <= canvas->core.x + canvas->core.width && y_win >= canvas->core.y && y_win <= canvas->core.y + canvas->core.height)
    {
        CanvasDrawRectangleArgs *args = malloc(sizeof(CanvasDrawRectangleArgs));
        *args = (CanvasDrawRectangleArgs){.color = color, .height = height, .width = width, .x = x_win, .y = y_win, .is_filled = is_filled};
        canvas->elements[canvas->element_count++] = (CanvaElement){.operation = CANVA_DRAW_RECT, .args = args};
        LOG_INFO("Drew %s rectangle with dimensions x=%d, y=%d, w=%d, h=%d in canvas<x=%d, y=%d, w=%d, h=%d>.", is_filled ? "filled" : "hollow", x, y, width, height, canvas->core.x, canvas->core.y, canvas->core.width, canvas->core.height);
    }
    else
    {
        LOG_ERROR("Canvas<%d, %d>: Rectangle<%d, %d> is out of boundaries. will be skipped. \n", canvas->core.x, canvas->core.y, x, y);
    }
}

void GooeyCanvas_DrawLine(GooeyCanvas *canvas, int x1, int y1, int x2, int y2, const char *color_hex)
{

    unsigned long color = (unsigned long)strtol(color_hex, NULL, 0);

    int x1_win = x1 + canvas->core.x;
    int y1_win = y1 + canvas->core.y;

    int x2_win = x2 + canvas->core.x;
    int y2_win = y2 + canvas->core.y;

    if (x1_win >= canvas->core.x && x1_win <= canvas->core.x + canvas->core.width && y1_win >= canvas->core.y && y1_win <= canvas->core.y + canvas->core.height && x2_win >= canvas->core.x && x2_win <= canvas->core.x + canvas->core.width && y2_win >= canvas->core.y && y2_win <= canvas->core.y + canvas->core.height)
    {
        CanvasDrawLineArgs *args = malloc(sizeof(CanvasDrawLineArgs));
        *args = (CanvasDrawLineArgs){.color = color, .x1 = x1_win, .x2 = x2_win, .y1 = y1_win, .y2 = y2_win};
        canvas->elements[canvas->element_count++] = (CanvaElement){.operation = CANVA_DRAW_LINE, .args = args};
        LOG_INFO("Drew line with dimensions x1=%d, y1=%d, x2=%d, y2=%d", x1, y1, x2, y2);
    }
    else
    {
        LOG_ERROR("Canvas<%d, %d>: Line<%d, %d, %d, %d> is out of boundaries. will be skipped. \n", canvas->core.x, canvas->core.y, x1, y1, x2, y2);
    }
}

void GooeyCanvas_DrawArc(GooeyCanvas *canvas, int x_center, int y_center, int width, int height, int angle1, int angle2)
{

    int x_win = x_center + canvas->core.x;
    int y_win = y_center + canvas->core.y;

    if (x_win + width >= canvas->core.x && x_win + width <= canvas->core.x + canvas->core.width && y_win + height >= canvas->core.y && y_win + height <= canvas->core.y + canvas->core.height)
    {
        CanvasDrawArcArgs *args = malloc(sizeof(CanvasDrawArcArgs));
        *args = (CanvasDrawArcArgs){.height = height, .width = width, .x_center = x_win, .y_center = y_win, .angle1 = angle1, .angle2 = angle2};
        canvas->elements[canvas->element_count++] = (CanvaElement){.operation = CANVA_DRAW_ARC, .args = args};
        LOG_INFO("Drew arc with dimensions x_center=%d, y_center=%d, w=%d, h=%d in Canvas<x=%d, y=%d, w=%d, h=%d>.", x_center, y_center, width, height, canvas->core.x, canvas->core.y, canvas->core.width, canvas->core.height);
    }
    else
    {
        LOG_ERROR("Canvas<%d, %d>: Arc<%d, %d> with Dimensions<%d, %d> is out of boundaries. will be skipped. \n", canvas->core.x, canvas->core.y, x_center, y_center, width, height);
    }
}

void GooeyCanvas_SetForeground(GooeyCanvas *canvas, const char *color_hex)
{
    unsigned long color = (unsigned long)strtol(color_hex, NULL, 0);
    CanvasSetFGArgs *args = malloc(sizeof(CanvasSetFGArgs));
    *args = (CanvasSetFGArgs){.color = color};
    canvas->elements[canvas->element_count++] = (CanvaElement){.operation = CANVA_DRAW_SET_FG, .args = args};
    LOG_INFO("Set foreground with color %lX.", color);
}

void GooeyWindow_Cleanup(int num_windows, GooeyWindow *first_win, ...)
{

    va_list args;
    GooeyWindow *windows[num_windows];

    va_start(args, first_win);
    windows[0] = first_win;
    GooeyWindow *win = NULL;

    for (int i = 1; i < num_windows; ++i)
    {
        win = va_arg(args, GooeyWindow *);
        windows[i] = win;
    }
    va_end(args);
    for (int i = 0; i < num_windows; ++i)
    {
        win = windows[i];
        for (int i = 0; i < win->canvas_count; ++i)
        {
            if (!win->canvas[i].elements)
                continue;

            for (int j = 0; j < win->canvas[i].element_count; ++j)
            {
                CanvaElement *element = &win->canvas[i].elements[j];
                if (element->args)
                {
                    free(element->args);
                    element->args = NULL;
                }
            }

            free(win->canvas[i].elements);
            win->canvas[i].elements = NULL;
        }

        if (win->canvas)
        {
            free(win->canvas);
            win->canvas = NULL;
            win->canvas_count = 0;
        }

        if (win->buttons)
        {
            free(win->buttons);
            win->buttons = NULL;
        }
        if (win->labels)
        {
            free(win->labels);
            win->labels = NULL;
        }
        if (win->checkboxes)
        {
            free(win->checkboxes);
            win->checkboxes = NULL;
        }
        if (win->radio_buttons)
        {
            free(win->radio_buttons);
            win->radio_buttons = NULL;
        }
        if (win->radio_button_groups)
        {
            free(win->radio_button_groups);
            win->radio_button_groups = NULL;
        }
        if (win->menu)
        {
            free(win->menu);
            win->menu = NULL;
        }
        if (win->sliders)
        {
            free(win->sliders);
            win->sliders = NULL;
        }
        if (win->dropdowns)
        {
            free(win->dropdowns);
            win->dropdowns = NULL;
        }
        if (win->textboxes)
        {
            free(win->textboxes);
            win->textboxes = NULL;
        }
        if (win->layouts)
        {
            free(win->layouts);
            win->layouts = NULL;
        }

        if (win->lists)
        {
            for (int i = 0; i < win->list_count; ++i)
            {
                if (win->lists[i].items)
                {
                    free(win->lists[i].items);
                    win->lists[i].items = NULL;
                }
            }
            free(win->lists);
            win->lists = NULL;
        }
    }
    LOG_INFO("Cleanup.");
    active_backend->DestroyWindow();
    active_backend->Cleanup();
}

void GooeyWindow_Run(int num_windows, GooeyWindow *first_win, ...)
{
    GooeyEvent *event;
    bool running = true;

    va_list args;
    GooeyWindow *windows[num_windows];

    va_start(args, first_win);
    windows[0] = first_win;
    GooeyWindow *win = NULL;

    for (int i = 1; i < num_windows; ++i)
    {
        win = va_arg(args, GooeyWindow *);
        windows[i] = win;
    }
    va_end(args);
    int i = 0;

    while (running)
    {
        event = active_backend->HandleEvents();
        for (int i = 0; i < num_windows; ++i)
        {
            win = windows[i];

            int x = event->mouse_move.x;
            int y = event->mouse_move.y;

            switch (event->type)
            {
            case GOOEY_EVENT_EXPOSE:
                GooeyWindow_Redraw(win);
                break;

            case GOOEY_EVENT_KEY_PRESS:
                if (win->creation_id == event->attached_window)
                    GooeyTextbox_HandleKeyPress(win, event);
                break;

            case GOOEY_EVENT_CLICK_PRESS:
                if (win->creation_id == event->attached_window)
                {

                    GooeyMenu_HandleClick(win, x, y);
                    if (
                        GooeySlider_HandleDrag(win, x, y, event->type) || GooeyButton_HandleClick(win, x, y) ||
                        GooeyRadioButtonGroup_HandleClick(win, x, y) ||
                        GooeyCheckbox_HandleClick(win, x, y) ||
                        GooeyRadioButton_HandleClick(win, x, y) ||
                        GooeyDropdown_HandleClick(win, x, y) ||
                        GooeyTextbox_HandleClick(win, x, y))
                    {
                        GooeyWindow_Redraw(win);
                    }
                }
                break;

            case GOOEY_EVENT_CLICK_RELEASE:
                if (win->creation_id == event->attached_window)
                {

                    if (GooeySlider_HandleDrag(win, x, y, event->type))
                    {
                        GooeyWindow_Redraw(win);
                    }
                }
                break;

            case GOOEY_EVENT_MOUSE_SCROLL:
                if (win->creation_id == event->attached_window)
                {

                    if (GooeyList_HandleScroll(win, event))
                    {
                        GooeyWindow_Redraw(win);
                    }
                }
                break;

            case GOOEY_EVENT_WINDOW_CLOSE:

                running = false;
                break;

            default:

                break;
            }
        }
    }
}

GooeySignal GooeySignal_Create(void)
{
    GooeySignal signal;
    signal.slots = NULL;
    return signal;
}

void GooeySignal_Link(GooeySignal *signal, GooeySignal_CallbackFunction callback, void *context)
{
    GooeySignal_Slot *slot = (GooeySignal_Slot *)malloc(sizeof(GooeySignal_Slot));
    if (!slot)
    {
        LOG_ERROR("Couldn't allocate memory to signal slot with context \"%s\".", context);
    }
    slot->callback = callback;
    slot->context = context;
    slot->next = signal->slots;
    signal->slots = slot;
}

void GooeySignal_Emit(GooeySignal *signal, void *data)
{
    if (signal->slots)
        for (GooeySignal_Slot *slot = signal->slots; slot != NULL; slot = slot->next)
        {
            if (slot->callback)
                slot->callback(slot->context, data);
        }
}

void GooeySignal_UnLinkAll(GooeySignal *signal)
{
    GooeySignal_Slot *slot = signal->slots;
    while (slot != NULL)
    {
        free(slot);
        slot = slot->next;
    }
    signal->slots = NULL;
}
