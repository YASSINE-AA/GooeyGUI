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

GooeyList *GooeyList_Add(GooeyWindow *win, int x, int y, int width, int height)
{

    GooeyList *list = &win->lists[win->list_count++];
    *list = (GooeyList){0};
    list->core.x = x;
    list->core.y = y;
    list->core.width = width;
    list->core.height = height;
    list->items = (GooeyListItem *)malloc(sizeof(GooeyListItem) * 1024);
    list->item_count = 0;
    list->scroll_offset = 1;
    list->thumb_y = list->core.y;
    list->thumb_height = -1;
    // TODO: Make these customizable
    list->thumb_width = 10;
    list->item_spacing = 40;
    GooeyWindow_RegisterWidget(win, (GooeyWidget *)&list->core);

    return list;
}

void GooeyList_AddItem(GooeyList *list, const char *title, const char *description, void (*callback)(void))
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

        int total_content_height = list->item_count * list->item_spacing;
        if (total_content_height > 0)
        {
            int visible_height = list->core.height;
            int max_scroll_offset = total_content_height > visible_height
                                        ? total_content_height - visible_height
                                        : 0;

            if (list->scroll_offset < -max_scroll_offset)
                list->scroll_offset = -max_scroll_offset;
            if (list->scroll_offset > 0)
                list->scroll_offset = 0;
            int current_y_offset = list->core.y + list->scroll_offset + 10;
            list->thumb_height = total_content_height <= visible_height ? list->core.height : (int)visible_height * ((float)visible_height / (total_content_height));
            list->thumb_y = list->core.y - (int)(float)((list->scroll_offset) * visible_height) / (total_content_height);
            active_backend->FillRectangle(
                list->core.x + list->core.width,
                list->thumb_y,
                list->thumb_width,
                list->thumb_height,
                active_theme->primary,
                win->creation_id);

            for (size_t j = 0; j < list->item_count; ++j)
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

                int line_seperator_y = current_y_offset + list->item_spacing - 10;

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
                current_y_offset += list->item_spacing;
            }
        }
    }
}

bool GooeyList_HandleScroll(GooeyWindow *window, GooeyEvent *scroll_event)
{
    static int mouse_prev = -1;
    static size_t test = 0;

    if (mouse_prev == -1)
        mouse_prev = scroll_event->mouse_move.y;

    static bool is_dragging = false;

    for (int i = 0; i < window->list_count; ++i)
    {
        GooeyList *list = &window->lists[i];

        int mouse_x = scroll_event->mouse_move.x;
        int mouse_y = scroll_event->mouse_move.y;

        int thumb_width = list->thumb_width;
        int thumb_height = list->thumb_height;
        int thumb_x = list->core.x + list->core.width;
        int thumb_y = list->thumb_y;
        const int scroll_speed_multiplier = 2;
        int total_content_height = list->item_count * list->item_spacing;
        int visible_height = list->core.height;

        if (mouse_x >= thumb_x && mouse_x <= thumb_x + thumb_width && mouse_y >= thumb_y && mouse_y <= thumb_y + thumb_height && scroll_event->type == GOOEY_EVENT_CLICK_PRESS)
        {
            // Inhibit other events for drag.
            is_dragging = true;
        }
        else if (mouse_x >= list->core.x && mouse_x <= list->core.x + list->core.width && mouse_y >= list->core.y && mouse_y <= list->core.y + list->core.height && scroll_event->type == GOOEY_EVENT_MOUSE_SCROLL)
        {
            int scroll_offset_amount = scroll_event->mouse_scroll.y * (total_content_height / visible_height) * scroll_speed_multiplier;
            list->scroll_offset += scroll_offset_amount;
            return true;
        }

        if (is_dragging)
        {
            active_backend->InhibitResetEvents(1);

            if (scroll_event->type != GOOEY_EVENT_CLICK_RELEASE)
            {

                if (mouse_prev - mouse_y > 0)
                    list->scroll_offset -= (mouse_y - mouse_prev) * (total_content_height / visible_height);
                else
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
