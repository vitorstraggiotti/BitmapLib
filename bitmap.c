/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Source code for bitmap image creation                                       *
 * Image characteristics: 24 bit color depth, no color table, no compression   *
 *                                                                             *
 * Autor: Vitor Henrique Andrade Helfensteller Satraggiotti Silva              *
 * Iniciado em: 28/05/2020                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//unsigned char = 1
//unsigned short int  = 2
//unsigned int = 4
//unsigned long int = 8
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include"bitmap.h"

/********************************************************************************/
//Create image file (return header structure)
void create_BMP(int Width,
				int Heigth,
				unsigned int ResolutionX,
				unsigned int ResolutionY,
				pixel_t **PixelMatrix,
				const char *Filename)
{
	header_t ImageHeader;
	u_int8_t ByteZero = 0;

	//evaluate image dimensions
	if((Width > 20000)||(Heigth > 20000))
	{
		printf("Error: Image dimensions exceed creation limits of 20000 by 20000");
		exit(EXIT_FAILURE);
	}
	
	if((Width < 2)||(Heigth < 2))
	{
		printf("Error: Image dimensions for creation should be equal or greater than 2 by 2");
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
	int SizeWidthByte = Width * 3;			//size of one line of the image in bytes
	int TotalWidthMod4 = SizeWidthByte % 4;
	
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
	
	for(int row=(Heigth-1); row >= 0; row--)
	{
		for(int column=0; column < Width; column++)
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
/******************************************************************************/
//Find dimensions of the BMP image
dimensions_t dimensions_BMP(const char *Filename)
{
	header_t FileHeader;
	dimensions_t Dimension;
	FILE *Image;
	
	Image = fopen(Filename, "rb");
	if(Image == NULL)
	{
		printf("Error: problem ocurred while reading file for width information");
		exit(EXIT_FAILURE);
	}
	
	fread(&FileHeader, sizeof(header_t), 1, Image);
	fclose(Image);
	
	if((FileHeader.CharID_1 == 0x42) && (FileHeader.CharID_2 == 0x4D))
	{
		Dimension.Width = FileHeader.Width;
		Dimension.Height = FileHeader.Height;
		return Dimension;
	}else{
		printf("Error: Input file is not a supported BMP image for width extraction");
		exit(EXIT_FAILURE);
	}
}
/******************************************************************************/
//Read BMP image to a pixel matrix
pixel_t **read_BMP(const char *Filename)
{
	header_t FileHeader;
	pixel_t **PixelMatrix;
	unsigned char Trash;
	FILE *Image;
	
	Image = fopen(Filename, "rb");
	if(Image == NULL)
	{
		printf("Error: problem occured while readiing file for pixel matrix extraction");
		exit(EXIT_FAILURE);
	}
	
	//read file header
	fread(&FileHeader, sizeof(header_t), 1, Image);
	
	//alocate space for pixel matrix
	PixelMatrix = malloc(FileHeader.Height * sizeof(pixel_t*));
	
	for(int row = 0; row < FileHeader.Height; row++)
	{
		PixelMatrix[row] = malloc(FileHeader.Width * sizeof(pixel_t));
	}

	for(int row = (FileHeader.Height - 1); row >= 0; row--)
	{
		for(int column = 0; column < FileHeader.Width; column++)
		{
			fread(&PixelMatrix[row][column], sizeof(pixel_t), 1, Image);
			
			if(column == (FileHeader.Width - 1))
			{
				if(((FileHeader.Width * 3) % 4) == 1)
				{
					fwrite(&Trash, sizeof(unsigned char), 3, Image);
				}else if(((FileHeader.Width * 3) % 4) == 2){
					fwrite(&Trash, sizeof(unsigned char), 2, Image);
				}else if(((FileHeader.Width * 3) % 4) == 3){
					fwrite(&Trash, sizeof(unsigned char), 1, Image);
				}
			}
		}
	}
	
	fclose(Image);
	
	return PixelMatrix;
}
/******************************************************************************/
//Draw a circle on the pixel matrix
void circle(dimensions_t Dimension, pixel_t **PixelMatrix, int Pos_x, int Pos_y, int Radius, pixel_t Color)
{

}
/******************************************************************************/
//Draw a circumference on the pixel matrix
void circumference(dimensions_t Dimension, pixel_t **PixelMatrix, int Pos_x, int Pos_y, int Radius, pixel_t Color)
{
	int RowMin, RowMax, ColumnMin, ColumnMax;
	int CircumferenceX, CircumferenceY, StepX, StepY;
	int PreviousStepY;
	
	//Verifying coordinates boundaries
	if((Pos_x < 0) || (Pos_x > Dimension.Width))
	{
		printf("Error: circumference X coordinate are out of the screen. Drawing terminated");
		exit(EXIT_FAILURE);
	}else if((Pos_y < 0) || (Pos_y > Dimension.Height))
	{
		printf("Error: circumference Y coordinate are out of the screen. Drawing terminated");
		exit(EXIT_FAILURE);
	}
	
	
	//Drawing circumference
	for(StepX = 0; StepX <= Radius; StepX++)
	{
		StepY = (int)((float)Radius * sqrt(1.0 - pow((float)StepX/(float)Radius, 2)));
		
		//1° quadrant
		CircumferenceX = Pos_x + StepX;
		CircumferenceY = (Dimension.Height - Pos_y) + StepY;
		if((CircumferenceX <= Dimension.Width) && (CircumferenceY <= Dimension.Height))
		{
			PixelMatrix[CircumferenceY][CircumferenceX].Red = Color.Red;
			PixelMatrix[CircumferenceY][CircumferenceX].Green = Color.Green;
			PixelMatrix[CircumferenceY][CircumferenceX].Blue = Color.Blue;
			
			//Filing absent pixels
			if(StepX > 0)
			{
				PreviousStepY = (int)((float)Radius * sqrt(1.0 - pow((float)(StepX - 1)/(float)Radius, 2)));
				for(int tmp = (PreviousStepY - StepY); tmp > 0; tmp--)
				{
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Red = Color.Red;
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Green = Color.Green;
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Blue = Color.Blue;
				}
			}
		}
		
		//2° quadrant
		CircumferenceX = Pos_x - StepX;
		CircumferenceY = (Dimension.Height - Pos_y) + StepY;
		if((CircumferenceX >= 0) && (CircumferenceY <= Dimension.Height))
		{
			PixelMatrix[CircumferenceY][CircumferenceX].Red = Color.Red;
			PixelMatrix[CircumferenceY][CircumferenceX].Green = Color.Green;
			PixelMatrix[CircumferenceY][CircumferenceX].Blue = Color.Blue;
			
			//Filing absent pixels
			if(StepX > 0)
			{
				PreviousStepY = (int)((float)Radius * sqrt(1.0 - pow((float)(StepX - 1)/(float)Radius, 2)));
				for(int tmp = (PreviousStepY - StepY); tmp > 0; tmp--)
				{
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Red = Color.Red;
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Green = Color.Green;
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Blue = Color.Blue;
				}
			}
		}
		
		//3° quadrant
		CircumferenceX = Pos_x - StepX;
		CircumferenceY = (Dimension.Height - Pos_y) - StepY;
		if((CircumferenceX >= 0) && (CircumferenceY >= 0))
		{
			PixelMatrix[CircumferenceY][CircumferenceX].Red = Color.Red;
			PixelMatrix[CircumferenceY][CircumferenceX].Green = Color.Green;
			PixelMatrix[CircumferenceY][CircumferenceX].Blue = Color.Blue;
			
			//Filing absent pixels
			if(StepX > 0)
			{
				PreviousStepY = (int)((float)Radius * sqrt(1.0 - pow((float)(StepX - 1)/(float)Radius, 2)));
				for(int tmp = (StepY - PreviousStepY); tmp < 0; tmp++)
				{
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Red = Color.Red;
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Green = Color.Green;
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Blue = Color.Blue;
				}
			}
		}
		
		//4° quadrant
		CircumferenceX = Pos_x + StepX;
		CircumferenceY = (Dimension.Height - Pos_y) - StepY;
		if((CircumferenceX <= Dimension.Width) && (CircumferenceY >= 0))
		{
			PixelMatrix[CircumferenceY][CircumferenceX].Red = Color.Red;
			PixelMatrix[CircumferenceY][CircumferenceX].Green = Color.Green;
			PixelMatrix[CircumferenceY][CircumferenceX].Blue = Color.Blue;
			
			//Filing absent pixels
			if(StepX > 0)
			{
				PreviousStepY = (int)((float)Radius * sqrt(1.0 - pow((float)(StepX - 1)/(float)Radius, 2)));
				for(int tmp = (StepY - PreviousStepY); tmp < 0; tmp++)
				{
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Red = Color.Red;
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Green = Color.Green;
					PixelMatrix[CircumferenceY + tmp][CircumferenceX].Blue = Color.Blue;
				}
			}
		}
		
	}

} 






















