# Last updated: 2022-05-11
CFLAGS=-Wextra -Iinclude $(shell pkg-config --cflags raylib)
CXXFLAGS=-std=c++20
TARGET=bin/test
RM=rm -rf
OBJ=$(patsubst src/%,bin/%.o,$(basename $(wildcard src/*)))
DBGR=lldb
LIBS=$(shell pkg-config --libs raylib)

all: $(TARGET)

test: $(TARGET)
	$(TARGET)

debug: CFLAGS+=-g
debug: TARGET=bin/debug
debug: $(TARGET)
	$(DBGR) $(TARGET)

release: CFLAGS+=-Ofast
release: TARGET=bin/conway
release: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LIBS) $^ -o $(TARGET)

bin/%.o: CFLAGS+=-c
bin/%.o: src/%.c bin
	$(CC) $< $(CFLAGS) -o $@

bin/%.o: src/%.cpp bin
	$(CXX) $< $(CFLAGS) $(CXXFLAGS) -o $@

bin:
	mkdir bin

clean:
	$(RM) bin

.PHONY: all test debug clean
