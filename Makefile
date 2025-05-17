# Compiler
CC = g++ -std=c++17

# Compiler flags
CFLAGS = -Wall -g

# Directory structure
SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main # output executable

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)   # All .cpp files in /src
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)  # Map .cpp to .o in /build


# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ `sdl2-config --cflags --libs`

# Compile the source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the generated files
clean:
	rm -rf $(BUILD_DIR)


.PHONY: all clean help
