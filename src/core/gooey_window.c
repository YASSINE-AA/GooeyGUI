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

#include "core/gooey_window.h"
#include "gooey_event_internal.h"

void GooeyWindow_RegisterWidget(GooeyWindow *win, GooeyWidget *widget)
{
    if (win && win->widgets)
        win->widgets[win->widget_count++] = widget;
}

void GooeyWindow_MakeVisible(GooeyWindow *win, bool visibility)
{
    active_backend->MakeWindowVisible(win->creation_id, visibility);
    GooeyWindow_Redraw(win);
}

void GooeyWindow_MakeResizable(GooeyWindow *msgBoxWindow, bool is_resizable)
{
    active_backend->MakeWindowResizable(0, msgBoxWindow->creation_id);
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