CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -ansi

tester: tester.o
	gcc -o tester objeto.o jugador.o personaje.o tester.o habitacion.o mapa.o

tester.o: tester.c jugador.o mapa.o
	gcc -c tester.c

mapa.o: mapa.c mapa.h
	gcc -c mapa.c

jugador.o: jugador.c jugador.h habitacion.o personaje.o
	gcc -c jugador.c

personaje.o: personaje.c habpers.h objeto.o
	gcc -c personaje.c

habitacion.o: habitacion.c habpers.h objeto.o
	gcc -c habitacion.c

objeto.o: objeto.c objeto.h types.h
	gcc -c objeto.c

clean :
	rm -f *.o core $(EXE)


