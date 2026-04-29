# Makefile for Omnitrix Universe

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Iinclude -Wall -std=c++17

# Libraries (SFML)
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Source and Object files
SRCS = $(filter-out $(SRC_DIR)/launcher.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Target executable
TARGET = $(BIN_DIR)/OmnitrixUniverse

# Release flags
RELEASE_FLAGS = -O3 -DNDEBUG
DEBUG_FLAGS = -g -O0

# Default rule (Debug)
all: CXXFLAGS += $(DEBUG_FLAGS)
all: dirs $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

# Create necessary directories
dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# Release target
release: CXXFLAGS += $(RELEASE_FLAGS)
release: clean dirs $(TARGET)

# Compile .cpp files to .o files in build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | dirs
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Clean up build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the game
run: all
	cd $(BIN_DIR) && ./$(notdir $(TARGET))
