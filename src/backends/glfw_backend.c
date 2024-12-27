/*
 Copyright (c) 2024 Yassine Ahmed Ali

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include "utils/glfw/glfw_utils.h"

typedef struct userPtr
{
    int id;
} userPtr;

typedef struct
{
    GooeyEvent *current_event;
    GLFWwindow *window;
    GLFWwindow **child_windows;
    GLFWcursor *cursor;
    GLuint *text_programs;
    GLuint shape_program;
    GLuint text_vbo;
    GLuint shape_vbo;
    userPtr *user_ptrs;
    GLuint *text_vaos;
    GLuint *shape_vaos;
    mat4x4 projection;
    GLuint text_fragment_shader;
    GLuint text_vertex_shader;
    Character characters[128];
    char font_path[256];
    int window_count;
    bool inhibit_reset; /**< useful for continuesly happening events like dragging a slider. */
    unsigned int selected_color;
} GooeyBackendContext;
;

static GooeyBackendContext ctx = {0};

void check_shader_compile(GLuint shader)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG_ERROR("ERROR: Shader compilation failed\n%s\n", infoLog);
        exit(EXIT_FAILURE);
    }
}

void check_shader_link(GLuint program)
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        LOG_ERROR("ERROR: Program linking failed\n%s\n", infoLog);
        exit(EXIT_FAILURE);
    }
}

void glfw_setup_shared()
{

    glGenBuffers(1, &ctx.text_vbo);

    ctx.text_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ctx.text_vertex_shader, 1, &text_vertex_shader_source, NULL);
    glCompileShader(ctx.text_vertex_shader);
    check_shader_compile(ctx.text_vertex_shader);

    ctx.text_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(ctx.text_fragment_shader, 1, &text_fragment_shader_source, NULL);
    glCompileShader(ctx.text_fragment_shader);
    check_shader_compile(ctx.text_fragment_shader);

    glGenBuffers(1, &ctx.shape_vbo);

    GLuint shape_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shape_vertex_shader, 1, &rectangle_vertex_shader, NULL);
    glCompileShader(shape_vertex_shader);
    check_shader_compile(shape_vertex_shader);

    GLuint shape_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shape_fragment_shader, 1, &rectangle_fragment_shader, NULL);
    glCompileShader(shape_fragment_shader);
    check_shader_compile(shape_fragment_shader);

    ctx.shape_program = glCreateProgram();
    glAttachShader(ctx.shape_program, shape_vertex_shader);
    glAttachShader(ctx.shape_program, shape_fragment_shader);
    glLinkProgram(ctx.shape_program);
    check_shader_link(ctx.shape_program);

    glDeleteShader(shape_vertex_shader);
    glDeleteShader(shape_fragment_shader);
}

void setup_seperate_vao(int window_id)
{

    ctx.text_programs[window_id] = glCreateProgram();
    glAttachShader(ctx.text_programs[window_id], ctx.text_vertex_shader);
    glAttachShader(ctx.text_programs[window_id], ctx.text_fragment_shader);
    glLinkProgram(ctx.text_programs[window_id]);
    check_shader_link(ctx.text_programs[window_id]);

    GLuint text_vao;
    glGenVertexArrays(1, &text_vao);

    glBindVertexArray(text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.text_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ctx.text_vaos[window_id] = text_vao;
    glBindVertexArray(0);

    GLuint shape_vao;
    glGenVertexArrays(1, &shape_vao);
    glBindVertexArray(shape_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.shape_vbo);

    GLint position_attrib = glGetAttribLocation(ctx.shape_program, "pos");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));

    GLint col_attrib = glGetAttribLocation(ctx.shape_program, "col");
    glEnableVertexAttribArray(col_attrib);
    glVertexAttribPointer(col_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));
    ctx.shape_vaos[window_id] = shape_vao;
}

void glfw_fill_rectangle(int x, int y, int width, int height, long unsigned int color, int window_id)
{

    GLFWwindow *target_window = (window_id == 0) ? ctx.window : ctx.child_windows[window_id - 1];
    glfwMakeContextCurrent(target_window);
    float ndc_x, ndc_y;
    float ndc_width, ndc_height;
    vec3 color_rgb;

    convert_coords_to_ndc(target_window, &ndc_x, &ndc_y, x, y);
    convert_dimension_to_ndc(target_window, &ndc_width, &ndc_height, width, height);
    convert_hex_to_rgb(&color_rgb, color);

    Vertex vertices[6];

    for (int i = 0; i < 6; i++)
    {
        vertices[i].col[0] = color_rgb[0];
        vertices[i].col[1] = color_rgb[1];
        vertices[i].col[2] = color_rgb[2];
    }

    vertices[0].pos[0] = ndc_x;
    vertices[0].pos[1] = ndc_y;
    vertices[1].pos[0] = ndc_x + ndc_width;
    vertices[1].pos[1] = ndc_y;
    vertices[2].pos[0] = ndc_x;
    vertices[2].pos[1] = ndc_y + ndc_height;

    vertices[3].pos[0] = ndc_x + ndc_width;
    vertices[3].pos[1] = ndc_y;
    vertices[4].pos[0] = ndc_x + ndc_width;
    vertices[4].pos[1] = ndc_y + ndc_height;
    vertices[5].pos[0] = ndc_x;
    vertices[5].pos[1] = ndc_y + ndc_height;

    glUseProgram(ctx.shape_program);

    glBindBuffer(GL_ARRAY_BUFFER, ctx.shape_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(ctx.shape_vaos[window_id]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void glfw_set_foreground(long unsigned int color)
{
    ctx.selected_color = color;
}

void glfw_draw_rectangle(int x, int y, int width, int height, long unsigned int color, int window_id)
{
    GLFWwindow *target_window = (window_id == 0) ? ctx.window : ctx.child_windows[window_id - 1];
    glfwMakeContextCurrent(target_window);
    float ndc_x, ndc_y;
    float ndc_width, ndc_height;
    vec3 color_rgb;

    convert_coords_to_ndc(target_window, &ndc_x, &ndc_y, x, y);
    convert_dimension_to_ndc(target_window, &ndc_width, &ndc_height, width, height);
    convert_hex_to_rgb(&color_rgb, color);

    Vertex vertices[4];

    for (int i = 0; i < 4; i++)
    {
        vertices[i].col[0] = color_rgb[0];
        vertices[i].col[1] = color_rgb[1];
        vertices[i].col[2] = color_rgb[2];
    }

    vertices[0].pos[0] = ndc_x;
    vertices[0].pos[1] = ndc_y;
    vertices[1].pos[0] = ndc_x + ndc_width;
    vertices[1].pos[1] = ndc_y;
    vertices[2].pos[0] = ndc_x + ndc_width;
    vertices[2].pos[1] = ndc_y + ndc_height;

    vertices[3].pos[0] = ndc_x;
    vertices[3].pos[1] = ndc_y + ndc_height;
    glUseProgram(ctx.shape_program);

    glBindBuffer(GL_ARRAY_BUFFER, ctx.shape_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(ctx.shape_vaos[window_id]);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void glfw_draw_line(int x1, int y1, int x2, int y2, long unsigned int color, int window_id)
{

    GLFWwindow *target_window = (window_id == 0) ? ctx.window : ctx.child_windows[window_id - 1];
    glfwMakeContextCurrent(target_window);
    float ndc_x1, ndc_y1;
    float ndc_x2, ndc_y2;
    vec3 color_rgb;

    convert_coords_to_ndc(target_window, &ndc_x1, &ndc_y1, x1, y1);
    convert_coords_to_ndc(target_window, &ndc_x2, &ndc_y2, x2, y2);

    convert_hex_to_rgb(&color_rgb, color);

    Vertex vertices[2];

    for (int i = 0; i < 2; i++)
    {
        vertices[i].col[0] = color_rgb[0];
        vertices[i].col[1] = color_rgb[1];
        vertices[i].col[2] = color_rgb[2];
    }

    vertices[0].pos[0] = ndc_x1;
    vertices[0].pos[1] = ndc_y1;
    vertices[1].pos[0] = ndc_x2;
    vertices[1].pos[1] = ndc_y2;
    glUseProgram(ctx.shape_program);

    glBindBuffer(GL_ARRAY_BUFFER, ctx.shape_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(ctx.shape_vaos[window_id]);
    glDrawArrays(GL_LINES, 0, 2);
}

void glfw_fill_arc(int x_center, int y_center, int width, int height, int angle1, int angle2, int window_id)
{

    GLFWwindow *target_window = (window_id == 0) ? ctx.window : ctx.child_windows[window_id - 1];
    glfwMakeContextCurrent(target_window);
    const int segments = 10;

    float ndc_x_center, ndc_y_center;
    convert_coords_to_ndc(target_window, &ndc_x_center, &ndc_y_center, x_center, y_center);

    vec3 color_rgb;
    convert_hex_to_rgb(&color_rgb, ctx.selected_color);

    Vertex vertices[segments + 2];

    vertices[0].pos[0] = ndc_x_center;
    vertices[0].pos[1] = ndc_y_center;
    vertices[0].col[0] = color_rgb[0];
    vertices[0].col[1] = color_rgb[1];
    vertices[0].col[2] = color_rgb[2];

    for (int i = 0; i <= segments; ++i)
    {
        float angle = (float)i / segments * 2.0f * M_PI;
        float x = x_center + (width * 0.5f * cosf(angle));
        float y = y_center + (height * 0.5f * sinf(angle));

        float ndc_x, ndc_y;
        convert_coords_to_ndc(window_id == 0 ? ctx.window : ctx.child_windows[window_id - 1], &ndc_x, &ndc_y, x, y);

        vertices[i + 1].pos[0] = ndc_x;
        vertices[i + 1].pos[1] = ndc_y;
        vertices[i + 1].col[0] = color_rgb[0];
        vertices[i + 1].col[1] = color_rgb[1];
        vertices[i + 1].col[2] = color_rgb[2];
    }

    glUseProgram(ctx.shape_program);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.shape_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(ctx.shape_vaos[window_id]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);
}

void set_projection(GLFWwindow *window, int width, int height, int window_id)
{
    mat4x4 projection;
    mat4x4_ortho(projection, 0.0f, width, height, 0.0f, -1.0f, 1.0f);
    glUseProgram(ctx.text_programs[window_id]);
    glUniformMatrix4fv(glGetUniformLocation(ctx.text_programs[window_id], "projection"), 1, GL_FALSE, (const GLfloat *)projection);
    glBindVertexArray(ctx.text_vaos[window_id]);
}
static void error_callback(int error, const char *description)
{
    LOG_ERROR("Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    userPtr *data = glfwGetWindowUserPointer(window);
    ctx.current_event->attached_window = data->id;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        ctx.current_event->type = GOOEY_EVENT_WINDOW_CLOSE;
    }

    if (action == GLFW_PRESS && (key >= 65 && key <= 90 || key == 259))
    {
        ctx.current_event->type = GOOEY_EVENT_KEY_PRESS;
        ctx.current_event->key_press.keycode = key;
    }
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    userPtr *data = glfwGetWindowUserPointer(window);
    ctx.current_event->attached_window = data->id;
    ctx.current_event->type = GOOEY_EVENT_MOUSE_SCROLL;
    ctx.current_event->mouse_scroll.x = xoffset;
    ctx.current_event->mouse_scroll.y = yoffset;
}

static void click_callback(GLFWwindow *window, int button, int action, int mods)
{
    userPtr *data = glfwGetWindowUserPointer(window);

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        ctx.current_event->type = action == GLFW_PRESS ? GOOEY_EVENT_CLICK_PRESS : GOOEY_EVENT_CLICK_RELEASE;
        ctx.current_event->click.x = ctx.current_event->mouse_move.x;
        ctx.current_event->click.y = ctx.current_event->mouse_move.y;
        ctx.current_event->attached_window = data->id;
    }
}

static void cursor_callback(GLFWwindow *window, double posX, double posY)
{
    userPtr *data = glfwGetWindowUserPointer(window);
    ctx.current_event->attached_window = data->id;
    ctx.current_event->mouse_move.x = posX;
    ctx.current_event->mouse_move.y = posY;
}

static void refresh_callback(GLFWwindow *window)
{
    ctx.current_event->type = GOOEY_EVENT_EXPOSE;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    userPtr *data = glfwGetWindowUserPointer(window);
    glfwMakeContextCurrent(window);
    set_projection(window, width, height, data->id);
    glViewport(0, 0, width, height);
    ctx.current_event->attached_window = data->id;
    LOG_CRITICAL("%d", data->id);
    ctx.current_event->type = GOOEY_EVENT_EXPOSE;
}

int glfw_init_ft()
{

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        LOG_ERROR("Could not initialize FreeType library\n");
        return -1;
    }

    FT_Face face;

    if (FT_New_Face(ft, "roboto.ttf", 0, &face))
    {
        LOG_ERROR("Failed to load font: %s\n", "Roboto");
        FT_Done_FreeType(ft);
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            LOG_ERROR("Failed to load Glyph for character: '%c'\n", c);
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            (int)face->glyph->advance.x};
        ctx.characters[c] = character;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return 0;
}

int glfw_init()
{

    ctx.inhibit_reset = 0;
    ctx.selected_color = 0x000000;
    ctx.window_count = 0;
    ctx.child_windows = (GLFWwindow **)malloc(sizeof(GLFWwindow *) * 100);
    ctx.current_event = (GooeyEvent *)malloc(sizeof(GooeyEvent));
    ctx.text_vaos = (GLuint *)malloc(sizeof(GLuint) * 100);
    ctx.shape_vaos = (GLuint *)malloc(sizeof(GLuint) * 100);
    ctx.user_ptrs = (userPtr *)malloc(sizeof(userPtr) * 100);
    ctx.text_programs = (GLuint *)malloc(sizeof(GLuint) * 100);
    ctx.current_event->type = -1;
    ctx.current_event->attached_window = -1;
    ctx.current_event->click.x = -1;
    ctx.current_event->click.y = -1;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        return -1;
    }

    return 0;
}

void glfw_reset_events(bool state)
{
    ctx.inhibit_reset = !state;
}
void glfw_window_dim(int *width, int *height, int window_id)
{

    get_window_size(window_id == 0 ? ctx.window : window_id == 0 ? ctx.window
                                                                 : ctx.child_windows[window_id - 1],
                    width, height);
}

int glfw_get_current_clicked_window(void)
{
    return ctx.current_event->attached_window;
}

void glfw_draw_text(int x, int y, const char *text, unsigned long color, float font_size, int window_id)
{
    GLFWwindow *target_window = window_id == 0 ? ctx.window : ctx.child_windows[window_id - 1];
    glfwMakeContextCurrent(target_window);
    vec3 color_rgb;
    float ndc_x, ndc_y;
    int window_width, window_height;

    glfw_window_dim(&window_width, &window_height, window_id);
    int initial_x = x;

    convert_coords_to_ndc(target_window, &ndc_x, &ndc_y, x, y);
    convert_hex_to_rgb(&color_rgb, color);
    glUseProgram(ctx.text_programs[window_id]);
    glUniform3f(glGetUniformLocation(ctx.text_programs[window_id], "textColor"), color_rgb[0], color_rgb[1], color_rgb[2]);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(ctx.text_vaos[window_id]);

    int line_count = 0;
    int last_space_pos = -1;
    int split_pos[128];
    int line_height = 20;
    float current_x = x;

    for (size_t i = 0; i < strlen(text); i++)
    {
        Character ch = ctx.characters[(unsigned char)text[i]];
        if (ch.textureID == 0)
            continue;

        float xpos = current_x + ch.bearingX * font_size;

        if (text[i] == ' ')
        {
            last_space_pos = current_x;
        }

        if (xpos + ch.width * font_size > window_width)
        {
            if (last_space_pos != -1)
            {
                split_pos[line_count] = last_space_pos;
                line_count++;
            }
            current_x = initial_x;
            y += (ch.height + ch.bearingY) * font_size;
        }
        else
        {
            current_x += (ch.advance >> 6) * font_size;
        }
    }

    int current_rendering_line = 0;
    current_x = initial_x;

    for (size_t i = 0; i < strlen(text); i++)
    {
        Character ch = ctx.characters[(unsigned char)text[i]];
        if (ch.textureID == 0)
            continue;

        float xpos = current_x + ch.bearingX * font_size;
        float ypos = y - ch.bearingY * font_size;
        float w = ch.width * font_size;
        float h = ch.height * font_size;

        if (current_rendering_line < line_count && xpos >= split_pos[current_rendering_line])
        {
            current_x = initial_x;
            y += (ch.height + ch.bearingY) * font_size + line_height;
            xpos = current_x + ch.bearingX * font_size;
            ypos = y - ch.bearingY * font_size;
            current_rendering_line++;
        }

        if (y + h > window_height)
        {
            break;
        }

        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, ctx.text_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        current_x += (ch.advance >> 6) * font_size;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GooeyWindow glfw_create_window(const char *title, int width, int height)
{

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ctx.window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwSetWindowSizeLimits(ctx.window, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE);
    GooeyWindow window = (GooeyWindow) {0};

    window.creation_id = ctx.window_count;

    if (!ctx.window)
    {
        glfwTerminate();
        free(ctx.current_event);
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(ctx.window, key_callback);
    glfwSetMouseButtonCallback(ctx.window, click_callback);
    glfwSetCursorPosCallback(ctx.window, cursor_callback);
    glfwSetWindowRefreshCallback(ctx.window, refresh_callback);
    glfwSetFramebufferSizeCallback(ctx.window, framebuffer_size_callback);
    glfwSetScrollCallback(ctx.window, scroll_callback);
    glfwMakeContextCurrent(ctx.window);

    ctx.user_ptrs[window.creation_id] = (userPtr){.id = window.creation_id};

    glfwSetWindowUserPointer(ctx.window, &ctx.user_ptrs[window.creation_id]);

    glfwSwapInterval(1);

    if (gladLoadGL() == 0)
        exit(EXIT_FAILURE);
        
    glfw_init_ft();

    glfw_setup_shared();

    setup_seperate_vao(window.creation_id);

    set_projection(ctx.window, width, height, window.creation_id);

    return window;
}

void glfw_make_window_visible(int window_id, bool visibility)
{
    if (visibility)
    {
        glfwShowWindow(ctx.child_windows[window_id == 0 ? window_id : window_id - 1]);
    }
    else
        glfwHideWindow(ctx.child_windows[window_id == 0 ? window_id : window_id - 1]);
}

void glfw_set_window_resizable(bool value, int window_id)
{
    int FLAG = value ? GLFW_TRUE : GLFW_FALSE;
    glfwSetWindowAttrib(window_id == 0 ? ctx.window : ctx.child_windows[window_id - 1], GLFW_RESIZABLE, FLAG);
}

GooeyWindow glfw_spawn_window(const char *title, int width, int height, bool visibility)
{
    GooeyWindow window;

    int is_visible = visibility ? GLFW_TRUE : GLFW_FALSE;

    glfwWindowHint(GLFW_VISIBLE, is_visible);

    GLFWwindow *glfw_window = glfwCreateWindow(width, height, title, NULL, ctx.window);
    glfwSetWindowSizeLimits(glfw_window, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE);

    ctx.child_windows[ctx.window_count] = glfw_window;
    window.creation_id = ctx.window_count + 1;
    ctx.window_count++;
    glfwMakeContextCurrent(glfw_window);
    glViewport(0, 0, width, height);
    setup_seperate_vao(window.creation_id);

    set_projection(glfw_window, width, height, window.creation_id);

    glfwSetKeyCallback(glfw_window, key_callback);
    glfwSetMouseButtonCallback(glfw_window, click_callback);
    glfwSetCursorPosCallback(glfw_window, cursor_callback);
    glfwSetWindowRefreshCallback(glfw_window, refresh_callback);
    glfwSetScrollCallback(glfw_window, scroll_callback);
    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);

    ctx.user_ptrs[window.creation_id] = (userPtr){.id = window.creation_id};

    glfwSetWindowUserPointer(glfw_window, &ctx.user_ptrs[window.creation_id]);

    glfwSwapInterval(1);
    return window;
}

GooeyEvent *glfw_handle_events()
{
    if (!ctx.current_event)
    {
        LOG_ERROR("Error: HandleEvents called without a valid window\n");
        return NULL;
    }

    if (ctx.inhibit_reset)
    {
        ctx.current_event->type = -1;
    }

    glfwPollEvents();

    return ctx.current_event;
}

void glfw_hide_current_child(void)
{

    if (ctx.current_event->attached_window > 0)
    {
        glfwHideWindow(ctx.child_windows[ctx.current_event->attached_window - 1]);
    }
}

void glfw_destroy_windows()
{
    if (ctx.window)
    {
        glfwDestroyWindow(ctx.window);
        ctx.window = NULL;
    }

    if (ctx.child_windows)
    {

        for (int i = 0; i < ctx.window_count; ++i)
        {
            glfwDestroyWindow(ctx.child_windows[i]);
            ctx.child_windows[i] = NULL;
        }
        free(ctx.child_windows);
        ctx.child_windows = NULL;
    }
}

void glfw_clear(int window_id)
{
    glfwMakeContextCurrent(window_id == 0 ? ctx.window : ctx.child_windows[window_id - 1]);

    glClear(GL_COLOR_BUFFER_BIT);
    vec3 color;
    convert_hex_to_rgb(&color, active_theme->base);
    glClearColor(color[0], color[1], color[2], 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void glfw_cleanup()
{
    if (ctx.current_event)
    {
        free(ctx.current_event);
        ctx.current_event = NULL;
    }

    if (ctx.cursor)
    {
        glfwDestroyCursor(ctx.cursor);
        ctx.cursor = NULL;
    }

    if (ctx.text_vaos)
    {
        free(ctx.text_vaos);
        ctx.text_vaos = NULL;
    }

    if (ctx.shape_vaos)
    {
        free(ctx.shape_vaos);
        ctx.shape_vaos = NULL;
    }

    if (ctx.user_ptrs)
    {
        free(ctx.user_ptrs);
        ctx.user_ptrs = NULL;
    }

    if (ctx.text_programs)
    {
        free(ctx.text_programs);
        ctx.text_programs = NULL;
    }

    glDeleteShader(ctx.text_vertex_shader);
    glDeleteShader(ctx.text_fragment_shader);

    glfwTerminate();
}

void glfw_update_background()
{

    for (int i = 0; i < ctx.window_count; ++i)
    {
        glfwMakeContextCurrent(i == 0 ? ctx.window : ctx.child_windows[i - 1]);

        vec3 color;
        convert_hex_to_rgb(&color, active_theme->base);
        glClearColor(color[0], color[1], color[2], 1.0f);
    }
}

void glfw_render(int window_id)
{

    GLFWwindow *context = window_id == 0 ? ctx.window : ctx.child_windows[window_id - 1];
    if (!context)
    {
        LOG_ERROR("Error: Render called without a valid window\n");
        return;
    }

    glfwSwapBuffers(context);
}

float glfw_get_text_width(const char *text, int length)
{
    float total_width = 0;
    for (int i = 0; i < length; ++i)
    {
        total_width += (ctx.characters[text[i]].width + ctx.characters[text[i]].bearingX) * 0.25f;
    }
    return total_width;
}

float glfw_get_text_height(const char *text, int length)
{
    float max_height = 0;

    for (int i = 0; i < length; ++i)
    {
        float char_height = ctx.characters[text[i]].height * 0.25f;
        if (char_height > max_height)
        {
            max_height = char_height;
        }
    }
    return max_height;
}

char *glfw_get_key_from_code(GooeyEvent *gooey_event)
{
    return LookupString(gooey_event->key_press.keycode);
}

void glfw_set_cursor(GOOEY_CURSOR cursor)
{
    switch (cursor)
    {

    case GOOEY_CURSOR_ARROW:
        ctx.cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        glfwSetCursor(ctx.window, ctx.cursor);
        break;

    case GOOEY_CURSOR_HAND:
        ctx.cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        glfwSetCursor(ctx.window, ctx.cursor);
        break;
    case GOOEY_CURSOR_TEXT:
        ctx.cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        glfwSetCursor(ctx.window, ctx.cursor);
        break;
    case GOOEY_CURSOR_CROSSHAIR:
        ctx.cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
        glfwSetCursor(ctx.window, ctx.cursor);
        break;
    case GOOEY_CURSOR_RESIZE_TL_BR:
        ctx.cursor = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
        glfwSetCursor(ctx.window, ctx.cursor);
        break;
    case GOOEY_CURSOR_RESIZE_TR_BL:
        ctx.cursor = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
        glfwSetCursor(ctx.window, ctx.cursor);
        break;

    case GOOEY_CURSOR_RESIZE_H:
        ctx.cursor = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
        glfwSetCursor(ctx.window, ctx.cursor);
        break;

    case GOOEY_CURSOR_RESIZE_V:
        ctx.cursor = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
        glfwSetCursor(ctx.window, ctx.cursor);
        break;

    case GOOEY_CURSOR_RESIZE_ALL:
        ctx.cursor = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
        glfwSetCursor(ctx.window, ctx.cursor);
        break;

    case GOOEY_CURSOR_NOT_ALLOWED:
        ctx.cursor = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
        glfwSetCursor(ctx.window, ctx.cursor);
        break;
    default:
        break;
    }
}

void glfw_destroy_window_from_id(int window_id)
{
    glfwDestroyWindow(ctx.child_windows[window_id]);
}

GooeyBackend glfw_backend = {
    .Init = glfw_init,
    .CreateWindow = glfw_create_window,
    .SpawnWindow = glfw_spawn_window,
    .GetWinDim = glfw_window_dim,
    .DestroyWindows = glfw_destroy_windows,
    .DestroyWindowFromId = glfw_destroy_window_from_id,
    .MakeWindowResizable = glfw_set_window_resizable,
    .GetCurrentClickedWindow = glfw_get_current_clicked_window,
    .HideCurrentChild = glfw_hide_current_child,
    .MakeWindowVisible = glfw_make_window_visible,
    .UpdateBackground = glfw_update_background,
    .Cleanup = glfw_cleanup,
    .Render = glfw_render,
    .HandleEvents = glfw_handle_events,
    .InhibitResetEvents = glfw_reset_events,
    .FillArc = glfw_fill_arc,
    .FillRectangle = glfw_fill_rectangle,
    .DrawRectangle = glfw_draw_rectangle,
    .DrawLine = glfw_draw_line,
    .SetForeground = glfw_set_foreground,
    .GetTextWidth = glfw_get_text_width,
    .GetTextHeight = glfw_get_text_height,
    .DrawText = glfw_draw_text,
    .GetKeyFromCode = glfw_get_key_from_code,
    .SetCursor = glfw_set_cursor,
    .Clear = glfw_clear};
