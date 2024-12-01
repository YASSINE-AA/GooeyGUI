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
void onRadioBtnClicked(bool selected){ 
    printf("Radio btn clikec");
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

    GooeyRadioButton_Add(&win,10,10,10,onRadioBtnClicked);
    GooeyRadioButton_Add(&win,10,30,10,onRadioBtnClicked);
    GooeyRadioButton_Add(&win,10,50,10,onRadioBtnClicked);
    const char* st[3] = { "main", "let", "vim"}; 
    GooeyDropdown_Add(&win, 90,300, 50,50,st,3,NULL); 
    // GooeyCheckbox_Add(&win, 0, 0, OnCheck);

    selected_textbox = GooeyTextBox_Add(&win, OnTextChanged); 

    GooeyWindow_Run(&win); 
    return 0;
}
