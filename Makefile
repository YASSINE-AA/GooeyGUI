C_SRC := ./
C_BACKENDS := ./backends
BUILD_DIR := ./build

SRC_FILES := $(C_BACKENDS)/x11_backend.c \
             $(C_BACKENDS)/wayland_backend.c \
             $(C_BACKENDS)/glfw_backend.c \
             $(C_BACKENDS)/glad.c \
             $(C_BACKENDS)/utils/glfw_utils.c \
             $(C_BACKENDS)/xdg-shell-protocol.c \
             $(C_SRC)/tiny.c \
             $(C_SRC)/main.c

OBJS_gui := $(SRC_FILES:$(C_SRC)/%.c=$(BUILD_DIR)/%.o)

CC := gcc
CFLAGS ?= -I$(C_BACKENDS)

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
	$(CC) $(OBJS_gui) -o $@ -lwayland-client -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm

clrobj:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/utils/*.o

clean: clrobj
	rm -f $(BUILD_DIR)/gui
