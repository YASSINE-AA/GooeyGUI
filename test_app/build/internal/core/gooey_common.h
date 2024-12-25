#ifndef GOOEY_COMMON_H
#define GOOEY_COMMON_H

#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>

#include "gooey_widgets_internal.h"
#include "utils/theme/gooey_theme_internal.h"
#include "gooey_event_internal.h"
#include "utils/logger/gooey_logger_internal.h"


typedef enum
{
    WINDOW_REGULAR,
    WINDOW_MSGBOX

} WINDOW_TYPE;

/**
 * @brief A structure representing a window containing various widgets.
 */
typedef struct
{

    int creation_id; /**< Unique window ID. */
    bool visibility;
    WINDOW_TYPE type;

    GooeyButton *buttons;                       /**< List of buttons in the window */
    GooeyLabel *labels;                         /**< List of labels in the window */
    GooeyCheckbox *checkboxes;                  /**< List of checkboxes in the window */
    GooeyRadioButton *radio_buttons;            /**< List of radio buttons in the window */
    GooeySlider *sliders;                       /**< List of sliders in the window */
    GooeyDropdown *dropdowns;                   /**< List of dropdown menus in the window */
    GooeyRadioButtonGroup *radio_button_groups; /**< List of radio button groups in the window */
    GooeyTextbox *textboxes;                    /**< List of textboxes in the window */
    GooeyLayout *layouts;                       /**< List of layouts in the window */
    GooeyMenu *menu;                            /**< Menu in the window */
    GooeyList *lists;                           /**< List of list widgets in the window. */
    GooeyCanvas *canvas;                        /**< List of canvas widgets in the window. */
    GooeyWidget **widgets;                      /**< List containing unified definition of every widget. */

    int list_count;
    int scrollable_count;         /**< Number of scrollables in the window */
    int button_count;             /**< Number of buttons in the window */
    int label_count;              /**< Number of labels in the window */
    int checkbox_count;           /**< Number of checkboxes in the window */
    int radio_button_count;       /**< Number of radio buttons in the window */
    int slider_count;             /**< Number of sliders in the window */
    int dropdown_count;           /**< Number of dropdown menus in the window */
    int textboxes_count;          /**< Number of textboxes in the window */
    int layout_count;             /**< Number of layouts in the window */
    int radio_button_group_count; /**< Number of radio button groups in the window */
    int canvas_count;             /**< Number of all canvas widgets in the window */
    int widget_count;             /**< Total number of registered widgets in the window. */

} GooeyWindow;

#include "widgets/gooey_button.h"
#include "widgets/gooey_canvas.h"
#include "widgets/gooey_checkbox.h"
#include "widgets/gooey_dropdown.h"
#include "widgets/gooey_label.h"
#include "widgets/gooey_layout.h"
#include "widgets/gooey_list.h"
#include "widgets/gooey_menu.h"
#include "widgets/gooey_messagebox.h"
#include "widgets/gooey_radiobutton.h"
#include "widgets/gooey_slider.h"
#include "widgets/gooey_textbox.h"
#include "signals/gooey_signals.h"

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
 * @brief Cleans up the resources associated with the Gooey windows.
 * @param num_windows The number of windows to clean up.
 * @param first_window The first window to clean up.
 */
void GooeyWindow_Cleanup(int num_windows, GooeyWindow *first_win, ...);
void GooeyWindow_RegisterWidget(GooeyWindow *win, GooeyWidget *widget);
/**
 * @brief Sets the resizable property of a window.
 *
 * Allows the user to resize a window dynamically, depending on the value of `is_resizable`.
 *
 * @param msgBoxWindow A pointer to the window whose resizable property is to be set.
 * @param is_resizable `true` to make the window resizable; `false` to make it fixed-size.
 */
void GooeyWindow_MakeResizable(GooeyWindow *msgBoxWindow, bool is_resizable);

void GooeyWindow_Redraw(GooeyWindow *win);

extern GooeyTheme *active_theme;

#endif