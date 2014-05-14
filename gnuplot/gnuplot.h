//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : gnuplot.h
//  Author  : Andreas Linggi, Stefan Steiner
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------
#ifndef gnuplot_h
#define gnuplot_h


//generiert eine Folge von Bildern aus CSV files
//startNumber ist die Nummer des ersten Bildes und stopNummer die letzte
//int makeEPSCollection(int n, int startNumber, int stopNummer, int numthreads, char* dataFoldName, int mode);

int makeEPSCollectionEnum(int n, int startNumber, int stopNumber, int numthreads, char* dataFoldName, int distance, int mode);
#endif
//-------------------------------------------------------------------------------------------------------------------------------

