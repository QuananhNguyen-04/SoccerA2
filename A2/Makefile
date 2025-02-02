# Compiler and Flags
CXX := g++
CXXFLAGS := -std=c++17 -I src64/include -I include
LDFLAGS := -L src64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

# Source Files
SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)

# Target Executable
TARGET := main.exe

# Build Target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile Object Files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run Executable
run: $(TARGET)
	./$(TARGET)

# Clean Files (Cross-platform)
clean:
	rm -f $(OBJ) $(TARGET) 2>/dev/null || cmd /C del /F /Q $(OBJ) $(TARGET)

# Rebuild
rebuild: clean all run

# Test
test:
	$(CXX) -I ./Eigen -L ./Eigen -fopenmp -o NN NN.cpp && ./NN

# Sample Build
sample:
	$(CXX) -std=c++17 a.cpp -o a.out && ./a.out
