CC=gcc
CFLAGS=-ansi -pedantic
ALL= src/main.o src/basics/libbmp.o src/basics/basic_funcs.o src/basics/terminal.o src/interface/interface.o src/maps/maps.o src/npcs/personajes.o src/basics/global.o

all: 	$(ALL)
	$(CC) -pthread -o exe $(ALL) -lm
	rm $(ALL)

clean:
	rm $(ALL)
