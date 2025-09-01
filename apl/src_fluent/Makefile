# Simplified build for src_fluent
CXX ?= g++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -pedantic
LDFLAGS ?=

OBJDIR := bild

AMAIN_EXE := $(OBJDIR)/amain
BDEMO_EXE := $(OBJDIR)/bdemo

.PHONY: all clean run run_amain run_bdemo help amain bdemo

all: $(AMAIN_EXE) $(BDEMO_EXE)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(AMAIN_EXE): amain.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ amain.cpp

$(BDEMO_EXE): bdemo.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ bdemo.cpp

amain: $(AMAIN_EXE)
	@echo "=== Built amain executable ==="

bdemo: $(BDEMO_EXE)
	@echo "=== Built bdemo executable ==="

run_amain: $(AMAIN_EXE)
	@echo "=== Running Main Demo (amain) ==="
	./$(AMAIN_EXE)

run_bdemo: $(BDEMO_EXE)
	@echo "=== Running bdemo Demo ==="
	./$(BDEMO_EXE)

run: run_amain

help:
	@echo "=== Fluent Compile-Time Builder ==="

clean:
	rm -rf $(OBJDIR)
