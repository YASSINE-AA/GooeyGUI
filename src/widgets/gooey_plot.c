/*
 Copyright (c) 2025 Yassine Ahmed Ali

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

#include <widgets/gooey_plot.h>
#include <stdint.h>
#include <math.h>

static int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

GooeyPlot *GooeyPlot_Add(GooeyWindow *win, GOOEY_PLOT_TYPE plot_type, GooeyPlotData *data, int x, int y, int width, int height)
{
    if (win->plot_count >= MAX_PLOT_COUNT)
    {
        LOG_ERROR("Couldn't add plot, exceeded max plot count.");
        return NULL;
    }

    GooeyPlot *plot = &win->plots[win->plot_count++];
    plot->core.height = height;
    plot->core.width = width;
    plot->core.x = x;
    plot->core.y = y;
    plot->data = data;
    // Quick sort the data
    if (data)
    {
        if (data->x_data)
            qsort(plot->data->x_data, plot->data->data_count, sizeof(float), compare);
        if (data->y_data)
            qsort(plot->data->y_data, plot->data->data_count, sizeof(float), compare);
    }

    plot->data->plot_type = plot_type;
    plot->core.type = WIDGET_PLOT;

    return plot;
}
void GooeyPlot_Draw(GooeyWindow *win)
{
    const uint8_t margin = 40;
    const uint8_t value_indicator_offset = 5;
    float max_x_value = -1.0f;
    float max_y_value = -1.0f;

    // Find max x and y values
    for (size_t i = 0; i < win->plot_count; ++i)
    {
        GooeyPlot *plot = &win->plots[i];
        for (size_t j = 0; j < plot->data->data_count; ++j)
        {
            if (plot->data->x_data[j] > max_x_value)
                max_x_value = plot->data->x_data[j];

            if (plot->data->y_data[j] > max_y_value)
                max_y_value = plot->data->y_data[j];
        }
    }

    for (size_t i = 0; i < win->plot_count; ++i)
    {
        GooeyPlot *plot = &win->plots[i];

        int x_indicator_count = roundf((max_x_value / plot->data->x_step));
        int y_indicator_count = roundf((max_y_value / plot->data->y_step));

        // Background
        active_backend->FillRectangle(
            plot->core.x,
            plot->core.y,
            plot->core.width,
            plot->core.height,
            active_theme->widget_base,
            win->creation_id);

        // X axis
        active_backend->DrawLine(
            plot->core.x + margin,
            plot->core.y + plot->core.height - margin,
            plot->core.x + plot->core.width - margin,
            plot->core.y + plot->core.height - margin,
            active_theme->neutral,
            win->creation_id);

        // Y axis
        active_backend->DrawLine(
            plot->core.x + margin,
            plot->core.y + plot->core.height - margin,
            plot->core.x + margin,
            plot->core.y + margin,
            active_theme->neutral,
            win->creation_id);

        // Plot title
        active_backend->DrawText(
            plot->core.x + ((plot->core.width / 2) - (active_backend->GetTextWidth(plot->data->title, strlen(plot->data->title)) / 2)),
            plot->core.y + margin,
            plot->data->title,
            active_theme->info,
            0.26f, win->creation_id);

        size_t x_data_idx = 0;
        size_t y_data_idx = 0;

        switch (plot->data->plot_type)
        {
        case GOOEY_PLOT_LINE:
            int x_value_spacing = ((float)(plot->core.width - 2 * margin) / x_indicator_count);
            int y_value_spacing = ((float)(plot->core.height - 2 * margin) / y_indicator_count);

            // Draw X axis indicators
            for (size_t idx = 0; idx < x_indicator_count; ++idx)
            {
                // X axis data indicators
                active_backend->DrawLine(
                    plot->core.x + x_value_spacing * idx + margin,
                    plot->core.y + plot->core.height - margin + value_indicator_offset,
                    plot->core.x + x_value_spacing * idx + margin,
                    plot->core.y + plot->core.height - margin - value_indicator_offset,
                    active_theme->info,
                    win->creation_id);

                char x_value_str[64];
                float x_value = plot->data->x_step * idx;
                snprintf(x_value_str, sizeof(x_value_str), "%.2f", x_value);

                active_backend->DrawText(
                    plot->core.x + x_value_spacing * idx + margin,
                    plot->core.y + plot->core.height - margin + value_indicator_offset + 15,
                    x_value_str,
                    active_theme->neutral,
                    0.26f,
                    win->creation_id);

                // Only draw points if data exists within the range
                while (x_data_idx < plot->data->data_count && plot->data->x_data[x_data_idx] <= x_value)
                {
                    // Calculate position based on max_x_value
                    float x_pos = (plot->core.width - 2 * margin) * (plot->data->x_data[x_data_idx] / max_x_value);
                    active_backend->DrawLine(
                        plot->core.x + x_pos + margin,
                        plot->core.y + plot->core.height - margin + value_indicator_offset,
                        plot->core.x + x_pos + margin,
                        plot->core.y + plot->core.height - margin - value_indicator_offset,
                        active_theme->danger,
                        win->creation_id);

                    x_data_idx++;
                }
            }

            // Draw Y axis indicators
            for (size_t idx = 0; idx < y_indicator_count; ++idx)
            {
                active_backend->DrawLine(
                    plot->core.x + margin - value_indicator_offset,
                    plot->core.y + plot->core.height - margin - y_value_spacing * idx,
                    plot->core.x + margin + value_indicator_offset,
                    plot->core.y + plot->core.height - margin - y_value_spacing * idx,
                    active_theme->info,
                    win->creation_id);

                char y_value_str[64];
                float y_value = plot->data->y_step * idx;
                snprintf(y_value_str, sizeof(y_value_str), "%.2f", y_value);

                active_backend->DrawText(
                    plot->core.x,
                    plot->core.y + plot->core.height - margin - y_value_spacing * idx,
                    y_value_str,
                    active_theme->neutral,
                    0.26f,
                    win->creation_id);

                // Only draw points if data exists within the range
                while (y_data_idx < plot->data->data_count && plot->data->y_data[y_data_idx] <= y_value)
                {
                    // Calculate position based on max_y_value
                    float y_pos = (plot->core.height - 2 * margin) * (plot->data->y_data[y_data_idx] / max_y_value);
                    active_backend->DrawLine(
                        plot->core.x + margin - value_indicator_offset,
                        plot->core.y + plot->core.height - margin - y_pos,
                        plot->core.x + margin + value_indicator_offset,
                        plot->core.y + plot->core.height - margin - y_pos,
                        active_theme->danger,
                        win->creation_id);

                    y_data_idx++;
                }
            }
            break;
        }
    }
}

void GooeyPlot_Update(GooeyPlot *plot, GooeyPlotData *new_data)
{
    if (!new_data)
    {
        LOG_ERROR("Couldn't update plot data, new data is NULL");
        return;
    }

    plot->data = new_data;
}
