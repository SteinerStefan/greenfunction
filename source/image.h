//-------------------------------------------------------------------------------------------------------------------------------
//  Projekt : greenfunction
//  file    : image.h
//  Author  : Dr Andreas Mueller
//  Copyright (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
//-------------------------------------------------------------------------------------------------------------------------------
#ifndef _image_h
#define _image_h
//typedef, beinhaltet daten und informationen ueber breite und hoehe
typedef struct {
	int	width;
	int	height;
	float	*data;
} image_t;

//liest ein fitsfile und speichert es in ein image_t struct.
extern image_t	*readimage(const char *filename);
//schreibt ein image_t in ein file
extern void	writeimage(const image_t *image, const char *filename);
#endif /* _image_h */
//-------------------------------------------------------------------------------------------------------------------------------







