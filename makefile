.PHONY: default
default: compile

# Compiler options
LINK_LIBRARIES=
COMPILER=$(CXX)
ifeq ($(COMPILER),)
	COMPILER:=g++
endif
CC=$(COMPILER) -std=c++11
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
TEST_NAMES=gvec atom mesh tetrahedron
TEST_PROGRAMS=$(TEST_NAMES:%=build/test/bin/test_%)
IS_LIBRT_REQUIRED:=$(shell echo "int main() {}" | gcc -x c - -lrt 2>&1)
ifeq ($(IS_LIBRT_REQUIRED),)
	TEST_LIBRT:=-lrt
else
	TEST_LIBRT:=
endif
IS_LIBSUBUNIT_REQUIRED:=$(shell echo "int main() {}" | gcc -x c - -lsubunit 2>&1)
ifeq ($(IS_LIBSUBUNIT_REQUIRED),)
	TEST_LIBSUBUNIT:=-lsubunit
else
	TEST_LIBSUBUNIT:=
endif
TEST_LINK_LIBRARIES=-lgtest $(TEST_LIBRT) -lpthread $(TEST_LIBSUBUNIT)

# rather than throwing make errors for missing dependencies and making the
# user sort it out, we have a go at installing them automatically:
.PHONY: get-check-deps
get-check-deps:
	if [ -z "`which admesh`" ]; then ./try-install.sh admesh; fi
	if [ -n "`echo "int main() {}" | gcc -x c - -lgtest 2>&1`" ]; then ./try-install.sh libgtest-dev; fi
	rm a.out || true

build/test/test_gvec.o: test/test_gvec.cpp include/gvec.hpp
	$(CC) -I./include/ -c $(CXXFLAGS) $(TESTFLAGS) $< -o $@

build/test/bin/test_gvec: build/test/test_gvec.o build/gvec.o
	$(CC) -o $@ $^ $(TEST_LINK_LIBRARIES)

build/test/test_atom.o: test/test_atom.cpp include/atom.hpp include/mesh.hpp include/gvec.hpp
	$(CC) -I./include/ -c $(CXXFLAGS) $(TESTFLAGS) $< -o $@

build/test/bin/test_atom: build/test/test_atom.o build/gvec.o build/atom.o build/mesh.o
	$(CC) -o $@ $^ $(TEST_LINK_LIBRARIES)

build/test/test_mesh.o: test/test_mesh.cpp include/mesh.hpp include/gvec.hpp
	$(CC) -I./include/ -c $(CXXFLAGS) $(TESTFLAGS) $< -o $@

build/test/bin/test_mesh: build/test/test_mesh.o build/gvec.o build/mesh.o
	$(CC) -o $@ $^ $(TEST_LINK_LIBRARIES)

build/test/test_tetrahedron.o: test/test_tetrahedron.cpp include/tetrahedron.hpp include/atom.hpp include/gvec.hpp include/mesh.hpp
	$(CC) -I./include/ -c $(CXXFLAGS) $(TESTFLAGS) $< -o $@

build/test/bin/test_tetrahedron: build/test/test_tetrahedron.o build/gvec.o build/tetrahedron.o build/atom.o build/mesh.o
	$(CC) -o $@ $^ $(TEST_LINK_LIBRARIES)


.PHONY: check
check: runner build/test/bin get-check-deps $(TEST_PROGRAMS)
	./runner

.PHONY: check-valgrind
check-valgrind: runner build/test/bin $(TEST_PROGRAMS) $(TEST_RUNTIME_DEPS)
	./runner --valgrind

.PHONY: clean
clean:
	rm -Rf build

# Hack to stop make deleting intermediate files:
.SECONDARY: $(OBJECTS) $(TEST_OBJECTS) $(TEST_HO_OBJECTS)

