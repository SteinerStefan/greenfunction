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
	if (argc <= 2) 
	{
		system("cat usage.txt");
		return EXIT_FAILURE;
	}

	//Bild einlesen
	image_t	*image = NULL;      //beinhaltet daten, sowie infos ueber aufbau des bildes	
	image = readimage(argv[1]); //
//--------------------------------------------------------------------
// Modus einlesen:
//--------------------------------------------------------------------
	int mode = atoi(argv[2]);
	int video = mode/1000;
	mode = mode%1000;
	printf("# --------------------------- #\n");
	printf("# Modus: %4d, Video: %4d    #\n",mode,video);

//--------------------------------------------------------------------
// Bild einlesen
//--------------------------------------------------------------------
	if(image == NULL)           
	{
		fprintf(stderr, "cannot read image\n");
		return  EXIT_FAILURE;
	}
	else printf("# Bild gefunden und geoeffnet #\n");
	printf("# width: %4d, height: %4d   #\n",image->width, image->height);
	printf("# --------------------------- #\n");
//--------------------------------------------------------------------
//Variablen
//--------------------------------------------------------------------
	int n = image->width; 		//laenge des Bildes
	int n2 = n*n;         		//Flaeche des Bildes
    int iterations = 0;   		//anzahl Iterationen
    int numthreads = 4;   		//Threads zum Brauchen
	time_t startTime;
	time(&startTime);		//timer starten
	printf("startTime: %i\n",startTime);
	char dataFolderName[fileNameLength];  //name des Ordners

//--------------------------------------------------------------------
//Iterationsschritte und Anzahl der verwendeten Threads angeben
//--------------------------------------------------------------------
	do{
	printf("iterations, numthreads:\n");
	//printf("Vorgeschlagene Iterationszahl: %d\n", image->width);
	scanf("%d %d",&iterations,&numthreads);
	fseek(stdin,0,SEEK_END); //Buffer leeren
	}while(iterations < 1);
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
	mkdir(dataFolderName,0766);                    //Ordner erstellen
//--------------------------------------------------------------------	
//Berechnung durchfuehren
//--------------------------------------------------------------------
writeTime2File(startTime,"vor Berechnung           ", dataFolderName); 
	printf("Starte Berechnung...\n");
	float* x = (float *) malloc(sizeof(float) * n2);  //x lösungsvektor für diesen Schritt
	float* v ; // =(float *) malloc(sizeof(float) * n2);  //x lösungsvektor für diesen Schritt
	int greenstep = 0;

	if (500 <= mode  && mode < 700)
	{
		int distance = mode%100;
		if (mode >= 600) distance +=100;
		printf("Mode %d: High Perfomance, every %d-th iteration wil be saved\n", mode,distance);
		v = image->data; //BildVektor
		for (int i=0; i<n2; i++) x[i] = 0;		//mit null initialisieren

		//void gaussSeidelHP(float* f, float* x, int n, int maxIterations, int numthreads, char* dataFoldName, int mode) 
		gaussSeidelHP(v, x, n,iterations,numthreads, dataFolderName, mode);
	} 
	else if ((100 <= mode  && mode < 200) || (300 <= mode  && mode < 400)) // mode 100 := last step, 
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
			for(int q = -greenstep; q<greenstep;q++)     
				for(int w = -greenstep; w<greenstep;w++) 
				{

					v[(n/2+1+q)*(n) + (n/2+1+w)] = image->data[(n/2+1+q)*(n) + (n/2+1+w)];	
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
	writeTime2File(startTime,"Generiere Plot           ", dataFolderName); 
	printf("\nGeneriere Plot:\n");

	if (500 <= mode  && mode < 700)
	{
	//int makeEPSCollectionEnum(int n, int startNumber, int stopNumber, int numthreads, char* dataFoldName, int mode)
	makeEPSCollectionEnum(n,1, iterations, numthreads, dataFolderName, mode);	
	}
	else if ((100 <= mode  && mode < 200) || (300 <= mode  && mode < 400))
	{
		if (mode == 101) // mode 101 := last step, alle Bilder ausgeben
		{
			makeEPSCollectionEnum(n,1,iterations, numthreads, dataFolderName, mode); //n = dimension der Matrix 
		}
		else if (mode == 102) // mode 102 := last step, letztes Bild ausgegben
		{
			makeEPSCollectionEnum(n,iterations,iterations, numthreads, dataFolderName, mode); //n = dimension der Matrix 
		}else 
		if ((110 <= mode  && mode < 200) || (310 <= mode  && mode < 400))
		{
			makeEPSCollectionEnum(n,1, iterations, numthreads, dataFolderName, mode);
		}
		free(image);    	
	} else
	if (200 <= mode  && mode < 300)
	{
		makeEPSCollectionEnum(n,1,n/2+1, numthreads, dataFolderName, mode); //n = dimension der Matrix 

		free(image);    
		writeTime2File(startTime,"nach gnuplot", dataFolderName); 	
	}
//--------------------------------------------------------------------
//Video erstellen
//--------------------------------------------------------------------

	if (video != 0)
	{
	writeTime2File(startTime,"erstelle Video           ", dataFolderName); 
	printf("Generiere movie\n");
	writeTime2File(startTime,"Generiere movie", dataFolderName); 	
	char moviePath[fileNameLength];  //name des Ordners

  	char str1[200];
  	char str2[40];
	char str3[100];
	snprintf(str1, 100,"avconv -i  %s/",dataFolderName);
  	strcpy (str2,"step%4d.png");
  	strncat (str1, str2, 40);
	snprintf(str3, 100," -vb 8000k  -threads %d %s/%dsteps%04diterations.mp4",numthreads,dataFolderName,n/2+1,iterations);
	strncat (str1, str3, 100);
	system(str1); //film aus png erstellen	
	writeTime2File(startTime, "Programmlaufzeit", dataFolderName); 	
	}
//	system("cat timing.txt");
	writeTime2File(startTime,"Programm ende            ", dataFolderName); 
	return EXIT_SUCCESS;
}
//-------------------------------------------------------------------------------------------------------------------------------
