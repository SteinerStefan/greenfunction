//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : main.c
//  Author  : Andreas Linggi, Stefan Steiner
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
//Includes
//-------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <fitsio.h>
#include "stdlib.h"
#include <time.h>
#include "GaussSeidel.h"
#include "visuGreFuc.h"
#include "matrix2csv.h"
#include "../gnuplot/gnuplot.h"
#include "image.h"

//-------------------------------------------------------------------------------------------------------------------------------
//main
//-------------------------------------------------------------------------------------------------------------------------------
int main(int argc, const char * argv[])
{

	//Bild einlesen
	image_t	*image = NULL;      //beinhaltet daten, sowie infos über aufbau des bildes	
	image = readimage(argv[1]); 
	printf("Bild eingelesen");
	if(image == NULL)
	{
		fprintf(stderr, "cannot read image\n");
		return  EXIT_FAILURE;
	}
	else printf("file geoeffnet\n");
	printf("width: %d, height: %d\n",image->width, image->height);
	//Variablen
	int n = image->width; //laenge des Bildes
	int n2 = n*n;         //Flaeche des Bildes
    int iterations = 0;   
    int numthreads = 8;
	int numPictures = 0; // Anzahl der Bilder, die während Simulation erstellt wurden
    time_t timer1,timer2; //Timer für berechnungslänge
    time(&timer1);

	//Iterationsschritte und Anzahl der verwendeten Threads angeben
	do{
	printf("iterations, numthreads:\n");
	printf("Vorgeschlagene Iterationszahl: %d\n", image->width);
	scanf("%d %d",&iterations,&numthreads);
	fseek(stdin,0,SEEK_END); //Buffer leeren
	}while(iterations<=1 && iterations>1000);
	printf("Eingabe ok...\n");
	//Von Eingegebenem Bild Plot erstellen
	float** outImage = newMatrixFromVector(image->data, n);	
	writeMatrixAsCSVtoFile(outImage, n, 123);       // in csv File scheiben
	makeEPSWithCSV(123);
	free(outImage);

    int maxstep = n/2+1; //n/2+1
	int startstep =  n/2+1;
	for (int step = startstep; step<=maxstep; step++) 
	{
		float* v = image->data; //BildVektor
		float* x = (float *) malloc(sizeof(float) * n2); //x lösungsvektor für diesen Schritt
		for (int i=0; i<n2; i++) x[i] = 0;               //mit null initialisieren
		gaussSeidel(v,x,n,iterations,numthreads); // Berechnungen durchführen
		outImage = newMatrixFromVector(x,n);     // Matrix aus Vector erstellen
		writeMatrixAsCSVtoFile(outImage, n, step);       // in csv File scheiben
		freeMatrix(outImage,n);                          
		free(v);
		free(x);	
	}
	printf("make plot\n");
	
	makeEPSCollection(1,maxstep);

	free(image);    

    
    time(&timer2);
    double seconds = difftime(timer2,timer1);
    int hours = seconds /3600;
    seconds -= hours*3600;
    int min = seconds/60;
    int sec = seconds - min*60;
    printf("time: %d:%d:%d\n",hours,min,sec);
}
//-------------------------------------------------------------------------------------------------------------------------------
