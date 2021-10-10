

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
	
	img24_t *Imagem;
	
	Imagem = read_BMP(argv[1]);
	
	for(int i = Imagem->Height / 3; i < ((Imagem->Height * 2) / 3); i++)
	{
		for(int j = Imagem->Width / 3; j < ((Imagem->Width * 2) / 3); j++)
		{
			Imagem->Pixel[i][j].Red = 200;
		}
	}

	printf("Saving BMP ...\n");
	create_BMP(Imagem, "saida.bmp");
	fputc('\a', stdout);
	
	free_img(Imagem);

}







