

#include "tiny.h"

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

void onSliderChange(long int value)
{
    printf("Slider value changed: %ld\n", value);
}

void onDropdownChange(int selectedIndex)
{
    printf("Dropdown selected index: %d\n", selectedIndex);
}

void onTextChange(char *text)
{
    printf("Text changed: %s\n", text);
}
GooeyTheme theme;
void activateDarkTheme()
{
    GooeyWindow_setTheme("dark.json");
}

void activateLightTheme()
{
    GooeyWindow_setTheme("light.json");
}

int main()
{



    Gooey_Init(GLFW);

    GooeyWindow win = GooeyWindow_Create("Gooey Showcase", 400, 700);

    GooeyButton_Add(&win, "Click Me", 50, 50, 80, 30, onButtonClick);

    GooeyLabel_Add(&win, "This is a label", 200, 60);

    GooeyCheckbox_Add(&win, 50, 120, "Enable Option 1", onCheckboxToggle);
    GooeyCheckbox_Add(&win, 50, 160, "Enable Option 2", onCheckboxToggle);
    GooeyCheckbox_Add(&win, 50, 200, "Enable Option 3", onCheckboxToggle);

    GooeyRadioButton_Add(&win, 200, 120, "Option A", onRadioButtonSelect);
    GooeyRadioButton_Add(&win, 200, 160, "Option B", onRadioButtonSelect);
    GooeyRadioButton_Add(&win, 200, 200, "Option C", onRadioButtonSelect);

    GooeySlider_Add(&win, 50, 250, 200, 0, 100, true, onSliderChange);

    const char *options[] = {"Option 1", "Option 2", "Option 3"};
    GooeyDropdown_Add(&win, 50, 350, 150, 30, options, 3, onDropdownChange);

    GooeyTextBox_Add(&win, 50, 300, 200, 25, "test", onTextChange);

    GooeyMenu *menu = GooeyMenu_Set(&win);
    GooeyMenuChild *fileMenu = GooeyMenu_AddChild(&win, "File");
    GooeyMenuChild_AddElement(fileMenu, "Open", NULL);
    GooeyMenuChild_AddElement(fileMenu, "Save", NULL);

    GooeyMenuChild *editMenu = GooeyMenu_AddChild(&win, "Settings");
    GooeyMenuChild_AddElement(editMenu, "Dark Theme", activateDarkTheme);
    GooeyMenuChild_AddElement(editMenu, "Light Theme", activateLightTheme);

    GooeyRadioButtonGroup *rbg = GooeyRadioButtonGroup_Create(&win);
    GooeyRadioButtonGroup_AddChild(&win, rbg, 50, 440, "test", NULL);

    GooeyRadioButtonGroup_AddChild(&win, rbg, 50, 470, "test", NULL);

    GooeyRadioButtonGroup_AddChild(&win, rbg, 50, 500, "test", NULL);

    GooeyRadioButtonGroup_AddChild(&win, rbg, 50, 530, "test", NULL);
    GooeyWindow_Run(&win);

    GooeyWindow_Cleanup(&win);

    return 0;
}