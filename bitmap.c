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
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bitmap.h"

/********************************************************************************/
//Create BMP image file (header used: BITMAPINFOHEADER)
void create_BMP(int Width,
				int Height,
				unsigned int ResolutionX,
				unsigned int ResolutionY,
				pixel_t **PixelMatrix,
				const char *Filename)
{
	header_t ImageHeader;
	u_int8_t ByteZero = 0;

	//evaluate image dimensions
	if((Width > 20000)||(Height > 20000))
	{
		printf("Error: Image dimensions exceed creation limits of 20000 by 20000\n\n");
		exit(EXIT_FAILURE);
	}
	
	if((Width < 2)||(Height < 2))
	{
		printf("Error: Dimensions for image creation should be equal or greater than 2 by 2\n\n");
		exit(EXIT_FAILURE);
	}

	ImageHeader.CharID_1 = 0x42;
	ImageHeader.CharID_2 = 0x4D;
	ImageHeader.Reserved_1 = 0;
	ImageHeader.Reserved_2 = 0;
	ImageHeader.OffsetPixelMatrix = 54;
	ImageHeader.SizeHeader = 40;
	ImageHeader.Width = Width;
	ImageHeader.Height = Height;
	ImageHeader.Planes = 1;
	ImageHeader.ColorDepth = 24;
	ImageHeader.Compression = 0;
	ImageHeader.ResolutionX = ResolutionX;
	ImageHeader.ResolutionY = ResolutionY;
	ImageHeader.NumColorsInTable = 0;
	ImageHeader.NumImportantColors = 0;

	//Finding pixel matrix size and adding padding
	int SizeWidthByte = Width * 3;			//size of one line in bytes
	int TotalWidthMod4 = SizeWidthByte % 4;
	
	if(TotalWidthMod4 != 0)
	{
		SizeWidthByte = SizeWidthByte + 4 - TotalWidthMod4;
	}
	
	ImageHeader.SizePixelMatrix = SizeWidthByte * Height;

	//Finding total image file size
	ImageHeader.FileSize = 54 + ImageHeader.SizePixelMatrix;

	//creating image file
	FILE *ImageFile;
	
	ImageFile = fopen(Filename, "wb");
	if(ImageFile == NULL)
	{
		printf("Error: problem ocurred while creating image file\n\n");
		exit(EXIT_FAILURE);
	}
	
	fwrite(&ImageHeader, sizeof(header_t), 1, ImageFile);
	
	for(int row = 0; row < Height; row++)
	{
		for(int column = 0; column < Width; column++)
		{
			fwrite(&PixelMatrix[row][column], sizeof(pixel_t), 1, ImageFile);
			
			if(column == (Width - 1))
			{
				if(TotalWidthMod4 == 1)
				{
					fwrite(&ByteZero, sizeof(unsigned char), 3, ImageFile);
					
				}else if(TotalWidthMod4 == 2)
				{
					fwrite(&ByteZero, sizeof(unsigned char), 2, ImageFile);
					
				}else if(TotalWidthMod4 == 3)
				{
					fwrite(&ByteZero, sizeof(unsigned char), 1, ImageFile);
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
		printf("Error: problem ocurred while reading file for width information\n\n");
		exit(EXIT_FAILURE);
	}
	
	fread(&FileHeader, sizeof(header_t), 1, Image);
	fclose(Image);
	
	if((FileHeader.CharID_1 != 0x42) || (FileHeader.CharID_2 != 0x4D))
	{
		printf("Error: input file have incompatible BMP file identifier. (should be: \"BM\")\n");
		printf("       current file identifier: \"%c%c\"\n\n", FileHeader.CharID_1, FileHeader.CharID_2);
		exit(EXIT_FAILURE);
	}/*else if(FileHeader.SizeHeader != 40)
	{
		printf("Error: input image file have incompatible BMP header size. (should be: 40 bytes)\n");
		printf("       current BMP header size value: %u bytes\n\n", FileHeader.SizeHeader);
		exit(EXIT_FAILURE);
	}*/else
	{
		Dimension.Width = FileHeader.Width;
		Dimension.Height = FileHeader.Height;
		return Dimension;
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
		printf("Error: problem occured while readiing file for pixel matrix extraction\n\n");
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

	for(int row = 0; row < FileHeader.Height; row++)
	{
		for(int column = 0; column < FileHeader.Width; column++)
		{
			fread(&PixelMatrix[row][column], sizeof(pixel_t), 1, Image);
			
			if(column == (FileHeader.Width - 1))
			{
				if(((FileHeader.Width * 3) % 4) == 1)
				{
					fread(&Trash, sizeof(unsigned char), 3, Image);
				}else if(((FileHeader.Width * 3) % 4) == 2){
					fread(&Trash, sizeof(unsigned char), 2, Image);
				}else if(((FileHeader.Width * 3) % 4) == 3){
					fread(&Trash, sizeof(unsigned char), 1, Image);
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
	int RowMin, RowMax, ColumnMin, ColumnMax;
	int CircumferenceX, CircumferenceY, StepX, StepY;
	int PreviousStepY;
	
	//Verifying coordinates boundaries
	if((Pos_x < 0) || (Pos_x > Dimension.Width))
	{
		printf("Error: circumference X coordinate are out of the screen. Drawing terminated\n\n");
		exit(EXIT_FAILURE);
	}else if((Pos_y < 0) || (Pos_y > Dimension.Height))
	{
		printf("Error: circumference Y coordinate are out of the screen. Drawing terminated\n\n");
		exit(EXIT_FAILURE);
	}
	
	
	//Drawing circumference
	for(StepX = 0; StepX <= Radius; StepX++)
	{
		StepY = (int)((float)Radius * sqrt(1.0 - pow((float)StepX/(float)Radius, 2)));
		
		//1° quadrant (4°)
		CircumferenceX = Pos_x + StepX;
		CircumferenceY = (Dimension.Height - Pos_y) + StepY;
		for(int tmp = CircumferenceY; tmp >= (CircumferenceY - StepY); tmp--)
		{
			if((CircumferenceX < Dimension.Width) && (tmp < Dimension.Height))
			{
				PixelMatrix[tmp][CircumferenceX].Red = Color.Red;
				PixelMatrix[tmp][CircumferenceX].Green = Color.Green;
				PixelMatrix[tmp][CircumferenceX].Blue = Color.Blue;
			}
		}
		
		//4° quadrant (1°)
		CircumferenceX = Pos_x + StepX;
		CircumferenceY = (Dimension.Height - Pos_y) - StepY;
		for(int tmp = CircumferenceY; tmp <= (CircumferenceY + StepY); tmp++)
		{
			if((CircumferenceX < Dimension.Width) && (tmp >= 0))
			{
				PixelMatrix[tmp][CircumferenceX].Red = Color.Red;
				PixelMatrix[tmp][CircumferenceX].Green = Color.Green;
				PixelMatrix[tmp][CircumferenceX].Blue = Color.Blue;
			}
		}
		
		//3° quadrant (2°)
		CircumferenceX = Pos_x - StepX;
		CircumferenceY = (Dimension.Height - Pos_y) + StepY;
		for(int tmp = CircumferenceY; tmp >= (CircumferenceY - StepY); tmp--)
		{
			if((CircumferenceX >= 0) && (tmp < Dimension.Height))
			{
				PixelMatrix[tmp][CircumferenceX].Red = Color.Red;
				PixelMatrix[tmp][CircumferenceX].Green = Color.Green;
				PixelMatrix[tmp][CircumferenceX].Blue = Color.Blue;
			}
		}
		
		//2° quadrant (3°)
		CircumferenceX = Pos_x - StepX;
		CircumferenceY = (Dimension.Height - Pos_y) - StepY;
		for(int tmp = CircumferenceY; tmp <= (CircumferenceY + StepY); tmp++)
		{
			if((CircumferenceX >= 0) && (tmp >= 0))
			{
				PixelMatrix[tmp][CircumferenceX].Red = Color.Red;
				PixelMatrix[tmp][CircumferenceX].Green = Color.Green;
				PixelMatrix[tmp][CircumferenceX].Blue = Color.Blue;
			}
		}
				
	}

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
		printf("Error: circumference X coordinate are out of the screen. Drawing terminated\n\n");
		exit(EXIT_FAILURE);
	}else if((Pos_y < 0) || (Pos_y > Dimension.Height))
	{
		printf("Error: circumference Y coordinate are out of the screen. Drawing terminated\n\n");
		exit(EXIT_FAILURE);
	}
	
	
	//Drawing circumference
	for(StepX = 0; StepX <= Radius; StepX++)
	{
		StepY = (int)((float)Radius * sqrt(1.0 - pow((float)StepX/(float)Radius, 2)));
		
		//1° quadrant (4°)
		CircumferenceX = Pos_x + StepX;
		CircumferenceY = (Dimension.Height - Pos_y) + StepY;
		if((CircumferenceX < Dimension.Width) && (CircumferenceY < Dimension.Height))
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
					if((CircumferenceY + tmp) < Dimension.Height)
					{
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Red = Color.Red;
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Green = Color.Green;
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Blue = Color.Blue;
					}
				}
			}

		}
		
		//2° quadrant (3°)
		CircumferenceX = Pos_x - StepX;
		CircumferenceY = (Dimension.Height - Pos_y) + StepY;
		if((CircumferenceX >= 0) && (CircumferenceY < Dimension.Height))
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
					if((CircumferenceY + tmp) < Dimension.Height)
					{
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Red = Color.Red;
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Green = Color.Green;
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Blue = Color.Blue;
					}
				}
			}

		}
		
		//3° quadrant (2°)
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
					if((CircumferenceY + tmp) >= 0)
					{
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Red = Color.Red;
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Green = Color.Green;
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Blue = Color.Blue;
					}
				}
			}

		}
		
		//4° quadrant (1°)
		CircumferenceX = Pos_x + StepX;
		CircumferenceY = (Dimension.Height - Pos_y) - StepY;
		if((CircumferenceX < Dimension.Width) && (CircumferenceY >= 0))
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
					if((CircumferenceY + tmp) >= 0)
					{
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Red = Color.Red;
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Green = Color.Green;
						PixelMatrix[CircumferenceY + tmp][CircumferenceX].Blue = Color.Blue;
					}
				}
			}

		}
		
	}

} 
/******************************************************************************/
//Display header information
void display_header(const char *Filename)
{		
	file_header_t FileHeader;
	FILE *File;
	
	//Opening image
	File = fopen(Filename, "rb");
	if(File == NULL)
	{
		printf("Error: problem occurred while reading file for displaying header information\n\n");
	}
	
	//Print file header information
	fread(&FileHeader, sizeof(file_header_t), 1, File);
	printf("\n");
	printf("Character ID_1: '%c'\n", FileHeader.CharID_1);
	printf("Character ID_2: '%c'\n", FileHeader.CharID_2);
	printf("File size: %u bytes\n", FileHeader.FileSize);
	printf("Reserved_1: %u\n", FileHeader.Reserved_1);
	printf("Reserved_2: %u\n", FileHeader.Reserved_2);
	printf("Offset until pixel matrix: %u\n\n", FileHeader.OffsetPixelMatrix);
	
	//Print BMP header information
	if((FileHeader.OffsetPixelMatrix - sizeof(file_header_t)) == BITMAP_V1_INFOHEADER)
	{
		bmp_headerV1_t BMPHeaderV1;
		
		fread(&BMPHeaderV1, sizeof(bmp_headerV1_t), 1, File);
		printf("BMP header type: BITMAPINFOHEADER (V1)\n");
		printf("BMP header size ............... %u bytes\n", BMPHeaderV1.SizeHeader);
		printf("Image width ................... %d pixels\n", BMPHeaderV1.Width);
		printf("Image Height .................. %d pixels\n", BMPHeaderV1.Height);
		printf("Number of color plnaes ........ %u\n", BMPHeaderV1.Planes);
		printf("Bits per pixel ................ %u\n", BMPHeaderV1.ColorDepth);
		printf("Image size .................... %u", BMPHeaderV1.SizePixelMatrix);
		printf("X axis printing resolution .... %d", BMPHeaderV1.ResolutionX);
		printf("Y axis printing resolution .... %d", BMPHeaderV1.ResolutionY);
		printf("Number of palette colors ...... %u", BMPHeaderV1.NumColorsInTable);
		printf("Number of important colors .... %u", BMPHeaderV1.NumImportantColors);
		
		//Print compression method
		printf("Compression method ............ ");
		switch(BMPHeaderV1.Compression)
		{
			case 0 :
				printf("none (ID code: BI_RGB)\n");
				break;
				
			case 1 :
				printf("RLE 8-bit/pixel (ID code: BI_RLE8)\n");
				break;
				
			case 2 :
				printf("RLE 4-bit/pixel (ID code: BI_RLE4)\n");
				break;
				
			case 3 :
				printf("OS22XBITMAPHEADER: Huffman 1D (ID code: BI_BITFIELDS)\n");
				break;
				
			case 4 :
				printf("OS22XBITMAPHEADER: RLE-24 (ID code: BI_JPEG)\n");
				break;
				
			case 5 :
				printf("Undefined (ID code: BI_PNG)\n");
				break;
				
			case 6 :
				printf("RGBA bit field masks (ID code: BI_ALPHABITFIELDS)\n");
				break;
				
			case 11 :
				printf("none (ID code: BI_CMYK)\n");
				break;
				
			case 12 :
				printf("RLE-8 (ID code: BI_CMYKRLE8)");
				break;
				
			case 13 :
				printf("RLE-4 (ID code: BI_CMYKRLE4)");
				break;
				
			default :
				printf("Error! invalid value");
		}

	}else if((FileHeader.OffsetPixelMatrix - sizeof(file_header_t)) == BITMAP_V2_INFOHEADER)
	{
	
		bmp_headerV2_t BMPHeaderV2;
		
		fread(&BMPHeaderV2, sizeof(bmp_headerV2_t), 1, File);
		printf("BMP header type: BITMAPINFOHEADER (V1)\n");
		printf("BMP header size ............... %u bytes\n", BMPHeaderV2.SizeHeader);
		printf("Image width ................... %d pixels\n", BMPHeaderV2.Width);
		printf("Image Height .................. %d pixels\n", BMPHeaderV2.Height);
		printf("Number of color plnaes ........ %u\n", BMPHeaderV2.Planes);
		printf("Bits per pixel ................ %u\n", BMPHeaderV2.ColorDepth);
		printf("Image size .................... %u", BMPHeaderV2.SizePixelMatrix);
		printf("X axis printing resolution .... %d", BMPHeaderV2.ResolutionX);
		printf("Y axis printing resolution .... %d", BMPHeaderV2.ResolutionY);
		printf("Number of palette colors ...... %u", BMPHeaderV2.NumColorsInTable);
		printf("Number of important colors .... %u", BMPHeaderV2.NumImportantColors);
		
		//Print compression method
		printf("Compression method ............ ");
		switch(BMPHeaderV2.Compression)
		{
			case 0 :
				printf("none (ID code: BI_RGB)\n");
				break;
				
			case 1 :
				printf("RLE 8-bit/pixel (ID code: BI_RLE8)\n");
				break;
				
			case 2 :
				printf("RLE 4-bit/pixel (ID code: BI_RLE4)\n");
				break;
				
			case 3 :
				printf("OS22XBITMAPHEADER: Huffman 1D (ID code: BI_BITFIELDS)\n");
				break;
				
			case 4 :
				printf("OS22XBITMAPHEADER: RLE-24 (ID code: BI_JPEG)\n");
				break;
				
			case 5 :
				printf("Undefined (ID code: BI_PNG)\n");
				break;
				
			case 6 :
				printf("RGBA bit field masks (ID code: BI_ALPHABITFIELDS)\n");
				break;
				
			case 11 :
				printf("none (ID code: BI_CMYK)\n");
				break;
				
			case 12 :
				printf("RLE-8 (ID code: BI_CMYKRLE8)");
				break;
				
			case 13 :
				printf("RLE-4 (ID code: BI_CMYKRLE4)");
				break;
				
			default :
				printf("Error! invalid value");
		}
		
		printf("Red mask ...................... %u", BMPHeaderV2.RedMask);
		printf("Green mask .................... %u", BMPHeaderV2.GreenMask);
		printf("Blue mask ..................... %u", BMPHeaderV2.BlueMask);
	
	}else if((FileHeader.OffsetPixelMatrix - sizeof(file_header_t)) == BITMAP_V3_INFOHEADER)
	{
	
		bmp_headerV3_t BMPHeaderV3;
		
		fread(&BMPHeaderV3, sizeof(bmp_headerV2_t), 1, File);
		printf("BMP header type: BITMAPINFOHEADER (V1)\n");
		printf("BMP header size ............... %u bytes\n", BMPHeaderV3.SizeHeader);
		printf("Image width ................... %d pixels\n", BMPHeaderV3.Width);
		printf("Image Height .................. %d pixels\n", BMPHeaderV3.Height);
		printf("Number of color plnaes ........ %u\n", BMPHeaderV3.Planes);
		printf("Bits per pixel ................ %u\n", BMPHeaderV3.ColorDepth);
		printf("Image size .................... %u", BMPHeaderV3.SizePixelMatrix);
		printf("X axis printing resolution .... %d", BMPHeaderV3.ResolutionX);
		printf("Y axis printing resolution .... %d", BMPHeaderV3.ResolutionY);
		printf("Number of palette colors ...... %u", BMPHeaderV3.NumColorsInTable);
		printf("Number of important colors .... %u", BMPHeaderV3.NumImportantColors);
		
		//Print compression method
		printf("Compression method ............ ");
		switch(BMPHeaderV3.Compression)
		{
			case 0 :
				printf("none (ID code: BI_RGB)\n");
				break;
				
			case 1 :
				printf("RLE 8-bit/pixel (ID code: BI_RLE8)\n");
				break;
				
			case 2 :
				printf("RLE 4-bit/pixel (ID code: BI_RLE4)\n");
				break;
				
			case 3 :
				printf("OS22XBITMAPHEADER: Huffman 1D (ID code: BI_BITFIELDS)\n");
				break;
				
			case 4 :
				printf("OS22XBITMAPHEADER: RLE-24 (ID code: BI_JPEG)\n");
				break;
				
			case 5 :
				printf("Undefined (ID code: BI_PNG)\n");
				break;
				
			case 6 :
				printf("RGBA bit field masks (ID code: BI_ALPHABITFIELDS)\n");
				break;
				
			case 11 :
				printf("none (ID code: BI_CMYK)\n");
				break;
				
			case 12 :
				printf("RLE-8 (ID code: BI_CMYKRLE8)");
				break;
				
			case 13 :
				printf("RLE-4 (ID code: BI_CMYKRLE4)");
				break;
				
			default :
				printf("Error! invalid value");
		}
		
		printf("Red mask ...................... %u", BMPHeaderV3.RedMask);
		printf("Green mask .................... %u", BMPHeaderV3.GreenMask);
		printf("Blue mask ..................... %u", BMPHeaderV3.BlueMask);
		
		printf("Alpha mask .................... %u", BMPHeaderV3.AlphaMask);
	
	}else if((FileHeader.OffsetPixelMatrix - sizeof(file_header_t)) == BITMAP_V4_INFOHEADER)
	{
	
		bmp_headerV4_t BMPHeaderV4;
	
	}else if((FileHeader.OffsetPixelMatrix - sizeof(file_header_t)) == BITMAP_V5_INFOHEADER)
	{
	
		bmp_headerV5_t BMPHeaderV5;
	
	}else
	{
		printf("Error: wrong size of BMP header");
		exit(EXIT_FAILURE);
	}
	
	fclose(File);
	
}






















