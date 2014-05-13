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
	int n2 = n*n; //Flaeche n^2

	if (100 <= mode  && mode < 400) // all modes
	{	
		for(int step = 1; step <= iter; step++)	
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

			//printf("\nIteration %3d of %3d: Give out f[%d, %d] = %f ",step,iter, testx,testy,f[testx*n+testy]); 
			//printf("     %4.0f\n", x[(testy-1)*n + testx]);
			//printf("%4.0f %4.0f %4.0f\n",x[testy*n + (testx-1)], x[testy*n + testx], x[(testy)*n + testx]);
			//printf("     %4.0f\n", x[(testy+1)*n + testx]);
			printf("Iteration %3d of %3d\r",step,iter); 

			//printf("write to csv\r\r");
			outImage = newMatrixFromVector(x,n);     // Matrix aus Vector erstellen
			writeMatrixAsCSVtoFile(outImage, n, step, dataFoldName);  // in csv File scheiben
			freeMatrix(outImage,n);                  		    
		}   
	}
}
//-------------------------------------------------------------------------------------------------------------------------------


