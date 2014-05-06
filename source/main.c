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
#include <stdlib.h>
#include <stdio.h>
#include <fitsio.h>
#include <time.h>
#include "GaussSeidel.h"
#include "../gnuplot/gnuplot.h"
#include "image.h"
#include "timeinfo.h"
#include <string.h>

//-------------------------------------------------------------------------------------------------------------------------------
//main
//-------------------------------------------------------------------------------------------------------------------------------
int main(int argc, const char * argv[])
{
	//Bild einlesen
	image_t	*image = NULL;      //beinhaltet daten, sowie infos über aufbau des bildes	
	image = readimage(argv[1]); //
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
    int iterations = 0;   //anzahl Iterationen
    int numthreads = 4;   //Threads zum Brauchen
 	long zeitArray[3];    //1. gesamte Programmlaenge, 2. Berechnungslaenge, 3. Bilderstellung
	char dataFolderName[fileNameLength];  //name des Ordners
    time_t timerBeginn,timerEnde;         //Timer für berechnungslänge
    time(&timerBeginn);                   //Programm fängt an

	//Iterationsschritte und Anzahl der verwendeten Threads angeben
	do{
	printf("iterations, numthreads:\n");
	//printf("Vorgeschlagene Iterationszahl: %d\n", image->width);
	scanf("%d %d",&iterations,&numthreads);
	fseek(stdin,0,SEEK_END); //Buffer leeren
	}while(iterations <= 1);

	//Ordnerstruktur hertellen
	printf("Ordner erstellen\n");
	char* helpString = argv[1];
	while( strchr(helpString+1, '/') !=NULL )  // basename herausfinden
		helpString = strchr(helpString+1, '/');//
	helpString += 1;                           //
	snprintf(dataFolderName, fileNameLength, "../data_%s_%d", helpString	, iterations);
	printf("dataFolderName: %s\n", dataFolderName);
	mkdir(dataFolderName);                    //Ordner erstellen
	
	

	//Berechnung durchfuehren
	printf("Starte Berechnung...\n");
	float* x = (float *) malloc(sizeof(float) * n2);  //x lösungsvektor für diesen Schritt
	float* v ; // =(float *) malloc(sizeof(float) * n2);  //x lösungsvektor für diesen Schritt
	int greenstep = 0;	
//	for(greenstep = 0; greenstep <=n/2; greenstep++) //Greensche Funktion 
	{
		printf("Step %d of %d\n", greenstep+1, n/2+1);	
		//Berechnung welche Pixeldaten verrechnet werden
//		for(int i = 0; i<n2; i++)v[i] = 0	;
//		for(int q = -greenstep; q<=greenstep;q++)     
//			for(int w= -greenstep; w<=greenstep;w++)
//				v[(n/2+1+q)*n + (n/2+1+w)] = image->data[(n/2+1+q)*n + (n/2+1+w)];
		v = image->data; //BildVektor
		for (int i=0; i<n2; i++) x[i] = 0;                //mit null initialisieren
		
		//to debug
		//for (int i=0; i<n2; i++) v[i] = 0;                //mit null initialisieren
		//v[4*n+4] = -5; 		v[4*n+5] = -5; 		v[4*n+6] = -5; 
		//v[5*n+4] = -5; 		v[5*n+5] = -0; 		v[5*n+6] = -5; 
		//v[6*n+4] = -5; 		v[6*n+5] = -5; 		v[6*n+6] = -5; 

		
		gaussSeidel(v,x,n,iterations,numthreads, dataFolderName, greenstep); 	// Berechnungen durchführen
	for (int i=0; i<n2; i++)
	{
		printf("%2.3f ",x[i]);                //ausgeben
		if((i+1)%n == 0) printf("\n"); 
	}
		
	}
	free(v);
	free(x);


	//Gnuplot erstellen
	printf("Generiere Plot\n:");	
//	makeEPSCollection(0,n/2+1, numthreads, dataFolderName); 
	makeEPSCollection(0,iterations, numthreads, dataFolderName); 
	free(image);    
	//printf("Generiere movie\n");
    //system("avconv -i ../data/step%4d.png -b:v 1000k ../data/greenmovie.mp4"); //film aus png erstellen

	//Berechnung und Ausgabe der Zeit
    time(&timerEnde);
	zeitArray[0] = difftime(timerEnde,timerBeginn);		
    plotTime(zeitArray, 0); //Programmzeit ausgeben
    writeTime2File(1, zeitArray, dataFolderName); 	
	
	return EXIT_SUCCESS;
}
//-------------------------------------------------------------------------------------------------------------------------------
