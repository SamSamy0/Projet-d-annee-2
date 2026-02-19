CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -I/opt/homebrew/include -I.. -I../..
LDFLAGS = -L/opt/homebrew/lib \
          -lsfml-graphics \
          -lsfml-window \
          -lsfml-system \
          -lsfml-network \
          -ltgui

TARGET = app

SRC = main.cpp \
      displayWindow.cpp \
      Identifier.cpp \
      ../server/clientnetwork.cpp \
      ../server/handler.cpp \
      ../project/project.cpp \
      ../project/map.cpp \
      ../project/layer.cpp \
      ../project/tool.cpp \
      ../project/toolbar.cpp \
      ../project/move.cpp \
      ../project/zoom.cpp

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

re: clean all
