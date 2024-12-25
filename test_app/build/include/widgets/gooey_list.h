#ifndef GOOEY_LIST_H
#define GOOEY_LIST_H

#include "core/gooey_backend_internal.h"
#include "gooey_widgets_internal.h"

/**
 * @brief Adds a list widget to the window specified.
 *
 * @param window The window which you're adding the list widget to.
 * @param x The x-coordinate of the list widget.
 * @param y The y-coordinate of the list widget.
 * @param width The width of the list widget.
 * @param height The height of the list widget.
 */
GooeyList *GooeyList_Add(GooeyWindow *window, int x, int y, int width, int height);

/**
 * @brief Adds a list widget to the window specified.
 *
 * @param list The List widget you're adding the item to.
 * @param title The title of the list widget item.
 * @param description The description of the list widget item.
 * @param callback The callback attached to the said item.
 */
void GooeyList_AddItem(GooeyList *list, const char *title, const char *description, void (*callback)(void));

/**
 * @brief Handles scroll events.
 *
 * @param window The window containing the list.
 * @param event The scroll event.
 * @return if mouse scroll event then returns true, otherwise returns false. 
 */
bool GooeyList_HandleScroll(GooeyWindow *window, GooeyEvent *event);

/**
 * @brief Draws All attached list widgets onto window.
 *
 * @param window The window which you're adding the list widget to.
 */
void GooeyList_Draw(GooeyWindow *window);



#endif