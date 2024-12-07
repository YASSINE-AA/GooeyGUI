//
// Created by yassine-ahmed-ali on 12/7/24.
//

#ifndef GLFW_UTILS_H
#define GLFW_UTILS_H
#include "../gooey_backend.h"
#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include "../linmath.h"
#include <stdio.h>
#include <stdlib.h>



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

void get_window_size(GLFWwindow *window, int *window_width, int *window_height);

void convert_coords_to_ndc(GLFWwindow *window,float *ndc_x, float *ndc_y, int x, int y);


void convert_dimension_to_ndc(GLFWwindow *window,float *ndc_w, float *ndc_h, int width, int height);


void convert_hex_to_rgb(vec3 *rgb, unsigned int color_hex);


#endif //GLFW_UTILS_H
