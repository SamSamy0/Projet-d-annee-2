CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = carte
SRC = carte.cpp

all: 

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)