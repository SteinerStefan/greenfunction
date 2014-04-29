//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : visuGreFuc.h
//  Author  : Andreas Linggi, Stefan Steiner
//  Function: Algorithms to visualize green's function
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------
#ifndef visuGreFuc_h
#define visuGreFuc_h

//print the coefficient matrix with size n^2 x n^2
void printCoefficientMatrix(int n);

//print a matrix with nxn
void printMatrix(float** M, int n);
void printVector(float* v,int n);

//make a new vector of the coloums of matrix F
float* newVectorFromMatrixSpecial(float** F, int step, int n);

//make a new Matrix from a vector with size n^2
float** newMatrixFromVector(float* v, int n);

//return a new Matrix initiated with f
float** newMatrix(int n, float f);

//free an allocated matrix
void freeMatrix(float ** M, int n);

//adds matrix A2 to A1
void addMatrixElementWise(float** A1, float** A2, int n);

#endif
//-------------------------------------------------------------------------------------------------------------------------------


