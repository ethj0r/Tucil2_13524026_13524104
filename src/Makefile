CXX      := clang++
RAYLIB_ISYSTEM := $(patsubst -I%,-isystem %,$(shell pkg-config --cflags raylib))
CXXFLAGS := -std=c++17 -Wall -Wextra -I. $(RAYLIB_ISYSTEM)
LDFLAGS  := $(shell pkg-config --libs raylib) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
BINDIR   := ../bin
TARGET   := $(BINDIR)/voxelizer

SRCS := $(shell find . -name '*.cpp')
OBJS := $(patsubst ./%.cpp,$(BINDIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/%.o: ./%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BINDIR)/*

.PHONY: all run clean
