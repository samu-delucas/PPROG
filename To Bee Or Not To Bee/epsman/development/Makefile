#
# Compiles and installs the programs for creating a context using self-organizing maps
#
#
# directories for the installation of the program and for the source files
#
PROGRAM_HOME=..

#
# Change this to wherever you want to install the program.
#
INSTALL_DIR=$(PROGRAM_HOME)/install


SOURCE_DIR=$(PROGRAM_HOME)/development/src
WINDOW_DIR=$(PROGRAM_HOME)/development/src/scrwindow
UTIL_DIR=$(PROGRAM_HOME)/development/src/util
AUX_DIR=$(PROGRAM_HOME)/development/aux_files


LIBS=

#
# These are some things about the compiler
#
CC=gcc -c -std=c99
LN=gcc

CFLAGS= -I$(SOURCE_DIR)  -I$(WINDOW_DIR) -I$(UTIL_DIR) -DIS_WINDOWS

OBJFILES=epsman.o rectangles.o intrf.o lineread.o qrnd.o

LFLAGS=

#
# Object files
#
rectangles.o:	$(WINDOW_DIR)/rectangles.c  $(WINDOW_DIR)/rectangles.h 
	$(CC) $(CFLAGS) $(WINDOW_DIR)/rectangles.c

intrf.o:	$(WINDOW_DIR)/intrf.c  $(WINDOW_DIR)/intrf.h $(WINDOW_DIR)/rectangles.h 
	$(CC) $(CFLAGS) $(WINDOW_DIR)/intrf.c

lineread.o:	$(UTIL_DIR)/lineread.c  $(UTIL_DIR)/lineread.h 
	$(CC) $(CFLAGS) $(UTIL_DIR)/lineread.c

qrnd.o:	$(UTIL_DIR)/qrnd.c  $(UTIL_DIR)/qrnd.h 
	$(CC) $(CFLAGS) $(UTIL_DIR)/qrnd.c

epsman.o:	$(SOURCE_DIR)/epsman.c  $(UTIL_DIR)/lineread.h $(WINDOW_DIR)/intrf.h $(WINDOW_DIR)/rectangles.h 
	$(CC) $(CFLAGS) $(SOURCE_DIR)/epsman.c


#
# General targets
#
epsman: $(OBJFILES)
	$(LN) $(LFLAGC) -o $(INSTALL_DIR)/epsman $(OBJFILES) -lm -lpthread


all:	epsman
	cp $(AUX_DIR)/* $(INSTALL_DIR)

clean:
	rm *.o
	rm $(INSTALL_DIR)/epsman
