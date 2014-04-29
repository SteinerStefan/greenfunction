//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : GaussSeidel.c
//  Author  : Andreas Linggi
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------

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
void gaussSeidel(float* f, float* x, int n, int iter, int numthreads) {
// f and x have the same size n^2    
    int dim = n*n;
    int i,j,k;
    float sum;
    
    for (k = 0; k<iter; k++) {
		printf("Iteration %3d of %3d:\n",k+1,iter);
		#pragma omp parallel for num_threads(numthreads)
        for (i = 0; i < dim; i++)
		{
            // sum of the i. row without i=j
            sum = 0;
            for (j = 0; j<dim; j++) 
			{  
                if (i>j) 
				{
                    if ((i-n==j)) // Einheitsmatrix links von i=j
					{		
                    	sum += x[j];
                    }
					else if((i-1==j)) // der Wert links von j=i
					{		
                    	if ((i)%n != 0)  sum += x[j]; // aber nicht der n. Wert
                    }
				} 
				else if (i<j) 
				{
                    if ((j-n==i))// Einheitsmatrix rechts von i=j 
					{
                        sum += x[j];
                    } 
					else if ((j-1==i)) // der n. Wert links von j=i
					{		
                    	if ((i+1)%n != 0) sum += x[j]; // aber nicht der n. Wert
                    }

                } // else i=j
            }
	        x[i] = (f[i]-sum)/-4;
        }
		float** outImage;
		outImage = newMatrixFromVector(x,n);     // Matrix aus Vector erstellen
		writeMatrixAsCSVtoFile(outImage, n, k);  // in csv File scheiben
		freeMatrix(outImage,n);                  		    
    }
}
//-------------------------------------------------------------------------------------------------------------------------------


