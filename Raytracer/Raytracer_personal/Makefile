CXX = g++

CXXFLAGS = -Wall -std=c++17 -Iinclude

TARGET = raytracer

BUILD_DIR = build

# Find all .cpp files in the src directory
SRCS = $(wildcard src/*.cpp)

# Generate object files in the build directory
OBJS = $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $^
	cp $(BUILD_DIR)/$@ .

# Compile the .cpp files into .o files in the build directory
$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
