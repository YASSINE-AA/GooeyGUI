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

#include "widgets/gooey_checkbox.h"


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