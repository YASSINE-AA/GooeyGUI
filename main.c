#include "tiny.h"

int main()
{

    GooeyBackend b = glfw_backend;

    printf("%d\n", b.Init());

    b.CreateWindow("nice", 800, 600);

    while (true)
    {
        b.Clear();
        b.DrawText(50, 50, "hellfgfggo", 0x000000);
        b.DrawRectangle(50, 50, 200, 200, 0xFF0000);
        b.Render();

        GooeyEvent event = b.HandleEvents();
        if (event.type == GOOEY_EVENT_WINDOW_CLOSE)
            break;
    }

    b.Cleanup();

    return 0;
}