//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : GaussSeidel.h
//  Author  : Andreas Linggi
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------
#ifndef Gauss_Seidel_GaussSeidel_h
#define Gauss_Seidel_GaussSeidel_h

// f and x have the same size n^2
void gaussSeidel(float* f, float* x, int n, int iter, int numthreads, char* dataFoldName, int greenstep, int mode); 

#endif
//-------------------------------------------------------------------------------------------------------------------------------

