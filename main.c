#include "tiny.h"
#include <stdio.h>
#include <unistd.h>

void onButtonClick()
{
    printf("clicked \n");
}

void onText(char *text)
{
    printf("%s \n", text);
}

void onValue(long value)
{
    printf("%ld \n", value);
}

int main()
{

    Gooey_Init(X11);

    GooeyWindow win = GooeyWindow_Create("test", 400, 400);

    GooeyMenu_Set(&win);
    ;
    GooeyMenuChild_AddElement(GooeyMenu_AddChild(&win, "test"), "hello", NULL);
    GooeyMenuChild_AddElement(GooeyMenu_AddChild(&win, "test"), "hello", NULL);
    GooeyMenuChild_AddElement(GooeyMenu_AddChild(&win, "test"), "hello", NULL);
    GooeyMenuChild_AddElement(GooeyMenu_AddChild(&win, "test"), "hello", NULL);
    GooeyMenuChild_AddElement(GooeyMenu_AddChild(&win, "test"), "hello", NULL);

    GooeyLayout *layout = GooeyLayout_Create(&win, LAYOUT_VERTICAL, 40, 40, 300, 200);
    GooeyLayout_AddChild(layout, GooeyButton_Add(&win, "test", 40, 40, 200, 30, onButtonClick));
    GooeyLayout_AddChild(layout, GooeyTextBox_Add(&win, 30, 130, 200, 40, "placeholder1", onText));
        GooeyLayout_AddChild(layout, GooeyTextBox_Add(&win, 30, 130, 200, 40, "placeholder2", onText));

    GooeyLayout_AddChild(layout, GooeySlider_Add(&win, 30, 130, 200, 40, 100, true, onValue));

    GooeyRadioButtonGroup *group = GooeyRadioButtonGroup_Create(&win);

    GooeyLayout_AddChild(layout, GooeyRadioButtonGroup_AddChild(group, 10, 10, "radio 1", NULL));
    GooeyLayout_AddChild(layout,GooeyRadioButtonGroup_AddChild(group, 10, 60, "radio 1", NULL));
    GooeyLayout_AddChild(layout,GooeyRadioButtonGroup_AddChild(group, 10, 120, "radio 1", NULL));
    GooeyLayout_AddChild(layout,GooeyRadioButtonGroup_AddChild(group, 10, 180, "radio 1", NULL));
    GooeyLayout_AddChild(layout, GooeyCheckbox_Add(&win, 10, 10, "test", NULL));
    

    GooeyLayout_Build(layout);

    GooeyWindow_Run(&win);

    GooeyWindow_Cleanup(&win);

    return 0;
}
