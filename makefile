.PHONY: default
default: check

# Compiler options
LINK_LIBRARIES=
CC=g++ -std=c++11
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

# Unit tests (written out manually because of interdependecy)
TEST_SOURCES=test/test_gvec.cpp test/test_hull.cpp
TEST_OBJECTS=build/test/test_gvec.o build/test/test_hull.cpp

TEST_NAMES=gvec hull mesh
TEST_PROGRAMS=$(TEST_NAMES:%=build/test/bin/test_%)
TEST_LINK_LIBRARIES=-lcheck -lrt -lpthread -lsubunit
TEST_RUNTIME_DEPS=/usr/bin/admesh

build/test/test_gvec.o: test/test_gvec.cpp include/gvec.hpp
	$(CC) -I./include/ -c $(CXXFLAGS) $(TESTFLAGS) $< -o $@

build/test/bin/test_gvec: build/test/test_gvec.o build/gvec.o
	$(CC) -o $@ $^ $(TEST_LINK_LIBRARIES)

build/test/test_hull.o: test/test_hull.cpp include/hull.hpp include/gvec.hpp
	$(CC) -I./include/ -c $(CXXFLAGS) $(TESTFLAGS) $< -o $@

build/test/bin/test_hull: build/test/test_hull.o build/gvec.o
	$(CC) -o $@ $^ $(TEST_LINK_LIBRARIES)

build/test/test_mesh.o: test/test_mesh.cpp include/mesh.hpp include/gvec.hpp
	$(CC) -I./include/ -c $(CXXFLAGS) $(TESTFLAGS) $< -o $@

build/test/bin/test_mesh: build/test/test_mesh.o build/gvec.o build/mesh.o
	$(CC) -o $@ $^ $(TEST_LINK_LIBRARIES)

.PHONY: check
check: runner build/test/bin $(TEST_PROGRAMS) $(TEST_RUNTIME_DEPS)
	./runner

.PHONY: check-valgrind
check-valgrind: runner build/test/bin $(TEST_PROGRAMS) $(TEST_RUNTIME_DEPS)
	./runner --valgrind

.PHONY: clean
clean:
	rm -Rf build

# Hack to stop make deleting intermediate files:
.SECONDARY: $(OBJECTS) $(TEST_OBJECTS) $(TEST_HO_OBJECTS)

