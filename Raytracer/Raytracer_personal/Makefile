CXX = g++

CXXFLAGS = -Wall -std=c++17 -Iinclude -Iexternal -ggdb

TARGET = raytracer

# Find all .cpp files in the src directory
SRCS = $(wildcard src/*.cpp src/color.cpp)

# Generate object files in the main directory
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	@echo "Compiling and Linking Executable"
	@$(CXX) $(CXXFLAGS) -o $@ $^

# Compile the .cpp files into .o files in the main directory
%.o: %.cpp
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET) img.ppm
