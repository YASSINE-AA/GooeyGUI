
#include "utils/glfw_utils.h"
typedef struct {
    GooeyEvent *current_event;
    GLFWwindow *window;
    GLuint program;
    GLuint vertex_array_object;
    GLuint vertex_buffer_object;
    unsigned int selected_color;
} GooeyBackendContext;
static GooeyBackendContext ctx = {0};

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
void glfw_fill_rectangle(int x, int y, int width, int height, long unsigned int color)
{
    float ndc_x, ndc_y;
    float ndc_width, ndc_height;
    vec3 color_rgb;

    convert_coords_to_ndc(ctx.window, &ndc_x, &ndc_y, x, y);
    convert_dimension_to_ndc(ctx.window, &ndc_width, &ndc_height, width, height);
    convert_hex_to_rgb(&color_rgb, color);

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

void glfw_set_foreground(unsigned int color)
{
    ctx.selected_color = color;
}

void glfw_draw_rectangle(int x, int y, int width, int height, long unsigned int color)
{
    float ndc_x, ndc_y;
    float ndc_width, ndc_height;
    vec3 color_rgb;

    convert_coords_to_ndc(ctx.window, &ndc_x, &ndc_y, x, y);
    convert_dimension_to_ndc(ctx.window, &ndc_width, &ndc_height, width, height);
    convert_hex_to_rgb(&color_rgb, color);

    Vertex vertices[4];

    for (int i = 0; i < 4; i++) {
        vertices[i].col[0] = color_rgb[0];
        vertices[i].col[1] = color_rgb[1];
        vertices[i].col[2] = color_rgb[2];
    }

    vertices[0].pos[0] = ndc_x;          vertices[0].pos[1] = ndc_y;
    vertices[1].pos[0] = ndc_x + ndc_width;  vertices[1].pos[1] = ndc_y;
    vertices[2].pos[0] = ndc_x + ndc_width;          vertices[2].pos[1] = ndc_y + ndc_height;

    vertices[3].pos[0] = ndc_x;  vertices[3].pos[1] = ndc_y + ndc_height;

    glBindBuffer(GL_ARRAY_BUFFER, ctx.vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(ctx.vertex_array_object);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}



void glfw_draw_line(int x1, int y1, int x2, int y2, long unsigned int color)
{
    float ndc_x1, ndc_y1;
    float ndc_x2, ndc_y2;
    vec3 color_rgb;

    convert_coords_to_ndc(ctx.window, &ndc_x1, &ndc_y1, x1, y1);
    convert_coords_to_ndc(ctx.window, &ndc_x2, &ndc_y2, x2, y2);

    convert_hex_to_rgb(&color_rgb, color);

    Vertex vertices[2];

    for (int i = 0; i < 2; i++) {
        vertices[i].col[0] = color_rgb[0];
        vertices[i].col[1] = color_rgb[1];
        vertices[i].col[2] = color_rgb[2];
    }


    vertices[0].pos[0] = ndc_x1;  vertices[0].pos[1] = ndc_y1;
    vertices[1].pos[0] = ndc_x2;  vertices[1].pos[1] = ndc_y2;

    glBindBuffer(GL_ARRAY_BUFFER, ctx.vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(ctx.vertex_array_object);
    glDrawArrays(GL_LINES, 0, 2);
}

void glfw_fill_arc(int x_center, int y_center, int width, int height, int angle1, int angle2) {
    const int segments = 100;
    float ndc_x_center, ndc_y_center;
    int window_width, window_height;
    convert_coords_to_ndc(ctx.window, &ndc_x_center, &ndc_y_center, x_center, y_center);

    get_window_size(ctx.window, &window_width, &window_height);

    vec3 color_rgb;
    convert_hex_to_rgb(&color_rgb, ctx.selected_color);

    if (angle1 > angle2) {
        float temp = angle1;
        angle1 = angle2;
        angle2 = temp;
    }

    angle1 = fmodf(angle1, 2.0f * M_PI);
    angle2 = fmodf(angle2, 2.0f * M_PI);
    if (angle2 < angle1) {
        angle2 += 2.0f * M_PI;
    }

    int arc_segments = (int)((angle2 - angle1) / (2.0f * M_PI) * segments);
    if (arc_segments < 2) arc_segments = 2;

    Vertex vertices[arc_segments + 2];

    vertices[0].pos[0] = ndc_x_center;
    vertices[0].pos[1] = ndc_y_center;
    vertices[0].col[0] = color_rgb[0];
    vertices[0].col[1] = color_rgb[1];
    vertices[0].col[2] = color_rgb[2];

    for (int i = 0; i <= arc_segments; ++i) {
        float t = (float)i / arc_segments;
        float angle = angle1 + t * (angle2 - angle1);
        float x = x_center + (width * cosf(angle));
        float y = y_center + (height * sinf(angle));

        float ndc_x, ndc_y;
        convert_coords_to_ndc(ctx.window, &ndc_x, &ndc_y, x, y);

        vertices[i + 1].pos[0] = ndc_x;
        vertices[i + 1].pos[1] = ndc_y;
        vertices[i + 1].col[0] = color_rgb[0];
        vertices[i + 1].col[1] = color_rgb[1];
        vertices[i + 1].col[2] = color_rgb[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, ctx.vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(ctx.vertex_array_object);
    glDrawArrays(GL_TRIANGLE_FAN, 0, arc_segments + 2);
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
    ctx.selected_color = 0x000000;

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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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
    //glfw_draw_line(20, 20, 80, 40, 0xFF0000);
    //glfw_draw_rectangle(20, 20, 80, 40, 0xFF0000);
    //glfw_fill_arc(120, 120, 100, 50 ,20, 90);
    glfwSwapBuffers(ctx.window);
}

GooeyBackend glfw_backend = {
    .Init = glfw_init,
    .CreateWindow = glfw_create_window,
    .DestroyWindow = glfw_destroy_window,
    .Cleanup = glfw_cleanup,
    .Render = glfw_render,
    .HandleEvents = glfw_handle_events,
    .FillArc = glfw_fill_arc,
    .FillRectangle = glfw_fill_rectangle,
    .DrawRectangle = glfw_draw_rectangle,
    .DrawLine =  glfw_draw_line,

};
