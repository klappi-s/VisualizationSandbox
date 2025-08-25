# Makefile to build multiple executables

# CXX ?= g++
CXX ?= clang++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -pedantic #-ftime-report
LDFLAGS ?=

OBJDIR := bild

# Define executables
AMAIN_EXE := $(OBJDIR)/amain
TEST_PRINT_EXE := $(OBJDIR)/test_print_simple
TEST_RETRIEVAL_EXE := $(OBJDIR)/test_field_particle_retrieval

.PHONY: all clean run run_amain run_test_print run_test_retrieval test amain help

# Default target builds all executables
all: $(AMAIN_EXE) $(TEST_PRINT_EXE) $(TEST_RETRIEVAL_EXE)

# Create build directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Build amain executable
$(AMAIN_EXE): amain.cpp VisBase.h VisBase.hpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

# Build test_print_simple executable  
$(TEST_PRINT_EXE): test_print_simple.cpp VisBase.h VisBase.hpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

# Build test_field_particle_retrieval executable
$(TEST_RETRIEVAL_EXE): test_field_particle_retrieval.cpp VisBase.h VisBase.hpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

# Individual build targets
amain: $(AMAIN_EXE)

test: $(TEST_PRINT_EXE)

test_retrieval: $(TEST_RETRIEVAL_EXE)

# Run targets
run: run_amain

run_amain: $(AMAIN_EXE)
	@echo "=== Running amain ==="
	./$(AMAIN_EXE)

run_test_print: $(TEST_PRINT_EXE)
	@echo "=== Running test_print_simple ==="
	./$(TEST_PRINT_EXE)

run_test_retrieval: $(TEST_RETRIEVAL_EXE)
	@echo "=== Running test_field_particle_retrieval ==="
	./$(TEST_RETRIEVAL_EXE)

# Help target
help:
	@echo "Available targets:"
	@echo "  all           - Build all executables (amain, test_print_simple, test_field_particle_retrieval)"
	@echo "  amain         - Build only amain executable"
	@echo "  test          - Build only test_print_simple executable"
	@echo "  test_retrieval - Build only test_field_particle_retrieval executable"
	@echo "  run           - Run amain (default)"
	@echo "  run_amain     - Run amain executable"
	@echo "  run_test_print - Run test_print_simple executable"
	@echo "  run_test_retrieval - Run test_field_particle_retrieval executable"
	@echo "  clean         - Remove build directory and all executables"
	@echo "  help          - Show this help message"
	@echo "  test          - Build only test_print_simple executable"
	@echo "  run           - Run amain (default run target)"
	@echo "  run_amain     - Run amain executable"
	@echo "  run_test_print - Run test_print_simple executable"
	@echo "  clean         - Remove build directory"
	@echo "  help          - Show this help message"

# report:
# 	clang++ -Xclang -ast-dump -fsyntax-only amain.cpp

clean:
	rm -rf $(OBJDIR)
