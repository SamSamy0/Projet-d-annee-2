CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

SFML_PREFIX := $(shell brew --prefix sfml 2>/dev/null || brew --prefix)

INCLUDES := -I$(SFML_PREFIX)/include
LDFLAGS  := -L$(SFML_PREFIX)/lib
LDLIBS   := -lsfml-graphics -lsfml-window -lsfml-system

TARGET := test

# 🔥 AJOUT DE move.cpp ICI
SRC := test.cpp map.cpp zoom.cpp move.cpp layer.cpp project.cpp tool.cpp toolbar.cpp

OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
