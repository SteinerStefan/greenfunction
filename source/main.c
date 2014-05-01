//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : main.c
//  Author  : Andreas Linggi, Stefan Steiner
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------------------------------------
#define fileNameLength 100
//-------------------------------------------------------------------------------------------------------------------------------
//Includes
//-------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <fitsio.h>
#include <time.h>
#include "GaussSeidel.h"
#include "../gnuplot/gnuplot.h"
#include "image.h"
#include "timeinfo.h"
//#include <string.h> 

//-------------------------------------------------------------------------------------------------------------------------------
//main
//-------------------------------------------------------------------------------------------------------------------------------
int main(int argc, const char * argv[])
{
	//Bild einlesen
	image_t	*image = NULL;      //beinhaltet daten, sowie infos über aufbau des bildes	
	image = readimage(argv[1]); 
	if(image == NULL)
	{
		fprintf(stderr, "cannot read image\n");
		return  EXIT_FAILURE;
	}
	else printf("Bild gefunden und geoeffnet\n");

	printf("width: %d, height: %d\n",image->width, image->height);
	//Variablen
	int n = image->width; //laenge des Bildes
	int n2 = n*n;         //Flaeche des Bildes
    int iterations = 0;   
    int numthreads = 8;
	long zeitArray[3]; //1. gesamte Programmlaenge, 2. Berechnungslaenge, 3. Bilderstellung
	char dataFolderName[fileNameLength];//[fileNameLength]; //name des Ordners
    time_t timerBeginn,timerEnde; //Timer für berechnungslänge
    time(&timerBeginn);

	//Iterationsschritte und Anzahl der verwendeten Threads angeben
	do{
	printf("iterations, numthreads:\n");
	//printf("Vorgeschlagene Iterationszahl: %d\n", image->width);
	scanf("%d %d",&iterations,&numthreads);
	fseek(stdin,0,SEEK_END); //Buffer leeren
	}while(iterations<=1 && iterations>1000);

	printf("Ordner erstellen\n");

	char* helpString = argv[1];

	while( strchr(helpString+1, '/') !=NULL )  // basename herausfinden
		helpString = strchr(helpString+1, '/');
	helpString += 1;

	printf("basename: %s\n", helpString );	
	snprintf(dataFolderName, fileNameLength, "../dada_%s_%d", helpString	, iterations);
	printf("dataFolderName: %s\n", dataFolderName);
	mkdir(dataFolderName); //Ordner erstellen

	printf("Starte Berechnung...\n");	
	//Berechnung durchfuehren
//    int maxstep = n/2+1; //n/2+1
//	int startstep =  n/2+1;
//	for (int step = startstep; step<=maxstep; step++) 
//	{
		float* v = image->data; //BildVektor
//float* newVectorFromMatrixSpecial(float** F, int step, int n)
		float* x = (float *) malloc(sizeof(float) * n2); //x lösungsvektor für diesen Schritt
		for (int i=0; i<n2; i++) x[i] = 0;               //mit null initialisieren
		gaussSeidel(v,x,n,iterations,numthreads, dataFolderName); 		 // Berechnungen durchführen
		free(v);
		free(x);	
//	}
	
	//Gnuplot erstellen
	printf("Generiere Plot\n:");	
	makeEPSCollection(0,iterations-1, numthreads, dataFolderName); 
	free(image);    
	printf("Generiere movie\n");
    //system("avconv -i ../data/step%4d.png -b:v 1000k ../data/greenmovie.mp4"); //film aus png erstellen

	//Berechnung und Ausgabe der Zeit
    time(&timerEnde);
	zeitArray[0] = difftime(timerEnde,timerBeginn);		
			
	
    	printf("Programmlaufzeit: %d:%d:%d\n",getHours(zeitArray[0]),getMinutes(zeitArray[0]), getSeconds(zeitArray[0]) );

	char path[fileNameLength];
	FILE *timedatei;	
	snprintf(path, fileNameLength, "%s/timing.txt", dataFolderName);	
	timedatei = fopen(path, "w"); //writing	
    if (timedatei == NULL)
    {
        printf("Fehler beim oeffnen der Datei.");
        return EXIT_FAILURE;
    }
 	fprintf(timedatei, "Programmlaufzeit: %d:%d:%d\n",getHours(zeitArray[0]),getMinutes(zeitArray[0]), getSeconds(zeitArray[0]) );
	fclose(timedatei);
	return EXIT_SUCCESS;
}
//-------------------------------------------------------------------------------------------------------------------------------
