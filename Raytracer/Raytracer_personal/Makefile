CXX = g++

CXXFLAGS = -Wall -std=c++17 -Iinclude -I/usr/local/cuda/include

TARGET = raytracer

# Find all .cpp files in the src directory
SRCS = $(wildcard src/*.cpp src/color.cpp)

# Generate object files in the main directory
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile the .cpp files into .o files in the main directory
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET) img.ppm
