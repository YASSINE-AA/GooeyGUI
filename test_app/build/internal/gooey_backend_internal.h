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
#ifndef GOOEY_BACKEND_INTERNAL_H
#define GOOEY_BACKEND_INTERNAL_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include "utils/theme/gooey_theme_internal.h"
#include "gooey_event_internal.h"
#include "gooey_widgets_internal.h"
#include "utils/logger/gooey_logger_internal.h"

extern GooeyTheme *active_theme;

typedef enum GooeyBackends
{
    X11, /**< Linux X11 native api */
    GLFW /**< GLFW OpenGL Backend*/
} GooeyBackends;

/**
 * @brief The GooeyBackend structure contains function pointers
 *        for backend-specific operations.
 */
typedef struct GooeyBackend
{
    int (*Init)();
    void (*Cleanup)();
    GooeyWindow (*CreateWindow)(const char *title, int width, int height);
    GooeyWindow (*SpawnWindow)(const char *title, int width, int height, bool visibility);

    void (*MakeWindowVisible)(int window_id, bool visibility);
    void (*MakeWindowResizable)(bool value, int window_id);
    int (*GetCurrentClickedWindow)(void);
    void (*DestroyWindow)(void);
    void (*HideCurrentChild)(void);

    void (*SetContext)(GooeyWindow *win);
    void (*UpdateBackground)(void);
    void (*Clear)(int window_id);
    void (*Render)(int window_id);
    void (*SetForeground)(unsigned long color);
    void (*DrawText)(int x, int y, const char *text, unsigned long color, float font_size, int window_id);
    void (*FillRectangle)(int x, int y, int width, int height, unsigned long color, int window_id);
    void (*DrawRectangle)(int x, int y, int width, int height, unsigned long color, int window_id);
    void (*FillArc)(int x, int y, int width, int height, int angle1, int angle2, int window_id);
    char *(*GetKeyFromCode)(GooeyEvent *gooey_event);
    GooeyEvent* (*HandleEvents)(void);
    void (*InhibitResetEvents)(bool state);
    void (*GetWinDim)(int *width, int *height, int window_id);
    void (*DrawLine)(int x1, int y1, int x2, int y2, unsigned long color, int window_id);
    float (*GetTextWidth)(const char *text, int length);
    float (*GetTextHeight)(const char *text, int length);
    void (*SetCursor)(GOOEY_CURSOR cursor);
} GooeyBackend;

#endif
