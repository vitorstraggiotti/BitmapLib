
#include"create_bitmap.h"
#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	pixel_t **Image;
	int32_t Width = 12000;
	int32_t Height = 12000;
	
	Image = malloc(Height * sizeof(pixel_t*));
	for(int32_t i = 0; i < Height; i++)
	{
		Image[i] = malloc(Width * sizeof(pixel_t));
		for(int32_t j = 0; j < Width; j++)
		{
			Image[i][j].Red = 0;
			Image[i][j].Green = 0;
			Image[i][j].Blue = 0;
		}
	}


	printf("\n");
	for(int32_t row = 0; row < Height; row++)
	{
		for(int32_t column = 0; column < Width; column++)
		{
			Image[row][column].Red = ((Height - row) * 255)/Height;
			Image[row][column].Green = ((Height - row) * 255)/Height;
			Image[row][column].Blue = ((Height - row) * 255)/Height;
		}
	}
	
	create_bmp(Width, Height, RESOLUTION_X, RESOLUTION_Y, Image, "teste.bmp");

	return 0;
}
