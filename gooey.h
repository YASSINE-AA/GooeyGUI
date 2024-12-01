#ifndef GOOEY_H
#define GOOEY_H

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WIDGETS 100
#define CHECKBOX_SIZE 20

#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000
#define COLOR_BLUE 0x0000FF
#define COLOR_GRAY 0xCCCCCC
#define COLOR_HOVER 0xAAAAAA



typedef struct
{
  int x, y, width, height;
  const char *label;
  void (*callback)();
  bool hover;
} GooeyButton;

typedef struct
{
  int x, y, width, height;
  char text[256];
  bool focused;
  int cursor_pos;
  int scroll_offset = 0;
  void (*callback)(char *text);
} GooeyTextbox;

typedef struct
{
  int x, y;
  const char *text;
} GooeyLabel;

typedef struct
{
  int x, y, width, height;
  bool checked;
  void (*callback)(bool checked);
} GooeyCheckbox;

typedef struct
{
  int x, y, radius;
  bool selected;
  void (*callback)(bool selected);
} GooeyRadioButton;

typedef struct
{
  int x, y, width, height;
  int value;
  int min_value;
  int max_value;
  void (*callback)(int value);
} GooeySlider;

typedef struct
{
  int x, y, width, height;
  int selected_index;
  const char **options;
  int num_options;
  void (*callback)(int selected_index);
} GooeyDropdown;

typedef struct
{
  Display *display;
  Window window;
  GC gc;
  Atom wm_delete_window;

  GooeyButton buttons[MAX_WIDGETS];
  GooeyLabel labels[MAX_WIDGETS];
  GooeyCheckbox checkboxes[MAX_WIDGETS];
  GooeyRadioButton radio_buttons[MAX_WIDGETS];
  GooeySlider sliders[MAX_WIDGETS];
  GooeyDropdown dropdowns[MAX_WIDGETS];
  GooeyTextbox textboxes[MAX_WIDGETS];

  int button_count;
  int label_count;
  int checkbox_count;
  int radio_button_count;
  int slider_count;
  int dropdown_count;
  int textboxes_count;

  XFontStruct *font;
} GooeyWindow;

GooeyWindow GooeyWindow_Create(const char *title, int width, int height);
void GooeyWindow_Run(GooeyWindow *win);
void GooeyButton_Add(GooeyWindow *win, const char *label, int x, int y,
                      int width, int height, void (*callback)());
bool GooeyButton_HandleClick(GooeyWindow *win, int x, int y);
GooeyTextbox *GooeyTextBox_Add(GooeyWindow *win);
void GooeyTextbox_Draw(GooeyWindow *win);
bool GooeyTextbox_HandleClick(GooeyWindow *win, int x, int y);
void GooeyTextbox_HandleKeyPress(GooeyWindow *win, XKeyEvent *key_event);

void GooeyWindow_Redraw(GooeyWindow *win);
void GooeyLabel_Add(GooeyWindow *win, const char *text, int x, int y);
void GooeyCheckbox_Add(GooeyWindow *win, int x, int y, int width, int height,
                        void (*callback)(bool checked));
bool GooeyCheckbox_HandleClick(GooeyWindow *win, int x, int y);
void GooeyRadioButton_Add(GooeyWindow *win, int x, int y, int radius,
                           void (*callback)(bool selected));
bool GooeyRadioButton_HandleClick(GooeyWindow *win, int x, int y);
void GooeySlider_Add(GooeyWindow *win, int x, int y, int width, int height,
                      int min_value, int max_value,
                      void (*callback)(int value));
void GooeySlider_HandleDrag(GooeyWindow *win, int x, int y);
void GooeyDropdown_Add(GooeyWindow *win, int x, int y, int width, int height,
                        const char **options, int num_options,
                        void (*callback)(int selected_index));
bool GooeyDropdown_HandleClick(GooeyWindow *win, int x, int y);

GooeyWindow GooeyWindow_Create(const char *title, int width, int height)
{
  GooeyWindow win;
  win.display = XOpenDisplay(NULL);
  if (!win.display)
  {
    fprintf(stderr, "Unable to open X display\n");
    exit(1);
  }
  win.font = XLoadQueryFont(win.display, "fixed");
  if (!win.font)
  {
    fprintf(stderr, "Failed to load system font\n");
    exit(1);
  }

  int screen = DefaultScreen(win.display);
  win.window = XCreateSimpleWindow(
      win.display, RootWindow(win.display, screen), 10, 10, width, height, 1,
      BlackPixel(win.display, screen), WhitePixel(win.display, screen));
  XStoreName(win.display, win.window, title);

  win.gc = XCreateGC(win.display, win.window, 0, NULL);
  win.wm_delete_window = XInternAtom(win.display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(win.display, win.window, &win.wm_delete_window, 1);

  XSelectInput(win.display, win.window,
               ExposureMask | ButtonPressMask | ButtonMotionMask |
                   KeyPressMask);
  XMapWindow(win.display, win.window);

  win.button_count = 0;
  win.label_count = 0;
  win.checkbox_count = 0;
  win.radio_button_count = 0;
  win.slider_count = 0;
  win.dropdown_count = 0;
  win.textboxes_count = 0;

  return win;
}



GooeyTextbox *GooeyTextBox_Add(GooeyWindow *win, void (*onTextChanged)(char *text))
{
  GooeyTextbox new_textbox = (GooeyTextbox){
      .x = 50, .y = 200, .width = 200, .height = 30, .focused = false, .callback = onTextChanged};
  new_textbox.text[0] = '\0';
  win->textboxes[win->textboxes_count] = new_textbox;
  win->textboxes_count++;
  return &win->textboxes[win->textboxes_count - 1];
}

void GooeyButton_Draw(GooeyWindow *win, GooeyButton *button)
{
  XSetForeground(win->display, win->gc,
                 button->hover ? COLOR_HOVER : COLOR_GRAY);
  XFillRectangle(win->display, win->window, win->gc, button->x, button->y,
                 button->width, button->height);
  XSetForeground(win->display, win->gc, COLOR_BLACK);
  XDrawString(win->display, win->window, win->gc, button->x + 5, button->y + 20,
              button->label, strlen(button->label));
}
bool GooeyTextbox_HandleClick(GooeyWindow *win, int x, int y)
{
  for (int i = 0; i < win->textboxes_count; i++)
  {
    GooeyTextbox *textbox = &win->textboxes[i];
    if (x >= textbox->x && x <= textbox->x + textbox->width &&
        y >= textbox->y && y <= textbox->y + textbox->height)
    {
      textbox->focused = true;
    }
    else
    {
      textbox->focused = false;
    }
  }
  return true;
}
void GooeyTextbox_Draw(GooeyWindow *win, int index)
{
  GooeyTextbox *textbox = &win->textboxes[index];

  XSetForeground(win->display, win->gc, COLOR_WHITE);
  XFillRectangle(win->display, win->window, win->gc, textbox->x, textbox->y,
                 textbox->width, textbox->height);

  XSetForeground(win->display, win->gc, textbox->focused ? COLOR_BLUE : COLOR_BLACK);
  XDrawRectangle(win->display, win->window, win->gc, textbox->x, textbox->y,
                 textbox->width, textbox->height);

  XSetForeground(win->display, win->gc, COLOR_BLACK);
  int text_x = textbox->x + 5;
  int text_y = textbox->y + (textbox->height / 2) + 5;

  int max_text_width = textbox->width - 10;
  size_t len = strlen(textbox->text);
  size_t start_index = textbox->scroll_offset;

  while (start_index < len && XTextWidth(win->font, textbox->text + start_index, len - start_index) > max_text_width)
  {
    start_index++;
  }

  char display_text[256];
  strncpy(display_text, textbox->text + start_index, sizeof(display_text) - 1);
  display_text[sizeof(display_text) - 1] = '\0';

  XDrawString(win->display, win->window, win->gc, text_x, text_y, display_text, strlen(display_text));

  if (textbox->focused)
  {
    int cursor_x = text_x + XTextWidth(win->font, display_text, strlen(display_text));
    XDrawLine(win->display, win->window, win->gc, cursor_x, textbox->y + 5, cursor_x, textbox->y + textbox->height - 5);
  }
}
void GooeyTextbox_HandleKeyPress(GooeyWindow *win, XKeyEvent *key_event)
{
  char buf[32];
  KeySym key;
  XLookupString(key_event, buf, sizeof(buf), &key, NULL);

  for (int i = 0; i < win->textboxes_count; i++)
  {
    GooeyTextbox *textbox = &win->textboxes[i];
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
      }
    }
    else if (key == XK_Return || key == XK_Linefeed)
    {

      textbox->focused = false;
    }
    else if (isprint(buf[0]) && len < sizeof(textbox->text) - 1)
    {

      strncat(textbox->text, buf, 1);

      // notify callback
      textbox->callback(textbox->text);

      int text_width = XTextWidth(win->font, textbox->text, len + 1);
      int max_text_width = textbox->width - 10;

      if (text_width > max_text_width)
      {
        textbox->scroll_offset++;
      }
    }
  }
}

void GooeyWindow_Redraw(GooeyWindow *win)
{
  XClearWindow(win->display, win->window);

  for (int i = 0; i < win->textboxes_count; ++i)
  {
    GooeyTextbox_Draw(win, i);
  }

  for (int i = 0; i < win->button_count; ++i)
  {
    GooeyButton_Draw(win, &win->buttons[i]);
  }

  for (int i = 0; i < win->label_count; ++i)
  {
    GooeyLabel *label = &win->labels[i];
    XDrawString(win->display, win->window, win->gc, label->x, label->y,
                label->text, strlen(label->text));
  }

  for (int i = 0; i < win->checkbox_count; ++i)
  {
    GooeyCheckbox *checkbox = &win->checkboxes[i];
    XSetForeground(win->display, win->gc, COLOR_BLACK);
    XDrawRectangle(win->display, win->window, win->gc, checkbox->x, checkbox->y,
                   checkbox->width, checkbox->height);
    if (checkbox->checked)
    {
      XFillRectangle(win->display, win->window, win->gc, checkbox->x + 5,
                     checkbox->y + 5, checkbox->width - 10,
                     checkbox->height - 10);
    }
  }

  // for (int i = 0; i < win->radio_button_count; ++i)

    for (int i = 0; i < win->radio_button_count; ++i)
    {
      GooeyRadioButton *radio_button = &win->radio_buttons[i];
      XSetForeground(win->display, win->gc, COLOR_BLACK);
      XDrawArc(win->display, win->window, win->gc,
               radio_button->x - radio_button->radius,
               radio_button->y - radio_button->radius, 2 * radio_button->radius,
               2 * radio_button->radius, 0, 360 * 64);
      if (radio_button->selected)
      {

        XFillArc(win->display, win->window, win->gc,
                 radio_button->x - radio_button->radius / 2,
                 radio_button->y - radio_button->radius / 2,
                 radio_button->radius, radio_button->radius, 0, 360 * 64);
      }
    }

  for (int i = 0; i < win->slider_count; ++i)
  {
    GooeySlider *slider = &win->sliders[i];

    XSetForeground(win->display, win->gc, COLOR_GRAY);
    XFillRectangle(win->display, win->window, win->gc, slider->x, slider->y,
                   slider->width, slider->height);

    int thumb_x = slider->x + (slider->value - slider->min_value) *
                                  slider->width /
                                  (slider->max_value - slider->min_value);

    XFillArc(win->display, win->window, win->gc, thumb_x - 5, slider->y - 5, 10,
             slider->height + 10, 0, 360 * 64);
  }

  for (int i = 0; i < win->dropdown_count; ++i)
  {
    GooeyDropdown *dropdown = &win->dropdowns[i];

    XSetForeground(win->display, win->gc, COLOR_GRAY);
    XFillRectangle(win->display, win->window, win->gc, dropdown->x, dropdown->y,
                   dropdown->width, dropdown->height);

    XSetForeground(win->display, win->gc, COLOR_BLACK);
    XDrawString(win->display, win->window, win->gc, dropdown->x + 5,
                dropdown->y + 20, dropdown->options[dropdown->selected_index],
                strlen(dropdown->options[dropdown->selected_index]));
  }

  XFlush(win->display);
}

void GooeyLabel_Add(GooeyWindow *win, const char *text, int x, int y)
{
  GooeyLabel *label = &win->labels[win->label_count++];
  label->x = x;
  label->y = y;
  label->text = text;
}

void GooeyButton_Add(GooeyWindow *win, const char *label, int x, int y,
                      int width, int height, void (*callback)())
{
  GooeyButton *button = &win->buttons[win->button_count++];
  button->x = x;
  button->y = y;
  button->width = width;
  button->height = height;
  button->label = label;
  button->callback = callback;
  button->hover = false;
}

bool GooeyButton_HandleClick(GooeyWindow *win, int x, int y)
{
  for (int i = 0; i < win->button_count; ++i)
  {
    GooeyButton *button = &win->buttons[i];
    if (x >= button->x && x <= button->x + button->width && y >= button->y &&
        y <= button->y + button->height)
    {
      if (button->callback)
        button->callback();
      return true;
    }
  }
  return false;
}

void GooeyCheckbox_Add(GooeyWindow *win, int x, int y,
                        void (*callback)(bool checked))
{
  GooeyCheckbox *checkbox = &win->checkboxes[win->checkbox_count++];
  checkbox->x = x;
  checkbox->y = y;
  checkbox->width = CHECKBOX_SIZE;
  checkbox->height = CHECKBOX_SIZE;
  checkbox->checked = false;
  checkbox->callback = callback;
}

bool GooeyCheckbox_HandleClick(GooeyWindow *win, int x, int y)
{
  for (int i = 0; i < win->checkbox_count; ++i)
  {
    GooeyCheckbox *checkbox = &win->checkboxes[i];
    if (x >= checkbox->x && x <= checkbox->x + checkbox->width &&
        y >= checkbox->y && y <= checkbox->y + checkbox->height)
    {
      checkbox->checked = !checkbox->checked;
      if (checkbox->callback)
        checkbox->callback(checkbox->checked);
      return true;
    }
  }
  return false;
}

void GooeyRadioButton_Add(GooeyWindow *win, int x, int y, int radius,
                           void (*callback)(bool selected))
{
  GooeyRadioButton *radio_button =
      &win->radio_buttons[win->radio_button_count++];
  radio_button->x = x;
  radio_button->y = y;
  radio_button->radius = radius;
  radio_button->selected = false;
  radio_button->callback = callback;
}

bool GooeyRadioButton_HandleClick(GooeyWindow *win, int x, int y)
{
  int state = false ; 
  for (int i = 0; i < win->radio_button_count; ++i)
  {
    GooeyRadioButton *radio_button = &win->radio_buttons[i];
    int dx = x - radio_button->x;
    int dy = y - radio_button->y;
    if (dx * dx + dy * dy <= radio_button->radius * radio_button->radius)
    {
      radio_button->selected = !radio_button->selected;
      if (radio_button->callback)
        radio_button->callback(radio_button->selected);
      state =true; 
      // return true;
    }else { 
        if ( radio_button->selected){ 
              radio_button->selected = !radio_button->selected ; 
              // XSetForeground(win->display, win->gc, COLOR_WHITE);
        }
    }
  }
  return state;
}


void GooeySlider_Add(GooeyWindow *win, int x, int y, int width, int height,
                      int min_value, int max_value,
                      void (*callback)(int value))
{
  GooeySlider *slider = &win->sliders[win->slider_count++];
  slider->x = x;
  slider->y = y;
  slider->width = width;
  slider->height = height;
  slider->min_value = min_value;
  slider->max_value = max_value;
  slider->value = min_value;
  slider->callback = callback;
}

void GooeySlider_HandleDrag(GooeyWindow *win, int x, int y)
{
  for (int i = 0; i < win->slider_count; ++i)
  {
    GooeySlider *slider = &win->sliders[i];
    if (y >= slider->y && y <= slider->y + slider->height &&
        x <= slider->x + slider->width)
    {
      slider->value =
          slider->min_value +
          ((x - slider->x) * (slider->max_value - slider->min_value)) /
              slider->width;
      if (slider->callback)
        slider->callback(slider->value);
      return;
    }
  }
}

void GooeyDropdown_Add(GooeyWindow *win, int x, int y, int width, int height,
                        const char **options, int num_options,
                        void (*callback)(int selected_index))
{
  GooeyDropdown *dropdown = &win->dropdowns[win->dropdown_count++];
  dropdown->x = x;
  dropdown->y = y;
  dropdown->width = width;
  dropdown->height = height;
  dropdown->options = options;
  dropdown->num_options = num_options;
  dropdown->selected_index = 0;
  dropdown->callback = callback;
}

bool GooeyDropdown_HandleClick(GooeyWindow *win, int x, int y)
{
  for (int i = 0; i < win->dropdown_count; ++i)
  {
    GooeyDropdown *dropdown = &win->dropdowns[i];
    if (x >= dropdown->x && x <= dropdown->x + dropdown->width &&
        y >= dropdown->y && y <= dropdown->y + dropdown->height)
    {
      dropdown->selected_index =
          (dropdown->selected_index + 1) % dropdown->num_options;
      if (dropdown->callback)
        dropdown->callback(dropdown->selected_index);
      return true;
    }
  }
  return false;
}

void GooeyWindow_HandleMouseMotion(GooeyWindow *win, int x, int y)
{
  for (int i = 0; i < win->button_count; ++i)
  {
    GooeyButton *button = &win->buttons[i];
    if (x >= button->x && x <= button->x + button->width && y >= button->y &&
        y <= button->y + button->height)
    {
      button->hover = true;
    }
    else
    {
      button->hover = false;
    }
  }
  GooeyWindow_Redraw(win);
}

void GooeyWindow_Run(GooeyWindow *win)
{
  XEvent event;
  while (1)
  {
    XNextEvent(win->display, &event);
    if (event.type == Expose)
    {
      GooeyWindow_Redraw(win);
    }
    else if (event.type == ButtonPress)
    {
      int x = event.xbutton.x;
      int y = event.xbutton.y;
      if (GooeyButton_HandleClick(win, x, y))
      {
        GooeyWindow_Redraw(win);
      }
      else if (GooeyCheckbox_HandleClick(win, x, y))
      {
        GooeyWindow_Redraw(win);
      }
      else if (GooeyRadioButton_HandleClick(win, x, y))
      {
        GooeyWindow_Redraw(win);
      }
      else if (GooeyDropdown_HandleClick(win, x, y))
      {
        GooeyWindow_Redraw(win);
      }
      else if (GooeyTextbox_HandleClick(win, x, y))
      {
        GooeyWindow_Redraw(win);
      }
      else
      {
        GooeySlider_HandleDrag(win, x, y);
        GooeyWindow_Redraw(win);
      }
    }
    else if (event.type == MotionNotify)
    {
      GooeyWindow_HandleMouseMotion(win, event.xmotion.x, event.xmotion.y);
    }
    else if (event.type == KeyPress)
    {
      GooeyTextbox_HandleKeyPress(win, &event.xkey);
      GooeyWindow_Redraw(win);
    }
    else if (event.xclient.data.l[0] == win->wm_delete_window)
    {
      break;
    }
  }

  XCloseDisplay(win->display);
}

void GooeyWindow_Cleanup(GooeyWindow *win)
{
  XFreeFont(win->display, win->font);
}

#endif
