#include <Gooey/gooey.h>

int main() 
{
    Gooey_Init(GLFW);

    GooeyWindow win = GooeyWindow_Create("plot test", 800, 600, true);

    GooeyPlotData data;
    char plot_title[] = "a very very long plot title that should be shorter";
    data.title = plot_title;
    data.data_count = 5;
    float x_data[5] = {2, 5, 3, 13, 14};
    float y_data[5] = {2, 5, 8, 13, 14};

    data.x_step =2;
    data.y_step =2;

    data.x_data = x_data;
    data.y_data = y_data;

    GooeyPlot* plot = GooeyPlot_Add(&win, GOOEY_PLOT_LINE, &data, 20, 20, 600, 400);

    GooeyWindow_Run(1, &win);
    GooeyWindow_Cleanup(1, &win);

    return 0;
}
