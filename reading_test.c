

#include<stdio.h>
#include<stdlib.h>
#include"bitmap.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Error: wrong number of arguments.\n");
		printf("Correct use: reading_test <path_to_image>");
		exit(EXIT_FAILURE);
	}
	
	dimensions_t Dimension;
	pixel_t **PixelMatrix;
	pixel_t Color;
	
	Dimension = dimensions_BMP(argv[1]);
	PixelMatrix = read_BMP(argv[1]);
	
	for(int row = 0; row < Dimension.Height; row++)
	{
		for(int column = 0; column < Dimension.Width; column++)
		{
			if((column == Dimension.Width/2) || (row == Dimension.Height/2))
			{
				PixelMatrix[row][column].Red = 255;
				PixelMatrix[row][column].Green = 0;
				PixelMatrix[row][column].Blue = 0;
			}
		}
	}
	
	Color.Red = 0;
	Color.Green = 0;
	Color.Blue = 255;
	circumference(Dimension, PixelMatrix, 2000, 2000, 100, Color);
	
	create_BMP(Dimension.Width, Dimension.Height, RESOLUTION_X, RESOLUTION_Y, PixelMatrix, "saida.bmp");
}
