/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Source code for bitmap image creation                                       *
 * Image characteristics: 24 bit color depth, no color table, no compression   *
 *                                                                             *
 * Autor: Vitor Henrique Andrade Helfensteller Satraggiotti Silva              *
 * Iniciado em: 28/05/2020                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//undigned char = 1
//unsigned short int  = 2
//unsignde int = 4
#include<stdlib.h>
#include<stdio.h>
#include"create_bitmap.h"

/********************************************************************************/
//Inicialize image file (return header structure)
void create_bmp(int32_t Width,
				int32_t Heigth,
				u_int32_t ResolutionX,
				u_int32_t ResolutionY,
				pixel_t **PixelMatrix,
				const char *Filename)
{
	header_t ImageHeader;
	u_int8_t ByteZero = 0;

	//evaluate image dimensions
	if((Width > 20000)||(Heigth > 20000))
	{
		printf("Error: Image dimensions exceed limits of 20000 by 20000");
		exit(EXIT_FAILURE);
	}
	
	if((Width < 2)||(Heigth < 2))
	{
		printf("Error: Image dimensions should be equal or greater than 2 by 2");
		exit(EXIT_FAILURE);
	}

	ImageHeader.CharID_1 = 0x42;
	ImageHeader.CharID_2 = 0x4D;
	ImageHeader.Reserved_1 = 0;
	ImageHeader.Reserved_2 = 0;
	ImageHeader.PixelOffsetMatrix = 54;
	ImageHeader.SizeHeader2 = 40;
	ImageHeader.Width = Width;
	ImageHeader.Height = Heigth;
	ImageHeader.Planes = 1;
	ImageHeader.ColorDepth = 24;
	ImageHeader.Compression = 0;
	ImageHeader.ResolutionX = ResolutionX;
	ImageHeader.ResolutionY = ResolutionY;
	ImageHeader.NumColorsInTable = 0;
	ImageHeader.NumImportantColors = 0;

	//Finding pixel matrix size and adding padding
	int32_t SizeWidthByte = Width * 3;			//size of one line of the image in bytes
	int32_t TotalWidthMod4 = SizeWidthByte % 4;
	
	if(TotalWidthMod4 != 0)
	{
		SizeWidthByte = SizeWidthByte + 4 - TotalWidthMod4;
	}
	
	ImageHeader.SizePixelMatrix = (u_int32_t) (SizeWidthByte * Heigth);

	//Finding total image file size
	ImageHeader.FileSize = 54 + ImageHeader.SizePixelMatrix;

	//creating image file
	FILE *ImageFile;
	
	ImageFile = fopen(Filename, "wb");
	if(ImageFile == NULL)
	{
		printf("Error: problem ocurred while creating image file");
		exit(EXIT_FAILURE);
	}
	
	fwrite(&ImageHeader, sizeof(header_t), 1, ImageFile);
	
	for(int32_t row=(Heigth-1); row >= 0; row--)
	{
		for(int32_t column=0; column < Width; column++)
		{
			fwrite(&PixelMatrix[row][column], sizeof(pixel_t), 1, ImageFile);
			
			if(column == (Width - 1))
			{
				if(TotalWidthMod4 == 1)
				{
					fwrite(&ByteZero, sizeof(u_int8_t), 3, ImageFile);
				}else if(TotalWidthMod4 == 2){
					fwrite(&ByteZero, sizeof(u_int8_t), 2, ImageFile);
				}else if(TotalWidthMod4 == 3){
					fwrite(&ByteZero, sizeof(u_int8_t), 1, ImageFile);
				}
			}
		}
	}
	
	fclose(ImageFile);
}








