//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : GaussSeidel.h
//  Author  : Andreas Linggi
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------
#ifndef Gauss_Seidel_GaussSeidel_h
#define Gauss_Seidel_GaussSeidel_h

//-------------------------------------------------------------------------------------------------------------------------------
// gaussSeidel for high performance and low data storage: mode 500-700
//-------------------------------------------------------------------------------------------------------------------------------
void gaussSeidelHP(float* f, float* x, int n, int maxIterations, int numthreads, char* dataFoldName, int mode);


//-------------------------------------------------------------------------------------------------------------------------------
// gaussSeidel for vizualize grenns function
//-------------------------------------------------------------------------------------------------------------------------------
void gaussSeidelGF(float* f, float* x, int n, int maxIterations, int numthreads, char* dataFoldName, int greenstep);


// f and x have the same size n^2
void gaussSeidel(float* f, float* x, int n, int iter, int numthreads, char* dataFoldName, int greenstep, int mode); 

#endif
//-------------------------------------------------------------------------------------------------------------------------------

