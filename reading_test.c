

#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Error: wrong number of arguments.\n");
		printf("Correct use: reading_test <path_to_image>\n\n");
		exit(EXIT_FAILURE);
	}
	
	dimensions_t Dimension;
	pixel_t **PixelMatrix;
	pixel_t Color;
	
	Dimension = dimensions_BMP(argv[1]);
	
	PixelMatrix = read_BMP(argv[1]);
	RGB_to_grayscale(Dimension, PixelMatrix, GRAY_AVERAGE);
	create_BMP(Dimension.Width, Dimension.Height, RESOLUTION_X, RESOLUTION_Y, PixelMatrix, "gray_average.bmp");
	
	PixelMatrix = read_BMP(argv[1]);
	RGB_to_grayscale(Dimension, PixelMatrix, GRAY_LUMI_PERCEP);
	create_BMP(Dimension.Width, Dimension.Height, RESOLUTION_X, RESOLUTION_Y, PixelMatrix, "gray_lumi.bmp");
	
	PixelMatrix = read_BMP(argv[1]);
	RGB_to_grayscale(Dimension, PixelMatrix, GRAY_APROX_GAM_LUMI_PERCEP);
	create_BMP(Dimension.Width, Dimension.Height, RESOLUTION_X, RESOLUTION_Y, PixelMatrix, "gray_gamma_lumi.bmp");
	
/*	char *OutFilename;
	
	OutFilename = malloc(14 * sizeof(char));
	
	for(int Frame = 0; Frame < 300; Frame++)
	{
		sprintf(OutFilename, "Image-%d.bmp", Frame);
		circumference(Dimension, PixelMatrix, Dimension.Width/2, Dimension.Height/2, ((3 * Frame)/10) + 10, Color);
		create_BMP(Dimension.Width, Dimension.Height, RESOLUTION_X, RESOLUTION_Y, PixelMatrix, OutFilename);
		printf("Created frame: %d/300\n", Frame);
	}
*/
}







