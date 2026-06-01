CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I. -g
SRCS = $(wildcard *.c)
TARGET = program

ifeq ($(OS), Windows_NT)
    CFLAGS += -I./PDCurses
    LIBS = -L./PDCurses/wincon -lcurses -lwinmm
    TARGET := $(TARGET).exe
else
    LIBS = -lncurses
endif

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LIBS)

# Include dependency files
-include $(DEPS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
ifeq ($(OS), Windows_NT)
	del $(TARGET)
else
	rm -f $(TARGET)
endif

.PHONY: run clean
