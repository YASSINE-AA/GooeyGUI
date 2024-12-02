#include "gooey.h"
#include <stdio.h>

GooeyTextbox *textbox = NULL;

void onButtonClick()
{
    printf("Button clicked! \n");
}

void onButtonClick2()
{
    printf("Button clicked2!\n");
}
void onButtonClick3()
{
    printf("Button clicked3!\n");
}
void onButtonClick4 ()
{
    printf("Button clicked4!\n");
}



void onCheckboxToggle(bool checked)
{
    printf("Checkbox toggled: %s\n", checked ? "Checked" : "Unchecked");
}

void onRadioButtonSelect(bool selected)
{
    printf("Radio button selected: %s\n", selected ? "Yes" : "No");
}

void onSliderChange(int value)
{
    printf("Slider value changed: %d\n", value);
}

void onTest2()
{
    printf("test2\n");
}

void onTest1()
{
    printf("test1\n");
}

int main()
{
    GooeyWindow win = GooeyWindow_Create("Gooey Showcase", 400, 400);

    GooeyMenu_Set(&win);
    GooeyMenuChild *child = GooeyMenu_AddChild(&win, "File");
    GooeyMenuChild_AddElement(child, "test1", onTest1);

    GooeyMenuChild *child_ = GooeyMenu_AddChild(&win, "Edit");
    GooeyMenuChild_AddElement(child_, "test2", onTest2);

    GooeyWindowAttr attr = GooeyWindow_GetAttr(&win);
<<<<<<< HEAD
=======

    GooeyLayout_AddChild(layout, GooeySlider_Add(&win, 0, 0, 200,
                                                 0, 100, true, NULL));
    GooeyLayout_AddChild(layout, GooeyButton_Add(&win, "Click Me", 50, 50, 80, 30, onButtonClick));
    GooeyLayout_AddChild(layout, GooeyButton_Add(&win, "Click Me", 50, 50, 80, 30, NULL));
    GooeyLabel *label =  GooeyLabel_Add(&win, "Click Me", 50, 50);
    GooeyLayout_AddChild(layout,label);
    textbox = GooeyTextBox_Add(&win, 0, 0, 60, 20, NULL);
    GooeyLayout_AddChild(layout, textbox);
>>>>>>> main



    GooeySlider_Add(&win, 20, 200, 200, 0, 100, true, NULL);
    GooeyButton_Add(&win, "Click Me1", 50, 50, 80, 20, onButtonClick);
    GooeyButton_Add(&win, "Click Me2", 50, 80, 80, 20, onButtonClick2);
    GooeyButton_Add(&win, "Click Me3", 50, 110, 80, 20, onButtonClick3);
    GooeyButton_Add(&win, "Click Me4", 50, 140, 80, 20, onButtonClick4);


    GooeyWindow_Run(&win);

    GooeyWindow_Cleanup(&win);

    return 0;
}

