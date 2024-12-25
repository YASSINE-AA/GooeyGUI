#include "gooey.h"
#include <stdio.h>
#include <string.h>
GooeyWindow win;
GooeyLayout *layout;
GooeyLabel *todoList[10];
GooeyTextbox *inputBox;
int taskCount = 0;

void onAddTask(void)
{
    if (taskCount < 10)
    {
        todoList[taskCount] = GooeyLabel_Add(&win, GooeyTextbox_getText(inputBox), 0.25f, 0, 50);
        GooeyLayout_AddChild(layout, todoList[taskCount]);
        taskCount++;
        GooeyLayout_Build(layout);
    }
}

int main()
{
    Gooey_Init(GLFW);

    win = GooeyWindow_Create("To-Do List", 400, 300, true);
    layout = GooeyLayout_Create(&win, LAYOUT_VERTICAL, 10, 20, 380, 280);
    GooeyLayout *input_layout = GooeyLayout_Create(&win, LAYOUT_HORIZONTAL, 0, 0, 0, 0);

    inputBox = GooeyTextBox_Add(&win, 0, 0, 300, 30, "Add task", NULL);
    GooeyButton *addButton = GooeyButton_Add(&win, "Add Task", 0, 0, 20, 30, onAddTask);


    GooeyLayout_AddChild(input_layout, inputBox);

    GooeyLayout_AddChild(input_layout, addButton);

    GooeyLayout_AddChild(layout, input_layout);

    GooeyLayout_Build(layout);

    GooeyWindow_Run(1, &win);
    GooeyWindow_Cleanup(1, &win);

    return 0;
}
