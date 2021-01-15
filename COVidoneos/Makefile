CC = gcc
CFLAGS = -ansi -pedantic -Wall

INCLUDE=-I./lib/
LIBS = -lpthread

HEADERS = $(wildcard lib/*.h)
OBJ = config.o print_helper.o input_helper.o object.o personaje.o escenas.o menu.o main.o 
EXE = game.out

all: game

%.o: src/%.c $(HEADERS)
	@echo Compiling $<...
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<
	@echo 

$(EXE): $(OBJ)
	@echo Linking...
	$(CC) -o game.out $(OBJ) $(LIBS)
	@echo 

game: $(EXE) clean

config:
	@./res/gen_maps.sh
	@./res/gen_texts.sh

clean:
	@echo Cleaning object files...
	rm -f *.o core
	@echo 

remove: clean
	@echo Cleaning $(EXE)...
	rm -f $(EXE)
	@echo 