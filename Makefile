CC=g++
CFLAGS=-c -Wall -O3 -ggdb -std=c++0x -Wno-unused-result -Wno-parentheses -Wno-unused-variable -Wno-uninitialized

all: wachstum

wachstum: main.o
	$(CC) main.o -o wachstum

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -rf *o wachstum
