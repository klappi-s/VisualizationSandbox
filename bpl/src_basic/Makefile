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

# Define executables
AMAIN_EXE := $(OBJDIR)/amain
TEST_SIMPLIFIED_EXE := $(OBJDIR)/test_simplified_registry
AMAIN_SIMPLIFIED_EXE := $(OBJDIR)/amain_simplified

.PHONY: all clean run run_amain help test_simplified amain_simplified

# Default target builds all executables
all: $(AMAIN_EXE) $(TEST_SIMPLIFIED_EXE) $(AMAIN_SIMPLIFIED_EXE)

# Create build directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Build amain executable
$(AMAIN_EXE): amain.cpp VisBase.h VisBase.hpp VisBase.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ amain.cpp VisBase.cpp

# Build test_simplified_registry executable
$(TEST_SIMPLIFIED_EXE): test_simplified_registry.cpp VisBase.h VisBase.hpp VisBase.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test_simplified_registry.cpp VisBase.cpp

# Build amain_simplified executable
$(AMAIN_SIMPLIFIED_EXE): amain_simplified.cpp VisBase.h VisBase.hpp VisBase.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ amain_simplified.cpp VisBase.cpp

# Individual build targets
amain: $(AMAIN_EXE)
	@echo "=== Built amain executable ==="

# Build and run test_simplified_registry
test_simplified: $(TEST_SIMPLIFIED_EXE)
	@echo "=== Running Simplified Registry Test ==="
	@./$(TEST_SIMPLIFIED_EXE)

# Build and run amain_simplified
amain_simplified: $(AMAIN_SIMPLIFIED_EXE)
	@echo "=== Running Comprehensive Simplified Demo ==="
	@./$(AMAIN_SIMPLIFIED_EXE)

# Run targets# Fix the run_amain target
run_amain: $(AMAIN_EXE)
	@echo "=== Running Main Demo ==="
	./$(AMAIN_EXE)

	
# Run targets# Fix the run_amain target
run: $(AMAIN_EXE)
	@echo "=== Running Main Demo ==="
	./$(AMAIN_EXE)


# Help target
help:
	@echo "=== Simplified Type-Constrained Registry Build System ==="
	@echo "Available targets:"
	@echo "  all              - Build all executables (amain, test_simplified_registry, amain_simplified)"
	@echo "  amain            - Build only amain executable"
	@echo "  test_simplified  - Build and run simplified registry test"
	@echo "  amain_simplified - Build and run comprehensive simplified demo"
	@echo "  run              - Run amain (default run target)"
	@echo "  run_amain        - Run amain executable"
	@echo "  clean            - Remove build directory"
	@echo "  help             - Show this help message"
	@echo ""
	@echo "Key Features:"
	@echo "  - Type-constrained registry based on user-defined T and Dim"
		@echo "  - Automatic validation of field and particle types"
	@echo "  - Clear error messages for dimension mismatches"
	@echo "  - Support for scalar fields, vector fields, and particles"

clean:
	rm -rf $(OBJDIR)
