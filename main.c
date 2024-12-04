#include "tiny.h"
#include <stdio.h>
#include <unistd.h>

void onButtonClick() {
    printf("clicked \n");

}

int main()
{

    Gooey_Init();

    GooeyWindow win = GooeyWindow_Create("test", 400, 400);
    

    GooeyMenu_Set(&win);
    ;
    GooeyMenuChild_AddElement(GooeyMenu_AddChild(&win, "test"), "hello", NULL);
        GooeyMenuChild_AddElement(GooeyMenu_AddChild(&win, "test"), "hello", NULL);
    GooeyMenuChild_AddElement(GooeyMenu_AddChild(&win, "test"), "hello", NULL);
    GooeyMenuChild_AddElement(GooeyMenu_AddChild(&win, "test"), "hello", NULL);
    GooeyMenuChild_AddElement(GooeyMenu_AddChild(&win, "test"), "hello", NULL);
    GooeyTextBox_Add(&win, 30, 130, 200, 40, NULL);
    GooeyButton_Add(&win, "test", 40, 40, 200, 30, onButtonClick);
    
    GooeyWindow_Run(&win);
    

    GooeyWindow_Cleanup();
    
    return 0;
}
