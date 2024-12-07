#include "tiny.h"
#include <stdio.h>

int main()
{
    GooeyBackend b = glfw_backend;
    if (b.Init() != 0)
    {
        return -1;
    }

    GooeyWindow window = b.CreateWindow("Hi!!", 1000, 1000);
    bool running = true;
    while (running)
    {
        b.Render();

        GooeyEvent event = b.HandleEvents();
        if (event.type == GOOEY_EVENT_WINDOW_CLOSE)
            running = false;
    }

    b.DestroyWindow();
    b.Cleanup();

    return 0;
}
