CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS =
TARGET = dungeon_crawler
SOURCES = main.cpp game.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean run static

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Static build target - creates a single standalone executable
static: LDFLAGS += -static -static-libgcc -static-libstdc++
static: $(TARGET)

%.o: %.cpp game.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
