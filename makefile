CC = gcc
CFLAGS = -Wall -Wextra

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

TARGET := $(BIN_DIR)/gofish

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJS_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(OBJS_FILES:.o=.d)

.PHONY: all clean

all: $(TARGET)

# builds executable by building all object files
$(TARGET): $(OBJS_FILES) | $(BIN_DIR)
	$(CC) $(OBJS_FILES) -o $@

# builds .o from .c
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# apparently makes dir if not found
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all

-include $(DEP_FILES)
