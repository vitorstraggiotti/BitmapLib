/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Header file of the library that manage bitmap images              *
 *                                                                   *
 * Author: Vitor Henrique Andrade Helfensteller Straggiotti Silva    *
 * Created on: 28/05/2021 (DD/MM/YYYY)                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __CREATE_BITMAP.H__
#define __CREATE_BITMAP.H__

/* Resolution in pixel/meter (39.3701 * DPI) */
#define RESOLUTION_X	2835
#define RESOLUTION_Y	2835

typedef struct full_header			header_t;
typedef struct pixel_24bpp			pixel_t;

/******************************* FUNCTIONS ************************************/
//inicialize image file (return header structure)
void create_bmp(int Width,
				int Heigth,
				unsigned int ResolutionX,
				unsigned int ResolutionY,
				pixel_t **PixelMatrix,
				const char *Filename);

/*************************** STRUCTURES ***************************************/
//temporarily set memory alignment to 1 byte
#pragma pack(push, 1)
//file header structure
struct full_header{
	//General file header (header_1 = 14 bytes)
	unsigned char CharID_1;						//Identification code for BMP file
	unsigned char CharID_2;
	unsigned int FileSize;						//Total file size (headers + pixel matrix with padding) in byte
	unsigned short int Reserved_1;					//Reserved space for utilization by other programs
	unsigned short int Reserved_2;					//Reserved space for utilization by other programs
	unsigned int PixelOffsetMatrix;			//Bytes shifted until the start of the pixel matrix (header_1 + header_2)
	//------------------------------------------------------------------------------------
	//Header structure for BMP image version: BITMAPINFOHEADER (header_2 = 40 bytes)
	unsigned int SizeHeader2;					//header_2 size in bytes
	int Width;							//Image width in pixel (left ==> right)
	int Height;							//Image height in pixel (positive low ==> high)
	unsigned short int Planes;						//Number of color planes used
	unsigned short int ColorDepth;					//Number of bits in one pixel (1, 4, 8, 16, 24, 32)
	unsigned int Compression;					//Compression method being used (0 ==> no compression)
	unsigned int SizePixelMatrix;				//Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
	unsigned int ResolutionX;					//Horizontal printing resolution in pixel/meter (39,3701 * DPI)
	unsigned int ResolutionY;					//Vertical printing resolution in pixel/meter (39,3701 * DPI)
	unsigned int NumColorsInTable;			//Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
	unsigned int NumImportantColors;		//Number of important colors used
};

//24 bits pixel structure
struct pixel_24bpp{
  unsigned char Red;
  unsigned char Green;
  unsigned char Blue;
};

#pragma pack(pop)

#endif

