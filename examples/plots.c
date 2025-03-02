#include <Gooey/gooey.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define DATA_COUNT 4

pthread_mutex_t data_mutex;
int received_data_count = 0;

typedef struct
{
    GooeyPlotData *plot_data;
    GooeyPlot *plot;
} ThreadArgs;

/*void* data_receiver_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    GooeyPlotData* data = args->plot_data;

    float x_full[DATA_COUNT] = {-3, 4, 13.5, 14, 19};
    float y_full[DATA_COUNT] = {-2, -3, 8, 13, 13};

    for (int i = 0; i < DATA_COUNT; i++) {
        sleep(1);

        pthread_mutex_lock(&data_mutex);
        data->x_data[i] = x_full[i];
        data->y_data[i] = y_full[i];
        received_data_count++;
        pthread_mutex_unlock(&data_mutex);

        printf("[Server] Received new data point: (%.2f, %.2f)\n", x_full[i], y_full[i]);

        GooeyPlot_Update(args->plot, data);
    }

    return NULL;
}*/

int main()
{
    Gooey_Init(GLFW);
    GooeyWindow win = GooeyWindow_Create("plot test", 800, 600, true);

    pthread_mutex_init(&data_mutex, NULL);

    GooeyPlotData data;
    char plot_title[] = "Dynamic Data Stream";
    data.title = plot_title;
    data.data_count = DATA_COUNT;
    data.x_step = 2;
    data.y_step = 2;

    float x_partial[DATA_COUNT] = {-1, 3, 4, 8};
    float y_partial[DATA_COUNT] = {4, 5, -8, 12};
    data.x_data = x_partial;
    data.y_data = y_partial;

    GooeyPlot *plot = GooeyPlot_Add(&win, GOOEY_PLOT_LINE, &data, 20, 20, 600, 400);

    // pthread_t thread_id;
    // ThreadArgs args = {&data, plot};
    // thread_create(&thread_id, NULL, data_receiver_thread, &args);

    GooeyWindow_Run(1, &win);

    // pthread_join(thread_id, NULL);
    // pthread_mutex_destroy(&data_mutex);
    GooeyWindow_Cleanup(1, &win);

    return 0;
}
