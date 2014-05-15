#
# (c) 2014 Andreas Linggi
#

CFLAGS = -g  -O2 -std=gnu99

FILES = main.c gnuplot.c

all: 	gnuplotCSV


gnuplotCSV: 	$(FILES)
	$(CC) $(CFLAGS)  -o  gnuplotCSV $(FILES)



clean:
	rm -f *.o *.dSYM
