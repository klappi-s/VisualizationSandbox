# Makefile for dog.cpp

CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
LDFLAGS ?=

SRCFILE := dog.cpp
OBJFILE := dog.o
EXE := dog

.PHONY: all clean run

all: $(EXE)

$(OBJFILE): $(SRCFILE)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJFILE)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(OBJFILE) $(EXE)