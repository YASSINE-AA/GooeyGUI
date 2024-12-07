//
// Created by yassine-ahmed-ali on 12/7/24.
//
#include "glfw_utils.h"

void get_window_size(GLFWwindow *window, int *window_width, int *window_height)
{
    glfwGetFramebufferSize(window, window_width, window_height);
}
void convert_coords_to_ndc(GLFWwindow *window, float *ndc_x, float *ndc_y, int x, int y)
{
    int window_width, window_height;
    get_window_size(window, &window_width, &window_height);

    *ndc_x = (2.0f * x / window_width) - 1.0f;
    *ndc_y = 1.0f - (2.0f * y / window_height);
}

void convert_dimension_to_ndc(GLFWwindow *window, float *ndc_w, float *ndc_h, int width, int height)
{
    int window_width, window_height;
    get_window_size(window, &window_width, &window_height);

    *ndc_w= (2.0f * width) / window_width;
    *ndc_h= -(2.0f * height) / window_height;
}

void convert_hex_to_rgb(vec3 *rgb, unsigned int color_hex)
{
    (*rgb)[0] = ((color_hex >> 16) & 0xFF) / 255.0f;
    (*rgb)[1] = ((color_hex >> 8) & 0xFF) / 255.0f;
    (*rgb)[2] = ((color_hex) & 0xFF) / 255.0f;
}
