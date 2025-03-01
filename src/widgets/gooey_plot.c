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
#include <float.h>

static float compare(const void *a, const void *b)
{
    return (*(float *)a - *(float *)b);
}
GooeyPlot *GooeyPlot_Add(GooeyWindow *win, GOOEY_PLOT_TYPE plot_type, GooeyPlotData *data, int x, int y, int width, int height)
{
    // Check if the maximum number of plots has been reached
    if (win->plot_count >= MAX_PLOT_COUNT)
    {
        LOG_ERROR("Couldn't add plot, exceeded max plot count.");
        return NULL;
    }

    // Get the next available plot slot and increment the plot count
    GooeyPlot *plot = &win->plots[win->plot_count++];
    *plot = (GooeyPlot){0};

    // Initialize the plot's core properties
    plot->core.x = x;
    plot->core.y = y;
    plot->core.width = width;
    plot->core.height = height;
    plot->core.type = WIDGET_PLOT;

    // Assign the plot data
    plot->data = data;

    // Sort the data if it exists
    if (data)
    {
        if (data->x_data)
            qsort(data->x_data, data->data_count, sizeof(float), compare);
        if (data->y_data)
            qsort(data->y_data, data->data_count, sizeof(float), compare);
    }

    // Set the plot type
    data->plot_type = plot_type;

    return plot;
}

void GooeyPlot_Draw(GooeyWindow *win)
{
    const uint8_t MARGIN = 40;
    const uint8_t VALUE_TICK_OFFSET = 5;

    float max_x_value = -1.0f;
    float max_y_value = -1.0f;
    float min_x_value = FLT_MAX;
    float min_y_value = FLT_MAX;

    // Find the maximum x and y values across all plots
    for (size_t i = 0; i < win->plot_count; ++i)
    {
        GooeyPlot *plot = &win->plots[i];

        for (size_t j = 0; j < plot->data->data_count; ++j)
        {
            if (plot->data->x_data[j] > max_x_value)
                max_x_value = plot->data->x_data[j];

            if (plot->data->x_data[j] < min_x_value)
                min_x_value = plot->data->x_data[j];

            if (plot->data->y_data[j] < min_y_value)
                min_y_value = plot->data->y_data[j];

            if (plot->data->y_data[j] > max_y_value)
                max_y_value = plot->data->y_data[j];
        }
    }

    // Draw each plot
    for (size_t i = 0; i < win->plot_count; ++i)
    {
        GooeyPlot *plot = &win->plots[i];

        int x_tick_count = (int)(ceilf((float)(max_x_value + fabs(min_x_value)) / plot->data->x_step)) + 1;
        LOG_INFO("%d", x_tick_count);
        int y_tick_count = (int)(ceilf((float)(max_y_value + fabs(min_y_value)) / plot->data->y_step)) + 1;
        float plot_x_coords[plot->data->data_count];
        float plot_y_coords[plot->data->data_count];
        float plot_x_grid_coords[x_tick_count - 1];
        float plot_y_grid_coords[y_tick_count - 1];
        float y_default_value = min_y_value;
        float x_default_value = min_x_value;

        // Draw the plot background
        active_backend->FillRectangle(
            plot->core.x,
            plot->core.y,
            plot->core.width,
            plot->core.height,
            active_theme->widget_base,
            win->creation_id);

        // Draw the X axis
        active_backend->DrawLine(
            plot->core.x + MARGIN,
            plot->core.y + plot->core.height - MARGIN,
            plot->core.x + plot->core.width - MARGIN,
            plot->core.y + plot->core.height - MARGIN,
            active_theme->neutral,
            win->creation_id);

        // Draw the Y axis
        active_backend->DrawLine(
            plot->core.x + MARGIN,
            plot->core.y + plot->core.height - MARGIN,
            plot->core.x + MARGIN,
            plot->core.y + MARGIN,
            active_theme->neutral,
            win->creation_id);

        // Draw the plot title
        active_backend->DrawText(
            plot->core.x + ((plot->core.width / 2) - (active_backend->GetTextWidth(plot->data->title, strlen(plot->data->title)) / 2)),
            plot->core.y + MARGIN,
            plot->data->title,
            active_theme->info,
            0.26f,
            win->creation_id);

        size_t x_data_idx = 0;
        size_t y_data_idx = 0;
        int x_value_spacing = ((float)(plot->core.width - 2 * MARGIN) / x_tick_count);
        int y_value_spacing = ((float)(plot->core.height - 2 * MARGIN) / y_tick_count);

        // Draw X axis ticks and data points
        for (size_t idx = 0; idx < x_tick_count; ++idx)
        {
            // Draw X axis tick marks
            if (idx != 0)
            {
                active_backend->DrawLine(
                    plot->core.x + x_value_spacing * idx + MARGIN,
                    plot->core.y + plot->core.height - MARGIN + VALUE_TICK_OFFSET,
                    plot->core.x + x_value_spacing * idx + MARGIN,
                    plot->core.y + plot->core.height - MARGIN - VALUE_TICK_OFFSET,
                    active_theme->info,
                    win->creation_id);

                plot_x_grid_coords[idx - 1] = plot->core.x + x_value_spacing * idx + MARGIN;
            }

            // Label the X axis tick marks
            char x_value_str[64];
            snprintf(x_value_str, sizeof(x_value_str), "%.2f", x_default_value);

            active_backend->DrawText(
                plot->core.x + x_value_spacing * idx + MARGIN,
                plot->core.y + plot->core.height - MARGIN + VALUE_TICK_OFFSET + 15,
                x_value_str,
                active_theme->neutral,
                0.26f,
                win->creation_id);

            // Draw data points for the X axis
            while (x_data_idx < plot->data->data_count &&
                   (x_data_idx == 0 || plot->data->x_data[x_data_idx] <= x_default_value))
            {

                float normalized_x;
                float x_range = max_x_value - min_x_value;

                normalized_x = (plot->data->x_data[x_data_idx] - min_x_value) / x_range;


                float x_pos = (x_tick_count - 1) * x_value_spacing * normalized_x;

                active_backend->DrawLine(
                    plot->core.x + x_pos + MARGIN,
                    plot->core.y + plot->core.height - MARGIN + VALUE_TICK_OFFSET,
                    plot->core.x + x_pos + MARGIN,
                    plot->core.y + plot->core.height - MARGIN - VALUE_TICK_OFFSET,
                    active_theme->danger,
                    win->creation_id);
                plot_x_coords[x_data_idx] = plot->core.x + x_pos + MARGIN;
                x_data_idx++;
            }
            x_default_value += plot->data->x_step;
        }

        // Draw Y axis ticks and data points
        for (size_t idx = 0; idx < y_tick_count; ++idx)
        {
            // Draw Y axis tick marks
            if (idx != 0)
            {
                active_backend->DrawLine(
                    plot->core.x + MARGIN - VALUE_TICK_OFFSET,
                    plot->core.y + plot->core.height - MARGIN - y_value_spacing * idx,
                    plot->core.x + MARGIN + VALUE_TICK_OFFSET,
                    plot->core.y + plot->core.height - MARGIN - y_value_spacing * idx,
                    active_theme->info,
                    win->creation_id);
                plot_y_grid_coords[idx - 1] = plot->core.y + plot->core.height - MARGIN - y_value_spacing * idx;
            }

            // Label the Y axis tick marks
            char y_value_str[64];
            snprintf(y_value_str, sizeof(y_value_str), "%.2f", y_default_value);

            active_backend->DrawText(
                plot->core.x,
                plot->core.y + plot->core.height - MARGIN - y_value_spacing * idx,
                y_value_str,
                active_theme->neutral,
                0.26f,
                win->creation_id);

            // Draw data points for the Y axis
            while (y_data_idx < plot->data->data_count && plot->data->y_data[y_data_idx] <= y_default_value)
            {
                float normalized_y;
                float y_range = max_y_value - min_y_value;

                normalized_y = (plot->data->y_data[y_data_idx] - min_y_value) / y_range;

                float y_pos = (y_tick_count - 1) * y_value_spacing * normalized_y;
                active_backend->DrawLine(
                    plot->core.x + MARGIN - VALUE_TICK_OFFSET,
                    plot->core.y + plot->core.height - MARGIN - y_pos,
                    plot->core.x + MARGIN + VALUE_TICK_OFFSET,
                    plot->core.y + plot->core.height - MARGIN - y_pos,
                    active_theme->danger,
                    win->creation_id);
                plot_y_coords[y_data_idx] = plot->core.y + plot->core.height - MARGIN - y_pos;

                y_data_idx++;
            }

            y_default_value += plot->data->y_step;
        }
        // Draw Grid
        for (size_t i = 0; i < x_tick_count - 1; ++i)
        {
            active_backend->DrawLine(
                plot_x_grid_coords[i],
                plot->core.y - MARGIN + plot->core.height,
                plot_x_grid_coords[i],
                plot->core.y + MARGIN,
                active_theme->base,
                win->creation_id);
        }

        for (size_t i = 0; i < y_tick_count - 1; ++i)
        {
            active_backend->DrawLine(
                plot->core.x + MARGIN,
                plot_y_grid_coords[i],
                plot->core.x - MARGIN + plot->core.width,
                plot_y_grid_coords[i],
                active_theme->base,
                win->creation_id);
        }
        switch (plot->data->plot_type)
        {
        case GOOEY_PLOT_LINE:

            // Draw data lines
            for (size_t i = 0; i < plot->data->data_count - 1; ++i)
            {

                float line_x1 = plot_x_coords[i];
                float line_x2 = plot_x_coords[i + 1];

                float line_y1 = plot_y_coords[i];
                float line_y2 = plot_y_coords[i + 1];

                active_backend->DrawLine(
                    line_x1,
                    line_y1,
                    line_x2,
                    line_y2,
                    active_theme->info,
                    win->creation_id);
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
