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


#include "include/widgets/gooey_slider.h"



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
