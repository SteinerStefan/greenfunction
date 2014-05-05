//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : gnuplot.c
//  Author  : Andreas Linggi, Stefan Steiner
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------------------------------------
#define GNUPLOT "gnuplot -persist"
#define fileNameLength 100
//-------------------------------------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------------------------------------
#include "gnuplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
//-------------------------------------------------------------------------------------------------------------------------------
// makeEPSWithCSV
//-------------------------------------------------------------------------------------------------------------------------------
int makeEPSWithCSV(int pictureNumber, char* dataFoldName, float zMin, float zMax) 
{
	char pictureName [fileNameLength];  //Name des Bildes
	char sourceName[fileNameLength];    //Name des CSV Files

	snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,pictureNumber); 		//sourceName  -> CSV
	snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,pictureNumber	);	//pictureName -> png
	
	FILE *gp = popen(GNUPLOT,"w"); 					//gnuplop pipe schreiben
	if (gp==NULL) {
		printf("Error opening pipe to GNU plot\n");
		return  EXIT_FAILURE;	
	}
	fprintf(gp, "unset key\n");	
	//fprintf(gp, "set contour base\n");				// Höhenlinien
	fprintf(gp, "set terminal png enhanced size 1280,1024\n");	// Auflösung vom Plot
	fprintf(gp, "set output \"%s\"\n", pictureName);		// Dateiname 
	fprintf(gp, "set datafile separator \",\"\n");			// für CSV File
	fprintf(gp, "set view 50,30\n"); 				// zoom: z-Achse, Drehwinkel
	fprintf(gp, "set zrange [-0.1:0.2]\n"); 			// Skala z-Achse
	fprintf(gp, "set pm3d \n");					// Farbpalette aktivieren
	fprintf(gp, "set hidden3d\n");					// Gitternetzlinien ausblenden
	fprintf(gp, "set palette defined (  0 \"blue\", 8 \"white\", 16 \"red\")\n"); 	// Farbpalette
	fprintf(gp, "set cbrange [-0.1:0.2]\n"); 					// Skala von der Farbpalette
	fprintf(gp, "set colorbox vertical user origin 0.9, 0.15 size 0.02, 0.50\n");	// Position der Farbpalette
	fprintf(gp, "splot \"%s\" matrix  \n",sourceName);				// Plot
	pclose(gp);
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------
// makeEPSWithCSV
//-------------------------------------------------------------------------------------------------------------------------------
int makeEPSCollection(int startNumber, int stopNummer, int numthreads, char* dataFoldName)
{
	float zMin = -0.1;
	float zMax = 0.2;
	
	#pragma omp parallel for num_threads(numthreads)
	for(int n = startNumber; n <=stopNummer; n++) makeEPSWithCSV(n, dataFoldName, zMin, zMax);
	return 0;		
}
//-------------------------------------------------------------------------------------------------------------------------------
