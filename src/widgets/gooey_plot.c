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
#include <stdlib.h>
#include <string.h>

typedef struct
{
    float x;
    float y;
} DataPoint;

static int compare_data_points(const void *a, const void *b)
{
    return (*(DataPoint *)a).x - (*(DataPoint *)b).x;
}

static void sort_data(GooeyPlotData *data)
{
    if (!data || !data->x_data || !data->y_data || data->data_count == 0)
    {
        return;
    }

    DataPoint *points = malloc(data->data_count * sizeof(DataPoint));
    if (!points)
    {
        LOG_ERROR("Failed to allocate memory for sorting.");
        return;
    }

    for (size_t i = 0; i < data->data_count; ++i)
    {
        points[i].x = data->x_data[i];
        points[i].y = data->y_data[i];
    }

    qsort(points, data->data_count, sizeof(DataPoint), compare_data_points);

    for (size_t i = 0; i < data->data_count; ++i)
    {
        data->x_data[i] = points[i].x;
        data->y_data[i] = points[i].y;
    }

    free(points);
}

GooeyPlot *GooeyPlot_Add(GooeyWindow *win, GOOEY_PLOT_TYPE plot_type, GooeyPlotData *data, int x, int y, int width, int height)
{
    if (!win || !data)
    {
        LOG_ERROR("Invalid window or data provided.");
        return NULL;
    }

    if (win->plot_count >= MAX_PLOT_COUNT)
    {
        LOG_ERROR("Couldn't add plot, exceeded max plot count.");
        return NULL;
    }

    GooeyPlot *plot = &win->plots[win->plot_count++];
    *plot = (GooeyPlot){0};

    plot->core.x = x;
    plot->core.y = y;
    plot->core.width = width;
    plot->core.height = height;
    plot->core.type = WIDGET_PLOT;

    plot->data = data;
    plot->data->plot_type = plot_type;

    if (data)
    {
        sort_data(data);
    }

    return plot;
}

void GooeyPlot_Draw(GooeyWindow *win)
{
    if (!win || win->plot_count == 0)
    {
        return;
    }

    const uint8_t MARGIN = 40;
    const uint8_t VALUE_TICK_OFFSET = 5;

    float max_x_value = -FLT_MAX;
    float max_y_value = -FLT_MAX;
    float min_x_value = FLT_MAX;
    float min_y_value = FLT_MAX;

    for (size_t i = 0; i < win->plot_count; ++i)
    {
        GooeyPlot *plot = &win->plots[i];
        if (!plot->data || !plot->data->x_data || !plot->data->y_data)
        {
            continue;
        }

        for (size_t j = 0; j < plot->data->data_count; ++j)
        {
            if (plot->data->x_data[j] > max_x_value)
                max_x_value = plot->data->x_data[j];
            if (plot->data->x_data[j] < min_x_value)
                min_x_value = plot->data->x_data[j];
            if (plot->data->y_data[j] > max_y_value)
                max_y_value = plot->data->y_data[j];
            if (plot->data->y_data[j] < min_y_value)
                min_y_value = plot->data->y_data[j];
        }
    }

    float x_range = max_x_value - min_x_value;
    float y_range = max_y_value - min_y_value;
    if (x_range == 0)
        x_range = 1;
    if (y_range == 0)
        y_range = 1;

    for (size_t i = 0; i < win->plot_count; ++i)
    {
        GooeyPlot *plot = &win->plots[i];
        if (!plot->data || !plot->data->x_data || !plot->data->y_data)
        {
            continue;
        }

        uint32_t x_tick_count = (uint32_t)(ceilf(x_range / plot->data->x_step)) + 1;
        uint32_t y_tick_count = (uint32_t)(ceilf(y_range / plot->data->y_step)) + 1;

        float *plot_x_coords = malloc(plot->data->data_count * sizeof(float));
        float *plot_y_coords = malloc(plot->data->data_count * sizeof(float));
        float *plot_x_grid_coords = malloc((x_tick_count - 1) * sizeof(float));
        float *plot_y_grid_coords = malloc((y_tick_count - 1) * sizeof(float));

        if (!plot_x_coords || !plot_y_coords || !plot_x_grid_coords || !plot_y_grid_coords)
        {
            LOG_ERROR("Failed to allocate memory for plot coordinates.");
            free(plot_x_coords);
            free(plot_y_coords);
            free(plot_x_grid_coords);
            free(plot_y_grid_coords);
            continue;
        }

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
            plot->core.y + MARGIN / 2,
            plot->data->title,
            active_theme->primary,
            0.26f,
            win->creation_id);

        // Draw X axis ticks and data points
        float x_default_value = min_x_value;
        float x_value_spacing = (plot->core.width - 2 * MARGIN) / (x_tick_count - 1);
        for (size_t idx = 0; idx < x_tick_count; ++idx)
        {
            if (idx != 0)
            {
                active_backend->DrawLine(
                    plot->core.x + MARGIN + x_value_spacing * idx,
                    plot->core.y + plot->core.height - MARGIN + VALUE_TICK_OFFSET,
                    plot->core.x + MARGIN + x_value_spacing * idx,
                    plot->core.y + plot->core.height - MARGIN - VALUE_TICK_OFFSET,
                    active_theme->primary,
                    win->creation_id);

                plot_x_grid_coords[idx - 1] = plot->core.x + MARGIN + x_value_spacing * idx;
            }

            char x_value_str[64];
            snprintf(x_value_str, sizeof(x_value_str), "%.2f", x_default_value);
            active_backend->DrawText(
                plot->core.x + MARGIN + x_value_spacing * idx,
                plot->core.y + plot->core.height - MARGIN + VALUE_TICK_OFFSET + 15,
                x_value_str,
                active_theme->neutral,
                0.26f,
                win->creation_id);

            x_default_value += plot->data->x_step;
        }

        // Draw Y axis ticks and data points
        float y_default_value = min_y_value;
        float y_value_spacing = (plot->core.height - 2 * MARGIN) / (y_tick_count - 1);
        for (size_t idx = 0; idx < y_tick_count; ++idx)
        {
            if (idx != 0)
            {
                active_backend->DrawLine(
                    plot->core.x + MARGIN - VALUE_TICK_OFFSET,
                    plot->core.y + plot->core.height - MARGIN - y_value_spacing * idx,
                    plot->core.x + MARGIN + VALUE_TICK_OFFSET,
                    plot->core.y + plot->core.height - MARGIN - y_value_spacing * idx,
                    active_theme->primary,
                    win->creation_id);

                plot_y_grid_coords[idx - 1] = plot->core.y + plot->core.height - MARGIN - y_value_spacing * idx;
            }

            char y_value_str[64];
            snprintf(y_value_str, sizeof(y_value_str), "%.2f", y_default_value);
            active_backend->DrawText(
                plot->core.x,
                plot->core.y + plot->core.height - MARGIN - y_value_spacing * idx,
                y_value_str,
                active_theme->neutral,
                0.26f,
                win->creation_id);

            y_default_value += plot->data->y_step;
        }

        // Draw grid lines
        for (size_t i = 0; i < x_tick_count - 1; ++i)
        {
            active_backend->DrawLine(
                plot_x_grid_coords[i],
                plot->core.y + plot->core.height - MARGIN,
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
                plot->core.x + plot->core.width - MARGIN,
                plot_y_grid_coords[i],
                active_theme->base,
                win->creation_id);
        }

        // Draw data points and lines
        for (size_t j = 0; j < plot->data->data_count; ++j)
        {
            float x_axis_length = ((x_tick_count - 1)*plot->data->x_step);
            float y_axis_length = ((y_tick_count - 1)*plot->data->y_step);

            float normalized_x = (float)(plot->data->x_data[j] - min_x_value) / x_axis_length;
            float normalized_y = (float)(plot->data->y_data[j] - min_y_value) / y_axis_length;

            plot_x_coords[j] = plot->core.x + MARGIN + normalized_x * (plot->core.width - 2 * MARGIN);
            plot_y_coords[j] = plot->core.y + plot->core.height - MARGIN - normalized_y * (plot->core.height - 2 * MARGIN);
        }

        if (plot->data->plot_type == GOOEY_PLOT_LINE)
        {
            for (size_t j = 0; j < plot->data->data_count - 1; ++j)
            {
                active_backend->DrawLine(
                    plot_x_coords[j],
                    plot_y_coords[j],
                    plot_x_coords[j + 1],
                    plot_y_coords[j + 1],
                    active_theme->primary,
                    win->creation_id);
            }
        }
        else if (plot->data->plot_type == GOOEY_PLOT_BAR)
        {

            for (size_t j = 0; j < plot->data->data_count; ++j)
            {

                const uint8_t bar_width = 30;

                float bar_x = plot_x_coords[j] - ((float)bar_width / 2);
                float bar_height = (plot->core.y + plot->core.height - MARGIN) - plot_y_coords[j];
                float bar_y = plot_y_coords[j];
                active_backend->FillRectangle(
                    bar_x,
                    bar_y,
                    bar_width,
                    bar_height,
                    active_theme->primary,
                    win->creation_id);
            }
        }

        free(plot_x_coords);
        free(plot_y_coords);
        free(plot_x_grid_coords);
        free(plot_y_grid_coords);
    }
}

void GooeyPlot_Update(GooeyPlot *plot, GooeyPlotData *new_data)
{
    if (!plot || !new_data)
    {
        LOG_ERROR("Invalid plot or data provided.");
        return;
    }

    plot->data = new_data;
    sort_data(new_data);
}