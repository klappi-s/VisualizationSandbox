# Makefile to build vis.cpp and classes.cpp

CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
LDFLAGS ?=

# gather sources but exclude the old placeholder classes.cpp to avoid duplicate/unused objects
SRCS := $(filter-out classes.cpp,$(wildcard *.cpp))
OBJDIR := bild
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SRCS)))
EXE := $(OBJDIR)/vis

.PHONY: all clean run compile

all: $(EXE)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

run: $(EXE)
	./$(EXE)

compile: $(OBJDIR)/amain.o

clean:
	rm -rf $(OBJDIR)
