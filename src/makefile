CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

SFML_PREFIX := $(shell brew --prefix sfml 2>/dev/null || brew --prefix)
TGUI_PREFIX := $(shell brew --prefix tgui 2>/dev/null || brew --prefix)

INCLUDES := -I$(SFML_PREFIX)/include -I$(TGUI_PREFIX)/include
LDFLAGS  := -L$(SFML_PREFIX)/lib -L$(TGUI_PREFIX)/lib -Wl,-rpath,$(SFML_PREFIX)/lib -Wl,-rpath,$(TGUI_PREFIX)/lib

LDLIBS   := -ltgui -lsfml-graphics -lsfml-window -lsfml-system

TARGET := test

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
