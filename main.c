#include "gooey.h"
#include <stdio.h>

GooeyTextbox* selected_textbox = NULL;


void OnButtonClick() {
    if (selected_textbox && selected_textbox->text) {
        printf("%s\n", selected_textbox->text);
    } else {
        printf("texbox empty.\n");
    }
}

void OnTextChanged(char* text) {

    printf("Text changed %s.\n ",  text);
}

void OnCheck(bool state) {
    if(state) {
        printf("checked!\n");
    } else {
        printf("unchecked!\n");
    }
}

int main() {

    GooeyWindow win = GooeyWindow_Create("Gooey example", 500, 500);

    GooeyButton_Add(&win, "Click me", 50, 50, 100, 30, OnButtonClick);
    GooeyLabel_Add(&win, "enter text:", 50, 180);

    GooeyCheckbox_Add(&win, 0, 0, "option 1", OnCheck);
    
    GooeyCheckbox_Add(&win, 0, 40, "option 2", OnCheck);

    GooeyCheckbox_Add(&win, 0, 80, "option 3", OnCheck);

    GooeyCheckbox_Add(&win, 0, 120, "option 4", OnCheck);

    selected_textbox = GooeyTextBox_Add(&win, OnTextChanged); 

    GooeyWindow_Run(&win); 
    return 0;
}
