# Detect OS
UNAME_S := $(shell uname -s)

# Compiler and flags
ifeq ($(UNAME_S),Darwin)
    CC = clang
else
    CC = gcc
endif
CFLAGS = -Wall -Wextra -fPIC

# Automatically detect Python path and include directory
PYTHON_PATH := $(shell which python)
PYTHON_INCLUDE := $(shell python -c "import sysconfig; print(sysconfig.get_path('include'))")

# Target shared object (with platform-specific extension)
ifeq ($(UNAME_S),Darwin)
    TARGET = example.dylib
    LDFLAGS = -dynamiclib
else
    TARGET = example.so
    LDFLAGS = -shared
endif

# Source file
SRC = example.c

# Default target
all: $(TARGET)

# Rule to build the shared object
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -I$(PYTHON_INCLUDE)

# Clean rule
clean:
	rm -f $(TARGET)

# Print debug info
print-debug-info:
	@echo "OS: $(UNAME_S)"
	@echo "Compiler: $(CC)"
	@echo "Python executable: $(PYTHON_PATH)"
	@echo "Python include directory: $(PYTHON_INCLUDE)"
	@echo "Target: $(TARGET)"

# Phony targets
.PHONY: all clean print-debug-info