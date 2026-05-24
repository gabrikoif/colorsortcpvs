CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -MMD -MP -I. -g
BUILD_DIR = build
# Find all .c files in the current directory
SRCS = $(wildcard *.c)
# Transform 'main.c' into 'build/main.o'
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
# Dependency files will be 'build/main.d'
DEPS = $(OBJS:.o=.d)
TARGET = program

ifeq ($(OS), Windows_NT)
    CFLAGS += -I./PDCurses
    LIBS = -L./PDCurses/wincon -lcurses -lwinmm
    TARGET := $(TARGET).exe
    MKDIR = mkdir $(BUILD_DIR)
else
    LIBS = -lncurses
    MKDIR = mkdir -p $(BUILD_DIR)
endif

# Link the target from all object files
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

# Create the build directory
$(BUILD_DIR):
	$(MKDIR)

# Compile any .c file in the current directory into a .o file in $(BUILD_DIR)
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Include dependency files
-include $(DEPS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
ifeq ($(OS), Windows_NT)
	rmdir /s /q $(BUILD_DIR)
	del $(TARGET)
else
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
endif

.PHONY: clean run