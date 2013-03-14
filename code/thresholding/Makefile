# Compiler flags go here.
CFLAGS = -std=c99 -g -Wall -I/usr/local/include -MMD

# Linker flags go here.
LDFLAGS = -g -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui

# list of sources
SOURCES = $(shell find -name "*.cpp")

# Program executable file name.
TARGET = thresholding

# Default rule, to compile everything.
all: $(TARGET)


# -- The below lines should not need to be changed --

# Define dependencies
DEPS = $(shell find -name "*.d")

# Define all object files.
OBJECTS = $(SOURCES:.c=.o)

# rule to link the program
$(TARGET): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $^

# Compile: create object files from C source files.
%.o : %.c
	gcc $(CFLAGS) -c -o $@ $< 

# Include dependencies, if any.
-include $(DEPS)

# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) $(OBJECTS) $(DEPS)

rebuild: clean all

.PHONY: all rebuild clean

