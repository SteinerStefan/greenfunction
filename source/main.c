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
	image_t	*image = NULL;      //beinhaltet daten, sowie infos ueber aufbau des bildes	
	image = readimage(argv[1]); //
//--------------------------------------------------------------------
// Modus einlesen:
//--------------------------------------------------------------------
//	mode 100 := last step, 
//		mode 101 := last step, 	alle Bilder ausgeben
//		mode 102 := last step, 	letztes Bild ausgegben
//  mode 200 := all steps
//
//  mode...
//
//
//--------------------------------------------------------------------
	int mode = atoi(argv[2]);
	printf("Modus: %d\n",mode);

//--------------------------------------------------------------------
// Bild einlesen
//--------------------------------------------------------------------
	if(image == NULL)           
	{
		fprintf(stderr, "cannot read image\n");
		return  EXIT_FAILURE;
	}
	else printf("Bild gefunden und geoeffnet\n");
	printf("width: %d, height: %d\n",image->width, image->height);
//--------------------------------------------------------------------
//Variablen
//--------------------------------------------------------------------
	int n = image->width; 		//laenge des Bildes
	int n2 = n*n;         		//Flaeche des Bildes
    int iterations = 0;   		//anzahl Iterationen
    int numthreads = 4;   		//Threads zum Brauchen
 	long zeitArray[3];    		//1. gesamte Programmlaenge, 2. Berechnungslaenge, 3. Bilderstellung
	char dataFolderName[fileNameLength];  //name des Ordners
//---- Zeitmessung
    time_t timerBeginn,timerEnde;         //Timer für berechnungslaenge
    time(&timerBeginn);                   //Programm fängt an
//--------------------------------------------------------------------
//Iterationsschritte und Anzahl der verwendeten Threads angeben
//--------------------------------------------------------------------
	do{
	printf("iterations, numthreads:\n");
	//printf("Vorgeschlagene Iterationszahl: %d\n", image->width);
	scanf("%d %d",&iterations,&numthreads);
	fseek(stdin,0,SEEK_END); //Buffer leeren
	}while(iterations <= 1);
//--------------------------------------------------------------------
//Ordnerstruktur hertellen
//--------------------------------------------------------------------
	printf("Ordner erstellen\n");
	char* helpString = argv[1];
	while( strchr(helpString+1, '/') !=NULL )  // basename herausfinden
		helpString = strchr(helpString+1, '/');//
	helpString += 1;                           //
	snprintf(dataFolderName, fileNameLength, "../data_%s_%d", helpString	, iterations);
	printf("dataFolderName: %s\n", dataFolderName);
	mkdir(dataFolderName);                    //Ordner erstellen
//--------------------------------------------------------------------	
//Berechnung durchfuehren
//--------------------------------------------------------------------
	printf("Starte Berechnung...\n");
	float* x = (float *) malloc(sizeof(float) * n2);  //x lösungsvektor für diesen Schritt
	float* v ; // =(float *) malloc(sizeof(float) * n2);  //x lösungsvektor für diesen Schritt
	int greenstep = 0;

	if (100 <= mode  && mode < 200) // mode 100 := last step, 
	{
		printf("Mode %d: last step\n", mode);
		v = image->data; //BildVektor
		for (int i=0; i<n2; i++) x[i] = 0;		//mit null initialisieren		
		gaussSeidel(v,x,n,iterations,numthreads, dataFolderName, n/2+1,mode); 	// Berechnungen durchführen
	} else 
	if (200 <= mode  && mode < 300) // mode 200 := all steps
	{
		v  =(float *) malloc(sizeof(float) * n2);  			//x lösungsvektor für diesen Schritt
		for(greenstep = 0; greenstep <=n/2; greenstep++) 		
		{
			printf("Step %3d of %3d:     \n", greenstep+1, n/2+1);
			for(int i = 0; i<n2; i++) v[i] = 0;  				//Berechnung welche Pixeldaten verrechnet werden

			for(int q = -greenstep; q<=greenstep;q++)     
				for(int w= -greenstep; w<=greenstep;w++) 
				{
					v[(n/2+1+q)*(n-1) + (n/2+1+w)] = image->data[(n/2+1+q)*(n-1) + (n/2+1+w)];	
				}

			for (int i=0; i<n2; i++) 
			{
				x[i] = 0;                						// mit null initialisieren	
			}
			gaussSeidel(v,x,n,iterations,numthreads, dataFolderName, greenstep+1,mode); 	// Berechnungen durchführen
		}
	} 
free(v);
free(x);
//--------------------------------------------------------------------
		//to debug
		//for (int i=0; i<n2; i++) v[i] = 0;                //mit null initialisieren
		//v[4*n+4] = -5; 		v[4*n+5] = -5; 		v[4*n+6] = -5; 
		//v[5*n+4] = -5; 		v[5*n+5] = -0; 		v[5*n+6] = -5; 
		//v[6*n+4] = -5; 		v[6*n+5] = -5; 		v[6*n+6] = -5; 
//--------------------------------------------------------------------
//Gnuplot erstellen
//--------------------------------------------------------------------
	if (100 <= mode  && mode < 200)
	{
		printf("\nGeneriere Plot\n:");
		//---------------------------------------------------------------------------------------------------//
		// int makeEPSCollection(int n, int startNumber, int stopNummer, int numthreads, char* dataFoldName) //
		//---------------------------------------------------------------------------------------------------//
		if (mode == 101) // mode 101 := last step, alle Bilder ausgeben
		{
			makeEPSCollection(n,0,iterations, numthreads, dataFolderName); //n = dimension der Matrix 
		}
		else if (mode == 102) // mode 102 := last step, letztes Bild ausgegben
		{
			makeEPSCollection(n,iterations,iterations, numthreads, dataFolderName); //n = dimension der Matrix 
		}
		free(image);    
		time(&timerEnde);
		zeitArray[0] = difftime(timerEnde,timerBeginn);		
		plotTime(zeitArray, 0); //Programmzeit ausgeben
		writeTime2File(1, zeitArray, dataFolderName); 	
	} else
	if (200 <= mode  && mode < 300)
	{
		printf("\nGeneriere Plot\n:");
		//---------------------------------------------------------------------------------------------------//
		// int makeEPSCollection(int n, int startNumber, int stopNummer, int numthreads, char* dataFoldName) //
		//---------------------------------------------------------------------------------------------------//
		makeEPSCollection(n,1,n/2+1, numthreads, dataFolderName); //n = dimension der Matrix 
		free(image);    
		time(&timerEnde);
		zeitArray[0] = difftime(timerEnde,timerBeginn);		
		plotTime(zeitArray, 0); //Programmzeit ausgeben
		writeTime2File(1, zeitArray, dataFolderName); 
	}
//--------------------------------------------------------------------
/*
	printf("Generiere Plot\n:");	
//	makeEPSCollection(0,n/2+1, numthreads, dataFolderName); 
	makeEPSCollection(n,0,iterations, numthreads, dataFolderName); //n = dimension der Matrix 
	free(image);    
	//printf("Generiere movie\n");
    //system("avconv -i ../data/step%4d.png -b:v 1000k ../data/greenmovie.mp4"); //film aus png erstellen

	//Berechnung und Ausgabe der Zeit
    time(&timerEnde);
	zeitArray[0] = difftime(timerEnde,timerBeginn);		
    plotTime(zeitArray, 0); //Programmzeit ausgeben
    writeTime2File(1, zeitArray, dataFolderName); 	
	*/
	return EXIT_SUCCESS;
}
//-------------------------------------------------------------------------------------------------------------------------------
