

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
	
	int Width, Height;
	pixel_t **PixelMatrix;
	
	Width = width_BMP(argv[1]);
	Height = height_BMP(argv[1]);
	
	PixelMatrix = read_BMP(argv[1]);
	
	for(int row = 0; row < Height; row++)
	{
		for(int column = 0; column < Width; column++)
		{
			if((column == Width/2) || (row == Height/2))
			{
				PixelMatrix[row][column].Red = 255;
				PixelMatrix[row][column].Green = 0;
				PixelMatrix[row][column].Blue = 0;
			}
		}
	}
	
	create_BMP(Width, Height, RESOLUTION_X, RESOLUTION_Y, PixelMatrix, "saida.bmp");
}
