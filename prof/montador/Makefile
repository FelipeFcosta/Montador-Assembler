# Define the compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g

# Source files
SOURCES = Source.cpp montador.cpp Assembler.cpp Instruction.cpp Error.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = montador

# Default target (first target in the file)
all: $(EXECUTABLE)

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to build object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove object files and the executable
clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

.PHONY: all clean
