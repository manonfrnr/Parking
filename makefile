all : compile run clean

clean : rm *.o

run: ./parking

parking.o : parking.h parking.c
	gcc -Wall parking.c -o parking -lncurses

compile : parking.o 
	gcc -Wall -o parking.o
