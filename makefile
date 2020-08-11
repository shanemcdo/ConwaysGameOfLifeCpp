CC = g++
CFLAGS = -c -Wall
INCLUDE = -Iinclude
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

all: main
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
