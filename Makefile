C_SRC := ./
C_BACKENDS := ./backends
BUILD_DIR := ./build

SRC_FILES :=  $(C_SRC)/main.c \
			  $(C_BACKENDS)/x11_backend.c

OBJS_gui := $(patsubst $(C_BACKENDS)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

CC := gcc
CFLAGS := -I$(C_BACKENDS) -I$(C_BACKENDS)/unity -g

all: $(BUILD_DIR)/gui

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(C_BACKENDS)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -lX11

$(BUILD_DIR)/gui: $(BUILD_DIR) $(OBJS_gui)
	$(CC) $(OBJS_gui) -o $@ -lX11

clrobj:
	rm -f $(BUILD_DIR)/*.o

.PHONY: clean
clean: clrobj
	rm -f $(BUILD_DIR)/gui
