#include "gooey.h"
#include <stdio.h>

// Callback functions
void onButtonClick()
{
    printf("Button clicked!\n");
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


    GooeyLayout *layout = GooeyLayout_Create(&win, LAYOUT_VERTICAL, 40, 40, 200, 200);
    GooeyWindowAttr attr = GooeyWindow_GetAttr(&win);
    
    GooeyLayout_AddChild(layout, GooeySlider_Add(&win, 0, 0, 200,
                                                 0, 100, true, NULL));
    GooeyLayout_AddChild(layout, GooeyButton_Add(&win, "Click Me", 50, 50, 80, 30, onButtonClick));
    GooeyLayout_AddChild(layout, GooeyButton_Add(&win, "Click Me", 50, 50, 80, 30, NULL));
    GooeyLayout_AddChild(layout, GooeyLabel_Add(&win, "Click Me", 50, 50));

    GooeyLayout_Build(layout);

    GooeyWindow_Run(&win);

    GooeyWindow_Cleanup(&win);

    return 0;
}
