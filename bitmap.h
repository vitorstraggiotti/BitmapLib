/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Header file of the library that manage bitmap images              *
 *                                                                   *
 * Author: Vitor Henrique Andrade Helfensteller Straggiotti Silva    *
 * Created on: 28/05/2021 (DD/MM/YYYY)                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __BITMAP_H__
#define __BITMAP_H__


/*******************************************************************************
 *                            MACROS AND TYPEDEF                               *
 *******************************************************************************/
 
//========================================= IMAGE FILE MANIPULATION ============

//Sizes of bitmap headers in bytes
#define BITMAP_V1_INFOHEADER	40
#define BITMAP_V2_INFOHEADER	52
#define BITMAP_V3_INFOHEADER	56
#define BITMAP_V4_INFOHEADER	108
#define BITMAP_V5_INFOHEADER	124

//Resolution in pixel/meter (39.3701 * DPI)
#define RESOLUTION_X	2834
#define RESOLUTION_Y	2834


//bmp_headerV1_t ==> BITMAPINFOHEADER	(40 bytes)
typedef struct bmp_headerV1			bmp_headerV1_t;

//bmp_headerV2_t ==> BITMAPV2INFOHEADER	(52 bytes)
typedef struct bmp_headerV2			bmp_headerV2_t;

//bmp_headerV3_t ==> BITMAPV3INFOHEADER	(56 bytes)
typedef struct bmp_headerV3			bmp_headerV3_t;

//bmp_headerV4_t ==> BITMAPV4HEADER		(108 bytes)
typedef struct bmp_headerV4			bmp_headerV4_t;

//bmp_headerV5_t ==> BITMAPV5HEADER		(124 bytes)
typedef struct bmp_headerV5			bmp_headerV5_t;

typedef struct file_header			file_header_t; //(14 bytes)
typedef struct pixel_24bpp			pixel_t;
typedef struct dimensions			dimensions_t;


/*******************************************************************************
 *                                  FUNCTIONS                                  *
 *******************************************************************************/

//========================================= IMAGE FILE MANIPULATION ============

//------------------------------------------------------------------------------
//create image file (return header structure)
void create_BMP(int Width,
				int Heigth,
				unsigned int ResolutionX,
				unsigned int ResolutionY,
				pixel_t **PixelMatrix,
				const char *Filename);
//------------------------------------------------------------------------------
//Read BMP image to a pixel matrix
pixel_t **read_BMP(const char *Filename);
//------------------------------------------------------------------------------
//Find Width of the BMP image
dimensions_t dimensions_BMP(const char *Filename);
//------------------------------------------------------------------------------
//Display header information
void display_header(const char *Filename);
//------------------------------------------------------------------------------
//Frees space occupied by PixelMatrix
void free_pixel_matrix(dimensions_t Dimension, pixel_t **PixelMatrix);


/*******************************************************************************
 *                                   STRUCTURES                                *
 *******************************************************************************/

//temporarily set memory alignment to 1 byte
#pragma pack(push, 1)

//24 bits pixel structure
struct pixel_24bpp
{
  unsigned char Blue;
  unsigned char Green;
  unsigned char Red;
};

//file header structure
struct file_header
{
	//General file header (header_1 = 14 bytes)
	unsigned char CharID_1;				//Identification code for BMP file
	unsigned char CharID_2;
	unsigned int FileSize;				//Total file size (headers + pixel matrix with padding) in byte
	unsigned short int Reserved_1;		//Reserved space for utilization by other programs
	unsigned short int Reserved_2;		//Reserved space for utilization by other programs
	unsigned int OffsetPixelMatrix;		//Bytes shifted until the start of the pixel matrix (header_1 + header_2)
};

//Header structure for BMP image version: BITMAPINFOHEADER (40 bytes)
struct bmp_headerV1
{
	unsigned int SizeHeader;			//header_2 size in bytes
	int Width;							//Image width in pixel (left ==> right)
	int Height;							//Image height in pixel (positive low ==> high)
	unsigned short int Planes;			//Number of color planes used
	unsigned short int ColorDepth;		//Number of bits in one pixel (1, 4, 8, 16, 24, 32)
	unsigned int Compression;			//Compression method being used (0 ==> no compression)
	unsigned int SizePixelMatrix;		//Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
	int ResolutionX;					//Horizontal printing resolution in pixel/meter (39,3701 * DPI)
	int ResolutionY;					//Vertical printing resolution in pixel/meter (39,3701 * DPI)
	unsigned int NumColorsInTable;		//Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
	unsigned int NumImportantColors;	//Number of important colors used
};

//Header structure for BMP image version: BITMAPV2INFOHEADER (52 bytes)
struct bmp_headerV2
{
	unsigned int SizeHeader;			//header_2 size in bytes
	int Width;							//Image width in pixel (left ==> right)
	int Height;							//Image height in pixel (positive low ==> high)
	unsigned short int Planes;			//Number of color planes used
	unsigned short int ColorDepth;		//Number of bits in one pixel (1, 4, 8, 16, 24, 32)
	unsigned int Compression;			//Compression method being used (0 ==> no compression)
	unsigned int SizePixelMatrix;		//Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
	int ResolutionX;					//Horizontal printing resolution in pixel/meter (39,3701 * DPI)
	int ResolutionY;					//Vertical printing resolution in pixel/meter (39,3701 * DPI)
	unsigned int NumColorsInTable;		//Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
	unsigned int NumImportantColors;	//Number of important colors used
	
	//New additions compared to the latest version
	unsigned int RedMask;		//Color mask valid only if Compression is set
	unsigned int GreenMask;		//Color mask valid only if Compression is set
	unsigned int BlueMask;		//Color mask valid only if Compression is set
};

//Header structure for BMP image version: BITMAPV3INFOHEADER (56 bytes)
struct bmp_headerV3
{
	unsigned int SizeHeader;			//header_2 size in bytes
	int Width;							//Image width in pixel (left ==> right)
	int Height;							//Image height in pixel (positive low ==> high)
	unsigned short int Planes;			//Number of color planes used
	unsigned short int ColorDepth;		//Number of bits in one pixel (1, 4, 8, 16, 24, 32)
	unsigned int Compression;			//Compression method being used (0 ==> no compression)
	unsigned int SizePixelMatrix;		//Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
	int ResolutionX;					//Horizontal printing resolution in pixel/meter (39,3701 * DPI)
	int ResolutionY;					//Vertical printing resolution in pixel/meter (39,3701 * DPI)
	unsigned int NumColorsInTable;		//Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
	unsigned int NumImportantColors;	//Number of important colors used
	
	unsigned int RedMask;		//Color mask valid only if Compression is set
	unsigned int GreenMask;		//Color mask valid only if Compression is set
	unsigned int BlueMask;		//Color mask valid only if Compression is set
	
	//New addition compared to the latest version
	unsigned int AlphaMask;		//Color mask that specifies the alpha component of each pixel
};

//Header structure for BMP image version: BITMAPV4HEADER (108 bytes)
struct bmp_headerV4
{
	unsigned int SizeHeader;			//header_2 size in bytes
	int Width;							//Image width in pixel (left ==> right)
	int Height;							//Image height in pixel (positive low ==> high)
	unsigned short int Planes;			//Number of color planes used
	unsigned short int ColorDepth;		//Number of bits in one pixel (1, 4, 8, 16, 24, 32)
	unsigned int Compression;			//Compression method being used (0 ==> no compression)
	unsigned int SizePixelMatrix;		//Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
	int ResolutionX;					//Horizontal printing resolution in pixel/meter (39,3701 * DPI)
	int ResolutionY;					//Vertical printing resolution in pixel/meter (39,3701 * DPI)
	unsigned int NumColorsInTable;		//Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
	unsigned int NumImportantColors;	//Number of important colors used
	
	unsigned int RedMask;		//Color mask valid only if Compression is set
	unsigned int GreenMask;		//Color mask valid only if Compression is set
	unsigned int BlueMask;		//Color mask valid only if Compression is set
	
	unsigned int AlphaMask;		//Color mask that specifies the alpha component of each pixel
	
	//New additions compared to the latest version
	unsigned int CSType;		//Define color space
	int RedX;					//X coordinate of red endpoint
	int RedY;					//Y coordinate of red endpoint
	int RedZ;					//Z coordinate of red endpoint
	int GreenX;					//X coordinate of green endpoint
	int GreenY;					//Y coordinate of green endpoint
	int GreenZ;					//Z coordinate of green endpoint
	int BlueX;					//X coordinate of blue endpoint
	int BlueY;					//Y coordinate of blue endpoint
	int BlueZ;					//Z coordinate of blue endpoint
	unsigned int GammaRed;		//Toned response curve for red. Used if CDType is set
	unsigned int GammaGreen;	//Toned response curve for green. Used if CDType is set
	unsigned int GammaBlue;		//Toned response curve for blue. Used if CDType is set
};

//Header structure for BMP image version: BITMAPV5HEADER (124 bytes)
struct bmp_headerV5
{
	unsigned int SizeHeader;			//header_2 size in bytes
	int Width;							//Image width in pixel (left ==> right)
	int Height;							//Image height in pixel (positive low ==> high)
	unsigned short int Planes;			//Number of color planes used
	unsigned short int ColorDepth;		//Number of bits in one pixel (1, 4, 8, 16, 24, 32)
	unsigned int Compression;			//Compression method being used (0 ==> no compression)
	unsigned int SizePixelMatrix;		//Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
	int ResolutionX;					//Horizontal printing resolution in pixel/meter (39,3701 * DPI)
	int ResolutionY;					//Vertical printing resolution in pixel/meter (39,3701 * DPI)
	unsigned int NumColorsInTable;		//Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
	unsigned int NumImportantColors;	//Number of important colors used
	
	unsigned int RedMask;		//Color mask valid only if Compression is set
	unsigned int GreenMask;		//Color mask valid only if Compression is set
	unsigned int BlueMask;		//Color mask valid only if Compression is set
	
	unsigned int AlphaMask;		//Color mask that specifies the alpha component of each pixel
	
	unsigned int CSType;		//Define color space
	int RedX;					//X coordinate of red endpoint
	int RedY;					//Y coordinate of red endpoint
	int RedZ;					//Z coordinate of red endpoint
	int GreenX;					//X coordinate of green endpoint
	int GreenY;					//Y coordinate of green endpoint
	int GreenZ;					//Z coordinate of green endpoint
	int BlueX;					//X coordinate of blue endpoint
	int BlueY;					//Y coordinate of blue endpoint
	int BlueZ;					//Z coordinate of blue endpoint
	unsigned int GammaRed;		//Toned response curve for red. Used if CDType is set
	unsigned int GammaGreen;	//Toned response curve for green. Used if CDType is set
	unsigned int GammaBlue;		//Toned response curve for blue. Used if CDType is set
	
	//New additions compared to the latest version
	unsigned int Intent;		//Rendering intent for bitmap
	unsigned int ProfileData;   //The offset, in bytes, from beginning of BITMAPV5HEADER to profile data
	unsigned int ProfileSize;	//Size, in bytes, of embedded profile data
	unsigned int Reserved;		//Should be 0
};

#pragma pack(pop)

struct dimensions
{
	int Width;
	int Height;
};
#endif

