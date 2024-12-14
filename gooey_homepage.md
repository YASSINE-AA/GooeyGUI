
<p align="center">
  <img src="gooey.png" width=128>
</p>

# Gooey GUI Library

Gooey is a lightweight and feature-rich GUI (Graphical User Interface) library designed for X11 environments. It provides a range of customizable widgets and layouts for creating responsive and modern desktop applications. The library is optimized for simplicity and offers a flexible API for developers.

## Features

- **Wide Range of Widgets**:
  - Labels
  - Buttons
  - Checkboxes
  - Radio Buttons (and Groups)
  - Sliders
  - Dropdown Menus
  - Textboxes

- **Layouts**:
  - Horizontal
  - Vertical
  - Grid-based layouts

- **Customization Options**:
  - Support for widget-specific callbacks
  - Styling with predefined color schemes
  - Interactive hover and click states

- **Event Handling**:
  - Mouse clicks, motion, and keypress events
  - Dynamic updates through an efficient redraw system

## Getting Started

### Prerequisites

Ensure your development environment includes:
- **X11 Development Libraries**: Gooey relies on X11 (`<X11/X.h>`, `<X11/Xlib.h>`, `<X11/Xutil.h>`).

### Installation

To integrate Gooey into your project:
1. Include `gooey.h` in your project directory.
2. Add `#include "gooey.h"` to your source files.
3. Link your project with the X11 libraries.

### Example Usage

```c
#include "gooey.h"

void onButtonClick() {
    printf("Button clicked!\n");
}

int main() {
    GooeyWindow win = GooeyWindow_Create("Gooey Example", 800, 600);

    GooeyButton *btn = GooeyButton_Add(&win, "Click Me", 50, 50, 100, 30, onButtonClick);

    GooeyWindow_Run(&win);
    GooeyWindow_Cleanup(&win);

    return 0;
}
```

## API Overview

### Window Management

- `GooeyWindow GooeyWindow_Create(const char *title, int width, int height)`
- `void GooeyWindow_Run(GooeyWindow *win)`
- `void GooeyWindow_Redraw(GooeyWindow *win)`
- `void GooeyWindow_Cleanup(GooeyWindow *win)`

### Widgets

- **Buttons**: `GooeyButton_Add(...)`
- **Checkboxes**: `GooeyCheckbox_Add(...)`
- **Radio Buttons**: `GooeyRadioButton_Add(...)`
- **Sliders**: `GooeySlider_Add(...)`
- **Textboxes**: `GooeyTextBox_Add(...)`
- **Dropdown Menus**: `GooeyDropdown_Add(...)`

### Layouts

- Create layouts with `GooeyLayout_Create(...)` and add widgets using `GooeyLayout_AddChild(...)`.

## Contributing

Contributions are welcome! If you have suggestions or encounter issues, feel free to open an issue or submit a pull request.

## License

Gooey is released under the MIT License.

---

Start building interactive and modern desktop applications with Gooey today!
