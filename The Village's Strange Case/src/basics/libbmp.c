/************************************************************************/
/*                                                                      */
/* Descripción: Librería para la manipulación de imagenes en bitmap     */
/*                                                                      */
/* Archivo: libbmp.c                                                    */
/* Autor: https://github.com/marc-q/libbmp                              */
/*                                                                      */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "libbmp.h"

/* BMP_HEADER */

void bmp_header_init_df (bmp_header *header, const int   width, const int   height){
	header->bfSize = (sizeof (bmp_pixel) * width + BMP_GET_PADDING (width)) * abs (height);
	header->bfReserved = 0;
	header->bfOffBits = 54;
	header->biSize = 40;
	header->biWidth = width;
	header->biHeight = height;
	header->biPlanes = 1;
	header->biBitCount = 24;
	header->biCompression = 0;
	header->biSizeImage = 0;
	header->biXPelsPerMeter = 0;
	header->biYPelsPerMeter = 0;
	header->biClrUsed = 0;
	header->biClrImportant = 0;
}

enum bmp_error bmp_header_write (const bmp_header *header, FILE *img_file){
	const unsigned short magic = BMP_MAGIC;

	if (header == NULL) return BMP_HEADER_NOT_INITIALIZED; 
	else if (img_file == NULL) return BMP_FILE_NOT_OPENED;
	
	fwrite (&magic, sizeof (magic), 1, img_file);
	
	fwrite (header, sizeof (bmp_header), 1, img_file);
	return BMP_OK;
}

enum bmp_error bmp_header_read (bmp_header *header, FILE *img_file){
	unsigned short magic;

	if (img_file == NULL) return BMP_FILE_NOT_OPENED;
	
	if (fread (&magic, sizeof (magic), 1, img_file) != 1 || magic != BMP_MAGIC) return BMP_INVALID_FILE;
	
	if (fread (header, sizeof (bmp_header), 1, img_file) != 1) return BMP_ERROR;

	return BMP_OK;
}


/* BMP_PIXEL */

void bmp_pixel_init (bmp_pixel *pxl, const unsigned char red, const unsigned char green, const unsigned char blue){
	pxl->red = red;
	pxl->green = green;
	pxl->blue = blue;
}


/* BMP_IMG */

void bmp_img_alloc (bmp_img *img){
	const size_t h = abs (img->img_header.biHeight);
	size_t y;
	
	img->img_pixels = malloc (sizeof (bmp_pixel*) * h);
	
	for (y = 0; y < h; y++) img->img_pixels[y] = malloc (sizeof (bmp_pixel) * img->img_header.biWidth);
}

void bmp_img_init_df (bmp_img   *img, const int  width, const int  height){
	bmp_header_init_df (&img->img_header, width, height);
	bmp_img_alloc (img);
}

void bmp_img_free (bmp_img *img){
	const size_t h = abs (img->img_header.biHeight);
	size_t y;
	
	for (y = 0; y < h; y++) free (img->img_pixels[y]);
	free (img->img_pixels);
}

enum bmp_error bmp_img_write (const bmp_img *img, const char    *filename){
	FILE *img_file = fopen (filename, "wb");
	const enum bmp_error err = bmp_header_write (&img->img_header, img_file);
	const size_t h = abs (img->img_header.biHeight);
	const size_t offset = (img->img_header.biHeight > 0 ? h - 1 : 0);
	
	const unsigned char padding[3] = {'\0', '\0', '\0'};

	size_t y;
	
	if (img_file == NULL) return BMP_FILE_NOT_OPENED;
	
	if (err != BMP_OK){
		fclose (img_file);
		return err;
	}
	
	for (y = 0; y < h; y++){
		fwrite (img->img_pixels[abs (offset - y)], sizeof (bmp_pixel), img->img_header.biWidth, img_file);
		fwrite (padding, sizeof (unsigned char), BMP_GET_PADDING (img->img_header.biWidth), img_file);
	}
	
	fclose (img_file);
	return BMP_OK;
}

enum bmp_error bmp_img_read (bmp_img    *img, const char *filename){
	FILE *img_file = fopen (filename, "rb");
	const enum bmp_error err = bmp_header_read (&img->img_header, img_file);
	const size_t h = abs (img->img_header.biHeight);
	const size_t offset = (img->img_header.biHeight > 0 ? h - 1 : 0);
	const size_t padding = BMP_GET_PADDING (img->img_header.biWidth);
	
	const size_t items = img->img_header.biWidth;

	size_t y;
	
	if (img_file == NULL) return BMP_FILE_NOT_OPENED;
	
	if (err != BMP_OK){
		fclose (img_file);
		return err;
	}
	
	bmp_img_alloc (img);
	
	for (y = 0; y < h; y++){
		if (fread (img->img_pixels[abs (offset - y)], sizeof (bmp_pixel), items, img_file) != items){
			fclose (img_file);
			return BMP_ERROR;
		}
		fseek (img_file, padding, SEEK_CUR);
	}
	
	fclose (img_file);
	return BMP_OK;
}
