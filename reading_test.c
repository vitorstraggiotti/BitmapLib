

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
	printf("Width = %d // Height = %d", Dimension.Width, Dimension.Height);
/*	PixelMatrix = read_BMP(argv[1]);
	
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
*/	
	//print file header information
	display_header(argv[1]);
	
/*	Color.Red = 0;
	Color.Green = 0;
	Color.Blue = 255;
	circle(Dimension, PixelMatrix, Dimension.Width/2, Dimension.Height/2, 300, Color);
	
	Color.Red = 0;
	Color.Green = 255;
	Color.Blue = 0;
	circumference(Dimension, PixelMatrix, Dimension.Width/2, Dimension.Height/2, 310, Color);
	
		Color.Red = 0;
	Color.Green = 0;
	Color.Blue = 255;
	circle(Dimension, PixelMatrix, 100, 100, 300, Color);
	
	Color.Red = 0;
	Color.Green = 255;
	Color.Blue = 0;
	circumference(Dimension, PixelMatrix, 100, 100, 310, Color);
	
		Color.Red = 0;
	Color.Green = 0;
	Color.Blue = 255;
	circle(Dimension, PixelMatrix, Dimension.Width - 100, 100, 300, Color);
	
	Color.Red = 0;
	Color.Green = 255;
	Color.Blue = 0;
	circumference(Dimension, PixelMatrix, Dimension.Width - 100, 100, 310, Color);
	
		Color.Red = 0;
	Color.Green = 0;
	Color.Blue = 255;
	circle(Dimension, PixelMatrix, 100, Dimension.Height - 100, 300, Color);
	
	Color.Red = 0;
	Color.Green = 255;
	Color.Blue = 0;
	circumference(Dimension, PixelMatrix, 100, Dimension.Height - 100, 310, Color);
	
		Color.Red = 0;
	Color.Green = 0;
	Color.Blue = 255;
	circle(Dimension, PixelMatrix, Dimension.Width - 100, Dimension.Height - 100, 300, Color);
	
	Color.Red = 0;
	Color.Green = 255;
	Color.Blue = 0;
	circumference(Dimension, PixelMatrix, Dimension.Width - 100, Dimension.Height - 100, 310, Color);
*/	
//	create_BMP(Dimension.Width, Dimension.Height, RESOLUTION_X, RESOLUTION_Y, PixelMatrix, "Saida.bmp");

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







