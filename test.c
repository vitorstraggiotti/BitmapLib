

#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "comp_vision.h"

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
	pixel_t **BlurPixelMatrix;
	pixel_t Color;
	
	Dimension = dimensions_BMP(argv[1]);
	PixelMatrix = read_BMP(argv[1]);
	
	for(int i = 1; i < 40; i++)
	{
		printf("blur convolution: %d\n", i);
		BlurPixelMatrix = box_blur_kernel_conv(Dimension, PixelMatrix);
		free_pixel_matrix(Dimension, PixelMatrix);
		PixelMatrix = BlurPixelMatrix;
	}
	
	create_BMP(Dimension.Width, Dimension.Height, RESOLUTION_X, RESOLUTION_Y, PixelMatrix, "saida.bmp");




// Image creation
/*	pixel_t **Image;
	int Width = 10;
	int Height = 10;

	Image = malloc(Height * sizeof(pixel_t*));
	for(int i = 0; i < Height; i++)
	{
		Image[i] = malloc(Width * sizeof(pixel_t));
		for(int j = 0; j < Width; j++)
		{
			Image[i][j].Red = 0;
			Image[i][j].Green = 0;
			Image[i][j].Blue = 0;
		}
	}


	printf("\n");
	for(int row = 0; row < Height; row++)
	{
		for(int column = 0; column < Width; column++)
		{
			Image[row][column].Red = ((Height - row) * 255)/Height;
			Image[row][column].Green = ((Height - row) * 255)/Height;
			Image[row][column].Blue = ((Height - row) * 255)/Height;
		}
	}
	
	create_BMP(Width, Height, RESOLUTION_X, RESOLUTION_Y, Image, "teste.bmp");
*/




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







