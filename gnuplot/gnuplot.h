//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : gnuplot.h
//  Author  : Andreas Linggi, Stefan Steiner
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------
#ifndef gnuplot_h
#define gnuplot_h

//generiert GNU-Plot aus einem CSV file
//pictureNumber ist die Number, die das Bild bekommt.
int makeEPSWithCSV(int pictureNumber, char* dataFoldName, float zMin, float zMax); 
//generiert eine Folge von Bildern aus CSV files
//startNumber ist die Nummer des ersten Bildes und stopNummer die letzte
int makeEPSCollection(int startNumber, int stopNummer , int numthreads, char* dataFoldName);
#endif
//-------------------------------------------------------------------------------------------------------------------------------

