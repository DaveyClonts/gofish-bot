CC = gcc
CFLAGS = -Wall -Wextra -O0 -g
SANITIZER_FLAGS := -fsanitize=address,undefined \
                   -fno-omit-frame-pointer \
CVERSION = -std=c23

CFLAGS += $(SANITIZER_FLAGS)
LDFLAGS += $(SANITIZER_FLAGS)

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

TARGET := $(BIN_DIR)/gofish

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJS_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(OBJS_FILES:.o=.d)

.PHONY: all clean rebuild run debug stats format tidy

all: $(TARGET)

# builds executable by building all object files
$(TARGET): $(OBJS_FILES) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $(OBJS_FILES) -o $@

# builds .o from .c
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CVERSION) -MMD -MP -c $< -o $@

# apparently makes dir if not found
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

stats:
	sh scripts/update_readme_stats.sh

format:
	clang-format -i $(SRC_DIR)/*.c $(SRC_DIR)/*.h

tidy:
	clang-tidy $(SRC_DIR)/*.c -- $(CVERSION) -Isrc

debug: $(TARGET)
	gdb ./$(TARGET)

-include $(DEP_FILES)
