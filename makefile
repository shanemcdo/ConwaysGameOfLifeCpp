CC = g++
CFLAGS = -c -Wall -Ofast
INCLUDE = -Iinclude
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

all: main ConwaysGame Tile
	$(CC) bin/*.o -o bin/test.exe $(INCLUDE) $(LIBS)
	@echo

%: src/%.cpp
	$(CC) $< $(CFLAGS) -o bin/$@.o $(INCLUDE) $(LIBS)

clean:
	rm bin/*

new:
	mkdir bin

test: all
	bin/test.exe
