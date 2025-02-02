# all:
CXXFLAGS := -std=c++17 -I src/include -I include
OBJ := main.o component1.o component2.o

TARGET := main.exe
LDFLAGS := -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lkernel32

SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)

all: 
	$(TARGET)

$(TARGET): $(OBJ)
	g++ -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	cmd /C del /F $(OBJ) $(TARGET)

rebuild: clean run

test:
	g++ -I ./Eigen -L ./Eigen  -fopenmp -o NN NN.cpp && NN

sample:
	g++ -std=c++17 a.cpp -o 
