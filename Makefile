# Makefile 0.1 2025-05-17

# Directories
SRC_DIR		:= src
ASM_DIR		:= $(SRC_DIR)/asm
LIB_DIR		:= lib
RSC_DIR		:= rsc
BUILD_DIR	:= build
BIN_DIR		:= bin

# Compilers and flags
CC			:= gcc
XC			:= g++
CFLAGS	:= -Wall -Wextra -g -I$(LIB_DIR)
AS			:= nasm
ASFLAGS	:= -f elf64

# Executable name
TARGET	:= $(BIN_DIR)/roguex86

# Source code files
C_SRCS		:= $(wildcard $(SRC_DIR)/*.c)
CPP_SRCS	:= $(wildcard $(SRC_DIR)/*.cpp)
ASM_SRCS	:= $(wildcard $(ASM_DIR)/*.asm)

# Object files
C_OBJS		:= $(patsubs $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRCS))
CPP_OBJS	:= $(patsubs $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_SRCS))
ASM_OBJS	:= $(patsubs $(ASM_DIR)/%.asm, $(BUILD_DIR)/%.o, $(ASM_SRCS))

# Rules
.PHONY all clean

# Default target
all: $(TARGET)

# Create object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(XC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(ASM_DIR)/%.asm
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# Create executable
$(TARGET): $(C_OBJS) $(CPP_OBJS) $(ASM_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Delete temporary files and directories created during compilation
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
