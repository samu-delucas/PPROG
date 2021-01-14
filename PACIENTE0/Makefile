

LIBS=

#
# These are some things about the compiler
#
CC=gcc -c
LN=gcc

FLAGS = -g -I./src -I./src/MenuInicio -I./src/Contenido -I./src/Graficos -I./src/TADs -I./src/Interfaz -I./src/Comandos -I./src/Laberinto -I./src/Colorear -I./src/Birds -I./src/Snake
CFLAGS= -g -I./src -I./src/MenuInicio -I./src/Contenido -I./src/Graficos -I./src/TADs -I./src/Interfaz -I./src/Comandos -I./src/Laberinto -I./src/Colorear -I./src/Birds -I./src/Snake

OBJFILES=juego.o menu.o start.o acertijosPistas.o types.o lineread.o rectangles.o jugador.o habitacion.o personaje.o objeto.o intrf.o cop.o sco.o lab_mapa.o lab_intrf.o laberinto.o mapa.o imprimirZonaCuarentena.o imprimirBiblioteca.o imprimirHall.o imprimirLab.o imprimirCafeteria.o imprimirEscaleras.o imprimirAula10.o imprimirDesp.o colorear.o birds.o birds_intrf.o snk_rectangles.o snk_qrnd.o snk_intrf.o snake.o

LFLAGS=

all: juego
#
# Object files
#
#$(OBJFILES): %: src/%.c src/%.h
#	$(CC) $(CFLAGS) ./src/$@.c

menu.o:	./src/MenuInicio/menu.c  ./src/MenuInicio/menu.h 
	$(CC) $(FLAGS) ./src/MenuInicio/menu.c

juego.o:	./src/juego.c  ./src/MenuInicio/menu.h ./src/Contenido/start.h
	$(CC) $(FLAGS) ./src/juego.c

acertijosPistas.o: ./src/Contenido/acertijosPistas.c  ./src/Contenido/acertijosPistas.h
	$(CC) $(FLAGS) ./src/Contenido/acertijosPistas.c

types.o: ./src/Contenido/types.c  ./src/TADs/types.h 
	$(CC) $(FLAGS) ./src/Contenido/types.c

lineread.o: ./src/Birds/lineread.c ./src/Birds/lineread.h
	$(CC) $(FLAGS) ./src/Birds/lineread.c

start.o: ./src/Contenido/start.c  ./src/Contenido/start.h 
	$(CC) $(FLAGS) ./src/Contenido/start.c

rectangles.o: ./src/Graficos/rectangles.c  ./src/Graficos/rectangles.h 
	$(CC) $(FLAGS) ./src/Graficos/rectangles.c

imprimirBiblioteca.o: ./src/Graficos/imprimirBiblioteca.c  ./src/Graficos/imprimirBiblioteca.h 
	$(CC) $(FLAGS) ./src/Graficos/imprimirBiblioteca.c

imprimirHall.o: ./src/Graficos/imprimirHall.c  ./src/Graficos/imprimirHall.h 
	$(CC) $(FLAGS) ./src/Graficos/imprimirHall.c

imprimirLab.o: ./src/Graficos/imprimirLab.c  ./src/Graficos/imprimirLab.h 
	$(CC) $(FLAGS) ./src/Graficos/imprimirLab.c

imprimirCafeteria.o: ./src/Graficos/imprimirCafeteria.c  ./src/Graficos/imprimirCafeteria.h 
	$(CC) $(FLAGS) ./src/Graficos/imprimirCafeteria.c

imprimirEscaleras.o: ./src/Graficos/imprimirEscaleras.c  ./src/Graficos/imprimirEscaleras.h 
	$(CC) $(FLAGS) ./src/Graficos/imprimirEscaleras.c

imprimirZonaCuarentena.o: ./src/Graficos/imprimirZonaCuarentena.c  ./src/Graficos/imprimirZonaCuarentena.h 
	$(CC) $(FLAGS) ./src/Graficos/imprimirZonaCuarentena.c

imprimirAula10.o: ./src/Graficos/imprimirAula10.c  ./src/Graficos/imprimirAula10.h 
	$(CC) $(FLAGS) ./src/Graficos/imprimirAula10.c

imprimirDesp.o: ./src/Graficos/imprimirDesp.c  ./src/Graficos/imprimirDesp.h 
	$(CC) $(FLAGS) ./src/Graficos/imprimirDesp.c

jugador.o: ./src/TADs/jugador.c  ./src/TADs/jugador.h habitacion.o personaje.o 
	$(CC) $(FLAGS) ./src/TADs/jugador.c

personaje.o: ./src/TADs/personaje.c  ./src/TADs/habpers.h objeto.o
	$(CC) $(FLAGS) ./src/TADs/personaje.c

habitacion.o: ./src/TADs/habitacion.c  ./src/TADs/habpers.h objeto.o 
	$(CC) $(FLAGS) ./src/TADs/habitacion.c

objeto.o: ./src/TADs/objeto.c  ./src/TADs/objeto.h ./src/TADs/types.h 
	$(CC) $(FLAGS) ./src/TADs/objeto.c

mapa.o: ./src/TADs/mapa.c  ./src/TADs/mapa.h ./src/TADs/types.h 
	$(CC) $(FLAGS) ./src/TADs/mapa.c

intrf.o: ./src/Interfaz/intrf.c ./src/Interfaz/intrf.h rectangles.o jugador.o
	$(CC) $(FLAGS) ./src/Interfaz/intrf.c

cop.o: ./src/Comandos/cop.c ./src/Comandos/cop.h
	$(CC) $(FLAGS) ./src/Comandos/cop.c

sco.o: ./src/Comandos/sco.c ./src/Comandos/sco.h cop.o jugador.o rectangles.o
	$(CC) $(FLAGS) ./src/Comandos/sco.c

lab_mapa.o: ./src/Laberinto/lab_mapa.c ./src/Laberinto/lab_mapa.h
	$(CC) $(FLAGS) ./src/Laberinto/lab_mapa.c

lab_intrf.o: ./src/Laberinto/lab_intrf.c ./src/Laberinto/lab_intrf.h lineread.o lab_mapa.o
	$(CC) $(FLAGS) ./src/Laberinto/lab_intrf.c	

laberinto.o: ./src/Laberinto/laberinto.c ./src/Laberinto/laberinto.h lineread.o lab_mapa.o lab_intrf.o
	$(CC) $(FLAGS) ./src/Laberinto/laberinto.c

colorear.o: ./src/Colorear/colorear.c ./src/Colorear/colorear.h intrf.o rectangles.o
	$(CC) $(FLAGS) ./src/Colorear/colorear.c

birds_intrf.o: ./src/Birds/birds_intrf.c ./src/Birds/birds_intrf.h lineread.o
	$(CC) $(FLAGS) ./src/Birds/birds_intrf.c	

birds.o: ./src/Birds/birds.c ./src/Birds/birds.h lineread.o birds_intrf.o
	$(CC) $(FLAGS) ./src/Birds/birds.c

snk_qrnd.o: ./src/Snake/snk_qrnd.c ./src/Snake/snk_qrnd.h
	$(CC) $(FLAGS) ./src/Snake/snk_qrnd.c	

snk_rectangles.o: ./src/Snake/snk_rectangles.c ./src/Snake/snk_rectangles.h types.o
	$(CC) $(FLAGS) ./src/Snake/snk_rectangles.c

snk_intrf.o: ./src/Snake/snk_intrf.c ./src/Snake/snk_intrf.h snk_rectangles.o
	$(CC) $(FLAGS) ./src/Snake/snk_intrf.c	

snake.o: ./src/Snake/snake.c ./src/Snake/snake.h lineread.o snk_rectangles.o snk_intrf.o snk_qrnd.o
	$(CC) $(FLAGS) ./src/Snake/snake.c

#
# General targets
#
juego: $(OBJFILES)
	$(LN) $(LFLAGC) -o juego $(OBJFILES) -lpthread 
	rm *.o


clean:
	rm *.o
	rm juego
