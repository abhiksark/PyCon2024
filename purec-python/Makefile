# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -fPIC

# Automatically detect Python path and include directory
PYTHON_PATH := $(shell which python)
PYTHON_INCLUDE := $(shell python -c "import sysconfig; print(sysconfig.get_path('include'))")

# Target shared object
TARGET = example.so

# Source file
SRC = example.c

# Default target
all: $(TARGET)

# Rule to build the shared object
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ $< -I$(PYTHON_INCLUDE)

# Clean rule
clean:
	rm -f $(TARGET)

# Print Python paths (for debugging)
print-python-paths:
	@echo "Python executable: $(PYTHON_PATH)"
	@echo "Python include directory: $(PYTHON_INCLUDE)"

# Phony targets
.PHONY: all clean print-python-paths