

#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Error: wrong number of arguments.\n");
		printf("Correct use: test <path_to_image>\n\n");
		exit(EXIT_FAILURE);
	}
	
	dimensions_t Dimension;
	pixel_t **PixelMatrix;
	
	Dimension = dimensions_BMP(argv[1]);
	PixelMatrix = read_BMP(argv[1]);
	
	for(int i = Dimension.Height / 3; i < ((Dimension.Height * 2) / 3); i++)
	{
		for(int j = Dimension.Width / 3; j < ((Dimension.Width * 2) / 3); j++)
		{
			PixelMatrix[i][j].Red = 200;
		}
	}

	printf("Saving BMP ...\n");
	create_BMP(Dimension.Width, Dimension.Height, PixelMatrix, "saida.bmp");
	fputc('\a', stdout);

}







