/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Program that implement and tests functions from bitmap library    *
 *                                                                   *
 * Author: Vitor Henrique Andrade Helfensteller Straggiotti Silva    *
 * Created on: 01/01/2022 (DD/MM/YYYY)                               *
 * Tested for lib version: 1.2.0  ([major].[minor].[bugs])
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"


void exit_msg(const char *Msg)
{
	printf(Msg);
	exit(EXIT_FAILURE);
}

void print_img_struct(img_t * Img)
{
	printf("Image pointer: %p\n", (void *) Img);

	if (Img != NULL)
	{
	printf("Width: %d\n", Img->Width);
	printf("Height: %d\n", Img->Height);
	printf("Pixel24: %p\n", (void *) Img->Pixel24);
	printf("Pixel8: %p\n\n\n", (void *) Img->Pixel8);
	}
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Error: wrong number of arguments.\n");
		printf("Correct use: test <path_to_image>\n\n");
		exit(EXIT_FAILURE);
	}

	img_t	*ImgOrig;
	img_t	*ImgNew24,		*ImgNew8;
	img_t	*ImgAsSize24,	*ImgAsSize8;
	img_t	*ImgCopy24,		*ImgCopy8;


	/****************************************************************************/
	/*                      Testing function: display_header()                  */
	/****************************************************************************/
	printf("Printing image info ...\n\n");
	display_header(argv[1]);

	
	/****************************************************************************/
	/*                        Testing function: read_BMP()                      */
	/****************************************************************************/
	printf("\nReading image ...\n\n");
	ImgOrig = read_BMP(argv[1]);
	if (ImgOrig == NULL)
	{
		exit_msg("Error: [main] --> Could not read BMP.\n");
	}

	printf("Printing \"ImgOrig\"\n\n");
	print_img_struct(ImgOrig);

	/****************************************************************************/
	/*                        Testing function: new_BMP()                       */
	/****************************************************************************/
	printf("Creating new BMP (24bits) ...\n");
	ImgNew24 = new_BMP(1000, 1000, DEPTH24);
	if (ImgNew24 == NULL)
	{
		exit_msg("Error: [main] --> Could not create new BMP. (24bpp)\n");
	}

	printf("Printing \"ImgNew24\"\n\n");
	print_img_struct(ImgNew24);

	printf("Creating new BMP (8bits) ...\n");
	ImgNew8 = new_BMP(2000, 2000, DEPTH8);
	if (ImgNew8 == NULL)
	{
		exit_msg("Error: [main()] --> Could not create new BMP. (8bpp)\n");
	}

	printf("Printing \"ImgNew8\"\n\n");
	print_img_struct(ImgNew8);

	printf("Drawing on created images ...\n\n");
	for(int32_t Row = 0; Row < ImgNew24->Height; Row++)
	{
		for(int32_t Column = 0; Column < ImgNew24->Width; Column++)
		{
			if((Row % 10) == 0)
				ImgNew24->Pixel24[Row][Column].Red = 255;

			if((Row % 5) == 0)
				ImgNew24->Pixel24[Row][Column].Green = 255;

			if((Row % 7) == 0)
				ImgNew24->Pixel24[Row][Column].Blue = 255;

			if(((Row % 2) == 0) && ((Column % 2) == 0))
				ImgNew8->Pixel8[Row][Column] = 255;
		}
	}


	/****************************************************************************/
	/*                     Testing function: new_BMP_as_size()                  */
	/****************************************************************************/
	printf("Creating new BMP as size from another img (24bits) ...\n");
	ImgAsSize24 = new_BMP_as_size(ImgOrig, DEPTH24);
	if (ImgAsSize24 == NULL)
	{
		exit_msg("Error: [main()] --> Could not create new BMP. (new_BMP_as_size(); 24bpp)\n");
	}

	printf("Printing \"ImgAsSize24\"\n\n");
	print_img_struct(ImgAsSize24);

	printf("Creating new BMP as size from another img (8bits) ...\n");
	ImgAsSize8 = new_BMP_as_size(ImgOrig, DEPTH8);
	if (ImgAsSize8 == NULL)
	{
		exit_msg("Error: [main()] --> Could not create new BMP. (new_BMP_as_size(); 8bpp)\n");
	}

	printf("Printing \"ImgAsSize8\"\n\n");
	print_img_struct(ImgAsSize8);

	printf("Drawing on created images ...\n\n");
	for(int32_t Row = 0; Row < ImgAsSize24->Height; Row++)
	{
		for(int32_t Column = 0; Column < ImgAsSize24->Width; Column++)
		{
			if((Row % 10) == 0)
				ImgAsSize24->Pixel24[Row][Column].Red = 255;

			if((Row % 5) == 0)
				ImgAsSize24->Pixel24[Row][Column].Green = 255;

			if((Row % 7) == 0)
				ImgAsSize24->Pixel24[Row][Column].Blue = 255;

			if(((Row % 2) == 0) && ((Column % 2) == 0))
				ImgAsSize8->Pixel8[Row][Column] = 255;
		}
	}	


	/****************************************************************************/
	/*                        Testing function: copy_BMP()                      */
	/****************************************************************************/
	printf("Creating copy of img 24bits ...\n");
	ImgCopy24 = copy_BMP(ImgOrig);
	if (ImgCopy24 == NULL)
	{
		exit_msg("Error: [main()] --> Could not create copy. (24bpp)\n");
	}

	printf("Printing \"ImgCopy24\"\n\n");
	print_img_struct(ImgCopy24);

	printf("Creating copy of img 8bits ...\n");
	ImgCopy8 = copy_BMP(ImgAsSize8);
	if (ImgAsSize8 == NULL)
	{
		exit_msg("Error: [main()] --> Could not create copy. (8bpp)\n");
	}

	printf("Printing \"ImgCopy8\"\n\n");
	print_img_struct(ImgCopy8);

	/****************************************************************************/
	/*               Testing function: save_BMP() and free_img()                */
	/****************************************************************************/
	printf("Saving imges ...\n");
	if(save_BMP(ImgNew24, "out01-ImgNew24") == -1)
		exit_msg("Error: [main()] --> Could not save BMP to disk. (ImgNew24)\n\n");

	if(save_BMP(ImgNew8, "out02-ImgNew8") == -1)
		exit_msg("Error: [main()] --> Could not save BMP to disk. (ImgNew8)\n\n");

	if(save_BMP(ImgAsSize24, "out03-ImgAsSize24") == -1)
		exit_msg("Error: [main()] --> Could not save BMP to disk. (ImgAsSize24)\n\n");

	if(save_BMP(ImgAsSize8, "out04-ImgAsSize8") == -1)
		exit_msg("Error: [main()] --> Could not save BMP to disk. (ImgAsSize8)\n\n");

	if(save_BMP(ImgCopy24, "out05-ImgCopy24") == -1)
		exit_msg("Error: [main()] --> Could not save BMP to disk. (ImgCopy24)\n\n");

	if(save_BMP(ImgCopy8, "out06-ImgCopy8") == -1)
		exit_msg("Error: [main()] --> Could not save BMP to disk. (ImgCopy8)\n\n");


	/****************************************************************************/
	/*                        Testing function: free_img()                      */
	/****************************************************************************/
	free_img(ImgOrig);
	free_img(ImgNew24);
	free_img(ImgNew8);
	free_img(ImgAsSize24);
	free_img(ImgAsSize8);
	free_img(ImgCopy24);
	free_img(ImgCopy8);

	printf("Printing \"ImgOrig\"\n\n");
	print_img_struct(ImgOrig);

	return 0;
}
