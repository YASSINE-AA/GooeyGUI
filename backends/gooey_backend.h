#ifndef GOOEY_BACKEND_H
#define GOOEY_BACKEND_H

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "gooey_event.h"
#include "gooey_colors.h"
#include "gooey_widgets.h"

typedef struct
{
    GooeyWindowExt widgets; /**< Custom extension for managing widgets. */
    int width;
    int height;
} GooeyWindow;

/**
 * @brief Initializes the Gooey backend.
 *
 * Establishes a connection to the X server and prepares the backend for rendering.
 *
 * @return `true` if initialization succeeds, `false` otherwise.
 */
bool GooeyBackend_Init();

/**
 * @brief Cleans up the Gooey backend.
 *
 * Releases resources and closes the X server connection.
 */
void GooeyBackend_Cleanup();

/**
 * @brief Creates a new Gooey window.
 *
 * Initializes a new window with the specified dimensions and title.
 *
 * @param title The title of the window.
 * @param width The width of the window in pixels.
 * @param height The height of the window in pixels.
 * @return A `GooeyWindow` object representing the created window.
 */
GooeyWindow GooeyBackend_CreateWindow(const char *title, int width, int height);

/**
 * @brief Destroys an Gooey window.
 *
 * Releases resources associated with the specified window.
 *
 */
void GooeyBackend_DestroyWindow();

/**
 * @brief Clears the contents of a window.
 *
 * Fills the window with the background color, erasing its contents.
 *
 */
void GooeyBackend_Clear();

/**
 * @brief Renders all pending graphics.
 *
 * Flushes the Gooey graphics pipeline to ensure all pending operations are displayed.
 */
void GooeyBackend_Render();

/**
 * @brief Sets the foreground color for subsequent drawing operations.
 *
 * @param color The color in RGB format (e.g., 0xFFFFFF for white).
 */
void GooeyBackend_SetForeground(unsigned long color);

/**
 * @brief Draws text on the window.
 *
 * Renders the specified text string at the given position in the window.
 *
 * @param x The x-coordinate of the text's starting position.
 * @param y The y-coordinate of the text's baseline.
 * @param text The text string to draw.
 * @param color The color of the text in RGB format (e.g., 0xFFFFFF for white).
 */
void GooeyBackend_DrawText(int x, int y, const char *text, unsigned long color);

/**
 * @brief Draws a filled rectangle on the window.
 *
 * @param x The x-coordinate of the rectangle's top-left corner.
 * @param y The y-coordinate of the rectangle's top-left corner.
 * @param width The width of the rectangle in pixels.
 * @param height The height of the rectangle in pixels.
 * @param color The fill color in RGB format (e.g., 0xFF0000 for red).
 */
void GooeyBackend_FillRectangle(int x, int y, int width, int height, unsigned long color);
/**
 * @brief Draws the outline of a rectangle on the window.
 *
 * @param x The x-coordinate of the rectangle's top-left corner.
 * @param y The y-coordinate of the rectangle's top-left corner.
 * @param width The width of the rectangle in pixels.
 * @param height The height of the rectangle in pixels.
 * @param color The outline color in RGB format (e.g., 0x0000FF for blue).
 */
void GooeyBackend_DrawRectangle(int x, int y, int width, int height, unsigned long color);

/**
 * @brief Processes Gooey events for a window.
 *
 * Handles input and window interaction events. Custom event processing can be implemented via a callback.
 *
 * @return The processed `GooeyEvent`.
 */
GooeyEvent GooeyBackend_HandleEvents();

/**
 * @brief Gets height of window.
 *
 * @param window A pointer to the `GooeyWindow` whose events will be processed.
 * @return height integer.
 */
int GooeyBackend_getHeight(GooeyWindow *window);

/**
 * @brief Gets width of window.
 *
 * @param window A pointer to the `GooeyWindow` whose events will be processed.
 * @return width integer;
 */
int GooeyBackend_getWidth(GooeyWindow *window);


/**
 * @brief Draw line on window.
 *
 * @param x1 line start x-coordinate.
 * @param y1 line start y-coordinate.
 * @param x2 line end x-coordinate.
 * @param y2 line end y-coordinate.
 * @param color color of line.
 */

void GooeyBackend_DrawLine(int x1, int y1, int x2, int y2, unsigned long color);

/**
 * @brief Get width of text.
 *
 * @param text text string.
 * @param width of text.
 */
int GooeyBackend_GetTextWidth(const char *text, int length);

#endif