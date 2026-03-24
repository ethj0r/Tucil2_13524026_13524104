CXX      := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -I.
BINDIR   := ../bin
TARGET   := $(BINDIR)/voxelizer

UNAME_S := $(shell uname -s 2>/dev/null || echo Windows)
ifeq ($(UNAME_S),Darwin)
    #macOS
    RAYLIB_ISYSTEM := $(patsubst -I%,-isystem %,$(shell pkg-config --cflags raylib))
    CXXFLAGS += $(RAYLIB_ISYSTEM)
    LDFLAGS := $(shell pkg-config --libs raylib) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else ifeq ($(UNAME_S),Linux)
    #Linux
    RAYLIB_ISYSTEM := $(patsubst -I%,-isystem %,$(shell pkg-config --cflags raylib))
    CXXFLAGS += $(RAYLIB_ISYSTEM)
    LDFLAGS:= $(shell pkg-config --libs raylib) -lGL -lm -lpthread -ldl -lrt -lX11
else
    #MSYS2/MinGW
    CXX :=g++
    RAYLIB_ISYSTEM := $(patsubst -I%,-isystem %,$(shell pkg-config --cflags raylib 2>/dev/null))
    CXXFLAGS+=$(RAYLIB_ISYSTEM)
    LDFLAGS:=$(shell pkg-config --libs raylib 2>/dev/null || echo -lraylib) -lopengl32 -lgdi32 -lwinmm
endif

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
