# Compiler flags go here.
CFLAGS = -g -Wall -MMD -I/usr/local/include/opencv

# Linker flags go here.
LDFLAGS = -g -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui

# list of sources
SOURCES = $(shell find  -name "*.cpp") ../thresholding/thresholding.cpp ../region/region.cpp ../moment/moment.cpp

# Program executable file name.
TARGET = trafficsignapp

# Default rule, to compile everything.
all: $(TARGET)


# -- The below lines should not need to be changed --

# Define dependencies
DEPS = $(shell find -name "*.d")

# Define all object files.
OBJECTS = $(SOURCES:.cpp=.o)

# rule to link the program
$(TARGET): $(OBJECTS)
	g++ -o $@ $^ $(LDFLAGS) 

# Compile: create object files from C source files.
%.o : %.cpp
	g++ $(CFLAGS) -c -o $@ $< 

# Include dependencies, if any.
-include $(DEPS)

#Print variables
echo:
	echo "TARGET: $(TARGET) OBJECTS: $(OBJECTS) DEPS: $(DEPS)"

# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) $(OBJECTS) $(DEPS)

rebuild: clean all

.PHONY: all rebuild clean echo

