##
## MAKFILE
## author:  Samuel de Lucas Maroto
##
## To compile the game -> 'make compile'
## To run the game 	   -> 'make run'
##


## MACROS ###################################################################

CC = gcc
CFLAGS = -g -Wall -pedantic 
OURLIBSC = libs/grafiks.c libs/control.c libs/quest.c libs/player.c libs/body.c libs/permutations.c libs/queue.c libs/randomizer.c libs/musik.c libs/screen.c libs/achievement.c libs/menu.c
OURLIBS = $(OURLIBSC:.c=)
OURLIBSH = $(OURLIBSC:.c=.h)
OURLIBSOBJ = $(OURLIBSC:.c=.o)

## COMPILE THE GAME #########################################################

.PHONY : compile

compile: clean main clear
	@echo "#---------------------------"
	@echo "# To run the game just execute 'make run' or './main'"
	@echo "# Have fun!!!"

## RUN THE GAME #############################################################

.PHONY : run

run:
	./main
	
## MAIN INSTRUCTIONS ########################################################

main: main.o $(OURLIBSOBJ)
	@echo "#---------------------------"
	@echo "# Generating the game!!! "
	@echo " "
	$(CC) -o $@ $^ -pthread -lm -Wl,-rpath=$(PWD)/libs/fmod/api/core/lib/x86_64 -L$(PWD)/libs/fmod/api/core/lib/x86_64 -lfmod
	@echo " "


main.o: main.c
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo " "
	$(CC) $(CFLAGS) -c main.c
	@echo " "
	
## LIBS INSTRUCTIONS ########################################################

libs/%.o : libs/%.c libs/%.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo " "
	$(CC) $(CFLAGS) -c libs/$*.c -o libs/$*.o
	@echo " "

## CLEANING INSTRUCTIONS ####################################################

.PHONY : clean

clean:
	@echo "#---------------------------"
	@echo "#Removing object files and executables"
	@echo " "	
	rm -f *.o libs/*.o $(ALLFILES) main
	@echo " "

.PHONY : clear

clear:
	@echo "#---------------------------"
	@echo "#Removing object files"
	@echo " "
	rm -f *.o libs/*.o
	@echo " "
