#ifndef GOOEY_WIDGETS_H
#define GOOEY_WIDGETS_H

#define MAX_WIDGETS 100
#define CHECKBOX_SIZE 20
#define RADIO_BUTTON_RADIUS 10
#define SLIDER_WIDTH 100
#define SLIDER_HEIGHT 5
#define MAX_MENU_CHILDREN 10
#define MAX_RADIO_BUTTONS 10



typedef enum
{
  WIDGET_LABEL,
  WIDGET_SLIDER,
  WIDGET_RADIOBUTTON,
  WIDGET_CHECKBOX,
  WIDGET_BUTTON,
  WIDGET_TEXTBOX,
  WIDGET_DROPDOWN
} WIDGET_TYPE;

typedef struct
{
  WIDGET_TYPE type;
  int x, y, width, height;
} GooeyWidget;

typedef struct
{
  GooeyWidget core;
  char label[256];
  void (*callback)();
  bool clicked;
  bool hover;
} GooeyButton;

typedef struct
{
  GooeyWidget core;
  char text[256];
  char placeholder[256];
  bool focused;
  int cursor_pos;
  int scroll_offset;
  void (*callback)(char *text);
} GooeyTextbox;

typedef struct
{
  GooeyWidget core;
  char text[256];
} GooeyLabel;

typedef struct
{
  GooeyWidget core;
  bool checked;
  char label[256];
  void (*callback)(bool checked);
} GooeyCheckbox;

typedef struct
{
  GooeyWidget core;
  bool selected;
  char label[256];
  int radius;
  void (*callback)(bool selected);
} GooeyRadioButton;

typedef struct
{
  GooeyRadioButton buttons[MAX_RADIO_BUTTONS];
  int button_count;
} GooeyRadioButtonGroup;

typedef struct
{
  GooeyWidget core;
  long value;
  long min_value;
  long max_value;
  bool show_hints;
  void (*callback)(long value);
} GooeySlider;

typedef struct
{
  GooeyWidget core;
  int selected_index;
  const char **options;
  int num_options;
  void (*callback)(int selected_index);
} GooeyDropdown;

typedef struct
{
  char *title;
  char *menu_elements[MAX_MENU_CHILDREN];
  void (*callbacks[MAX_MENU_CHILDREN])();
  int menu_elements_count;
  bool is_open;
} GooeyMenuChild;

typedef struct
{
  GooeyMenuChild children[MAX_MENU_CHILDREN];
  int children_count;
} GooeyMenu;

typedef enum
{
  LAYOUT_HORIZONTAL,
  LAYOUT_VERTICAL,
  LAYOUT_GRID,
} GooeyLayoutType;

typedef struct
{
  GooeyLayoutType layout_type;
  int x, y, width, height;
  int padding;
  int margin;
  int rows;
  int cols;
  void *widgets[MAX_WIDGETS];
  int widget_count;
} GooeyLayout;

typedef struct
{

  GooeyButton buttons[MAX_WIDGETS];
  GooeyLabel labels[MAX_WIDGETS];
  GooeyCheckbox checkboxes[MAX_WIDGETS];
  GooeyRadioButton radio_buttons[MAX_WIDGETS];
  GooeySlider sliders[MAX_WIDGETS];
  GooeyDropdown dropdowns[MAX_WIDGETS];
  GooeyRadioButtonGroup radio_button_groups[MAX_WIDGETS];
  GooeyTextbox textboxes[MAX_WIDGETS];
  GooeyLayout layouts[MAX_WIDGETS];
  GooeyMenu *menu;


  int button_count;
  int label_count;
  int checkbox_count;
  int radio_button_count;
  int slider_count;
  int dropdown_count;
  int textboxes_count;
  int layout_count;
  int radio_button_group_count;

} GooeyWindowExt;

#endif