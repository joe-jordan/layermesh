.PHONY: default
default: check

# Compiler options
LINK_LIBRARIES=
CC=g++
CXXFLAGS=-Wall -Werror

# Build directories
build:
	mkdir -p build

build/test: build
	mkdir -p build/test

build/test/bin: build/test
	mkdir -p build/test/bin

# Source files
build/%.o: src/%.cpp include/%.hpp
	$(CC) -I./include/ -c $(CXXFLAGS) $< -o $@

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:src/%.cpp=build/%.o)

.PHONY: compile
compile: build $(OBJECTS)

# Unit tests
TEST_SOURCES=$(wildcard test/*.cpp)
TEST_OBJECTS=$(TEST_SOURCES:test/%.cpp=build/test/%.o)
DUT_OBJECTS=$(TEST_OBJECTS:build/test/test_%.o=build/%.o)
TEST_PROGRAMS=$(TEST_OBJECTS:build/test/%.o=build/test/bin/%)
TEST_LINK_LIBRARIES=-lcheck

build/test/test_%.o: test/test_%.cpp include/%.hpp
	$(CC) -I./include/ -c $(CXXFLAGS) $(TESTFLAGS) $< -o $@

build/test/bin/test_%: build/test/test_%.o build/%.o
	$(CC) -o $@ $^ $(TEST_LINK_LIBRARIES)

.PHONY: check
check: runner build/test/bin $(TEST_PROGRAMS)
	./runner

.PHONY: check-valgrind
check-valgrind: runner build/test/bin $(TEST_PROGRAMS)
	./runner --valgrind

.PHONY: clean
clean:
	rm -Rf build

# Hack to stop make deleting intermediate files:
.SECONDARY: $(OBJECTS) $(TEST_OBJECTS)

