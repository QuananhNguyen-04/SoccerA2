# Compiler and Flags
CXX := g++
NVCC := nvcc
CXXFLAGS := -std=c++17 -I src64/include -I include
NVCCFLAGS := -std=c++17 -I src64/include -I include
LDFLAGS := -L src64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf 

# Source Files
CPP_SRC := $(wildcard *.cpp)
CU_SRC := $(wildcard *.cu)
OBJ := $(CPP_SRC:.cpp=.o) # $(CU_SRC:.cu=.o)

# Target Executable
TARGET := main.exe

# Build Target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile C++ Object Files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile CUDA Object Files
%.o: %.cu
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

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
