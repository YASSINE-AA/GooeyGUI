#include "gooey_backend.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    GooeyEvent *current_event;
    GLFWwindow *window;
} GooeyBackendContext;

static GooeyBackendContext ctx = {0};

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        ctx.current_event->type = GOOEY_EVENT_WINDOW_CLOSE;
}

int glfw_init()
{

    ctx.current_event = malloc(sizeof(GooeyEvent));
    if (!ctx.current_event)
    {
        fprintf(stderr, "Error: Failed to allocate memory for current_event\n");
        return -1;
    }

    ctx.current_event->type = -1;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        free(ctx.current_event);
        return -1;
    }

    return 0;
}

GooeyWindow glfw_create_window(const char *title, int width, int height)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ctx.window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!ctx.window)
    {
        glfwTerminate();
        free(ctx.current_event);
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(ctx.window, key_callback);

    GooeyWindow window = {.width = width, .height = height};

    glfwMakeContextCurrent(ctx.window);
    gladLoadGL();
    glfwSwapInterval(1);

    return window;
}

GooeyEvent glfw_handle_events()
{
    if (!ctx.window)
    {
        fprintf(stderr, "Error: HandleEvents called without a valid window\n");
        return (GooeyEvent){.type = -1};
    }

    ctx.current_event->type = -1;

    glfwPollEvents();

    return *(ctx.current_event);
}

void glfw_destroy_window()
{
    if (ctx.window)
    {
        glfwDestroyWindow(ctx.window);
        ctx.window = NULL;
    }
}

void glfw_cleanup()
{

    if (ctx.current_event)
    {
        free(ctx.current_event);
        ctx.current_event = NULL;
    }

    glfwTerminate();
}

void glfw_render()
{
    if (!ctx.window)
    {
        fprintf(stderr, "Error: Render called without a valid window\n");
        return;
    }

    int width, height;
    glfwGetFramebufferSize(ctx.window, &width, &height);
    const float ratio = width / (float)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(ctx.window);
}

GooeyBackend glfw_backend =
    {
        .Init = glfw_init,
        .CreateWindow = glfw_create_window,
        .DestroyWindow = glfw_destroy_window,
        .Cleanup = glfw_cleanup,
        .Render = glfw_render,
        .HandleEvents = glfw_handle_events,
};
