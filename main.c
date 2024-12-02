#include "gooey.h"
#include <stdio.h>

GooeyTextbox *selected_textbox = NULL;

void OnButtonClick()
{
    if (selected_textbox && selected_textbox->text)
    {
        printf("%s\n", selected_textbox->text);
    }
    else
    {
        printf("texbox empty.\n");
    }
}

void OnTextChanged(char *text)
{

    printf("Text changed %s.\n ", text);
}
void onRadioBtnClicked(bool selected)
{
    printf("Radio btn clikec");
}

void OnCheck(bool state)
{
    if (state)
    {
        printf("checked!\n");
    }
    else
    {
        printf("unchecked!\n");
    }
}


int main()
{

    GooeyWindow win = GooeyWindow_Create("Gooey example", 500, 500);

    GooeyMenu_Set(&win);
   
    GooeyMenuChild *file_menu = GooeyMenu_AddChild(&win, "File");
    GooeyMenuChild_AddElement(file_menu, "New", NULL);
    GooeyMenuChild_AddElement(file_menu, "Open", NULL);
    GooeyMenuChild_AddElement(file_menu, "Save", NULL);

    GooeyMenuChild *edit_menu = GooeyMenu_AddChild(&win, "Edit");
    GooeyMenuChild_AddElement(edit_menu, "Copy", NULL);
    GooeyMenuChild_AddElement(edit_menu, "Paste", NULL);

    GooeyMenuChild *help_menu = GooeyMenu_AddChild(&win, "Help");
    GooeyMenuChild_AddElement(help_menu, "About", NULL);

    GooeyButton_Add(&win, "Click me", 50, 50, 100, 30, OnButtonClick);
    GooeyLabel_Add(&win, "enter text:", 50, 180);

    GooeySlider_Add(&win, 50, 100, 300, 100, 1000, false, NULL);
    selected_textbox = GooeyTextBox_Add(&win, OnTextChanged);

    GooeyWindow_Run(&win);
    return 0;
}
