//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : GaussSeidel.c
//  Author  : Andreas Linggi, Stefan Steiner
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------
#define testx 5
#define testy 5
#define tf 4 // tf = 4/h^2, normalerweise 4
#define timePrintf 100
//-------------------------------------------------------------------------------------------------------------------------------
//includes
//-------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "stdlib.h"
#include "GaussSeidel.h"
#include <omp.h>
#include "matrix2csv.h"
#include "visuGreFuc.h"
//-------------------------------------------------------------------------------------------------------------------------------
// gaussSeidel for high performance and low data storage: mode 500-700
//-------------------------------------------------------------------------------------------------------------------------------
void gaussSeidelHP(float* f, float* x, int n, int maxIterations, int numthreads, char* dataFoldName, int mode) 
{
	// pruefen auf mode
	if (mode >= 700 || mode <= 500) return;

	//Variablen
	float** outImage;
	int n2 = n*n; //Flaeche n^2
	int a = 0;
	int b = maxIterations/timePrintf;

	// distance zwischen iterationen, die abgespeichert werden
	int distance = mode%100;
	if (mode >= 600) distance +=100;
	
	// berechnung	
	for(int iteration = 1; iteration <= maxIterations; iteration++)	
	{
			#pragma omp parallel for num_threads(numthreads)
			for(int u = 0; u < n2; u++)
			{	
				if(u < n || u > n2-n || u % n == 0 || u %n == n-1)
				{
				   if(u > 0 && u < n-1)                             x[u]    = (f[u]      - x[u+1] - x[u-1] -x[u+n]) /-tf;//oben
				   else if(u > n2-n && u <n2-1)                     x[u]    = (f[u]      - x[u+1] - x[u-1] - x[u-n])/-tf;  //unten
				   else if(u % n == 0 && u >= n && u <= (n2-2*n))   x[u]    = (f[u]      - x[u+1] - x[u-n] - x[u+n])/-tf;   //links
				   else if(u %n == n-1 && u >=2*n-1 && u <= n2-n-1) x[u]    = (f[u]      - x[u-1] - x[u-n] - x[u+n])/-tf;//rechts
				   else if(u == 0)                                  x[0]    = (f[0]      - x[1]   - x[n])           /-tf; //links oben
				   else if(u == n2-n)                               x[n2-n] = (f[n2-1-n] - x[n2-n]- x[n2-2*n-1] )   /-tf;//links unten
				   else if(u == n-1)                                x[n-1]  = (f[n-1]    - x[n-2] - x[2*n-1])       /-tf; // rechts oben
				   else                                             x[n2-1] = (f[n2-1]   - x[n2-2]- x[n2-n-1])      /-tf;// rechts unten
				}
				else x[u] = (f[u] - x[u+1] - x[u-1] - x[u-n] -x[u+n])/-tf;//punkt ist nicht am rand!!
			}
		a++;
		if (a == b)
		{
			printf("Iteration %3d of %3d\r",iteration,maxIterations); 
			a=0;
		}
		// iteration abgeschlossen
		//------------------------------------------------------------------------
		// um datenflut zu verhindern wird nur die benoetigten daten abgespeichert
		if (iteration%distance == 0 || iteration==1 || iteration == maxIterations)
		{
			outImage = newMatrixFromVector(x,n);     						// Matrix aus Vector erstellen
			writeMatrixAsCSVtoFile(outImage, n, iteration, dataFoldName);  	// in csv File scheiben
			freeMatrix(outImage,n);  		
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
// gaussSeidel for vizualize grenns function
//-------------------------------------------------------------------------------------------------------------------------------
void gaussSeidelGF(float* f, float* x, int n, int maxIterations, int numthreads, char* dataFoldName, int greenstep) 
{
	//Variablen
	float** outImage;
	int n2 = n*n; //Flaeche n^2
	// berechnung	
	int a = 0;
	int b = maxIterations/timePrintf;
	for(int iteration = 1; iteration <= maxIterations; iteration++)	
	{
			#pragma omp parallel for num_threads(numthreads)
			for(int u = 0; u < n2; u++)
			{	
				if(u < n || u > n2-n || u % n == 0 || u %n == n-1)
				{
				   if(u > 0 && u < n-1)                             x[u]    = (f[u]      - x[u+1] - x[u-1] -x[u+n]) /-tf;//oben
				   else if(u > n2-n && u <n2-1)                     x[u]    = (f[u]      - x[u+1] - x[u-1] - x[u-n])/-tf;  //unten
				   else if(u % n == 0 && u >= n && u <= (n2-2*n))   x[u]    = (f[u]      - x[u+1] - x[u-n] - x[u+n])/-tf;   //links
				   else if(u %n == n-1 && u >=2*n-1 && u <= n2-n-1) x[u]    = (f[u]      - x[u-1] - x[u-n] - x[u+n])/-tf;//rechts
				   else if(u == 0)                                  x[0]    = (f[0]      - x[1]   - x[n])           /-tf; //links oben
				   else if(u == n2-n)                               x[n2-n] = (f[n2-1-n] - x[n2-n]- x[n2-2*n-1] )   /-tf;//links unten
				   else if(u == n-1)                                x[n-1]  = (f[n-1]    - x[n-2] - x[2*n-1])       /-tf; // rechts oben
				   else                                             x[n2-1] = (f[n2-1]   - x[n2-2]- x[n2-n-1])      /-tf;// rechts unten
				}
				else x[u] = (f[u] - x[u+1] - x[u-1] - x[u-n] -x[u+n])/-tf;//punkt ist nicht am rand!!
			}
		a++;
		if (a == b)
		{
			printf("Iteration %3d of %3d\r",iteration,maxIterations); 
			a=0;
		}
		// iteration abgeschlossen

	}
	outImage = newMatrixFromVector(x,n);     						// Matrix aus Vector erstellen
	writeMatrixAsCSVtoFile(outImage, n, greenstep, dataFoldName);  	// in csv File scheiben
	freeMatrix(outImage,n);  		

}
//-------------------------------------------------------------------------------------------------------------------------------
// gaussSeidel
//-------------------------------------------------------------------------------------------------------------------------------
void gaussSeidel(float* f, float* x, int n, int iter, int numthreads, char* dataFoldName, int greenstep, int mode) 
{//		x[l] = (f[l] - x[l+1] - x[l-1] - x[l-n] -x[l+n])/4;
	//Variablen
	float** outImage;
	int n2 = n*n; //Flaeche n^2
//printf("#########################\n start: \n mode: %d; iter: %d\n",mode,iter);
	if (100 <= mode  && mode < 400) // all modes
	{	
		for(int iterations = 1; iterations <= iter; iterations++)	
		{

			#pragma omp parallel for num_threads(numthreads)
			for(int u = 0; u < n2; u++)
			{	
				if(u < n || u > n2-n || u % n == 0 || u %n == n-1)
				{
				   if(u > 0 && u < n-1)                             x[u]    = (f[u]      - x[u+1] - x[u-1] -x[u+n]) /-tf;//oben
				   else if(u > n2-n && u <n2-1)                     x[u]    = (f[u]      - x[u+1] - x[u-1] - x[u-n])/-tf;  //unten
				   else if(u % n == 0 && u >= n && u <= (n2-2*n))   x[u]    = (f[u]      - x[u+1] - x[u-n] - x[u+n])/-tf;   //links
				   else if(u %n == n-1 && u >=2*n-1 && u <= n2-n-1) x[u]    = (f[u]      - x[u-1] - x[u-n] - x[u+n])/-tf;//rechts
				   else if(u == 0)                                  x[0]    = (f[0]      - x[1]   - x[n])           /-tf; //links oben
				   else if(u == n2-n)                               x[n2-n] = (f[n2-1-n] - x[n2-n]- x[n2-2*n-1] )   /-tf;//links unten
				   else if(u == n-1)                                x[n-1]  = (f[n-1]    - x[n-2] - x[2*n-1])       /-tf; // rechts oben
				   else                                             x[n2-1] = (f[n2-1]   - x[n2-2]- x[n2-n-1])      /-tf;// rechts unten
				}
				else x[u] = (f[u] - x[u+1] - x[u-1] - x[u-n] -x[u+n])/-tf;//punkt ist nicht am rand!!
			}		
			//printf("\nIteration %3d of %3d: Give out f[%d, %d] = %f ",iterations,iter, testx,testy,f[testx*n+testy]); 
			//printf("     %4.0f\n", x[(testy-1)*n + testx]);
			//printf("%4.0f %4.0f %4.0f\n",x[testy*n + (testx-1)], x[testy*n + testx], x[(testy)*n + testx]);
			//printf("     %4.0f\n", x[(testy+1)*n + testx]);
			printf("Iteration %3d of %3d\r",iterations,iter); 
//printf("\n mode: %d; Iteration: %d\n\n\n\n",mode,iterations);return;


			if(mode < 200 || mode >= 300) 
			{
				//printf("write to csv\r\r");
				outImage = newMatrixFromVector(x,n);     // Matrix aus Vector erstellen
				writeMatrixAsCSVtoFile(outImage, n, iterations, dataFoldName);  // in csv File scheiben
				freeMatrix(outImage,n);                  		    
			}
		}   
	}

	if(mode <= 200 && mode < 300) 
	{
		printf("write to csv\r\r");
		outImage = newMatrixFromVector(x,n);     // Matrix aus Vector erstellen
		writeMatrixAsCSVtoFile(outImage, n, greenstep, dataFoldName);  // in csv File scheiben
		freeMatrix(outImage,n);                  		    
	}

}
//-------------------------------------------------------------------------------------------------------------------------------
