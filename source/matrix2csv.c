//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : matrix2csv.c
//  Author  : Andreas Linggi, Stefan Steiner
//  Copyright (c) 2014 Andreas Linggi. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------------------------------------
#define fileNameLength 100
//-------------------------------------------------------------------------------------------------------------------------------
//includes
//-------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
//-------------------------------------------------------------------------------------------------------------------------------
//writeMatrixAsCSVtoFile
//-------------------------------------------------------------------------------------------------------------------------------
int writeMatrixAsCSVtoFile(float** A, int n, int step, char* dataFoldName)
{
    FILE *datei;
	char path[fileNameLength];

	snprintf(path, fileNameLength, "%s/step%04d.csv",dataFoldName, step); //path -> CSV

    datei = fopen (path, "w"); //writing 
    if (datei == NULL)
    {
        printf("Fehler beim oeffnen der Datei.");
        return EXIT_FAILURE;
    }
    for (int i =0; i<n; i++) 
	{
        for (int j =0; j<n; j++) 
		{
            fprintf (datei, "%5.1f",A[i][j]);
            if (!(n-1==j)) 
			{
                fprintf (datei, ", ");
            }
			else {fprintf (datei, "\n");}
        }
    }
    fclose (datei);
    return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------
