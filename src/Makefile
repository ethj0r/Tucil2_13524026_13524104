CXX      := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -I.
BINDIR   := ../bin
TARGET   := $(BINDIR)/voxelizer

SRCS := $(shell find . -name '*.cpp')
OBJS := $(patsubst ./%.cpp,$(BINDIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/%.o: ./%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BINDIR)/*

.PHONY: all run clean
