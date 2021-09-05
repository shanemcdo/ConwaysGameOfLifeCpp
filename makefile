# Last updated 2020-10-24 by Shane McDonough
CC = clang++#compiler
EXTENSION = .cpp#c++ extension
CFLAGS = -std=c++20 -Wall `pkg-config --libs --cflags raylib`# -c compile and assemble do not link, -Wall turns warnings on
INCLUDE = -Iinclude# folders to be included, anything following -I is the directory the compiler can now see
LIBS =# libs to be included, anything following -l is a library that is included
OBJECTS = $(patsubst src/%$(EXTENSION),bin/%.o,$(wildcard src/*$(EXTENSION)))# in bin/%.o format, all of the objects to be compiled
TARGET = bin/test
# previous line explained:
# patsubst replaces the first arg template with the second arg template on the variable in the third arg
# wildcard gets all the files that comply with its arg
# in this case wildcard returns every file with the main extension in the src directory
# e.g. src/main.cpp src/File1.cpp
# patsub replaces the src and the extension
# e.g. bin/main.o bin/File1/.o

all: bin $(OBJECTS)# compile everything
	$(CC) bin/*.o -o $(TARGET) $(INCLUDE) $(LIBS) $(CFLAGS)
	@echo
	
bin/%.o: src/%$(EXTENSION)# create object file for %
	$(CC) $< $(CFLAGS) -c -o $@ $(INCLUDE) $(LIBS)

clean:# remove contents of bin
	rm -rf bin

bin:# create folder bin
	mkdir bin

test: all# compile everything then run executible
	$(TARGET)
