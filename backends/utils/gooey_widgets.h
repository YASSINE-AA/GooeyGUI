#ifndef GOOEY_WIDGETS_H
#define GOOEY_WIDGETS_H

/** Maximum number of widgets that can be added to a window. */
#define MAX_WIDGETS 100

/** Size of a checkbox widget. */
#define CHECKBOX_SIZE 20

/** Radius of the radio button widget. */
#define RADIO_BUTTON_RADIUS 10

/** Width of the slider widget. */
#define SLIDER_WIDTH 100

/** Height of the slider widget. */
#define SLIDER_HEIGHT 5

/** Maximum number of menu children. */
#define MAX_MENU_CHILDREN 10

/** Maximum number of radio buttons in a group. */
#define MAX_RADIO_BUTTONS 10

/**
 * @brief Enumeration for widget types in the Gooey framework.
 */
typedef enum
{
  WIDGET_LABEL,       /**< Label widget */
  WIDGET_SLIDER,      /**< Slider widget */
  WIDGET_RADIOBUTTON, /**< Radio button widget */
  WIDGET_CHECKBOX,    /**< Checkbox widget */
  WIDGET_BUTTON,      /**< Button widget */
  WIDGET_TEXTBOX,     /**< Textbox widget */
  WIDGET_DROPDOWN     /**< Dropdown widget */
} WIDGET_TYPE;

/**
 * @brief A base structure for all Gooey widgets containing their position and size.
 */
typedef struct
{
  WIDGET_TYPE type;  /**< Type of the widget */
  int x, y;          /**< Position of the widget (top-left corner) */
  int width, height; /**< Dimensions of the widget */
} GooeyWidget;

/**
 * @brief A structure representing a button widget.
 */
typedef struct
{
  GooeyWidget core;   /**< Core widget properties */
  char label[256];    /**< Text label of the button */
  void (*callback)(); /**< Callback function for button click */
  bool clicked;       /**< State of the button (clicked or not) */
  bool hover;         /**< State of the button (hovered or not) */
} GooeyButton;

/**
 * @brief A structure representing a textbox widget.
 */
typedef struct
{
  GooeyWidget core;             /**< Core widget properties */
  char text[256];               /**< Text in the textbox */
  char placeholder[256];        /**< Placeholder text for the textbox */
  bool focused;                 /**< Whether the textbox is focused */
  int cursor_pos;               /**< Position of the cursor in the text */
  int scroll_offset;            /**< Scroll offset of the textbox */
  void (*callback)(char *text); /**< Callback when text changes */
} GooeyTextbox;

/**
 * @brief A structure representing a label widget.
 */
typedef struct
{
  GooeyWidget core; /**< Core widget properties */
  char text[256];   /**< Text displayed on the label */
} GooeyLabel;

/**
 * @brief A structure representing a checkbox widget.
 */
typedef struct
{
  GooeyWidget core;               /**< Core widget properties */
  bool checked;                   /**< Whether the checkbox is checked or not */
  char label[256];                /**< Label for the checkbox */
  void (*callback)(bool checked); /**< Callback function when the checkbox is clicked */
} GooeyCheckbox;

/**
 * @brief A structure representing a radio button widget.
 */
typedef struct
{
  GooeyWidget core;                /**< Core widget properties */
  bool selected;                   /**< Whether the radio button is selected or not */
  char label[256];                 /**< Label for the radio button */
  int radius;                      /**< Radius of the radio button */
  void (*callback)(bool selected); /**< Callback function when the radio button is selected */
} GooeyRadioButton;

/**
 * @brief A structure representing a group of radio buttons.
 */
typedef struct
{
  GooeyRadioButton buttons[MAX_RADIO_BUTTONS]; /**< List of radio buttons in the group */
  int button_count;                            /**< Number of radio buttons in the group */
} GooeyRadioButtonGroup;

/**
 * @brief A structure representing a slider widget.
 */
typedef struct
{
  GooeyWidget core;             /**< Core widget properties */
  long value;                   /**< Current value of the slider */
  long min_value;               /**< Minimum value of the slider */
  long max_value;               /**< Maximum value of the slider */
  bool show_hints;              /**< Whether to show hints for the slider */
  void (*callback)(long value); /**< Callback function when the slider value changes */
} GooeySlider;

/**
 * @brief A structure representing a dropdown menu widget.
 */
typedef struct
{
  GooeyWidget core;                     /**< Core widget properties */
  int selected_index;                   /**< Index of the selected item */
  const char **options;                 /**< List of options for the dropdown */
  int num_options;                      /**< Number of options in the dropdown */
  void (*callback)(int selected_index); /**< Callback function when an option is selected */
} GooeyDropdown;

/**
 * @brief A structure representing a menu item in the window.
 */
typedef struct
{
  char *title;                            /**< Title of the menu item */
  char *menu_elements[MAX_MENU_CHILDREN]; /**< List of child menu elements */
  void (*callbacks[MAX_MENU_CHILDREN])(); /**< List of callback functions for child menu elements */
  int menu_elements_count;                /**< Number of child menu elements */
  bool is_open;                           /**< Whether the menu is open or not */
} GooeyMenuChild;

/**
 * @brief A structure representing a menu widget.
 */
typedef struct
{
  GooeyMenuChild children[MAX_MENU_CHILDREN]; /**< List of menu items */
  int children_count;                         /**< Number of menu items */
  bool is_busy;
} GooeyMenu;

/**
 * @brief Enumeration for layout types of widgets in the Gooey framework.
 */
typedef enum
{
  LAYOUT_HORIZONTAL, /**< Horizontal layout */
  LAYOUT_VERTICAL,   /**< Vertical layout */
  LAYOUT_GRID        /**< Grid layout */
} GooeyLayoutType;

/**
 * @brief A structure representing the layout of widgets in a window.
 */
typedef struct
{
  GooeyLayoutType layout_type; /**< Type of the layout (horizontal, vertical, or grid) */
  int x, y;                    /**< Position of the layout */
  int width, height;           /**< Dimensions of the layout */
  int padding;                 /**< Padding around the layout */
  int margin;                  /**< Margin around the layout */
  int rows;                    /**< Number of rows in the layout (for grid layouts) */
  int cols;                    /**< Number of columns in the layout (for grid layouts) */
  void *widgets[MAX_WIDGETS];  /**< List of widgets in the layout */
  int widget_count;            /**< Number of widgets in the layout */
} GooeyLayout;

/**
 * @brief A structure representing a window containing various widgets.
 */
typedef struct
{
  GooeyButton buttons[MAX_WIDGETS];                       /**< List of buttons in the window */
  GooeyLabel labels[MAX_WIDGETS];                         /**< List of labels in the window */
  GooeyCheckbox checkboxes[MAX_WIDGETS];                  /**< List of checkboxes in the window */
  GooeyRadioButton radio_buttons[MAX_WIDGETS];            /**< List of radio buttons in the window */
  GooeySlider sliders[MAX_WIDGETS];                       /**< List of sliders in the window */
  GooeyDropdown dropdowns[MAX_WIDGETS];                   /**< List of dropdown menus in the window */
  GooeyRadioButtonGroup radio_button_groups[MAX_WIDGETS]; /**< List of radio button groups in the window */
  GooeyTextbox textboxes[MAX_WIDGETS];                    /**< List of textboxes in the window */
  GooeyLayout layouts[MAX_WIDGETS];                       /**< List of layouts in the window */
  GooeyMenu *menu;                                        /**< Menu in the window */

  int button_count;             /**< Number of buttons in the window */
  int label_count;              /**< Number of labels in the window */
  int checkbox_count;           /**< Number of checkboxes in the window */
  int radio_button_count;       /**< Number of radio buttons in the window */
  int slider_count;             /**< Number of sliders in the window */
  int dropdown_count;           /**< Number of dropdown menus in the window */
  int textboxes_count;          /**< Number of textboxes in the window */
  int layout_count;             /**< Number of layouts in the window */
  int radio_button_group_count; /**< Number of radio button groups in the window */
} GooeyWindowExt;

#endif
