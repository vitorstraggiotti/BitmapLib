/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Header file of the library that manage bitmap images              *
 *                                                                   *
 * Author: Vitor Henrique Andrade Helfensteller Straggiotti Silva    *
 * Created on: 28/05/2021 (DD/MM/YYYY)                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Resolution in pixel/meter (39.3701 * DPI) */
#define RESOLUTION_X	2835
#define RESOLUTION_Y	2835

typedef struct header				header_t;
typedef struct pixel_24bpp			pixel_t;

/******************************* FUNCTIONS ************************************/
//inicialize image file (return header structure)
header_t inicialize_bmp(int32_t Width,
						int32_t Heigth,
						uint32_t ResolutionX,
						uint32_t ResolutionY,
						pixel_t **PixelMatrix
						const char *Filename);

/*************************** STRUCTURES ***************************************/
//temporarily set memory alignment to 1 byte
#pragma pack(push, 1)
//file header structure
struct header{
	//General file header (header_1 = 14 bytes)
	uint8_t CharID_1='B';					//Identification code for BMP file
	uint8_t CharID_2='M';
	uint32_t FileSize;						//Total file size (headers + pixel matrix with padding) in byte
	uint16_t Reserved_1=0;					//Reserved space for utilization by other programs
	uint16_t Reserved_2=0;					//Reserved space for utilization by other programs
	uint32_t PixelOffsetMatrix=54;			//Bytes shifted until the start of the pixel matrix (header_1 + header_2)
	//------------------------------------------------------------------------------------
	//Header structure for BMP image version: BITMAPINFOHEADER (header_2 = 40 bytes)
	uint32_t SizeHeader2=40;				//header_2 size in bytes
	int32_t Width;							//Image width in pixel (left ==> right)
	int32_t Height;							//Image height in pixel (positive low ==> high)
	uint16_t Planes=1;						//Number of color planes used
	uint16_t ColorDepth=24;					//Number of bits in one pixel (1, 4, 8, 16, 24, 32)
	uint32_t Compression=0;					//Compression method being used (0 ==> no compression)
	uint32_t SizePixelMatriz;				//Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
	uint32_t ResolutionX;					//Horizontal printing resolution in pixel/meter (39,3701 * DPI)
	uint32_t ResolutionY;					//Vertical printing resolution in pixel/meter (39,3701 * DPI)
	uint32_t NumColorsInTable=0;			//Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
	uint32_t NumImportantColors=0;			//Number of important colors used
};

//24 bits pixel structure
struct pixel_24bpp{
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
};

#pragma pack(pop)

