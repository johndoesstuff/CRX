CC = gcc
CFLAGS = -Wall -g
TARGET = crx
SRC_DIR = src
BUILD_DIR = build
SRC = $(SRC_DIR)/crx.c
OBJ = $(BUILD_DIR)/crx.o

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET) $(OBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
