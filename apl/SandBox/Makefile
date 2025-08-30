# BPL one Library - Comprehensive Makefile
# Builds all one interface components with proper dependency management

# Compiler settings
CXX ?= clang++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -pedantic
LDFLAGS ?=

# Directories
OBJDIR := bild
SRCDIR := .

# # Core library files
# VIS_LIB_HEADERS := VisBase.h bpl.h field.h particle.h VisBase.hpp
# VIS_LIB_SOURCES := VisBase.cpp
# VIS_LIB_OBJECTS := $(OBJDIR)/VisBase.o

# Define all executables
AMAIN_EXE := $(OBJDIR)/amain
DEMO_ONE := $(OBJDIR)/demo_one
DEMO_TWO := $(OBJDIR)/demo_two
DEMO_3 := $(OBJDIR)/demo_3

# Define all targets for easy reference
ALL_EXECUTABLES := $(AMAIN_EXE) $(DEMO_ONE) $(DEMO_TWO)




# Phony targets
.PHONY: all clean run test demo help lib install one amain \
        run_amain run_one run_two run_all check \
        compile_lib

# Default target builds everything
all: lib $(ALL_EXECUTABLES)
	@echo "✅ All BPL one components built successfully!"
	@echo "   📁 Build directory: $(OBJDIR)/"
	@echo "   🎯 Available executables:"
	@ls -la $(OBJDIR)/ | grep '^-rwx' | awk '{print "      - " $$9 " (" int($$5/1024) " KB)"}'

# Create build directory
$(OBJDIR):
	@echo "📁 Creating build directory..."
	@mkdir -p $(OBJDIR)




# Build amain executable (original demo with one backend)
$(AMAIN_EXE): amain.cpp $(VIS_LIB_OBJECTS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ amain.cpp 

$(DEMO_ONE): bdemo_1.cpp $(VIS_LIB_OBJECTS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ bdemo_1.cpp 

$(DEMO_TWO): bdemo_2.cpp $(VIS_LIB_OBJECTS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ bdemo_2.cpp

$(DEMO_3): bdemo_3.cpp $(VIS_LIB_OBJECTS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ bdemo_3.cpp







# Individual build targets
amain: $(AMAIN_EXE)
	@echo "✅ amain built successfully"

one: $(DEMO_ONE)
	@echo "✅ one demo built successfully"

two: $(DEMO_TWO)
	@echo "✅ demo two built successfully"

three: $(DEMO_3)
	@echo "✅ demo two built successfully"





run: run_amain

run_all: $(ALL_EXECUTABLES)
	@echo "🚀 Running all demonstrations..."
	@echo ""
	@echo "=== 0. Original amain (one backend) ==="
	@./$(AMAIN_EXE)
	@echo ""
	@echo "=== demo ONE 1 ==="
	@./$(DEMO_ONE)
	@echo ""
	@echo "=== demo TWO 2 ==="
	@./$(DEMO_TWO)
	@echo ""
	@echo "=== demo 3 ==="
	@./$(DEMO_3)

run_amain: $(AMAIN_EXE)
	@./$(AMAIN_EXE)

run_one: $(DEMO_ONE)
	@./$(DEMO_ONE)

run_two: $(DEMO_TWO)
	@./$(DEMO_TWO)

run3: $(DEMO_3)
	@./$(DEMO_3)





# Development helpers
rebuild: clean all


# Documentation
help:
	@echo "================================================================"
	@echo "BPL one LIBRARY - MAKEFILE HELP"  
	@echo "================================================================"
	@echo ""
	@echo "🎯 Main Targets:"
	@echo "  all           - Build all components (default)"
	@echo "  amain         - Build / Compile only"
	@echo "  one           - Build / Compile only"
	@echo "  dtwo          - Build / Compile only"
	@echo ""
	@echo "🚀 Run Targets:"
	@echo "  run           - Run one interface demo (default)"
	@echo "  run_all       - Run all three demonstrations"
	@echo "  run_amain     - Run original amain"
	@echo "  run_one  	   - Run one interface demo"
	@echo "  run_two      - Run user experience comparison"
	@echo ""
	@echo "🔍 Testing:"
	@echo "  test          - Run one interface tests"
	@echo "  check         - Comprehensive test of all components"
	@echo ""
	@echo "🛠️  Development:"
	@echo "  clean         - Remove build directory"
	@echo "  rebuild       - Clean and build all"
	@echo ""
	@echo ""
	@echo "📚 Example Usage:"
	@echo "  make all       # Build everything"
	@echo "  make run_all   # See all demos"
	@echo "================================================================"

# Clean up
clean:
	@echo "🧹 Cleaning build directory..."
	@rm -rf $(OBJDIR)
	@echo "✅ Clean completed"

# Dependency declarations (for make to understand file dependencies)
# $(AMAIN_EXE): amain.cpp $(VIS_LIB_HEADERS)
# $(DEMO_ONE): bdemo_one.cpp $(VIS_LIB_HEADERS)  
# $(DEMO_TWO): bdemo_two.cpp $(VIS_LIB_HEADERS)
# $(VIS_LIB_OBJECTS): $(VIS_LIB_HEADERS)
