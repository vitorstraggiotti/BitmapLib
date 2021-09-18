

#include <stdio.h>
#include <stdlib.h>
#include "../include/bitmap.h"
#include "../include/comp_vision.h"
#include "../include/load_animation_v1.0.h"

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
	pixel_t **ResultPixelMatrix;
	pixel_t Color;
	
	Dimension = dimensions_BMP(argv[1]);
	PixelMatrix = read_BMP(argv[1]);
	
	printf("\nTransform to grayscale ...\n");
	RGB_to_grayscale(Dimension, PixelMatrix, GRAY_LUMI_PERCEP);
	fputc('\n', stdout);
	
	for(int i = 1; i < 6; i++)
	{
		printf("Blur convolution: %d\n", i);
		ResultPixelMatrix = gauss_blur_kernel_conv(Dimension, PixelMatrix);
		fputc('\n', stdout);
		free_pixel_matrix(Dimension, PixelMatrix);
		PixelMatrix = ResultPixelMatrix;
	}
	
	printf("\nDetecting sobel edge ...\n");
	ResultPixelMatrix = sobel_edge_kernel_conv(Dimension, PixelMatrix);
	fputc('\n', stdout);
	free_pixel_matrix(Dimension, PixelMatrix);

	printf("Saving BMP ...\n");
	create_BMP(Dimension.Width, Dimension.Height, ResultPixelMatrix, "saida.bmp");
	fputc('\a', stdout);

}







