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
// private/ Hilfsfunktionen
//-------------------------------------------------------------------------------------------------------------------------------
int contourPlot(char* pictureName, char* sourceName, float zMin, float zMax)
{
	FILE *gp = popen(GNUPLOT,"w"); 					//gnuplop pipe schreiben
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
}
int normal3DPlot(char* pictureName, char* sourceName, float zMin, float zMax)
{
	FILE *gp = popen(GNUPLOT,"w"); 					//gnuplop pipe schreiben
	if (gp==NULL) {
		printf("Error opening pipe to GNU plot\n");
		return  EXIT_FAILURE;	
	}
	fprintf(gp, "unset key\n");	
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
}
// getfield um .csv einzulesen
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
// min und max für die Skalierung der z-Achse aus .csv file bestimmen
void calculateMinMax(char* dataFoldName,int stopNumber, int n, float* zMin, float* zMax)
{

	char path[fileNameLength];  		// Pfad des letzten Bildes
	int length =10*n;					// länge der Datei
    char line[length];					// buffer
	snprintf(path, fileNameLength, "%s/step%04d.csv",dataFoldName,stopNumber);

	//aus file lesen
	FILE *datei = fopen(path, "r");			// stream öffnen
    while (fgets(line, length, datei))		// in buffer lesen
    {
	for (int i=1;i<n;i++) {
		char* tmp = strdup(line);
		float tmp2 = atof(getfield(tmp, i));
		if (tmp2 > *zMax) *zMax = tmp2;
		if (tmp2 < *zMin) *zMin = tmp2;
        free(tmp);
		}
    }
	fclose(datei);
}
//-------------------------------------------------------------------------------------------------------------------------------
// makeEPSWithCSVEnum
//-------------------------------------------------------------------------------------------------------------------------------
int makeEPSCollectionEnum(int n, int startNumber, int stopNumber, int numthreads, char* dataFoldName, int mode)
{	
	float zMin = 0;
	float zMax = 0;
	int j =1;
	int distance = mode%100;
	if (mode >= 600) distance +=100;

	char pictureName [fileNameLength];  //Name des Bildes
	char sourceName  [fileNameLength];  //Name des CSV Files

	calculateMinMax(dataFoldName, stopNumber, n, &zMin, &zMax);
	printf("zMin: %f, zMax: %f\n",zMin,zMax);
	
	if (100 <= mode  && mode < 500) 
	{
		if (100 <= mode  && mode < 300)
		{
			//erstes Bild
			snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,1); 	//sourceName  -> CSV
			snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,j);	//pictureName -> png
			j++;
			normal3DPlot(pictureName,sourceName,zMin,zMax);
			// jedes distance-te Bild
			for (int i = distance;i<stopNumber;i+=distance)
			{
				snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,i); 	//sourceName  -> CSV
				snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,j);	//pictureName -> png
				j++;
				normal3DPlot(pictureName,sourceName,zMin,zMax);
			}
			// letztes Bild
			snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,stopNumber); 	//sourceName  -> CSV
			snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,j);	//pictureName -> png
			j++;
			normal3DPlot(pictureName,sourceName,zMin,zMax);
		} 
		else if (310 <= mode  && mode < 400)
		{
			//erstes Bild
			snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,1); 	//sourceName  -> CSV
			snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,j);	//pictureName -> png
			j++;
			contourPlot(pictureName,sourceName,zMin,zMax);	
			// jedes distance-te Bild
			for (int i = distance;i<stopNumber;i+=distance)
			{
				snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,i); 	//sourceName  -> CSV
				snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,j);	//pictureName -> png
				j++;
			contourPlot(pictureName,sourceName,zMin,zMax);	
			}
			// letztes Bild
			snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,stopNumber); 	//sourceName  -> CSV
			snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,j);	//pictureName -> png
			j++;
			contourPlot(pictureName,sourceName,zMin,zMax);	
		}
	}
	else if (500 <= mode  && mode < 700)
	{
		//erstes Bild
		snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,1); 	//sourceName  -> CSV
		snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,j);	//pictureName -> png
		j++;
		normal3DPlot(pictureName,sourceName,zMin,zMax);	
		// jedes distance-te Bild
		for (int i = distance;i<stopNumber;i+=distance)
		{
			snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,i); 	//sourceName  -> CSV
			snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,j);	//pictureName -> png
			j++;
		normal3DPlot(pictureName,sourceName,zMin,zMax);	
		}
		// letztes Bild
		snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,stopNumber); 	//sourceName  -> CSV
		snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,j);	//pictureName -> png
		j++;
		normal3DPlot(pictureName,sourceName,zMin,zMax);
	}
	return 0;	
}
//-------------------------------------------------------------------------------------------------------------------------------
