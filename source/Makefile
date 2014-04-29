
#
# (c) 2014 Andreas Linggi & Stefan Steiner
#
CC = gcc
CFLAGS = -g  -O2 -std=gnu99 -Wall -I/usr/include/cfitsio 
LDFLAGS =   -lcfitsio 
OFILES = main.o  matrix2csv.o visuGreFuc.o GaussSeidel.o image.o gnuplot.o 
OUTPUT = VisualizeGreensFunction 

all: 	$(OUTPUT)

$(OUTPUT): 	$(OFILES)
	$(CC) $(CFLAGS) -fopenmp  -o  $(OUTPUT) $(OFILES) $(LDFLAGS) 

main.o : main.c
	$(CC) $(CFLAGS)-c main.c

image.o : image.c 
	$(CC) $(CFLAGS) -c image.c 

matrix2csv.o : matrix2csv.c
	$(CC) $(CFLAGS)-c matrix2csv.c

visuGreFuc.o : visuGreFuc.c
	$(CC) $(CFLAGS)-c visuGreFuc.c

GaussSeidel.o : GaussSeidel.c
	$(CC) $(CFLAGS)-c GaussSeidel.c

gnuplot.o : ../gnuplot/gnuplot.c
	$(CC) $(CFLAGS)-c ../gnuplot/gnuplot.c

clean:
	rm -f *.o *.dSYM $(OUTPUT)
