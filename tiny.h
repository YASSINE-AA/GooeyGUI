#ifndef TINY_H
#define TINY_H

#include "backends/gooey_backend.h"

extern GooeyBackend *active_backend;
extern GooeyBackend x11_backend;
extern GooeyBackend wayland_backend;
extern GooeyBackend glfw_backend;

int Gooey_Init(GooeyBackends backend);

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
                               int height, char* placeholder, void (*onTextChanged)(char *text));

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
void GooeyTextbox_setText(GooeyTextbox *textbox, const char *text);

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
GooeyLabel *GooeyLabel_Add(GooeyWindow *win, const char *text, int x, int y);

/**
 * @brief Sets the text of the label.
 *
 * @param label The label to set the text for.
 * @param text The new text to display on the label.
 */
void GooeyLabel_setText(GooeyLabel *label, const char *text);

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

GooeyRadioButton *GooeyRadioButton_Add(GooeyWindow *win, int x, int y,
                                       char *label,
                                       void (*callback)(bool selected));

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
 * @param x The x-coordinate of the drag event.
 * @param y The y-coordinate of the drag event.
 * @return True if the slider was dragged, false otherwise.
 */
bool GooeySlider_HandleDrag(GooeyWindow *win, int x, int y);

/**
 * @brief Gets the current value of the slider.
 *
 * @param slider The slider to get the value from.
 * @return The current value of the slider.
 */
long GooeySlider_getValue(GooeySlider *slider);

/**
 * @brief Sets the value of the slider.
 *
 * @param slider The slider to set the value for.
 * @param value The new value for the slider.
 */
void GooeySlider_setValue(GooeySlider *slider, long value);

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
 * @brief Creates a radio button group widget.
 *
 * @param win The window containing the radio button group.
 * @return GooeyRadioButtonGroup widget pointer.
 */
GooeyRadioButtonGroup *GooeyRadioButtonGroup_Create(GooeyWindow *win);
/**
 * @brief Creates a radio button group widget.
 *
 * @param win The window containing the radio button group.
 * @param x The x-coordinate of the radio button.
 * @param y The y-coordinate of the radio button.
 * @param label The text (label) of the radio button.
 * @param callback The callback for selecting that specific radio button, pass NULL if not needed.
 * @return GooeyRadioButtonGroup widget pointer.
 */
GooeyRadioButton *GooeyRadioButtonGroup_AddChild(GooeyRadioButtonGroup *group, int x, int y, const char *label, void (*callback)(bool));
void GooeyRadioButtonGroup_Draw(GooeyWindow *win);

/**
 * @brief Handles RadioButtonGroup click events.
 *
 * @param win The window containing the dropdown menu.
 * @param x The x-coordinate of the click event.
 * @param y The y-coordinate of the click event.
 * @return True if the dropdown menu was clicked, false otherwise.
 */
bool GooeyRadioButtonGroup_HandleClick(GooeyWindow *win, int x, int y);

void GooeyWindow_Cleanup(GooeyWindow *win);
#endif