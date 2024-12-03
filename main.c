#include "gooey.h"

void onRadioButtonSelected(bool selected)
{
    if (selected)
    {
        printf("Radio button selected\n");
    }
}

int main()
{

    GooeyWindow win = GooeyWindow_Create("Radio Button Group Example", 400, 300);

    GooeyRadioButtonGroup *group1 = GooeyRadioButtonGroup_Create(&win);
    GooeyRadioButtonGroup *group2 = GooeyRadioButtonGroup_Create(&win);

    GooeyRadioButtonGroup_AddChild(group1, 50, 50, "Option 1", onRadioButtonSelected);
    GooeyRadioButtonGroup_AddChild(group1, 50, 100, "Option 2", onRadioButtonSelected);
    GooeyRadioButtonGroup_AddChild(group1, 50, 150, "Option 3", onRadioButtonSelected);

    GooeyRadioButtonGroup_AddChild(group2, 200, 50, "Option A", onRadioButtonSelected);
    GooeyRadioButtonGroup_AddChild(group2, 200, 100, "Option B", onRadioButtonSelected);

    GooeyCheckbox_Add(&win, 20, 20, "hello", NULL);

    GooeyWindow_Run(&win);

    GooeyWindow_Cleanup(&win);
    return 0;
}
