C_SRC := ./
C_BACKENDS := ./backends
BUILD_DIR := ./build

SRC_FILES := $(C_BACKENDS)/x11_backend.c \
$(C_BACKENDS)/wayland_backend.c \
$(C_BACKENDS)/glfw_backend.c \
$(C_BACKENDS)/xdg-shell-protocol.c \
$(C_BACKENDS)/glad.c \
 $(C_SRC)/tiny.c \
             $(C_SRC)/main.c 

OBJS_gui := $(patsubst $(C_SRC)/%.c, $(BUILD_DIR)/%.o, \
            $(patsubst $(C_BACKENDS)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES)))

CC := gcc
CFLAGS := -I$(C_BACKENDS)

all: $(BUILD_DIR)/gui

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(C_BACKENDS)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(C_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/gui: $(BUILD_DIR) $(OBJS_gui)
	$(CC) $(OBJS_gui) -o $@ -lwayland-client -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm 

clrobj:
	rm -f $(BUILD_DIR)/*.o

.PHONY: clean
clean: clrobj
	rm -f $(BUILD_DIR)/gui
