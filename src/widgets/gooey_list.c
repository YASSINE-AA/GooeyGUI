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

#include "widgets/gooey_list.h"

#define DEFAULT_THUMB_WIDTH 10
#define DEFAULT_ITEM_SPACING 40
#define DEFAULT_SCROLL_OFFSET 1

GooeyList *GooeyList_Add(GooeyWindow *win, int x, int y, int width, int height, void (*callback)(int index))
{
    GooeyList *list = &win->lists[win->list_count++];
    *list = (GooeyList){0};

    list->core.x = x;
    list->core.y = y;
    list->core.width = width;
    list->core.height = height;
    list->items = (GooeyListItem *)malloc(sizeof(GooeyListItem) * 1024);
    list->item_count = 0;
    list->scroll_offset = DEFAULT_SCROLL_OFFSET;
    list->thumb_y = y;
    list->thumb_height = -1;
    list->thumb_width = DEFAULT_THUMB_WIDTH;
    list->item_spacing = DEFAULT_ITEM_SPACING;
    list->callback = callback;

    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&list->core);
    return list;
}

void GooeyList_AddItem(GooeyList *list, const char *title, const char *description)
{
    GooeyListItem item = {0};
    strcpy(item.title, title);
    strcpy(item.description, description);
    list->items[list->item_count++] = item;
}

void GooeyList_Draw(GooeyWindow *win)
{
    const int title_description_spacing = 15;

    for (int i = 0; i < win->list_count; ++i)
    {
        GooeyList *list = &win->lists[i];

        active_backend->FillRectangle(
            list->core.x, list->core.y,
            list->core.width, list->core.height,
            active_theme->widget_base, win->creation_id);

        active_backend->DrawRectangle(
            list->core.x, list->core.y,
            list->core.width, list->core.height,
            active_theme->neutral, win->creation_id);

        active_backend->FillRectangle(
            list->core.x + list->core.width, list->core.y,
            list->thumb_width, list->core.height,
            active_theme->widget_base, win->creation_id);

        active_backend->DrawRectangle(
            list->core.x + list->core.width, list->core.y,
            list->thumb_width, list->core.height,
            active_theme->neutral, win->creation_id);

        int total_content_height = list->item_count * list->item_spacing;
        int visible_height = list->core.height;
        int max_scroll_offset = (total_content_height > visible_height)
                                    ? total_content_height - visible_height
                                    : 0;

        if (list->scroll_offset < -max_scroll_offset)
            list->scroll_offset = -max_scroll_offset;
        if (list->scroll_offset > 0)
            list->scroll_offset = 0;

        int current_y_offset = list->core.y + list->scroll_offset + 10;
        list->thumb_height = (total_content_height <= visible_height)
                                 ? list->core.height
                                 : (int)((float)visible_height * visible_height / total_content_height);

        list->thumb_y = list->core.y - (int)((float)list->scroll_offset * visible_height / total_content_height);

        active_backend->FillRectangle(
            list->core.x + list->core.width, list->thumb_y,
            list->thumb_width, list->thumb_height,
            active_theme->primary, win->creation_id);

        for (size_t j = 0; j < list->item_count; ++j)
        {
            GooeyListItem item = list->items[j];

            int title_y = current_y_offset + active_backend->GetTextHeight(item.title, strlen(item.title));
            int description_y = title_y + title_description_spacing;

            if (title_y < list->core.y + list->core.height && title_y > list->core.y + 5)
            {
                active_backend->DrawText(
                    list->core.x + 10, title_y,
                    item.title, active_theme->neutral,
                    0.25f, win->creation_id);
            }

            if (description_y < list->core.y + list->core.height && description_y > list->core.y + 5)
            {
                active_backend->DrawText(
                    list->core.x + 10, description_y,
                    item.description, active_theme->neutral,
                    0.25f, win->creation_id);
            }

            int line_separator_y = current_y_offset + list->item_spacing - 10;
            if (j < list->item_count - 1 &&
                line_separator_y < list->core.y + list->core.height - 10 &&
                line_separator_y > list->core.y + 5)
            {
                active_backend->DrawLine(
                    list->core.x, line_separator_y,
                    list->core.x + list->core.width,
                    line_separator_y, active_theme->neutral,
                    win->creation_id);
            }

            current_y_offset += list->item_spacing;
        }
    }
}

bool GooeyList_HandleScroll(GooeyWindow *window, GooeyEvent *scroll_event)
{
    const int scroll_speed_multiplier = 2;

    for (int i = 0; i < window->list_count; ++i)
    {
        GooeyList *list = &window->lists[i];

        int mouse_x = scroll_event->mouse_move.x;
        int mouse_y = scroll_event->mouse_move.y;

        int total_content_height = list->item_count * list->item_spacing;
        int visible_height = list->core.height;

        if (mouse_x >= list->core.x && mouse_x <= list->core.x + list->core.width &&
            mouse_y >= list->core.y && mouse_y <= list->core.y + list->core.height)
        {
            if (scroll_event->type == GOOEY_EVENT_MOUSE_SCROLL)
            {
                int scroll_offset_amount = scroll_event->mouse_scroll.y *
                                           (total_content_height / visible_height) *
                                           scroll_speed_multiplier;
                list->scroll_offset += scroll_offset_amount;

                return true;
            }
            else if (scroll_event->type == GOOEY_EVENT_KEY_PRESS)
            {
                const char *key = active_backend->GetKeyFromCode(scroll_event);
                LOG_ERROR("%s", key);

                if (strcmp(key, "Up") == 0)
                    list->scroll_offset += (total_content_height / visible_height) * scroll_speed_multiplier;
                else if (strcmp(key, "Down") == 0)
                    list->scroll_offset -= (total_content_height / visible_height) * scroll_speed_multiplier;
            }
        }
    }
    return false;
}

bool GooeyList_HandleClick(GooeyWindow *window, int mouse_x, int mouse_y)
{
    for (int i = 0; i < window->list_count; ++i)
    {
        GooeyList *list = &window->lists[i];
        if (mouse_x >= list->core.x && mouse_x <= list->core.x + list->core.width &&
            mouse_y >= list->core.y && mouse_y <= list->core.y + list->core.height)
        {
            int scroll_offset = abs(list->scroll_offset);
            int current_list_scroll_position = scroll_offset + list->core.y;
            int mouse_y_in_list = mouse_y - list->core.y;
            int selected_index = (int)-1 + (float)(current_list_scroll_position + mouse_y_in_list) / DEFAULT_ITEM_SPACING;

            if (list->callback)
                list->callback(selected_index);
            return true;
        }
    }
}

bool GooeyList_HandleThumbScroll(GooeyWindow *window, GooeyEvent *scroll_event)
{
    static int mouse_prev = -1;
    static bool is_dragging = false;

    if (mouse_prev == -1)
    {
        mouse_prev = scroll_event->mouse_move.y;
    }

    for (int i = 0; i < window->list_count; ++i)
    {
        GooeyList *list = &window->lists[i];

        int mouse_x = scroll_event->mouse_move.x;
        int mouse_y = scroll_event->mouse_move.y;

        int thumb_width = list->thumb_width;
        int thumb_height = list->thumb_height;
        int thumb_x = list->core.x + list->core.width;
        int thumb_y = list->thumb_y;
        int total_content_height = list->item_count * list->item_spacing;
        int visible_height = list->core.height;

        if (mouse_x >= thumb_x && mouse_x <= thumb_x + thumb_width &&
            mouse_y >= thumb_y && mouse_y <= thumb_y + thumb_height &&
            scroll_event->type == GOOEY_EVENT_CLICK_PRESS)
        {
            is_dragging = true;
        }

        if (is_dragging)
        {
            active_backend->InhibitResetEvents(1);

            if (scroll_event->type != GOOEY_EVENT_CLICK_RELEASE)
            {
                list->scroll_offset -= (mouse_y - mouse_prev) * (total_content_height / visible_height);
                mouse_prev = mouse_y;
                return true;
            }
            else
            {
                is_dragging = false;
                active_backend->InhibitResetEvents(0);
            }
        }
    }

    active_backend->InhibitResetEvents(0);
    return false;
}