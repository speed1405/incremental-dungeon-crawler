CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS =
STATIC_LDFLAGS = -static -static-libgcc -static-libstdc++
TARGET = dungeon_crawler
SOURCES = main.cpp game.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Windows cross-compilation settings
MINGW_CXX = x86_64-w64-mingw32-g++
WIN_TARGET = dungeon_crawler.exe
WIN_OBJECTS = $(SOURCES:.cpp=.win.o)

.PHONY: all clean run static windows windows-static clean-windows

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Static build target - creates a single standalone executable
static: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(STATIC_LDFLAGS)

%.o: %.cpp game.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

clean-windows:
	rm -f $(WIN_OBJECTS) $(WIN_TARGET)

run: $(TARGET)
	./$(TARGET)

# Windows build target - creates a Windows executable using MinGW cross-compiler
windows: $(WIN_OBJECTS)
	$(MINGW_CXX) $(CXXFLAGS) -o $(WIN_TARGET) $(WIN_OBJECTS) $(LDFLAGS)

# Windows static build - creates a standalone Windows executable
windows-static: $(WIN_OBJECTS)
	$(MINGW_CXX) $(CXXFLAGS) -o $(WIN_TARGET) $(WIN_OBJECTS) $(LDFLAGS) $(STATIC_LDFLAGS)

%.win.o: %.cpp game.h
	$(MINGW_CXX) $(CXXFLAGS) -c $< -o $@
