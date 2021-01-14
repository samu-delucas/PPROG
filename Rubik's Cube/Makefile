

CFLAGS=-ansi -pedantic -Wall -g
CC=gcc
HEADERS=*.h

EXE=game
OBJ=cube.o extra.o solver.o print_c.o interface.o bcd.o terminal_funct.o cube_interface.o menu.o lib_funct.o lib_struct.o SDL_interface.o utils.o $(EXE).o


LIB_SDL=-lSDL2main -lSDL2 -lSDL2_ttf -lGLU -lGL 
LIB2=-lpthread $(LIB_SDL)


MAIN=main
MAINOBJ=main.o utils.o interface.o  terminal_funct.o

all: $(EXE)


run: $(EXE)
	./$(EXE)


$(EXE): $(OBJ) $(HEADERS)
	@echo "# -------------------------------"
	@echo "Linking $@" 
	$(CC) $(OBJ) $(LIB2) -o  $@

$(MAIN): $(MAINOBJ) $(HEADERS)
	@echo "# -------------------------------"
	@echo "Linking $@" 
	$(CC) $(CFLAGS) $(MAINOBJ) -o $@

%.o: %.c $(HEADERS)
	@echo "# -------------------------------"
	@echo "# Compiling $<"
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f *.o $(EXE) main *.gch

cleantrash:
	rm -f *.o *.gch


#$(TEST): $(OBJTEST) $(HEADERS)
	#$(CC) $(OBJTEXT) -o  $@

#$(MENU): $(OBJ2) $(HEADERS)
#	$(CC) $(OBJ2) -o $@

#$(TEMPEXE): $(OBJTEMPEXE) $(HEADERS)
#	$(CC) $(OBJTEMPEXE) $(LIB) $(LIB2) -o $@

#lib_struct.o: lib_struct.c lib_funct.h
#	$(CC) $(CFLAGS) -c $<
#
#print_c.o: print_c.c print_c.h interface.h cube.h
#	$(CC) $(CFLAGS) -c $<
#
#solver.o: solver.c solver.h cube.h
#	$(CC) $(CFLAGS) -c $<


