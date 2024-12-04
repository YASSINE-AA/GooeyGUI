#include "gooey_backend.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    Display *display;      /**< X11 display connection. */
    Window window;         /**< X11 window identifier. */
    GC gc;                 /**< Graphics context for rendering. */
    Atom wm_delete_window; /**< Atom for handling window close events. */
    XFontStruct *font;     /**< X11 font structure for text rendering. */
} GooeyBackendContext;

static GooeyBackendContext ctx = {0};

bool GooeyBackend_Init()
{
    ctx.display = XOpenDisplay(NULL);
    if (!ctx.display)
    {
        fprintf(stderr, "Failed to open X display\n");
        return false;
    }

    ctx.font = XLoadQueryFont(ctx.display, "fixed");
    if (!ctx.font)
    {
        fprintf(stderr, "Failed to load system font\n");
        XCloseDisplay(ctx.display);
        return false;
    }
    return true;
}

void GooeyBackend_Cleanup()
{
    if (ctx.font)
    {
        XFreeFont(ctx.display, ctx.font);
    }
    if (ctx.display)
    {
        XCloseDisplay(ctx.display);
    }

    memset(&ctx, 0, sizeof(ctx));
}
GooeyWindow GooeyBackend_CreateWindow(const char *title, int width, int height)
{
    GooeyWindow window;

    window.width = width;
    window.height = height;

    int screen = DefaultScreen(ctx.display);

    ctx.window = XCreateSimpleWindow(
        ctx.display,
        RootWindow(ctx.display, screen),
        10, 10, width, height, 1,
        BlackPixel(ctx.display, screen),
        WhitePixel(ctx.display, screen));

    XStoreName(ctx.display, ctx.window, title);

    ctx.gc = XCreateGC(ctx.display, ctx.window, 0, NULL);

    ctx.wm_delete_window = XInternAtom(ctx.display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(ctx.display, ctx.window, &ctx.wm_delete_window, 1);

    XSelectInput(ctx.display, ctx.window,
                 ExposureMask | ButtonPressMask | KeyPressMask | StructureNotifyMask);

    XMapWindow(ctx.display, ctx.window);

    return window;
}

void GooeyBackend_DestroyWindow()
{

    if (ctx.window)
    {
        XDestroyWindow(ctx.display, ctx.window);
    }
}

void GooeyBackend_Render()
{
    XFlush(ctx.display);
}

void GooeyBackend_Clear()
{
    XClearWindow(ctx.display, ctx.window);
}

void GooeyBackend_SetForeground(unsigned long color)
{

    XSetForeground(ctx.display, ctx.gc,
                   color);
}

void GooeyBackend_DrawText(int x, int y, const char *text, unsigned long color)
{

    XSetForeground(ctx.display, ctx.gc, color);
    XDrawString(ctx.display, ctx.window, ctx.gc, x, y, text, strlen(text));
}

void GooeyBackend_FillRectangle(int x, int y, int width, int height, unsigned long color)
{

    XSetForeground(ctx.display, ctx.gc, color);
    XFillRectangle(ctx.display, ctx.window, ctx.gc, x, y, width, height);
}

void GooeyBackend_DrawRectangle(int x, int y, int width, int height, unsigned long color)
{

    XSetForeground(ctx.display, ctx.gc, color);
    XDrawRectangle(ctx.display, ctx.window, ctx.gc, x, y,
                   width, height);
}

int GooeyBackend_getHeight(GooeyWindow *window)
{
    return window->height;
}

int GooeyBackend_getWidth(GooeyWindow *window)
{
    return window->width;
}

void GooeyBackend_DrawLine(int x1, int y1, int x2, int y2, unsigned long color)
{
    XSetForeground(ctx.display, ctx.gc, color);
    XDrawLine(ctx.display, ctx.window, ctx.gc, x1,
              y1, x2,
              y2);
}

int GooeyBackend_GetTextWidth(const char *text, int length){
    return XTextWidth(ctx.font, text, length);}

GooeyEvent GooeyBackend_HandleEvents()
{
    XEvent event;
    GooeyEvent gooey_event = {0};

    if (XPending(ctx.display) > 0)
    {
        XNextEvent(ctx.display, &event);

        if (event.type == ClientMessage && event.xclient.data.l[0] == ctx.wm_delete_window)
        {
            gooey_event.type = GOOEY_EVENT_WINDOW_CLOSE;
            return gooey_event;
        }

        if (event.type == ButtonPress)
        {
            gooey_event.type = GOOEY_EVENT_CLICK;
            gooey_event.data.click.x = event.xbutton.x;
            gooey_event.data.click.y = event.xbutton.y;
            return gooey_event;
        }

        if (event.type == KeyPress)
        {
            gooey_event.type = GOOEY_EVENT_KEY_PRESS;
            gooey_event.data.key_press.keycode = event.xkey.keycode;
            return gooey_event;
        }

        if (event.type == MotionNotify)
        {
            gooey_event.type = GOOEY_EVENT_MOUSE_MOVE;
            gooey_event.data.mouse_move.x = event.xmotion.x;
            gooey_event.data.mouse_move.y = event.xmotion.y;
            return gooey_event;
        }

        if (event.type == Expose)
        {
            gooey_event.type = GOOEY_EVENT_EXPOSE;
            return gooey_event;
        }
    }

    return gooey_event;
}
