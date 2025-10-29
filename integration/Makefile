# Makefile for OpenVCAD Integration Test

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS =

# Python support (optional)
PYTHON_SUPPORT ?= 0

ifeq ($(PYTHON_SUPPORT), 1)
    PYTHON_CONFIG = python3-config
    PYTHON_CFLAGS = $(shell $(PYTHON_CONFIG) --includes)
    PYTHON_LDFLAGS = $(shell $(PYTHON_CONFIG) --ldflags --embed)
    CXXFLAGS += -DVCAD_PYTHON_SUPPORT $(PYTHON_CFLAGS)
    LDFLAGS += $(PYTHON_LDFLAGS)
    SOURCES = VCAD.cpp VCADPythonBridge.cpp test_vcad_integration.cpp
else
    SOURCES = VCAD.cpp test_vcad_integration.cpp
endif

OBJECTS = $(SOURCES:.cpp=.o)
TARGET = test_vcad_integration

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

# Dependencies
VCAD.o: VCAD.cpp VCAD.hpp
test_vcad_integration.o: test_vcad_integration.cpp VCAD.hpp

ifeq ($(PYTHON_SUPPORT), 1)
VCADPythonBridge.o: VCADPythonBridge.cpp VCADPythonBridge.hpp VCAD.hpp
endif

# Help target
help:
	@echo "OpenVCAD Integration Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all              - Build the test program (default)"
	@echo "  test             - Build and run tests"
	@echo "  clean            - Remove build artifacts"
	@echo "  help             - Show this help message"
	@echo ""
	@echo "Options:"
	@echo "  PYTHON_SUPPORT=1 - Enable Python bridge support"
	@echo ""
	@echo "Examples:"
	@echo "  make                    # Build without Python support"
	@echo "  make PYTHON_SUPPORT=1   # Build with Python support"
	@echo "  make test               # Build and run tests"
