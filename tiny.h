#ifndef TINY_H
#define TINY_H

#include "backends/gooey_backend.h"

int Gooey_Init()
{
    if (!GooeyBackend_Init())
    {
        return -1;
    }
}

/**
 * @brief Creates a new Gooey window.
 *
 * This function initializes a new window with the given title, width, and height.
 *
 * @param title The title of the window.
 * @param width The width of the window.
 * @param height The height of the window.
 * @return A new GooeyWindow object.
 */
GooeyWindow GooeyWindow_Create(const char *title, int width, int height);

/**
 * @brief Runs the Gooey window.
 *
 * This function enters the main event loop and begins processing user input
 * and window events for the provided Gooey window.
 *
 * @param win The window to run.
 */
void GooeyWindow_Run(GooeyWindow *win);

/**
 * @brief Adds a button to the window.
 *
 * @param win The window to add the button to.
 * @param label The label to display on the button.
 * @param x The x-coordinate of the button's position.
 * @param y The y-coordinate of the button's position.
 * @param width The width of the button.
 * @param height The height of the button.
 * @param callback The callback function to call when the button is clicked.
 * @return A new GooeyButton object.
 */
GooeyButton *GooeyButton_Add(GooeyWindow *win, const char *label, int x, int y,
                             int width, int height, void (*callback)());

/**
 * @brief Handles button click events.
 *
 * @param win The window containing the button.
 * @param x The x-coordinate of the click event.
 * @param y The y-coordinate of the click event.
 * @return True if the button was clicked, false otherwise.
 */
bool GooeyButton_HandleClick(GooeyWindow *win, int x, int y);

/**
 * @brief Sets the text of the button.
 *
 * @param button The button to set the text for.
 * @param text The new text to display on the button.
 */
void GooeyButton_setText(GooeyButton *button, const char *text);

/**
 * @brief Sets the menu for the Gooey window.
 *
 * Initializes and sets the menu for the given window.
 *
 * @param win The window to set the menu for.
 * @return A new GooeyMenu object.
 */
GooeyMenu *GooeyMenu_Set(GooeyWindow *win);

/**
 * @brief Draws the menu for the window.
 *
 * Renders the menu on the window.
 *
 * @param win The window to draw the menu for.
 */
void GooeyMenu_Draw(GooeyWindow *win);

/**
 * @brief Adds a child menu item to the window's menu.
 *
 * @param win The window to add the menu item to.
 * @param title The title of the menu item.
 * @return A new GooeyMenuChild object.
 */
GooeyMenuChild *GooeyMenu_AddChild(GooeyWindow *win, char *title);

/**
 * @brief Handles menu item click events.
 *
 * @param win The window containing the menu.
 * @param x The x-coordinate of the click event.
 * @param y The y-coordinate of the click event.
 */
void GooeyMenu_HandleClick(GooeyWindow *win, int x, int y);

void GooeyButton_Draw(GooeyWindow *win, GooeyButton *button);

/**
 * @brief Adds a textbox to the window.
 *
 * @param win The window to add the textbox to.
 * @param x The x-coordinate of the textbox's position.
 * @param y The y-coordinate of the textbox's position.
 * @param width The width of the textbox.
 * @param height The height of the textbox.
 * @param onTextChanged The callback function to call when the text changes.
 * @return A new GooeyTextbox object.
 */
GooeyTextbox *GooeyTextBox_Add(GooeyWindow *win, int x, int y, int width,
                               int height, void (*onTextChanged)(char *text));

/**
 * @brief Draws the textbox on the window.
 *
 * @param win The window to draw the textbox on.
 * @param index The index of the textbox to draw.
 */
void GooeyTextbox_Draw(GooeyWindow *win, int index);

/**
 * @brief Handles textbox click events.
 *
 * @param win The window containing the textbox.
 * @param x The x-coordinate of the click event.
 * @param y The y-coordinate of the click event.
 * @return True if the textbox was clicked, false otherwise.
 */
bool GooeyTextbox_HandleClick(GooeyWindow *win, int x, int y);

/**
 * @brief Handles key press events for the textbox.
 *
 * @param win The window containing the textbox.
 * @param key_event The key event to handle.
 */
void GooeyTextbox_HandleKeyPress(GooeyWindow *win, XKeyEvent *key_event);

/**
 * @brief Gets the text of the textbox.
 *
 * @param textbox The textbox to retrieve text from.
 * @return The current text of the textbox.
 */
const char *GooeyTextbox_getText(GooeyTextbox *textbox);

/**
 * @brief Sets the text of the textbox.
 *
 * @param textbox The textbox to set the text for.
 * @param text The new text to set in the textbox.
 */
void GooeyTextbox_setText(GooeyTextbox *textbox, const char *text);

GooeyWindow GooeyWindow_Create(const char *title, int width, int height)
{
    GooeyWindow win = GooeyBackend_CreateWindow(title, width, height);
    win.widgets.button_count = 0;
    win.widgets.label_count = 0;
    win.widgets.checkbox_count = 0;
    win.widgets.radio_button_count = 0;
    win.widgets.radio_button_group_count = 0;
    win.widgets.slider_count = 0;
    win.widgets.dropdown_count = 0;
    win.widgets.textboxes_count = 0;
    win.widgets.layout_count = 0;

    return win;
}

void GooeyButton_Draw(GooeyWindow *win, GooeyButton *button)
{

    GooeyBackend_FillRectangle(button->core.x,
                               button->core.y, button->core.width, button->core.height, button->clicked ? COLOR_BLUE : COLOR_GRAY);

    GooeyBackend_DrawText(button->core.x + 5,
                          button->core.y + 20, button->label, button->clicked ? COLOR_WHITE : COLOR_BLACK);
    GooeyBackend_SetForeground(COLOR_BLACK);
}

GooeyButton *GooeyButton_Add(GooeyWindow *win, const char *label, int x, int y,
                             int width, int height, void (*callback)())
{
    GooeyButton *button = &win->widgets.buttons[win->widgets.button_count++];
    button->core.type = WIDGET_BUTTON,
    button->core.x = x;
    button->core.y = y;
    button->core.width = width;
    button->core.height = height;
    strcpy(button->label, label);
    button->callback = callback;
    button->hover = false;
    button->clicked = false;
    return button;
}
bool GooeyButton_HandleClick(GooeyWindow *win, int x, int y)
{
    bool clicked_any_button = false;

    for (int i = 0; i < win->widgets.button_count; ++i)
    {
        GooeyButton *button = &win->widgets.buttons[i];
        bool is_within_bounds = (x >= button->core.x && x <= button->core.x + button->core.width) &&
                                (y >= button->core.y && y <= button->core.y + button->core.height);

        if (is_within_bounds)
        {
            button->clicked = !button->clicked;
            clicked_any_button = true;
            if (button->callback)
            {
                button->callback();
            }
        }
        else
        {
            button->clicked = false;
        }
    }

    return clicked_any_button;
}

void GooeyButton_setText(GooeyButton *button, const char *text)
{
    if (!button)
    {
        fprintf(stderr, "Widget<Button> Cannot be null \n");
        return;
    }

    strcpy(button->label, text);
}

GooeyMenu *GooeyMenu_Set(GooeyWindow *win)
{
    GooeyMenu *menu = malloc(sizeof(GooeyMenu));
    if (!menu)
    {
        perror("Failed to allocate memory for GooeyMenu");
        return NULL;
    }

    *menu = (GooeyMenu){.children_count = 0};
    win->widgets.menu = menu;

    return win->widgets.menu;
}
void GooeyMenu_Draw(GooeyWindow *win)
{
    if (win->widgets.menu)
    {

        GooeyBackend_FillRectangle(0, 0, GooeyBackend_getWidth(win), 20, COLOR_GRAY);

        int x_offset = 10;
        for (int i = 0; i < win->widgets.menu->children_count; i++)
        {
            GooeyMenuChild *child = &win->widgets.menu->children[i];

            int text_width = GooeyBackend_GetTextWidth(child->title, strlen(child->title));
            GooeyBackend_DrawText(x_offset, 15,
                                  child->title, COLOR_BLACK);

            if (child->is_open && child->menu_elements_count > 0)
            {
                int submenu_x = x_offset - 10;
                int submenu_y = 20;
                int submenu_width = 150;
                int submenu_height = 25 * child->menu_elements_count;

                GooeyBackend_FillRectangle(submenu_x, submenu_y,
                                           submenu_width, submenu_height, COLOR_GRAY);

                for (int j = 0; j < child->menu_elements_count; j++)
                {
                    int element_y = submenu_y + (j * 25);
                    GooeyBackend_DrawText(submenu_x + 5,
                                          element_y + 18, child->menu_elements[j], COLOR_BLACK);
                    if (j < child->menu_elements_count - 1)
                    {

                        GooeyBackend_DrawLine(submenu_x,
                                              element_y + 25 - 1, submenu_x + submenu_width,
                                              element_y + 25 - 1, COLOR_BLACK);
                    }
                }
            }

            x_offset += text_width + 20;
        }
    }
}

GooeyMenuChild *GooeyMenu_AddChild(GooeyWindow *win, char *title)
{
    if (!win->widgets.menu || win->widgets.menu->children_count >= MAX_MENU_CHILDREN)
    {
        fprintf(stderr, "Unable to add child: Menu is full or not initialized.\n");
        return NULL;
    }

    GooeyMenuChild *child = &win->widgets.menu->children[win->widgets.menu->children_count++];
    child->title = strdup(title);
    child->menu_elements_count = 0;
    child->is_open = false;
    return child;
}

void GooeyMenuChild_AddElement(GooeyMenuChild *child, char *title,
                               void (*callback)())
{
    child->menu_elements[child->menu_elements_count] = title;
    child->callbacks[child->menu_elements_count] = callback;
    child->menu_elements_count++;
}

void GooeyWindow_Redraw(GooeyWindow *win)
{
    GooeyBackend_Clear(win);

    for (int i = 0; i < win->widgets.button_count; ++i)
    {
        GooeyButton_Draw(win, &win->widgets.buttons[i]);
    }
    for (int i = 0; i < win->widgets.textboxes_count; ++i)
    {
        GooeyTextbox_Draw(win, i);
    }

    GooeyMenu_Draw(win);

    GooeyBackend_Render();
    usleep(16667);
}

void GooeyMenu_HandleClick(GooeyWindow *win, int x, int y)
{

    if (!win->widgets.menu)
        return;

    int x_offset = 10;
    for (int i = 0; i < win->widgets.menu->children_count; i++)
    {
        GooeyMenuChild *child = &win->widgets.menu->children[i];
        int text_width = GooeyBackend_GetTextWidth(child->title, strlen(child->title));

        if (y <= 20 && x >= x_offset && x <= x_offset + text_width)
        {

            for (int k = 0; k < win->widgets.menu->children_count; k++)
                win->widgets.menu->children[k].is_open = 0;

            child->is_open = !child->is_open;
            GooeyWindow_Redraw(win);
            return;
        }

        if (child->is_open)
        {
            int submenu_x = x_offset - 10;
            int submenu_y = 20;
            int submenu_width = 150;

            for (int j = 0; j < child->menu_elements_count; j++)
            {
                int element_y = submenu_y + (j * 25);
                if (x >= submenu_x && x <= submenu_x + submenu_width &&
                    y >= element_y && y <= element_y + 25)
                {

                    if (child->callbacks[j])
                        child->callbacks[j]();

                    for (int k = 0; k < win->widgets.menu->children_count; k++)
                        win->widgets.menu->children[k].is_open = 0;

                    GooeyWindow_Redraw(win);
                    return;
                }
            }
        }

        x_offset += text_width + 20;
    }
}

GooeyTextbox *GooeyTextBox_Add(GooeyWindow *win, int x, int y, int width,
                               int height, void (*onTextChanged)(char *text))
{

    GooeyTextbox *textbox = &win->widgets.textboxes[win->widgets.textboxes_count++];
    textbox->core.type = WIDGET_TEXTBOX;
    textbox->core.x = x;
    textbox->core.y = y;
    textbox->core.width = width;
    textbox->core.height = height;
    textbox->focused = false;
    textbox->callback = onTextChanged;
    textbox->scroll_offset = 0;
    textbox->text[0] = '\0';

    return textbox;
}

const char *GooeyTextbox_getText(GooeyTextbox *textbox)
{
    if (!textbox)
    {
        fprintf(stderr, "Widget<Textbox> cannot be null.");
        return NULL;
    }

    return (const char *)textbox->text;
}

void GooeyTextbox_setText(GooeyTextbox *textbox, const char *text)
{
    if (!textbox)
    {
        fprintf(stderr, "Widget<Textbox> cannot be null.");
        return;
    }
    strcpy(textbox->text, text);
}

void GooeyTextbox_Draw(GooeyWindow *win, int index)
{
    GooeyTextbox *textbox = &win->widgets.textboxes[index];

    GooeyBackend_FillRectangle(textbox->core.x, textbox->core.y,
                               textbox->core.width, textbox->core.height, COLOR_WHITE);

    GooeyBackend_DrawRectangle(textbox->core.x, textbox->core.y,
                               textbox->core.width, textbox->core.height, textbox->focused ? COLOR_BLUE : COLOR_BLACK);

    GooeyBackend_SetForeground(COLOR_BLACK);
    int text_x = textbox->core.x + 5;
    int text_y = textbox->core.y + (textbox->core.height / 2) + 5;

    int max_text_width = textbox->core.width - 10;
    size_t len = strlen(textbox->text);
    size_t start_index = textbox->scroll_offset;

    while (start_index < len && GooeyBackend_GetTextWidth(textbox->text + start_index, len - start_index) > max_text_width)
    {
        start_index++;
    }

    char display_text[256];
    strncpy(display_text, textbox->text + start_index, sizeof(display_text) - 1);
    display_text[sizeof(display_text) - 1] = '\0';

    GooeyBackend_DrawText(text_x, text_y, display_text,
                          COLOR_BLACK);

    if (textbox->focused)
    {
        int cursor_x =
            text_x + GooeyBackend_GetTextWidth(display_text, strlen(display_text));
        GooeyBackend_DrawLine(cursor_x, textbox->core.y + 5,
                              cursor_x, textbox->core.y + textbox->core.height - 5, COLOR_BLACK);
    }
}
void GooeyTextbox_HandleKeyPress(GooeyWindow *win, XKeyEvent *key_event)
{
    char buf[32];
    KeySym key;
    XLookupString(key_event, buf, sizeof(buf), &key, NULL);

    for (int i = 0; i < win->widgets.textboxes_count; i++)
    {
        GooeyTextbox *textbox = &win->widgets.textboxes[i];
        if (!textbox->focused)
            continue;

        size_t len = strlen(textbox->text);

        if (key == XK_BackSpace)
        {

            if (len > 0)
            {
                textbox->text[len - 1] = '\0';
                if (textbox->scroll_offset > 0)
                {

                    textbox->scroll_offset--;
                }

                if (textbox->callback)
                    textbox->callback(textbox->text);
            }
        }
        else if (key == XK_Return || key == XK_Linefeed)
        {

            textbox->focused = false;
        }
        else if (isprint(buf[0]) && len < sizeof(textbox->text) - 1)
        {

            strncat(textbox->text, buf, 1);

            if (textbox->callback)
                textbox->callback(textbox->text);

            int text_width = GooeyBackend_GetTextWidth(textbox->text, len + 1);
            int max_text_width = textbox->core.width - 10;

            if (text_width > max_text_width)
            {
                textbox->scroll_offset++;
            }
        }
    }
}

bool GooeyTextbox_HandleClick(GooeyWindow *win, int x, int y)
{
  for (int i = 0; i < win->widgets.textboxes_count; i++)
  {
    GooeyTextbox *textbox = &win->widgets.textboxes[i];
    if (x >= textbox->core.x && x <= textbox->core.x + textbox->core.width &&
        y >= textbox->core.y && y <= textbox->core.y + textbox->core.height)
    {
      textbox->focused = true;
      return true;
    }
    else
    {
      textbox->focused = false;
    }
  }
  return false;
}

void GooeyWindow_Cleanup()
{

    GooeyBackend_DestroyWindow();
    GooeyBackend_Cleanup();
}
void GooeyWindow_Run(GooeyWindow *win)
{

    GooeyEvent event;
    bool running = true;

    while (running)
    {
        event = GooeyBackend_HandleEvents(win);

        switch (event.type)
        {
        case GOOEY_EVENT_EXPOSE:
            GooeyWindow_Redraw(win);
            break;

        case GOOEY_EVENT_CLICK:
        {
            int x = event.data.click.x;
            int y = event.data.click.y;
            GooeyMenu_HandleClick(win, x, y);
            if (GooeyButton_HandleClick(win, x, y))
            {
                GooeyWindow_Redraw(win);
            } else if(GooeyTextbox_HandleClick(win, x, y)) {
                GooeyWindow_Redraw(win);
            }
            break;
        }

        case GOOEY_EVENT_WINDOW_CLOSE:
            running = false;
            break;

        default:
            break;
        }
    }

    GooeyWindow_Cleanup(win);
}

#endif