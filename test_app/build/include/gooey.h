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

#ifndef TINY_H
#define TINY_H
#include "../gooey_backend_internal.h"

/**
 * @brief The currently active Gooey backend.
 */
extern GooeyBackend *active_backend;

/**
 * @brief The X11 backend for Gooey.
 */
extern GooeyBackend x11_backend;

/**
 * @brief The GLFW backend for Gooey.
 */
extern GooeyBackend glfw_backend;

/**
 * @brief Initializes the Gooey system with the specified backend.
 *
 * @param backend The backend to use (e.g., X11, Wayland, GLFW).
 * @return 0 on success, non-zero on failure.
 */
int Gooey_Init(GooeyBackends backend);

/**
 * @brief Sets the theme for the Gooey window.
 *
 * @param fontPath The path to the font file to use for the window's theme.
 */
void GooeyWindow_SetTheme(const char *fontPath);

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
GooeyWindow GooeyWindow_Create(const char *title, int width, int height, bool visibility);

GooeyWindow GooeyWindow_CreateChild(const char *title, int width, int height, bool visibility);
void GooeyWindow_MakeVisible(GooeyWindow *win, bool visibility);

/**
 * @brief Runs the Gooey window.
 *
 * This function enters the main event loop and begins processing user input
 * and window events for the provided Gooey window.
 *
 * @param win The window to run.
 */
void GooeyWindow_Run(int num_windows, GooeyWindow *first_win, ...);

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
void GooeyButton_SetText(GooeyButton *button, const char *text);

/**
 * @brief Creates a layout for the specified window.
 *
 * Initializes a new layout with a specified layout type, position, and size
 * within the window.
 *
 * @param win The window to attach the layout to.
 * @param layout_type The type of layout (e.g., grid, box).
 * @param x The x-coordinate of the layout's position.
 * @param y The y-coordinate of the layout's position.
 * @param width The width of the layout.
 * @param height The height of the layout.
 * @return A new GooeyLayout object.
 */
GooeyLayout *GooeyLayout_Create(GooeyWindow *win, GooeyLayoutType layout_type,
                                int x, int y, int width, int height);

/**
 * @brief Adds a child widget to a layout.
 *
 * @param layout The layout to which the widget will be added.
 * @param widget The widget to add to the layout.
 */
void GooeyLayout_AddChild(GooeyLayout *layout, void *widget);

/**
 * @brief Builds the layout, arranging all child widgets according to the layout type.
 *
 * @param layout The layout to build.
 */
void GooeyLayout_Build(GooeyLayout *layout);

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

/**
 * @brief Draws the button on the window.
 *
 * @param win The window to draw the button on.
 * @param button The button to draw.
 */
void GooeyButton_Draw(GooeyWindow *win, GooeyButton *button);

/**
 * @brief Adds a textbox to the window.
 *
 * @param win The window to add the textbox to.
 * @param x The x-coordinate of the textbox's position.
 * @param y The y-coordinate of the textbox's position.
 * @param width The width of the textbox.
 * @param height The height of the textbox.
 * @param placeholder The placeholder text for the textbox.
 * @param onTextChanged The callback function to call when the text changes.
 * @return A new GooeyTextbox object.
 */
GooeyTextbox *GooeyTextBox_Add(GooeyWindow *win, int x, int y, int width,
                               int height, char *placeholder, void (*onTextChanged)(char *text));

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
void GooeyTextbox_HandleKeyPress(GooeyWindow *win, GooeyEvent *event);

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
void GooeyTextbox_SetText(GooeyTextbox *textbox, const char *text);

/**
 * @brief Adds a menu item to a menu child.
 *
 * @param child The menu child to add the item to.
 * @param title The title of the menu item.
 * @param callback The callback function to call when the item is selected.
 */
void GooeyMenuChild_AddElement(GooeyMenuChild *child, char *title, void (*callback)());

/**
 * @brief Adds a label to the window.
 *
 * @param win The window to add the label to.
 * @param text The text to display on the label.
 * @param x The x-coordinate of the label's position.
 * @param y The y-coordinate of the label's position.
 * @return A new GooeyLabel object.
 */
GooeyLabel *GooeyLabel_Add(GooeyWindow *win, const char *text, float font_size, int x, int y);

/**
 * @brief Sets the text of the label.
 *
 * @param label The label to set the text for.
 * @param text The new text to display on the label.
 */
void GooeyLabel_SetText(GooeyLabel *label, const char *text);

/**
 * @brief Draws the label on the window.
 *
 * @param win The window to draw the label on.
 */
void GooeyLabel_Draw(GooeyWindow *win);

/**
 * @brief Adds a checkbox to the window.
 *
 * @param win The window to add the checkbox to.
 * @param x The x-coordinate of the checkbox's position.
 * @param y The y-coordinate of the checkbox's position.
 * @param label The label for the checkbox.
 * @param callback The callback function to call when the checkbox is clicked.
 * @return A new GooeyCheckbox object.
 */
GooeyCheckbox *GooeyCheckbox_Add(GooeyWindow *win, int x, int y, char *label,
                                 void (*callback)(bool checked));

/**
 * @brief Handles checkbox click events.
 *
 * @param win The window containing the checkbox.
 * @param x The x-coordinate of the click event.
 * @param y The y-coordinate of the click event.
 * @return True if the checkbox was clicked, false otherwise.
 */
bool GooeyCheckbox_HandleClick(GooeyWindow *win, int x, int y);

/**
 * @brief Adds a radio button to the window.
 *
 * @param win The window to add the radio button to.
 * @param x The x-coordinate of the radio button's position.
 * @param y The y-coordinate of the radio button's position.
 * @param label The label of the radio button.
 * @param callback The callback function to call when the radio button is selected.
 * @return A new GooeyRadioButton object.
 */
GooeyRadioButton *GooeyRadioButton_Add(GooeyWindow *win, int x, int y,
                                       char *label,
                                       void (*callback)(bool selected));

/**
 * @brief Handles radio button click events.
 *
 * @param win The window containing the radio button.
 * @param x The x-coordinate of the click event.
 * @param y The y-coordinate of the click event.
 * @return True if the radio button was clicked, false otherwise.
 */
bool GooeyRadioButton_HandleClick(GooeyWindow *win, int x, int y);

/**
 * @brief Adds a slider to the window.
 *
 * @param win The window to add the slider to.
 * @param x The x-coordinate of the slider's position.
 * @param y The y-coordinate of the slider's position.
 * @param width The width of the slider.
 * @param min_value The minimum value of the slider.
 * @param max_value The maximum value of the slider.
 * @param show_hints Whether to show hints for the slider.
 * @param callback The callback function to call when the slider value changes.
 * @return A new GooeySlider object.
 */
GooeySlider *GooeySlider_Add(GooeyWindow *win, int x, int y, int width,
                             long min_value, long max_value, bool show_hints,
                             void (*callback)(long value));

/**
 * @brief Handles slider drag events.
 *
 * @param win The window containing the slider.
 * @param event The current event.
 * @return True if the slider was dragged, false otherwise.
 */
bool GooeySlider_HandleDrag(GooeyWindow *win, int x, int y, GooeyEventType type);
/**
 * @brief Gets the current value of the slider.
 *
 * @param slider The slider to get the value from.
 * @return The current value of the slider.
 */
long GooeySlider_GetValue(GooeySlider *slider);

/**
 * @brief Sets the value of the slider.
 *
 * @param slider The slider to set the value for.
 * @param value The new value for the slider.
 */
void GooeySlider_SetValue(GooeySlider *slider, long value);

/**
 * @brief Adds a dropdown menu to the window.
 *
 * @param win The window to add the dropdown menu to.
 * @param x The x-coordinate of the dropdown's position.
 * @param y The y-coordinate of the dropdown's position.
 * @param width The width of the dropdown menu.
 * @param height The height of the dropdown menu.
 * @param options The list of options for the dropdown menu.
 * @param num_options The number of options.
 * @param callback The callback function to call when an option is selected.
 * @return A new GooeyDropdown object.
 */
GooeyDropdown *GooeyDropdown_Add(GooeyWindow *win, int x, int y, int width,
                                 int height, const char **options,
                                 int num_options,
                                 void (*callback)(int selected_index));

/**
 * @brief Handles dropdown menu click events.
 *
 * @param win The window containing the dropdown menu.
 * @param x The x-coordinate of the click event.
 * @param y The y-coordinate of the click event.
 * @return True if the dropdown menu was clicked, false otherwise.
 */
bool GooeyDropdown_HandleClick(GooeyWindow *win, int x, int y);
/**
 * @brief Draws the drop down menu on the window.
 *
 * @param win The window to draw the drop down menu.
 */
void GooeyDropdown_Draw(GooeyWindow *win);

/**
 * @brief Creates a radio button group widget.
 *
 * @param win The window containing the radio button group.
 * @return GooeyRadioButtonGroup widget pointer.
 */
GooeyRadioButtonGroup *GooeyRadioButtonGroup_Create(GooeyWindow *win);

/**
 * @brief Adds a radio button to a radio button group.
 *
 * @param group The radio button group to add the button to.
 * @param x The x-coordinate of the radio button.
 * @param y The y-coordinate of the radio button.
 * @param label The label for the radio button.
 * @param callback The callback to execute when the radio button is selected.
 * @return The GooeyRadioButton object.
 */
GooeyRadioButton *GooeyRadioButtonGroup_AddChild(GooeyWindow *win, GooeyRadioButtonGroup *group, int x, int y, const char *label, void (*callback)(bool));

/**
 * @brief Draws the radio button group on the window.
 *
 * @param win The window to draw the radio button group on.
 */
void GooeyRadioButtonGroup_Draw(GooeyWindow *win);

/**
 * @brief Handles radio button group click events.
 *
 * @param win The window containing the radio button group.
 * @param x The x-coordinate of the click event.
 * @param y The y-coordinate of the click event.
 * @return True if the radio button group was clicked, false otherwise.
 */
bool GooeyRadioButtonGroup_HandleClick(GooeyWindow *win, int x, int y);

/**
 * @brief Adds a canvas to the window.
 *
 * @param win The window to add the canvas to.
 * @param x The x-coordinate of the canvas' position.
 * @param y The y-coordinate of the canvas' position.
 * @param width The width of the canvas.
 * @param height The height of the canvas.
 * @return A new GooeyCanvas object.
 */
GooeyCanvas *GooeyCanvas_Add(GooeyWindow *win, int x, int y, int width,
                             int height);

/**
 * @brief Draws a rectangle onto the user-defined canvas.
 *
 * @param canvas The user-defined canvas.
 * @param x The x-coordinate of the rectangle.
 * @param y The y-coordinate of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color_hex The color of the rectangle in hexadecimal.
 * @param is_filled If true, the rectangle is filled with a solid color. If false it is hollow.
 */
void GooeyCanvas_DrawRectangle(GooeyCanvas *canvas, int x, int y, int width, int height, const char *color_hex, bool is_filled);

/**
 * @brief Draws a line onto the user-defined canvas.
 *
 * @param canvas The user-defined canvas.
 * @param x1 The x-coordinate of the origin point of the line.
 * @param y1 The y-coordinate of the origin point of the line.
 * @param x2 The x-coordinate of the last point of the line.
 * @param y2 The y-coordinate of the last point of the line.
 * @param color_hex The color of the line in hexadecimal.
 */
void GooeyCanvas_DrawLine(GooeyCanvas *canvas, int x1, int y1, int x2, int y2, const char *color_hex);

/**
 * @brief Draws an arc onto the user-defined canvas.
 *
 * @param canvas The user-defined canvas.
 * @param x_center The x-coordinate of the arc.
 * @param y_center The y-coordinate of the arc.
 * @param width The width of the arc.
 * @param height The height of the arc.
 * @param angle1 starting angle of the arc.
 * @param angle2 end angle of the arc.
 */
void GooeyCanvas_DrawArc(GooeyCanvas *canvas, int x_center, int y_center, int width, int height, int angle1, int angle2);

/**
 * @brief Sets foreground of the user-defined canvas.
 *
 * @param canvas The user-defined canvas.
 * @param color_hex The foreground color in hexadecimal.
 */
void GooeyCanvas_SetForeground(GooeyCanvas *canvas, const char *color_hex);

/**
 * @brief Cleans up the resources associated with the Gooey windows.
 * @param num_windows The number of windows to clean up.
 * @param first_window The first window to clean up.
 */
void GooeyWindow_Cleanup(int num_windows, GooeyWindow *first_win, ...);
/**
 * @brief Creates a message box window.
 *
 * This function creates a modal message box with a specified title, message,
 * type, and a callback function to handle user interaction.
 *
 * @param title The title of the message box.
 * @param message The message to display in the message box.
 * @param type The type of the message box (e.g., INFO, WARNING, ERROR, QUESTION).
 * @param callback A function pointer to handle user input (e.g., button click).
 *                 The callback receives an integer representing the selected option.
 * @return GooeyWindow The created message box window.
 */
GooeyWindow GooeyMessageBox_Create(const char *title, const char *message, MSGBOX_TYPE type, void (*callback)(int option));

/**
 * @brief Displays the specified message box window.
 *
 * This function makes the message box visible on the screen. The message box must
 * have been created using `GooeyMessageBox_Create`.
 *
 * @param msgBoxWindow A pointer to the message box window to display.
 */
void GooeyMessageBox_Show(GooeyWindow *msgBoxWindow);

/**
 * @brief Sets the resizable property of a window.
 *
 * Allows the user to resize a window dynamically, depending on the value of `is_resizable`.
 *
 * @param msgBoxWindow A pointer to the window whose resizable property is to be set.
 * @param is_resizable `true` to make the window resizable; `false` to make it fixed-size.
 */
void GooeyWindow_MakeResizable(GooeyWindow *msgBoxWindow, bool is_resizable);

/**
 * @brief Highlights or unhighlights a button.
 *
 * This function visually indicates whether a button is highlighted.
 *
 * @param button A pointer to the button to modify.
 * @param is_highlighted `true` to highlight the button; `false` to remove the highlight.
 */
void GooeyButton_SetHighlight(GooeyButton *button, bool is_highlighted);

/**
 * @brief Sets the color of a label.
 *
 * Changes the text color of a label to the specified color string.
 *
 * @param label A pointer to the label whose color is to be set.
 * @param color A string representing the color (e.g., "red", "#FF0000").
 * @return A pointer to the modified label.
 */
void *GooeyLabel_SetColor(GooeyLabel *label, const char *color);

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

/**
 * @brief Creates a new signal object.
 *
 * Signals are used to manage event handling and callbacks. This function initializes
 * a new signal object.
 *
 * @return GooeySignal The created signal object.
 */
GooeySignal GooeySignal_Create(void);

/**
 * @brief Links a callback function to a signal.
 *
 * This function connects a signal to a specific callback function. When the signal
 * is emitted, the callback function will be executed.
 *
 * @param signal A pointer to the signal to which the callback is linked.
 * @param callback The callback function to execute when the signal is emitted.
 * @param context A user-defined context pointer passed to the callback.
 */
void GooeySignal_Link(GooeySignal *signal, GooeySignal_CallbackFunction callback, void *context);

/**
 * @brief Emits a signal.
 *
 * This function triggers the signal, invoking all linked callbacks in the order
 * they were added.
 *
 * @param signal A pointer to the signal to emit.
 * @param data A void pointer to the data.
 */
void GooeySignal_Emit(GooeySignal *signal, void *data);

/**
 * @brief Unlinks all callbacks from a signal.
 *
 * Removes all callback functions linked to the specified signal, effectively
 * clearing its event listeners.
 *
 * @param signal A pointer to the signal to modify.
 */
void GooeySignal_UnLinkAll(GooeySignal *signal);

void GooeyWindow_Redraw(GooeyWindow *win);

#endif
