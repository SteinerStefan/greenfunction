//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : image.c
//  Author  : Dr Andreas Mueller
//  Copyright (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <fitsio.h>
#include "image.h"
//-------------------------------------------------------------------------------------------------------------------------------
//globale variablen
//-------------------------------------------------------------------------------------------------------------------------------
extern int	debug;
//-------------------------------------------------------------------------------------------------------------------------------
// readimage
//-------------------------------------------------------------------------------------------------------------------------------
image_t	*readimage(const char *filename) 
{
	char	errmsg[80];
	image_t	*image = NULL;
	// open the FITS file
	int	status = 0;
	fitsfile	*fits = NULL;
	if (fits_open_file(&fits, filename, READONLY, &status)) 
	{
		fits_get_errstatus(status, errmsg);
		fprintf(stderr, "cannot read fits file %s: %s\n",
			filename, errmsg);
		return NULL;
	}
	// find dimensions of pixel array
	int	igt;
	int	naxis;
	long	naxes[3];
	if (fits_get_img_param(fits, 3, &igt, &naxis, naxes, &status)) 
	{
		fits_get_errstatus(status, errmsg);
		fprintf(stderr, "cannot read fits file info: %s\n", errmsg);
		goto bad;
	}
	image = (image_t *)malloc(sizeof(image_t));
	image->width = naxes[0];
	image->height = naxes[1];
	// read the pixel data
	long	npixels = image->width * image->height;
	image->data = (float *)malloc(npixels * sizeof(double)); //double oder float!!!
	long	firstpixel[3] = { 1, 1, 1 };
	if (fits_read_pix(fits, TFLOAT, firstpixel, npixels, NULL, image->data,
		NULL, &status)) {
		fits_get_errstatus(status, errmsg);
		fprintf(stderr, "cannot read pixel data: %s\n", errmsg);
		goto bad;
	}

	// close the file
	if (fits_close_file(fits, &status)) {
		fits_get_errstatus(status, errmsg);
		fprintf(stderr, "cannot close fits file %s: %s\n",
			filename, errmsg);
		goto bad;
	}
	//gaussSeidel needs negative input
	for(long i = 0;i < (image->width*image->height); i++)
			image->data[i] = - (image->data[i] * 10E-6);
	//return the image

	return image;
bad:
	if (image) {
		if (image->data) {
			free(image->data);
		}
		free(image);
	}
	if (fits) {
		fits_close_file(fits, &status);
	}
	return NULL;
}
//-------------------------------------------------------------------------------------------------------------------------------
//writeImage
//-------------------------------------------------------------------------------------------------------------------------------
void	writeimage(const image_t *image, const char *filename) {
	char	errmsg[80];
	int	status = 0;
	fitsfile	*fits = NULL;
	if (fits_create_file(&fits, filename, &status)) {
		fits_get_errstatus(status, errmsg);
		fprintf(stderr, "cannot create fits file %s: %s\n",
			filename, errmsg);
		return;
	}

	// find dimensions of pixel array
	int	naxis = 2;
	long	naxes[2] = { image->width, image->height };
	if (fits_create_img(fits, FLOAT_IMG, naxis, naxes, &status)) {
		fits_get_errstatus(status, errmsg);
		fprintf(stderr, "cannot create image: %s\n", errmsg);
		goto bad;
	}

	// read the pixel data
	long	npixels = image->width * image->height;
	long	firstpixel[3] = { 1, 1, 1 };
	if (fits_write_pix(fits, TDOUBLE, firstpixel, npixels,
			image->data, &status)) {
		fits_get_errstatus(status, errmsg);
		fprintf(stderr, "cannot write pixel data: %s\n", errmsg);
	}

bad:
	// close the file
	if (fits_close_file(fits, &status)) {
		fits_get_errstatus(status, errmsg);
		fprintf(stderr, "cannot close fits file %s: %s\n",
			filename, errmsg);
		goto bad;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
