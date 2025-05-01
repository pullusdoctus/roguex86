# Makefile 0.2 2025-05-17

# Directories and target
# -----------------------------------------
# Directories
SRC_DIR		:= src
ASM_DIR		:= $(SRC_DIR)/asm
LIB_DIR		:= lib
RSC_DIR		:= rsc
BUILD_DIR	:= build
BIN_DIR		:= bin
# Executable name
TARGET		:= $(BIN_DIR)/roguex86
# -----------------------------------------

# Compilers and flags
# -----------------------------------------
CC				:= gcc
XC				:= g++
SDLFLAGS	:= $(shell sdl2-config --cflags)
CFLAGS		:= -Wall -Wextra -g -I$(LIB_DIR) $(SDLFLAGS)
XCFLAGS		:= $(CFLAGS)
AS				:= nasm
ASFLAGS		:= -f elf64
SDL_LIBS		:= $(shell sdl2-config --libs) -lSDL2_ttf
# -----------------------------------------

# File variables
# -----------------------------------------
# Source code files
C_SRCS		:= $(wildcard $(SRC_DIR)/*.c)
CPP_SRCS	:= $(wildcard $(SRC_DIR)/*.cpp)
ASM_SRCS	:= $(wildcard $(ASM_DIR)/*.asm)
# Object files
C_OBJS		:= $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRCS))
CPP_OBJS	:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_SRCS))
ASM_OBJS	:= $(patsubst $(ASM_DIR)/%.asm, $(BUILD_DIR)/%.o, $(ASM_SRCS))
OBJS			:= $(C_OBJS) $(CPP_OBJS) $(ASM_OBJS)
# -----------------------------------------

# Rules
.PHONY: all run clean

# Default target
# -----------------------------------------
all: $(TARGET)

# Create executable
$(TARGET): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(XC) $(CFLAGS) -o $@ $^ $(SDL_LIBS)
# -----------------------------------------

# Run the executable
# -----------------------------------------
run: $(TARGET)
	./$(TARGET)
# -----------------------------------------

# Create object files
# -----------------------------------------
# Create build/ directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile C source code
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source code
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(XC) $(XCFLAGS) -c $< -o $@

# Compile ASM source code
$(BUILD_DIR)/%.o: $(ASM_DIR)/%.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@
# -----------------------------------------

# -----------------------------------------
# Delete temporary files and directories created during compilation
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
# -----------------------------------------
