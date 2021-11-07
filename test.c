

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
	
	img_t *Imagem;
	img_t *CgImg = new_BMP(1920, 1080);
	
	printf("Reading image ...\n");
	Imagem = read_BMP(argv[1]);
	
	printf("Display image info ...\n");
	display_header(argv[1]);
	
	for(int i = Imagem->Height / 3; i < ((Imagem->Height * 2) / 3); i++)
	{
		for(int j = Imagem->Width / 3; j < ((Imagem->Width * 2) / 3); j++)
		{
			Imagem->Pixel24[i][j].Red = 200;
		}
	}
	
	for(int i = CgImg->Height / 3; i < ((CgImg->Height * 2) / 3); i++)
	{
		for(int j = CgImg->Width / 3; j < ((CgImg->Width * 2) / 3); j++)
		{
			CgImg->Pixel24[i][j].Red = 200;
		}
	}

	printf("Saving BMP ...\n");
	save_BMP(Imagem, "saida.bmp");
	save_BMP(CgImg, "saida2.bmp");
	
	printf("Freeing memory ...\n");
	free_img(Imagem);
	free_img(CgImg);
	
	return 0;
}







