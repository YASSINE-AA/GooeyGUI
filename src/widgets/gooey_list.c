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

#include "include/widgets/gooey_list.h"


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

