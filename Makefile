# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Executable name
TARGET = circuit

# Source files
SRCS = main.cpp Node.cpp Resistor.cpp

# Object files (derived from source files)
OBJS = $(SRCS:.cpp=.o)

# Header files
DEPS = Node.h Resistor.h

# Default target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compilation step for each .cpp file to .o
%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
