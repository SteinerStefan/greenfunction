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
#include <string.h>
#include <omp.h>
//-------------------------------------------------------------------------------------------------------------------------------
// makeEPSWithCSV
//-------------------------------------------------------------------------------------------------------------------------------
int makeEPSWithCSV(int pictureNumber, char* dataFoldName, float zMin, float zMax) 
{
	char pictureName [fileNameLength];  //Name des Bildes
	char sourceName[fileNameLength];    //Name des CSV Files

	snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,pictureNumber); 	//sourceName  -> CSV
	snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,pictureNumber);	//pictureName -> png
	
	FILE *gp = popen(GNUPLOT,"w"); 					//gnuplop pipe schreiben
	if (gp==NULL) {
		printf("Error opening pipe to GNU plot\n");
		return  EXIT_FAILURE;	
	}
	fprintf(gp, "unset key\n");	
	//fprintf(gp, "set contour base\n");						// Höhenlinien
	fprintf(gp, "set terminal png enhanced size 1280,1024\n");	// Auflösung vom Plot
	fprintf(gp, "set output \"%s\"\n", pictureName);			// Dateiname 
	fprintf(gp, "set datafile separator \",\"\n");				// für CSV File
	fprintf(gp, "set view 50,30\n"); 							// zoom: z-Achse, Drehwinkel
	fprintf(gp, "set zrange [%f:%f]\n",zMin,zMax); 				// Skala z-Achse
	fprintf(gp, "set pm3d \n");									// Farbpalette aktivieren
	fprintf(gp, "set hidden3d\n");								// Gitternetzlinien ausblenden
	fprintf(gp, "set palette defined (  0 \"blue\", 8 \"white\", 16 \"red\")\n"); 	// Farbpalette
	fprintf(gp, "set cbrange [%f:%f]\n",zMin,zMax); 					// Skala von der Farbpalette
	fprintf(gp, "set colorbox vertical user origin 0.9, 0.15 size 0.02, 0.50\n");	// Position der Farbpalette
	fprintf(gp, "splot \"%s\" matrix  \n",sourceName);			// Plot
	pclose(gp);
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------
// makeEPSWithCSVContour
//-------------------------------------------------------------------------------------------------------------------------------
int makeEPSWithCSVContour(int pictureNumber, char* dataFoldName, float zMin, float zMax) 
{
	char pictureName [fileNameLength];  //Name des Bildes
	char sourceName[fileNameLength];    //Name des CSV Files

	snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,pictureNumber); 	//sourceName  -> CSV
	snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,pictureNumber);	//pictureName -> png
	
	FILE *gp = popen(GNUPLOT,"w"); 					//in gnuplop pipe schreiben
	if (gp==NULL) {
		printf("Error opening pipe to GNU plot\n");
		return  EXIT_FAILURE;	
	}
	fprintf(gp, "unset key\n");	
	fprintf(gp, "unset ztics\n");	
	fprintf(gp, "set contour base\n");						// Höhenlinien
	fprintf(gp, "set terminal png enhanced size 1280,1024\n");	// Auflösung vom Plot
	fprintf(gp, "set output \"%s\"\n", pictureName);			// Dateiname 
	fprintf(gp, "set datafile separator \",\"\n");				// für CSV File
	fprintf(gp, "set view 0,90\n"); 							// zoom: z-Achse, Drehwinkel
	fprintf(gp, "set pm3d \n");									// Farbpalette aktivieren
	fprintf(gp, "set hidden3d\n");								// Gitternetzlinien ausblenden
	fprintf(gp, "set palette defined (  0 \"blue\", 8 \"white\", 16 \"red\")\n"); 	// Farbpalette
	fprintf(gp, "set cbrange [%f:%f]\n",zMin,zMax); 					// Skala von der Farbpalette
	fprintf(gp, "set colorbox vertical user origin 0.9, 0.2 size 0.02, 0.60\n");	// Position der Farbpalette
	fprintf(gp, "splot \"%s\" matrix  \n",sourceName);			// Plot
	pclose(gp);
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------
// getfield		 um .csv einzulesen
//-------------------------------------------------------------------------------------------------------------------------------
const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ","))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}
//-------------------------------------------------------------------------------------------------------------------------------
// makeEPSWithCSV
//-------------------------------------------------------------------------------------------------------------------------------
int makeEPSCollection(int n, int startNumber, int stopNummer, int numthreads, char* dataFoldName, int mode)
{	
	// min und max für die Skalierung der z-Achse aus .csv file bestimmen
	float zMin = 0;
	float zMax = 0;
	
	char path[fileNameLength];   												//Name des CSV Files
	snprintf(path, fileNameLength, "%s/step%04d.csv",dataFoldName,stopNummer);	//file-path bauen

	FILE *datei = fopen(path, "r");			// stream öffnen
	int length =7*n*7*n;					// länge der Datei
    char line[length];						// buffer
    while (fgets(line, length, datei))		// in buffer lesen
    {
	for (int i=1;i<n;i++) {
		char* tmp = strdup(line);
		float tmp2 = atof(getfield(tmp, i));
		if (tmp2 > zMax) zMax = tmp2;
		if (tmp2 < zMin) zMin = tmp2;
        free(tmp);
		}
    }
	fclose(datei);

	printf("zMin: %f, zMax: %f\n",zMin,zMax);
	if (100 <= mode  && mode < 300)
	{
		#pragma omp parallel for num_threads(numthreads)
		for(int n = startNumber; n <=stopNummer; n++) 
		{
			makeEPSWithCSV(n, dataFoldName, zMin, zMax);
		}		
	} else 
	if (310 <= mode  && mode < 400)
	{
		#pragma omp parallel for num_threads(numthreads)
		for(int n = startNumber; n <=stopNummer; n++) 
		{
			makeEPSWithCSVContour(n, dataFoldName, zMin, zMax);
		}			
	}

	return 0;		
}
//-------------------------------------------------------------------------------------------------------------------------------
