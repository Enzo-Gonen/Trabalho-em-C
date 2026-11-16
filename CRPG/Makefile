CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -O2 -Iinclude
LDFLAGS :=
TARGET := build/rpg
SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c,build/%.o,$(SOURCES))
TEST_SOURCES := $(filter-out src/main.c,$(SOURCES))

.PHONY: all clean run test
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/test_core: tests/test_core.c $(TEST_SOURCES) | build
	$(CC) $(CFLAGS) tests/test_core.c $(TEST_SOURCES) -o $@

build:
	mkdir -p build

run: all
	./$(TARGET)

test: build/test_core
	./build/test_core

clean:
	rm -f build/*.o build/rpg build/rpg.exe build/test_core
