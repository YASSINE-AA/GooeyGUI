#include "tiny.h"
#include <ctype.h>

// Default theme
GooeyTheme default_theme;

GooeyTheme *active_theme = NULL;
GooeyBackend *active_backend = NULL;
GooeyBackends ACTIVE_BACKEND = -1;
int Gooey_Init(GooeyBackends backend)
{

    unsigned long primaryColor = (unsigned long)strtol("0x2196F3", NULL, 0);
    unsigned long baseColor = (unsigned long)strtol("0xFFFFFF", NULL, 0);
    unsigned long neutralColor = (unsigned long)strtol("0x000000", NULL, 0);
    unsigned long widgetBaseColor = (unsigned long)strtol("0xD3D3D3", NULL, 0);
    default_theme = (GooeyTheme){.base = baseColor, .neutral = neutralColor, .primary = primaryColor, .widget_base = widgetBaseColor};

    active_theme = &default_theme;

    switch (backend)
    {
    case X11:
        active_backend = &x11_backend;
        break;

    case GLFW:
        active_backend = &glfw_backend;

        break;
    default:
        break;
    }
    if (active_backend->Init() < 0)
    {
        return -1;
    }

    ACTIVE_BACKEND = backend;

    return 0;
}

GooeyWindow GooeyWindow_Create(const char *title, int width, int height)
{
    GooeyWindow win = active_backend->CreateWindow(title, width, height);
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

void GooeyWindow_setTheme(const char *fontPath)
{
    if (active_backend)
    {
        *active_theme = parser_load_theme_from_file(fontPath);
        active_backend->UpdateBackground();
    }
}

void GooeyRadioButtonGroup_Draw(GooeyWindow *win)
{
    for (int i = 0; i < win->widgets.radio_button_group_count; ++i)
    {
        GooeyRadioButtonGroup *group = &win->widgets.radio_button_groups[i];
        for (int j = 0; j < group->button_count; ++j)
        {
            GooeyRadioButton *button = &group->buttons[j];
            int button_center_x = button->core.x + RADIO_BUTTON_RADIUS;
            int button_center_y = button->core.y + RADIO_BUTTON_RADIUS;

            int label_width = active_backend->GetTextWidth(button->label, strlen(button->label));

            int label_x = ACTIVE_BACKEND == X11 ? button->core.x + RADIO_BUTTON_RADIUS * 2 + 10 : button->core.x + RADIO_BUTTON_RADIUS * 2;
            int label_y = ACTIVE_BACKEND == X11 ? button->core.y + RADIO_BUTTON_RADIUS + 5 : button->core.y + RADIO_BUTTON_RADIUS / 2;
            active_backend->DrawText(label_x, label_y, button->label, active_theme->neutral);
            active_backend->SetForeground(active_theme->neutral);
            active_backend->FillArc(button->core.x, button->core.y, RADIO_BUTTON_RADIUS * 2, RADIO_BUTTON_RADIUS * 2, 0, 360 * 64);
            if (button->selected)
            {
                active_backend->SetForeground(active_theme->primary);
                active_backend->FillArc(ACTIVE_BACKEND == X11 ? button->core.x + 5 : button->core.x, ACTIVE_BACKEND == X11 ? button->core.y + 5 : button->core.y, RADIO_BUTTON_RADIUS, RADIO_BUTTON_RADIUS, 0, 360 * 64);
            }
            else
            {
                active_backend->SetForeground(active_theme->base);

                active_backend->FillArc(ACTIVE_BACKEND == X11 ? button->core.x + 5 : button->core.x, ACTIVE_BACKEND == X11 ? button->core.y + 5 : button->core.y, RADIO_BUTTON_RADIUS, RADIO_BUTTON_RADIUS, 0, 360 * 64);
            }
        }
    }
}
GooeyRadioButtonGroup *GooeyRadioButtonGroup_Create(GooeyWindow *win)
{
    if (win->widgets.radio_button_group_count >= MAX_WIDGETS)
    {
        fprintf(stderr, "Cannot create more radio button groups. Maximum limit reached.\n");
        return NULL;
    }
    GooeyRadioButtonGroup *group = &win->widgets.radio_button_groups[win->widgets.radio_button_group_count++];
    group->button_count = 0;
    return group;
}

GooeyRadioButton *GooeyRadioButtonGroup_AddChild(GooeyRadioButtonGroup *group, int x, int y, const char *label, void (*callback)(bool))
{
    if (group->button_count >= MAX_RADIO_BUTTONS)
    {
        fprintf(stderr, "Cannot add more radio buttons to the group. Maximum limit reached.\n");
        return NULL;
    }
    GooeyRadioButton *button = &group->buttons[group->button_count++];
    button->core.x = x;
    button->core.y = y;
    button->selected = false;
    strncpy(button->label, label, sizeof(button->label) - 1);
    button->callback = callback;
    return button;
}

bool GooeyRadioButtonGroup_HandleClick(GooeyWindow *win, int x, int y)
{
    for (int i = 0; i < win->widgets.radio_button_group_count; ++i)
    {
        GooeyRadioButtonGroup *group = &win->widgets.radio_button_groups[i];
        for (int j = 0; j < group->button_count; ++j)
        {
            GooeyRadioButton *button = &group->buttons[j];
            int dx = x - (button->core.x + RADIO_BUTTON_RADIUS);
            int dy = y - (button->core.y + RADIO_BUTTON_RADIUS);

            if (dx * dx + dy * dy <= (RADIO_BUTTON_RADIUS + 10) * (RADIO_BUTTON_RADIUS + 10))
            {
                for (int k = 0; k < group->button_count; ++k)
                {
                    group->buttons[k].selected = false;
                }

                button->selected = true;

                if (button->callback)
                {
                    button->callback(true);
                }
                return true;
            }
        }
    }
    return false;
}

void GooeyWindow_Redraw(GooeyWindow *win)
{
    active_backend->Clear(win);

    for (int i = 0; i < win->widgets.button_count; ++i)
    {
        GooeyButton_Draw(win, &win->widgets.buttons[i]);
    }
    for (int i = 0; i < win->widgets.textboxes_count; ++i)
    {
        GooeyTextbox_Draw(win, i);
    }
    for (int i = 0; i < win->widgets.label_count; ++i)
    {
        GooeyLabel *label = &win->widgets.labels[i];
        active_backend->DrawText(label->core.x,
                                 label->core.y, label->text, active_theme->neutral);
    }
    for (int i = 0; i < win->widgets.checkbox_count; ++i)
    {
        GooeyCheckbox *checkbox = &win->widgets.checkboxes[i];

        int label_width = active_backend->GetTextWidth(checkbox->label, strlen(checkbox->label));
        int label_x = checkbox->core.x + CHECKBOX_SIZE + 10;
        int label_y = checkbox->core.y + (CHECKBOX_SIZE / 2) + 5;
        active_backend->DrawText(label_x, label_y, checkbox->label, active_theme->neutral);

        active_backend->DrawRectangle(checkbox->core.x, checkbox->core.y,
                                      checkbox->core.width, checkbox->core.height, active_theme->neutral);
        active_backend->FillRectangle(checkbox->core.x + 1, checkbox->core.y + 1,
                                      checkbox->core.width - 2, checkbox->core.height - 2, active_theme->base);

        if (checkbox->checked)
        {
            active_backend->FillRectangle(checkbox->core.x + 5, checkbox->core.y + 5,
                                          checkbox->core.width - 10, checkbox->core.height - 10, active_theme->primary);
        }
    }
    GooeyLabel_Draw(win);
    GooeyRadioButtonGroup_Draw(win);

    for (int i = 0; i < win->widgets.slider_count; ++i)
    {
        GooeySlider *slider = &win->widgets.sliders[i];

        active_backend->FillRectangle(slider->core.x,
                                      slider->core.y, slider->core.width, slider->core.height, active_theme->widget_base);

        int thumb_x = slider->core.x + (slider->value - slider->min_value) *
                                           slider->core.width /
                                           (slider->max_value - slider->min_value);

        active_backend->FillRectangle(thumb_x - 5,
                                      slider->core.y - 5, 10, slider->core.height + 10, active_theme->primary);

        if (slider->show_hints)
        {

            char min_value[20];
            char max_value[20];
            char value[20];
            sprintf(min_value, "%ld", slider->min_value);
            sprintf(max_value, "%ld", slider->max_value);
            sprintf(value, "%ld", slider->value);
            int min_value_width = active_backend->GetTextWidth(min_value, strlen(min_value));
            int max_value_width = active_backend->GetTextWidth(max_value, strlen(max_value));
            int value_width = active_backend->GetTextWidth(value, strlen(value));

            active_backend->DrawText(
                slider->core.x - min_value_width - 5, slider->core.y + 5,
                min_value, active_theme->neutral);
            active_backend->DrawText(
                slider->core.x + slider->core.width + 5, slider->core.y + 5,
                max_value, active_theme->neutral);
            if (slider->value != 0)
                active_backend->DrawText(thumb_x - 5,
                                         slider->core.y + 25, value, active_theme->neutral);
        }
        active_backend->SetForeground(active_theme->neutral);
    }

    // for (int i = 0; i < win->widgets.dropdown_count; ++i)
    // {
    //     GooeyDropdown *dropdown = &win->widgets.dropdowns[i];

    //     active_backend->FillRectangle(dropdown->core.x,
    //                                   dropdown->core.y, dropdown->core.width,
    //                                   dropdown->core.height, active_theme->widget_base);

    //     active_backend->DrawText(dropdown->core.x + 5,
    //                              dropdown->core.y + 20,
    //                              dropdown->options[dropdown->selected_index],
    //                              active_theme->neutral);
    // }
    GooeyDropdown_Draw(win);
    GooeyMenu_Draw(win);

    active_backend->Render();
    usleep(16667);
}

void GooeyButton_Draw(GooeyWindow *win, GooeyButton *button)
{

    active_backend->FillRectangle(button->core.x,
                                  button->core.y, button->core.width, button->core.height, button->clicked ? active_theme->primary : active_theme->widget_base);

    active_backend->DrawText(button->core.x + 5,
                             button->core.y + 20, button->label, button->clicked ? active_theme->base : active_theme->neutral);
    active_backend->SetForeground(active_theme->neutral);
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

GooeyLayout *GooeyLayout_Create(GooeyWindow *win, GooeyLayoutType layout_type,
                                int x, int y, int width, int height)
{
    if (!win || win->widgets.layout_count >= MAX_WIDGETS)
    {
        fprintf(stderr,
                "Window not initialized or unable to add more layouts (full).\n");
        return NULL;
    }
    GooeyLayout *layout = &win->widgets.layouts[win->widgets.layout_count++];

    layout->x = x;
    layout->y = y;
    layout->width = width;
    layout->height = height;
    layout->layout_type = layout_type;
    layout->widget_count = 0;

    return layout;
}
void GooeyLayout_AddChild(GooeyLayout *layout, void *widget)
{
    if (!layout)
    {
        fprintf(stderr, "Error: Invalid layout pointer.\n");
        return;
    }

    if (!widget)
    {
        fprintf(stderr, "Error: Invalid widget pointer.\n");
        return;
    }

    if (layout->widget_count >= MAX_WIDGETS)
    {
        fprintf(stderr, "Error: Maximum widgets reached for the layout.\n");
        return;
    }
    layout->widgets[layout->widget_count++] = widget;
}
void GooeyLayout_Build(GooeyLayout *layout)
{
    if (!layout)
    {
        fprintf(stderr, "Error: Invalid layout pointer.\n");
        return;
    }

    if (layout->widget_count == 0)
    {
        fprintf(stderr, "Error: Layout has no widgets to arrange.\n");
        return;
    }

    int spacing = 30;
    int current_x = layout->x;
    int current_y = layout->y;

    float max_widget_width = layout->layout_type == LAYOUT_HORIZONTAL ? layout->width / layout->widget_count : layout->width;

    switch (layout->layout_type)
    {
    case LAYOUT_VERTICAL:
        for (int i = 0; i < layout->widget_count; i++)
        {
            GooeyWidget *widget = layout->widgets[i];
            if (widget->type != WIDGET_CHECKBOX)
                widget->width = max_widget_width;
            widget->x = layout->x + (layout->width - widget->width) / 2;
            widget->y = current_y;
            current_y += widget->height + spacing;
        }
        break;

    case LAYOUT_HORIZONTAL:
        for (int i = 0; i < layout->widget_count; i++)
        {
            GooeyWidget *widget = layout->widgets[i];
            widget->width = max_widget_width;
            widget->x = current_x;
            widget->y = layout->y + (layout->height - widget->height) / 2;
            current_x += widget->width + spacing;
        }
        break;

    case LAYOUT_GRID:
    {
        break;
    }

    default:
        fprintf(stderr, "Error: Unsupported layout type.\n");
        break;
    }
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

void GooeyMenu_HandleClick(GooeyWindow *win, int x, int y)
{

    if (!win->widgets.menu)
        return;

    int x_offset = 10;
    for (int i = 0; i < win->widgets.menu->children_count; i++)
    {
        GooeyMenuChild *child = &win->widgets.menu->children[i];
        int text_width = active_backend->GetTextWidth(child->title, strlen(child->title));
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

GooeyLabel *GooeyLabel_Add(GooeyWindow *win, const char *text, int x, int y)
{
    GooeyLabel *label = &win->widgets.labels[win->widgets.label_count++];
    label->core.x = x;
    label->core.y = y;
    strcpy(label->text, text);
    return label;
}

void GooeyLabel_setText(GooeyLabel *label, const char *text)
{
    if (label)
        strcpy(label->text, text);
}

void GooeyLabel_Draw(GooeyWindow *win)
{
    for (int i = 0; i < win->widgets.label_count; ++i)
    {
        active_backend->DrawText(win->widgets.labels[i].core.x, win->widgets.labels[i].core.y, win->widgets.labels[i].text, active_theme->neutral);
    }
}

GooeyTextbox *GooeyTextBox_Add(GooeyWindow *win, int x, int y, int width,
                               int height, char *placeholder, void (*onTextChanged)(char *text))
{
    win->widgets.textboxes[win->widgets.textboxes_count].core.type = WIDGET_TEXTBOX;
    win->widgets.textboxes[win->widgets.textboxes_count].core.x = x;
    win->widgets.textboxes[win->widgets.textboxes_count].core.y = y;
    win->widgets.textboxes[win->widgets.textboxes_count].core.width = width;
    win->widgets.textboxes[win->widgets.textboxes_count].core.height = height;
    win->widgets.textboxes[win->widgets.textboxes_count].focused = false;
    win->widgets.textboxes[win->widgets.textboxes_count].callback = onTextChanged;
    win->widgets.textboxes[win->widgets.textboxes_count].scroll_offset = 0;
    win->widgets.textboxes[win->widgets.textboxes_count].text[0] = '\0';
    strcpy(win->widgets.textboxes[win->widgets.textboxes_count].placeholder, placeholder);

    win->widgets.textboxes_count++;
    return &win->widgets.textboxes[win->widgets.textboxes_count - 1];
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

    active_backend->FillRectangle(win->widgets.textboxes[index].core.x, win->widgets.textboxes[index].core.y,
                                  win->widgets.textboxes[index].core.width, win->widgets.textboxes[index].core.height, active_theme->base);

    active_backend->DrawRectangle(win->widgets.textboxes[index].core.x, win->widgets.textboxes[index].core.y,
                                  win->widgets.textboxes[index].core.width, win->widgets.textboxes[index].core.height,
                                  win->widgets.textboxes[index].focused ? active_theme->primary : active_theme->neutral);

    int text_x = win->widgets.textboxes[index].core.x + 5;
    int text_y = win->widgets.textboxes[index].core.y + (win->widgets.textboxes[index].core.height / 2) + 5;

    int max_text_width = win->widgets.textboxes[index].core.width - 10;
    size_t len = strlen(win->widgets.textboxes[index].text);
    size_t start_index = win->widgets.textboxes[index].scroll_offset;

    while (start_index < len &&
           active_backend->GetTextWidth(win->widgets.textboxes[index].text + start_index, len - start_index) > max_text_width)
    {
        start_index++;
    }

    char display_text[256];
    strncpy(display_text, win->widgets.textboxes[index].text + start_index, sizeof(display_text) - 1);
    display_text[sizeof(display_text) - 1] = '\0';

    active_backend->DrawText(text_x, text_y, display_text, active_theme->neutral);

    if (win->widgets.textboxes[index].focused)
    {
        int cursor_x = text_x + active_backend->GetTextWidth(display_text, strlen(display_text));
        active_backend->DrawLine(cursor_x, win->widgets.textboxes[index].core.y + 5,
                                 cursor_x, win->widgets.textboxes[index].core.y + win->widgets.textboxes[index].core.height - 5, active_theme->neutral);
    }
    else
    {

        if (win->widgets.textboxes[index].placeholder && strlen(win->widgets.textboxes[index].text) == 0)
            active_backend->DrawText(text_x, text_y, win->widgets.textboxes[index].placeholder, active_theme->widget_base);
    }
}
void GooeyTextbox_HandleKeyPress(GooeyWindow *win, GooeyEvent *key_event)
{
    printf("hey pressed \n");

    char *buf = active_backend->GetKeyFromCode(key_event);
    if (buf == NULL)
    {
        return;
    }

    for (int i = 0; i < win->widgets.textboxes_count; i++)
    {
        if (!win->widgets.textboxes[i].focused)
            continue;

        size_t len = strlen(win->widgets.textboxes[i].text);

        if (strcmp(buf, "Backspace") == 0)
        {

            if (len > 0)
            {
                win->widgets.textboxes[i].text[len - 1] = '\0';

                if (win->widgets.textboxes[i].scroll_offset > 0)
                {
                    win->widgets.textboxes[i].scroll_offset--;
                }

                if (win->widgets.textboxes[i].callback)
                {
                    win->widgets.textboxes[i].callback(win->widgets.textboxes[i].text);
                }
            }
        }
        else if (strcmp(buf, "Return") == 0)
        {

            win->widgets.textboxes[i].focused = false;
        }
        else if (strcmp(buf, "Tab") == 0)
        {
            // TODO IMPLEMENT TAB BEHAVIOR
        }
        else if (isprint(buf[0]) && len < sizeof(win->widgets.textboxes[i].text) - 1)
        {

            strncat(win->widgets.textboxes[i].text, buf, 1);

            if (win->widgets.textboxes[i].callback)
            {
                win->widgets.textboxes[i].callback(win->widgets.textboxes[i].text);
            }

            int text_width = active_backend->GetTextWidth(win->widgets.textboxes[i].text, len + 1);
            int max_text_width = win->widgets.textboxes[i].core.width - 10;

            if (text_width > max_text_width)
            {
                win->widgets.textboxes[i].scroll_offset++;
            }
        }
    }
    GooeyWindow_Redraw(win);

    if (!(strcmp(buf, "Backspace") == 0 || strcmp(buf, "Return") == 0 || strcmp(buf, "Tab") == 0))
    {
        free(buf);
    }
}
bool GooeyTextbox_HandleClick(GooeyWindow *win, int x, int y)
{

    for (int i = 0; i < win->widgets.textboxes_count; i++)
    {
        GooeyTextbox *textbox = &win->widgets.textboxes[i];
        if (x >= win->widgets.textboxes[i].core.x &&
            x <= win->widgets.textboxes[i].core.x + win->widgets.textboxes[i].core.width &&
            y >= win->widgets.textboxes[i].core.y &&
            y <= win->widgets.textboxes[i].core.y + win->widgets.textboxes[i].core.height)
        {
            win->widgets.textboxes[i].focused = true;
            for (int j = 0; j < win->widgets.textboxes_count; j++)
            {
                if (j != i)
                    win->widgets.textboxes[j].focused = false;
            }
            return true;
        }
    }
    return false;
}

GooeyCheckbox *GooeyCheckbox_Add(GooeyWindow *win, int x, int y, char *label,
                                 void (*callback)(bool checked))
{
    GooeyCheckbox *checkbox = &win->widgets.checkboxes[win->widgets.checkbox_count++];
    checkbox->core.type = WIDGET_CHECKBOX, checkbox->core.x = x;
    checkbox->core.y = y;
    checkbox->core.width = CHECKBOX_SIZE;
    checkbox->core.height = CHECKBOX_SIZE;
    if (label)
    {
        strcpy(checkbox->label, label);
    }
    else
    {
        sprintf(checkbox->label, "Checkbox %d", win->widgets.checkbox_count);
    }
    checkbox->checked = false;
    checkbox->callback = callback;
    return checkbox;
}

bool GooeyCheckbox_HandleClick(GooeyWindow *win, int x, int y)
{
    for (int i = 0; i < win->widgets.checkbox_count; ++i)
    {
        GooeyCheckbox *checkbox = &win->widgets.checkboxes[i];
        if (x >= checkbox->core.x && x <= checkbox->core.x + checkbox->core.width &&
            y >= checkbox->core.y &&
            y <= checkbox->core.y + checkbox->core.height)
        {
            checkbox->checked = !checkbox->checked;
            if (checkbox->callback)
                checkbox->callback(checkbox->checked);
            return true;
        }
    }
    return false;
}

GooeyRadioButton *GooeyRadioButton_Add(GooeyWindow *win, int x, int y,
                                       char *label,
                                       void (*callback)(bool selected))
{
    GooeyRadioButton *radio_button =
        &win->widgets.radio_buttons[win->widgets.radio_button_count++];

    radio_button->core.type = WIDGET_RADIOBUTTON;
    radio_button->core.x = x;
    radio_button->core.y = y;
    if (label)
        strcpy(radio_button->label, label);
    else
    {
        sprintf(radio_button->label, "Radio button %d", win->widgets.radio_button_count);
    }

    radio_button->radius = RADIO_BUTTON_RADIUS;
    radio_button->selected = false;
    radio_button->callback = callback;
    return radio_button;
}

bool GooeyRadioButton_HandleClick(GooeyWindow *win, int x, int y)
{
    int state = false;
    for (int i = 0; i < win->widgets.radio_button_count; ++i)
    {
        GooeyRadioButton *radio_button = &win->widgets.radio_buttons[i];
        int dx = x - radio_button->core.x;
        int dy = y - radio_button->core.y;
        if (dx * dx + dy * dy <= radio_button->radius * radio_button->radius)
        {
            radio_button->selected = !radio_button->selected;
            if (radio_button->callback)
                radio_button->callback(radio_button->selected);
            state = true;
        }
        else
        {
            if (radio_button->selected)
            {
                radio_button->selected = !radio_button->selected;
            }
        }
    }
    return state;
}

GooeySlider *GooeySlider_Add(GooeyWindow *win, int x, int y, int width,
                             long min_value, long max_value, bool show_hints,
                             void (*callback)(long value))
{
    if (!win)
    {
        fprintf(stderr, "Window cannot be NULL. \n");
        return NULL;
    }

    if (max_value <= min_value)
    {
        fprintf(stderr, "max_value should be greater than min_value. \n");
        return NULL;
    }

    GooeySlider *slider = &win->widgets.sliders[win->widgets.slider_count++];
    slider->core.type = WIDGET_SLIDER;
    slider->core.x = x;
    slider->core.y = y;
    slider->core.width = width;
    slider->core.height = SLIDER_HEIGHT;
    slider->min_value = min_value;
    slider->max_value = max_value;
    slider->value = min_value;
    slider->show_hints = show_hints;
    slider->callback = callback;
    return slider;
}
bool GooeySlider_HandleDrag(GooeyWindow *win, int x, int y)
{
    for (int i = 0; i < win->widgets.slider_count; ++i)
    {
        GooeySlider *slider = &win->widgets.sliders[i];
        if (y >= slider->core.y && y <= slider->core.y + slider->core.height &&
            x >= slider->core.x && x <= slider->core.x + slider->core.width)
        {
            slider->value =
                slider->min_value +
                ((x - slider->core.x) * (slider->max_value - slider->min_value)) /
                    slider->core.width;
            if (slider->callback)
                slider->callback(slider->value);
            return true;
        }
    }
    return false;
}

long GooeySlider_getValue(GooeySlider *slider)
{
    if (!slider)
    {
        fprintf(stderr, "Widget<Slider> cannot be NULL. \n");
        return -1;
    }

    return slider->value;
}

void GooeySlider_setValue(GooeySlider *slider, long value)
{
    if (!slider)
    {
        fprintf(stderr, "Widget<Slider> cannot be NULL. \n");
        return;
    }

    slider->value = value;
}

GooeyDropdown *GooeyDropdown_Add(GooeyWindow *win, int x, int y, int width,
                                 int height, const char **options,
                                 int num_options,
                                 void (*callback)(int selected_index))
{
    GooeyDropdown *dropdown = &win->widgets.dropdowns[win->widgets.dropdown_count++];
    dropdown->core.type = WIDGET_DROPDOWN;
    dropdown->core.x = x;
    dropdown->core.y = y;
    dropdown->core.width = width;
    dropdown->core.height = height;
    dropdown->options = options;
    dropdown->num_options = num_options;
    dropdown->selected_index = 0;
    dropdown->callback = callback;
    dropdown->is_open = false ; 
    return dropdown;
}
void GooeyMenu_Draw(GooeyWindow *win)
{
    if (win->widgets.menu)
    {

        active_backend->FillRectangle(0, 0, win->width, 20, active_theme->widget_base);

        int x_offset = 10;
        for (int i = 0; i < win->widgets.menu->children_count; i++)
        {
            GooeyMenuChild *child = &win->widgets.menu->children[i];
            int text_width = active_backend->GetTextWidth(child->title, strlen(child->title));
            active_backend->DrawText(x_offset, 15,
                                     child->title, active_theme->neutral);

            if (child->is_open && child->menu_elements_count > 0)
            {
                int submenu_x = x_offset - 10;
                int submenu_y = 20;
                int submenu_width = 150;
                int submenu_height = 25 * child->menu_elements_count;

                active_backend->FillRectangle(submenu_x, submenu_y,
                                              submenu_width, submenu_height, active_theme->widget_base);

                for (int j = 0; j < child->menu_elements_count; j++)
                {
                    int element_y = submenu_y + (j * 25);
                    active_backend->DrawText(submenu_x + 5,
                                             element_y + 18, child->menu_elements[j], active_theme->neutral);
                    if (j < child->menu_elements_count - 1)
                    {

                        active_backend->DrawLine(submenu_x,
                                                 element_y + 25 - 1, submenu_x + submenu_width,
                                                 element_y + 25 - 1, active_theme->neutral);
                    }
                }
            }

            x_offset += text_width + 20;
        }
 
    }
}
void GooeyDropdown_Draw(GooeyWindow *win)
{

    int x_offset = win->widgets.dropdowns[0].core.x;
    for (int i = 0; i < win->widgets.dropdown_count; i++)
    {
        // GooeyMenuChild *child = win->widgets.menu->children[i];
        GooeyDropdown *dropdown = &win->widgets.dropdowns[i];
        // active_backend->FillRectangle(0, 0, win->width, 20, active_theme->widget_base);
           active_backend->FillRectangle(dropdown->core.x,
                                  dropdown->core.y, dropdown->core.width,
                                  dropdown->core.height, active_theme->widget_base);
        active_backend->DrawText(dropdown->core.x + 5,
                             dropdown->core.y + 20,
                             dropdown->options[dropdown->selected_index],
                             active_theme->neutral);
        int text_width = active_backend->GetTextWidth(dropdown->options[dropdown->selected_index], strlen(dropdown->options[dropdown->selected_index]));
        active_backend->DrawText(x_offset, 15,
                                 dropdown->options[dropdown->selected_index], active_theme->neutral);
        if (dropdown->is_open && dropdown->num_options > 0)
        {
            int submenu_x = x_offset ;
            int submenu_y = win->widgets.dropdowns[0].core.y + win->widgets.dropdowns[0].core.height;
            int submenu_width = win->widgets.dropdowns[0].core.width;
            int submenu_height = 25 * dropdown->num_options;
            active_backend->FillRectangle(submenu_x, submenu_y,
                                          submenu_width, submenu_height, active_theme->widget_base);
            for (int j = 0; j < dropdown->num_options; j++)
            {
                int element_y = submenu_y + (j * 25);
                active_backend->DrawText(submenu_x + 5,
                                         element_y + 18, dropdown->options[j], active_theme->neutral);
                if (j < dropdown->num_options - 1)
                {
                active_backend->DrawLine(submenu_x,
                                             element_y + 25 - 1, submenu_x + submenu_width,
                                             element_y + 25 - 1, active_theme->neutral);
                }
            }
        }
        x_offset += text_width + 20;
    }
}

bool GooeyDropdown_HandleClick(GooeyWindow *win, int x, int y)
{
    // for (int i = 0; i < win->widgets.dropdown_count; ++i)
    // {
    //     GooeyDropdown *dropdown = &win->widgets.dropdowns[i];
    //     if (x >= dropdown->core.x && x <= dropdown->core.x + dropdown->core.width &&
    //         y >= dropdown->core.y &&  y <= dropdown->core.y + dropdown->core.height)
    //     {
    //         dropdown->is_open = ! dropdown->is_open ; 
    //         dropdown->selected_index = i ; // ; (dropdown->selected_index + 1) % dropdown->num_options;
    //         if (dropdown->callback)
    //             dropdown->callback(dropdown->selected_index);
    //         return true;
    //     }
    // }
    // return false;
    bool _btn_st = false ; 
    for (int i = 0; i < win->widgets.dropdown_count; i++)
    {   
    int x_offset = win->widgets.dropdowns[i].core.x;
        GooeyDropdown *dropdown = &win->widgets.dropdowns[i];
        int text_width =10 ;//  active_backend->GetTextWidth(dropdown->options[dropdown->selected_index], strlen(dropdown->options[dropdown->selected_index]));
         if (x >= dropdown->core.x && x <= dropdown->core.x + dropdown->core.width && y >= dropdown->core.y &&  y <= dropdown->core.y + dropdown->core.height)
        {
            
            dropdown->is_open = 0;
            printf("%d %d \n ", dropdown->selected_index, dropdown->num_options);
            dropdown->is_open = !dropdown->is_open;
            GooeyWindow_Redraw(win);
            _btn_st =  true;
            // return true ; 
        }
// 
        if (dropdown->is_open)
        {
            int submenu_x = x_offset ;
            int submenu_y = win->widgets.dropdowns[i].core.y + win->widgets.dropdowns[i].core.height;
            int submenu_width = win->widgets.dropdowns[i].core.width;
            for (int j = 0; j < dropdown->num_options; j++)
            {
                int element_y = submenu_y + (j * 25);
                if (x >= submenu_x && x <= submenu_x + submenu_width &&
                    y >= element_y && y <= element_y + 25)
                {
                    printf("test %d \n",j);
                    // GooeyDropdown* d = &win->widgets.dropdowns[j]; // ->callbacks[j]
                    dropdown->selected_index = j  ; // (dropdown->selected_index + 1) %   dropdown->num_options;

                    if (win->widgets.dropdowns[i].callback)
                        win->widgets.dropdowns[i].callback(j);

                    dropdown->is_open = 0;
                    GooeyWindow_Redraw(win);
                    // return true;
                }
            }
        }

        x_offset += text_width + 20;
    }
    if ( _btn_st){ 
        return true; 
    }else { 
        return false ;
    }
}
void GooeyWindow_Cleanup(GooeyWindow *win)
{
    if (win->widgets.menu)
    {
        free(win->widgets.menu);
        win->widgets.menu = NULL;
    }
    active_backend->DestroyWindow();
    active_backend->Cleanup();
}
void GooeyWindow_Run(GooeyWindow *win)
{

    GooeyEvent event;
    bool running = true;

    while (running)
    {
        event = active_backend->HandleEvents();

        switch (event.type)
        {
        case GOOEY_EVENT_EXPOSE:
            GooeyWindow_Redraw(win);
            break;
        case GOOEY_EVENT_KEY_PRESS:
        {
            GooeyTextbox_HandleKeyPress(win, &event);
            break;
        }
        case GOOEY_EVENT_CLICK:
        {
            int x = event.data.click.x;
            int y = event.data.click.y;
            GooeyMenu_HandleClick(win, x, y);
            if (GooeyButton_HandleClick(win, x, y))
            {
                GooeyWindow_Redraw(win);
            }
            else if (GooeyRadioButtonGroup_HandleClick(win, x, y))
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
            else if (GooeySlider_HandleDrag(win, x, y))
            {
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
