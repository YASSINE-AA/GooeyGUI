#include "tiny.h"
#include <stdio.h>

// Callback functions
void onButtonClick() {
    printf("Button clicked!\n");
}

void onCheckboxToggle(bool checked) {
    printf("Checkbox toggled: %s\n", checked ? "Checked" : "Unchecked");
}

void onRadioButtonSelect(bool selected) {
    printf("Radio button selected: %s\n", selected ? "Yes" : "No");
}

void onSliderChange(long int value) {
    printf("Slider value changed: %ld\n", value);
}

void onDropdownChange(int selectedIndex) {
    printf("Dropdown selected index: %d\n", selectedIndex);
}

void onTextChange(char *text) {
    printf("Text changed: %s\n", text);
}

int main() {
    Gooey_Init(GLFW);
    GooeyWindow win = GooeyWindow_Create("Gooey Showcase", 400, 400);

    GooeyButton_Add(&win, "Click Me", 50, 50, 80, 30, onButtonClick);

    GooeyWindow_Run(&win);

    GooeyWindow_Cleanup(&win);

    return 0;
}