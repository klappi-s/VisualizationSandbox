# Simplified Type-Constrained Registry Build System
# 
# This Makefile builds the simplified BPL registry system that enforces
# type constraints based on user-defined T and Dim parameters.
# 
# Key Features:
# - Single type configuration (T, Dim) defined in user code
# - Automatic type validation with clear error messages
# - Support for scalar fields, vector fields, and particles
# - Simple retrieval interface by ID
#
# Usage: make [target]
# Run 'make help' for available targets

# CXX ?= g++
CXX ?= clang++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -pedantic #-ftime-report
LDFLAGS ?=

OBJDIR := bild

# Define executables (only amain and bdemo1)
AMAIN_EXE := $(OBJDIR)/amain
BDEMO1_EXE := $(OBJDIR)/bdemo1

.PHONY: all clean run run_amain run_bdemo1 help amain bdemo1

# Default target builds both executables
all: $(AMAIN_EXE) $(BDEMO1_EXE)

# Create build directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Build amain executable
$(AMAIN_EXE): amain.cpp VisBase.h VisBase.hpp VisBase.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ amain.cpp VisBase.cpp

# Build bdemo1 example
$(BDEMO1_EXE): bdemo1.cpp VisBase.h VisBase.hpp VisBase.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ bdemo1.cpp VisBase.cpp

# Individual build targets
amain: $(AMAIN_EXE)
	@echo "=== Built amain executable ==="

bdemo1: $(BDEMO1_EXE)
	@echo "=== Built bdemo1 executable ==="

# Run targets
run_amain: $(AMAIN_EXE)
	@echo "=== Running Main Demo (amain) ==="
	./$(AMAIN_EXE)

run_bdemo1: $(BDEMO1_EXE)
	@echo "=== Running bdemo1 Demo ==="
	./$(BDEMO1_EXE)

# Default run target
run: run_amain

# Help target
help:
	@echo "=== Simplified Type-Constrained Registry Build System ==="
	@echo "Available targets:"
	@echo "  all            - Build all executables (amain, bdemo1)"
	@echo "  amain          - Build only amain executable"
	@echo "  bdemo1         - Build demo executable"
	@echo "  run            - Run amain (default run target)"
	@echo "  run_amain      - Run amain executable"
	@echo "  run_bdemo1     - Run bdemo1 executable"
	@echo "  clean          - Remove build directory"
	@echo "  help           - Show this help message"
	@echo ""
	@echo "Key Features:"
	@echo "  - Type-constrained registry based on user-defined T and Dim"
	@echo "  - Automatic validation of field and particle types"
	@echo "  - Clear error messages for dimension mismatches"
	@echo "  - Support for scalar fields, vector fields, and particles"

clean:
	rm -rf $(OBJDIR)
