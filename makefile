CC = gcc
CFLAGS = -Wall -Wextra
SANITIZER_FLAGS := -fsanitize=address,undefined \
                   -fno-omit-frame-pointer \
                   -g
CVERSION = -std=c23

CFLAGS += $(CVERSION)
CFLAGS += $(SANITIZER_FLAGS)
LDFLAGS += $(SANITIZER_FLAGS)

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

TARGET := $(BIN_DIR)/gofish

UNITY_SRC := $(SRC_DIR)/unity.c
UNITY_OBJ := $(BUILD_DIR)/unity.o
DEP_FILES := $(UNITY_OBJ:.o=.d)

.PHONY: all clean rebuild run stats format tidy

all: $(TARGET)

# Link the single unity-build object.
$(TARGET): $(UNITY_OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $(UNITY_OBJ) -o $@

# Compile every implementation as one translation unit.
$(UNITY_OBJ): $(UNITY_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

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
	clang-tidy $(UNITY_SRC) -- $(CVERSION) -Isrc

-include $(DEP_FILES)
