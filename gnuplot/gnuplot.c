//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : gnuplot.c
//  Author  : Andreas Linggi, Stefan Steiner
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------------------------------------
#define fileNameLength 30
#define GNUPLOT "gnuplot -persist"
//-------------------------------------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------------------------------------
#include "gnuplot.h"
#include <stdio.h>
#include <stdlib.h>
//-------------------------------------------------------------------------------------------------------------------------------
// makeEPSWithCSV
//-------------------------------------------------------------------------------------------------------------------------------
int makeEPSWithCSV(int pictureNumber) 
{
	char pictureName [fileNameLength];  //Name des Bildes
	char sourceName[fileNameLength];    //Name des CSV Files

	snprintf( sourceName, fileNameLength, "../data/step%03d.csv",pictureNumber); //sourceName  -> CSV
	snprintf( pictureName, fileNameLength, "../data/step%03d.png",pictureNumber	);//pictureName -> png
	
	FILE *gp = popen(GNUPLOT,"w"); //gnuplop pipe schr
	if (gp==NULL) 
	{
		printf("Error opening pipe to GNU plot\n");
		return  EXIT_FAILURE;	
	}
	fprintf(gp, "unset key\n");	
	fprintf(gp, "set contour base\n");
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
int makeEPSCollection(int startNumber, int stopNummer)
{
	for(int n = startNumber; n <=stopNummer; n++) makeEPSWithCSV(n);
	return 0;		
}
//-------------------------------------------------------------------------------------------------------------------------------
