# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include

# Source files
SRCS = $(wildcard src/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = hospital_management

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	del /Q $(OBJS) $(TARGET).exe

# Run the program
run: $(TARGET)
	$(TARGET)

.PHONY: all clean run