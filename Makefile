C_SRC := ./
C_BACKENDS := ./backends
BUILD_DIR := ./build

SRC_FILES := $(C_BACKENDS)/glfw_backend.c \
             $(C_BACKENDS)/utils/glad.c \
			$(C_BACKENDS)/utils/themes/theme_parser.c \
						$(C_BACKENDS)/utils/debugging/debugger.c \
             $(C_BACKENDS)/utils/glfw/glfw_utils.c \
             $(C_SRC)/gooey.c \
             $(C_SRC)/main.c

OBJS_gui := $(SRC_FILES:$(C_SRC)/%.c=$(BUILD_DIR)/%.o)

CC := gcc
CFLAGS ?= -I$(C_BACKENDS)  -I/usr/local/include/freetype2

all: $(BUILD_DIR)/gui

.PHONY: $(BUILD_DIR) clrobj clean all

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/utils 

$(BUILD_DIR)/%.o: $(C_BACKENDS)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/utils/%.o: $(C_BACKENDS)/utils/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(C_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/gui: $(OBJS_gui)
	$(CC) $(OBJS_gui) -o $@  -lz `pkg-config --cflags --libs freetype2 glfw3`  -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -lcjson -g 

docs:
	doxygen DoxyFile

clrobj:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/utils/*.o

clean: clrobj
	rm -f $(BUILD_DIR)/gui
