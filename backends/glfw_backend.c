#include "gooey_backend.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "linmath.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    GooeyEvent *current_event;
    GLFWwindow *window;
    GLuint program;
    GLuint vertex_array_object;
    GLuint vertex_buffer_object;
} GooeyBackendContext;

typedef struct Vertex {
    vec2 pos;
    vec3 col;
} Vertex;

static const char *rectangle_vertex_shader =
    "#version 330 core\n"
    "layout(location = 0) in vec2 pos;\n"
    "layout(location = 1) in vec3 col;\n"
    "out vec3 color;\n"
    "void main() {\n"
    "    gl_Position = vec4(pos, 0.0, 1.0);\n"
    "    color = col;\n"
    "}\n";

static const char *rectangle_fragment_shader =
    "#version 330 core\n"
    "in vec3 color;\n"
    "out vec4 fragment;\n"
    "void main() {\n"
    "    fragment = vec4(color, 1.0);\n"
    "}\n";

static GooeyBackendContext ctx = {0};

void get_window_size(int *window_width, int *window_height)
{
    glfwGetFramebufferSize(ctx.window, window_width, window_height);
}

void convert_coords_to_ndc(float *ndc_x, float *ndc_y, int x, int y)
{
    int window_width, window_height;
    get_window_size(&window_width, &window_height);

    *ndc_x = (2.0f * x / window_width) - 1.0f;
    *ndc_y = 1.0f - (2.0f * y / window_height);
}

void convert_dimension_to_ndc(float *ndc_w, float *ndc_h, int width, int height)
{
    int window_width, window_height;
    get_window_size(&window_width, &window_height);

    *ndc_w= (2.0f * width) / window_width;
    *ndc_h= -(2.0f * height) / window_height;
}

void convert_hex_to_rgb(vec3 *rgb, unsigned int color_hex)
{
    (*rgb)[0] = ((color_hex >> 16) & 0xFF) / 255.0f;
    (*rgb)[1] = ((color_hex >> 8) & 0xFF) / 255.0f;
    (*rgb)[2] = ((color_hex) & 0xFF) / 255.0f;
}

void setup_shaders() {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &rectangle_vertex_shader, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &rectangle_fragment_shader, NULL);
    glCompileShader(fragment_shader);

    ctx.program = glCreateProgram();
    glAttachShader(ctx.program, vertex_shader);
    glAttachShader(ctx.program, fragment_shader);
    glLinkProgram(ctx.program);
    glUseProgram(ctx.program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glGenVertexArrays(1, &ctx.vertex_array_object);
    glBindVertexArray(ctx.vertex_array_object);

    glGenBuffers(1, &ctx.vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.vertex_buffer_object);

    GLint position_attrib = glGetAttribLocation(ctx.program, "pos");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));

    GLint col_attrib = glGetAttribLocation(ctx.program, "col");
    glEnableVertexAttribArray(col_attrib);
    glVertexAttribPointer(col_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));
}
void glfw_draw_rectangle(float x, float y, float width, float height, unsigned int color)
{
    float ndc_x, ndc_y;
    float ndc_width, ndc_height;
    vec3 color_rgb;

    convert_coords_to_ndc(&ndc_x, &ndc_y, x, y);
    convert_dimension_to_ndc(&ndc_width, &ndc_height, width, height);
    convert_hex_to_rgb(&color_rgb, color);

    printf("%f %f %f\n", color_rgb[0], color_rgb[1], color_rgb[2]);

    Vertex vertices[6];

    for (int i = 0; i < 6; i++) {
        vertices[i].col[0] = color_rgb[0];
        vertices[i].col[1] = color_rgb[1];
        vertices[i].col[2] = color_rgb[2];
    }

    vertices[0].pos[0] = ndc_x;          vertices[0].pos[1] = ndc_y;
    vertices[1].pos[0] = ndc_x + ndc_width;  vertices[1].pos[1] = ndc_y;
    vertices[2].pos[0] = ndc_x;          vertices[2].pos[1] = ndc_y + ndc_height;

    vertices[3].pos[0] = ndc_x + ndc_width;  vertices[3].pos[1] = ndc_y;
    vertices[4].pos[0] = ndc_x + ndc_width;  vertices[4].pos[1] = ndc_y + ndc_height;
    vertices[5].pos[0] = ndc_x;          vertices[5].pos[1] = ndc_y + ndc_height;

    glBindBuffer(GL_ARRAY_BUFFER, ctx.vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(ctx.vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}



static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        ctx.current_event->type = GOOEY_EVENT_WINDOW_CLOSE;
}

int glfw_init() {
    ctx.current_event = malloc(sizeof(GooeyEvent));
    if (!ctx.current_event) {
        fprintf(stderr, "Error: Failed to allocate memory for current_event\n");
        return -1;
    }

    ctx.current_event->type = -1;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        free(ctx.current_event);
        return -1;
    }

    return 0;
}

GooeyWindow glfw_create_window(const char *title, int width, int height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ctx.window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!ctx.window) {
        glfwTerminate();
        free(ctx.current_event);
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(ctx.window, key_callback);

    GooeyWindow window = {.width = width, .height = height};

    glfwMakeContextCurrent(ctx.window);
    gladLoadGL();
    glfwSwapInterval(1);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    setup_shaders();

    return window;
}

GooeyEvent glfw_handle_events() {
    if (!ctx.window) {
        fprintf(stderr, "Error: HandleEvents called without a valid window\n");
        return (GooeyEvent){.type = -1};
    }

    ctx.current_event->type = -1;

    glfwPollEvents();

    return *(ctx.current_event);
}

void glfw_destroy_window() {
    if (ctx.window) {
        glfwDestroyWindow(ctx.window);
        ctx.window = NULL;
    }
}

void glfw_cleanup() {
    if (ctx.current_event) {
        free(ctx.current_event);
        ctx.current_event = NULL;
    }

    glfwTerminate();
}

void glfw_render() {
    if (!ctx.window) {
        fprintf(stderr, "Error: Render called without a valid window\n");
        return;
    }

    int width, height;
    glfwGetFramebufferSize(ctx.window, &width, &height);

    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width, height);



    glfw_draw_rectangle(20, 20, 200, 300, 0xFF0000);


    glfwSwapBuffers(ctx.window);
}

GooeyBackend glfw_backend = {
    .Init = glfw_init,
    .CreateWindow = glfw_create_window,
    .DestroyWindow = glfw_destroy_window,
    .Cleanup = glfw_cleanup,
    .Render = glfw_render,
    .HandleEvents = glfw_handle_events,
};
