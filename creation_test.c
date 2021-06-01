
#include"bitmap.h"
#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	pixel_t **Image;
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

	return 0;
}
