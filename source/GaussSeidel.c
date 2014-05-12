//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : GaussSeidel.c
//  Author  : Andreas Linggi, Stefan Steiner
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------
#define testx 5
#define testy 5
#define tf 4 // tf = 4/h^2, normalerweise 4
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
// gaussSeidel
//-------------------------------------------------------------------------------------------------------------------------------
void gaussSeidel(float* f, float* x, int n, int iter, int numthreads, char* dataFoldName, int greenstep, int mode) 
{//		x[l] = (f[l] - x[l+1] - x[l-1] - x[l-n] -x[l+n])/4;
	
	//Variablen
	float** outImage;
//	n = 11;//test
	int n2 = n*n; //Flaeche n^2
	//float* help = (float *) malloc(sizeof(float) * n2); //x lösungsvektor für diesen Schritt
//	free(f);//test
//	f = (float *) malloc(sizeof(float) * n2); //test
//	for(int u = 0; u< n2; u++) f[u] = 0;//test
//	f[2*n + 5] = 10;//test
//	f[8*n + 5] = 10;//test
//	printf("x gelesen: %d\n",x[5]);
//	printf("f gelesen: %d\n", f[5]);

//--------------------------------------------------------------------	
// ??????????????????
//--------------------------------------------------------------------
/*
	outImage = newMatrixFromVector(x,n);     // Matrix aus Vector erstellen
	writeMatrixAsCSVtoFile(outImage, n, 0, dataFoldName);  // in csv File scheiben
	freeMatrix(outImage,n);                  		    
	*/
//--------------------------------------------------------------------	
// Gauss-Seidel
//--------------------------------------------------------------------
if (200 <= mode  && mode < 300) // mode 200 := all steps
{	
		for(int iteration = 1; iteration <= iter; iteration++)
		{
			//Ecken
			x[0] = (f[0] - x[1] - x[n])/-tf;         //links oben
			x[n-1] = (f[n-1] - x[n-2] - x[2*n-1])/-tf; //rechts oben
			x[n2- n] =(f[n2-1-n] - x[n2-n] - x[n2-2*n-1] )/-tf; //links unten
			x[n2-1] = (f[n2-1]  - x[n2-2] - x[n2-n-1])/-tf; //rechts unten

			//Rand
			#pragma omp parallel for num_threads(numthreads)
			for(int l = 1; l<(n-1); l++)//oben
				x[l] = (f[l] - x[l+1] - x[l-1] -x[l+n])/-tf;
			#pragma omp parallel for num_threads(numthreads)
			for(int l = n2-n+1; l<n2-1; l++)//unten
				x[l] = (f[l] - x[l+1] - x[l-1] - x[l-n])/-tf;
			#pragma omp parallel for num_threads(numthreads)
			for(int l = n; l<=(n2-2*n); l+=n)//links
				x[l] = (f[l] - x[l+1] - x[l-n] - x[l+n])/-tf;
			#pragma omp parallel for num_threads(numthreads)	
			for(int l = (2*n-1); l<= (n2-n-1); l+=n)//rechts
				x[l] = (f[l]  - x[l-1] - x[l-n] - x[l+n])/-tf;

			//Rest
			#pragma omp parallel for num_threads(numthreads)
			for(int u = n+1; u <(n2-n-2); u+=n)
			{
				for(int l = u;l < u+n-2; l++) 
				{		
					//if(x!=0) schneller???
					x[l] = (f[l] - x[l+1] - x[l-1] - x[l-n] -x[l+n])/-tf;
				}
			}
		}
		outImage = newMatrixFromVector(x,n);     // Matrix aus Vector erstellen
		writeMatrixAsCSVtoFile(outImage, n, greenstep, dataFoldName);  // in csv File scheiben
		freeMatrix(outImage,n);     		
} else
if (100 <= mode  && mode < 200) // mode 100 := last step
	for(int step = 1; step <= iter; step++)	
	{
		if(step == 1)
		{
			//outImage = newMatrixFromVector(x,n);     // Matrix aus Vector erstellen
			//writeMatrixAsCSVtoFile(outImage, n, step, dataFoldName);  // in csv File scheiben
			//freeMatrix(outImage,n);                  		    

			//printf("\nIteration %3d of %3d: Give out f[%d, %d] = %f \n",step,iter, testx,testy,f[testx*n+testy]); 
			//printf("     %4.0f\n", x[(testy-1)*n + testx]);
			//printf("%4.0f %4.0f %4.0f\n",x[testy*n + (testx-1)], x[testy*n + testx], x[(testy)*n + testx]);
			//printf("     %4.0f\n", x[(testy+1)*n + testx]);

		}
		//Ecken
		x[0] = (f[0] - x[1] - x[n])/-tf;         //links oben
		x[n-1] = (f[n-1] - x[n-2] - x[2*n-1])/-tf; //rechts oben
		x[n2- n] =(f[n2-1-n] - x[n2-n] - x[n2-2*n-1] )/-tf; //links unten
		x[n2-1] = (f[n2-1]  - x[n2-2] - x[n2-n-1])/-tf; //rechts unten

		//Rand
		#pragma omp parallel for num_threads(numthreads)
		for(int l = 1; l<(n-1); l++)//oben
			x[l] = (f[l] - x[l+1] - x[l-1] -x[l+n])/-tf;
		#pragma omp parallel for num_threads(numthreads)
		for(int l = n2-n+1; l<n2-1; l++)//unten
			x[l] = (f[l] - x[l+1] - x[l-1] - x[l-n])/-tf;
		#pragma omp parallel for num_threads(numthreads)
		for(int l = n; l<=(n2-2*n); l+=n)//links
			x[l] = (f[l] - x[l+1] - x[l-n] - x[l+n])/-tf;
		#pragma omp parallel for num_threads(numthreads)	
		for(int l = (2*n-1); l<= (n2-n-1); l+=n)//rechts
			x[l] = (f[l]  - x[l-1] - x[l-n] - x[l+n])/-tf;

		//Rest
		#pragma omp parallel for num_threads(numthreads)
		for(int u = n+1; u <(n2-n-2); u+=n)
		{
			for(int l = u;l < u+n-2; l++) 
			{		
				//if(x!=0) schneller???
				x[l] = (f[l] - x[l+1] - x[l-1] - x[l-n] -x[l+n])/-tf;
			}
		}

		//for(int u = 0; u <n2; u++)x[u] = help[u];
			
//			printf("\nIteration %3d of %3d: Give out f[%d, %d] = %f \n",step,iter, testx,testy,f[testx*n+testy]); 
			printf("Iteration %3d of %3d\r",step,iter); 
/*
			printf("     %4.0f\n", x[(testy-1)*n + testx]);
			printf("%4.0f %4.0f %4.0f\n",x[testy*n + (testx-1)], x[testy*n + testx], x[(testy)*n + testx]);
			printf("     %4.0f\n", x[(testy+1)*n + testx]);*/
			//printf("write to csv\r\r");
			outImage = newMatrixFromVector(x,n);     // Matrix aus Vector erstellen
			writeMatrixAsCSVtoFile(outImage, n, step, dataFoldName);  // in csv File scheiben
			freeMatrix(outImage,n);                  		    
		}
	
//	printf("write to csv\n");
//	outImage = newMatrixFromVector(x,n);     // Matrix aus Vector erstellen
//	writeMatrixAsCSVtoFile(outImage, n, greenstep, dataFoldName);  // in csv File scheiben
//	freeMatrix(outImage,n);                  		    

   
}
//-------------------------------------------------------------------------------------------------------------------------------


