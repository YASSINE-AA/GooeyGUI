#ifndef GOOEY_BACKEND_H
#define GOOEY_BACKEND_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils/themes/theme_parser.h"
#include "utils/gooey_event.h"
#include "utils/gooey_widgets.h"

extern GooeyTheme *active_theme;

typedef enum GooeyBackends
{
    X11,
    GLFW
} GooeyBackends;

typedef struct GooeyWindow
{
    GooeyWindowExt widgets;
} GooeyWindow;

/**
 * @brief The GooeyBackend structure contains function pointers
 *        for backend-specific operations.
 */
typedef struct GooeyBackend
{
    int (*Init)();
    void (*Cleanup)();
    GooeyWindow (*CreateWindow)(const char *title, int width, int height);
    void (*DestroyWindow)(void);
    void (*UpdateBackground)(void);
    void (*Clear)();
    void (*Render)();
    void (*SetForeground)(unsigned long color);
    void (*DrawText)(int x, int y, const char *text, unsigned long color);
    void (*FillRectangle)(int x, int y, int width, int height, unsigned long color);
    void (*DrawRectangle)(int x, int y, int width, int height, unsigned long color);
    void (*FillArc)(int x, int y, int width, int height, int angle1, int angle2);
    char *(*GetKeyFromCode)(GooeyEvent *gooey_event);
    GooeyEvent (*HandleEvents)();
    void (*GetWinDim)(int *width, int *height);
    void (*DrawLine)(int x1, int y1, int x2, int y2, unsigned long color);
    float (*GetTextWidth)(const char *text, int length);
} GooeyBackend;

#endif
