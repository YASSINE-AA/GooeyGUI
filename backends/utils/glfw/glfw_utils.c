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



char* LookupString(int keycode) 
{
    switch (keycode)
    {
        case -1: return strdup("Unknown");
        case 32: return strdup("Space");
        case 39: return strdup("Apostrophe");
        case 44: return strdup("Comma");
        case 45: return strdup("Minus");
        case 46: return strdup("Period");
        case 47: return strdup("Slash");
        case 48: return strdup("Num0");
        case 49: return strdup("Num1");
        case 50: return strdup("Num2");
        case 51: return strdup("Num3");
        case 52: return strdup("Num4");
        case 53: return strdup("Num5");
        case 54: return strdup("Num6");
        case 55: return strdup("Num7");
        case 56: return strdup("Num8");
        case 57: return strdup("Num9");
        case 59: return strdup("Semicolon");
        case 61: return strdup("Equal");
        case 65: return strdup("A");
        case 66: return strdup("B");
        case 67: return strdup("C");
        case 68: return strdup("D");
        case 69: return strdup("E");
        case 70: return strdup("F");
        case 71: return strdup("G");
        case 72: return strdup("H");
        case 73: return strdup("I");
        case 74: return strdup("J");
        case 75: return strdup("K");
        case 76: return strdup("L");
        case 77: return strdup("M");
        case 78: return strdup("N");
        case 79: return strdup("O");
        case 80: return strdup("P");
        case 81: return strdup("Q");
        case 82: return strdup("R");
        case 83: return strdup("S");
        case 84: return strdup("T");
        case 85: return strdup("U");
        case 86: return strdup("V");
        case 87: return strdup("W");
        case 88: return strdup("X");
        case 89: return strdup("Y");
        case 90: return strdup("Z");
        case 91: return strdup("LeftBracket");
        case 92: return strdup("Backslash");
        case 93: return strdup("RightBracket");
        case 96: return strdup("GraveAccent");
        case 161: return strdup("World1");
        case 162: return strdup("World2");
        case 256: return strdup("Escape");
        case 257: return strdup("Enter");
        case 258: return strdup("Tab");
        case 259: return "Backspace";
        case 260: return strdup("Insert");
        case 261: return strdup("Delete");
        case 262: return strdup("Right");
        case 263: return strdup("Left");
        case 264: return strdup("Down");
        case 265: return strdup("Up");
        case 266: return strdup("PageUp");
        case 267: return strdup("PageDown");
        case 268: return strdup("Home");
        case 269: return strdup("End");
        case 280: return strdup("CapsLock");
        case 281: return strdup("ScrollLock");
        case 282: return strdup("NumLock");
        case 283: return strdup("PrintScreen");
        case 284: return strdup("Pause");
        case 290: return strdup("F1");
        case 291: return strdup("F2");
        case 292: return strdup("F3");
        case 293: return strdup("F4");
        case 294: return strdup("F5");
        case 295: return strdup("F6");
        case 296: return strdup("F7");
        case 297: return strdup("F8");
        case 298: return strdup("F9");
        case 299: return strdup("F10");
        case 300: return strdup("F11");
        case 301: return strdup("F12");
        case 302: return strdup("F13");
        case 303: return strdup("F14");
        case 304: return strdup("F15");
        case 305: return strdup("F16");
        case 306: return strdup("F17");
        case 307: return strdup("F18");
        case 308: return strdup("F19");
        case 309: return strdup("F20");
        case 310: return strdup("F21");
        case 311: return strdup("F22");
        case 312: return strdup("F23");
        case 313: return strdup("F24");
        case 314: return strdup("F25");
        case 320: return strdup("Keypad0");
        case 321: return strdup("Keypad1");
        case 322: return strdup("Keypad2");
        case 323: return strdup("Keypad3");
        case 324: return strdup("Keypad4");
        case 325: return strdup("Keypad5");
        case 326: return strdup("Keypad6");
        case 327: return strdup("Keypad7");
        case 328: return strdup("Keypad8");
        case 329: return strdup("Keypad9");
        case 330: return strdup("KeypadDecimal");
        case 331: return strdup("KeypadDivide");
        case 332: return strdup("KeypadMultiply");
        case 333: return strdup("KeypadSubtract");
        case 334: return strdup("KeypadAdd");
        case 335: return strdup("KeypadEnter");
        case 336: return strdup("KeypadEqual");
        case 340: return strdup("LeftShift");
        case 341: return strdup("LeftControl");
        case 342: return strdup("LeftAlt");
        case 343: return strdup("LeftSuper");
        case 344: return strdup("RightShift");
        case 345: return strdup("RightControl");
        case 346: return strdup("RightAlt");
        case 347: return strdup("RightSuper");
        case 348: return strdup("Menu");
        default: return strdup("Unknown");
    }
}
