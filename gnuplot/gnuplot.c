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
int makeEPSWithCSV(int pictureNumber, char* dataFoldName) 
{
	char pictureName [fileNameLength];  //Name des Bildes
	char sourceName[fileNameLength];    //Name des CSV Files

	snprintf( sourceName, fileNameLength, "%s/step%04d.csv",dataFoldName,pictureNumber); //sourceName  -> CSV
	snprintf( pictureName, fileNameLength, "%s/step%04d.png",dataFoldName,pictureNumber	);//pictureName -> png
	
	FILE *gp = popen(GNUPLOT,"w"); //gnuplop pipe schreiben
	if (gp==NULL) 
	{
		printf("Error opening pipe to GNU plot\n");
		return  EXIT_FAILURE;	
	}
	fprintf(gp, "unset key\n");	
	//fprintf(gp, "set contour base\n");
	fprintf(gp, "set terminal png enhanced size 1280,1024\n");
	fprintf(gp, "set output \"%s\"\n", pictureName);
	fprintf(gp, "set datafile separator \",\"\n");
	fprintf(gp, "set view 25,30\n");
	fprintf(gp, "set pm3d \n");
	fprintf(gp, "set hidden3d\n");
	fprintf(gp, "set palette defined (0 \"blue\", 40 \"orange\", 80 \"red\")\n");
	fprintf(gp, "set colorbox vertical user origin 0.9, 0.15 size 0.02, 0.50\n");
	fprintf(gp, "splot \"%s\" matrix  \n",sourceName); //notitle lt 1 lc rgb \"#000000\"
	pclose(gp);
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------
// makeEPSWithCSV
//-------------------------------------------------------------------------------------------------------------------------------
int makeEPSCollection(int startNumber, int stopNummer, int numthreads, char* dataFoldName)
{
	#pragma omp parallel for num_threads(numthreads)
	for(int n = startNumber; n <=stopNummer; n++) makeEPSWithCSV(n, dataFoldName);
	return 0;		
}
//-------------------------------------------------------------------------------------------------------------------------------
