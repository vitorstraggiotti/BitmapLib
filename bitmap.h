/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Header file of the library that manage bitmap images              *
 *                                                                   *
 * Author: Vitor Henrique Andrade Helfensteller Straggiotti Silva    *
 * Created on: 28/05/2021 (DD/MM/YYYY)                               *
 * Version: 1.2.0  ([major].[minor].[bugs])
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdint.h>

//Sizes of bitmap headers in bytes
#define BITMAP_V1_INFOHEADER    40
#define BITMAP_V2_INFOHEADER    52
#define BITMAP_V3_INFOHEADER    56
#define BITMAP_V4_INFOHEADER    108
#define BITMAP_V5_INFOHEADER    124

//Resolution in pixel/meter (39.3701 * DPI)
#define RESOLUTION_X    2834
#define RESOLUTION_Y    2834

/*******************************************************************************
 *                                   STRUCTURES                                *
 *******************************************************************************/

//temporarily set memory alignment to 1 byte
#pragma pack(push, 1)

//24 bits pixel structure
struct pixel_24bpp
{
  uint8_t Blue;
  uint8_t Green;
  uint8_t Red;
};

//file header structure
struct file_header
{
    //General file header (header_1 = 14 bytes)
    uint8_t CharID_1;                   //Identification code for BMP file
    uint8_t CharID_2;
    uint32_t FileSize;                  //Total file size (headers + pixel matrix with padding) in byte
    uint16_t Reserved_1;                //Reserved space for utilization by other programs
    uint16_t Reserved_2;                //Reserved space for utilization by other programs
    uint32_t OffsetPixelMatrix;         //Bytes shifted until the start of the pixel matrix (header_1 + header_2)
};

//Header structure for BMP image version: BITMAPINFOHEADER (40 bytes)
struct bmp_headerV1
{
    uint32_t SizeHeader;                //header_2 size in bytes
    int32_t Width;                      //Image width in pixel (left ==> right)
    int32_t Height;                     //Image height in pixel (positive low ==> high)
    uint16_t Planes;                    //Number of color planes used
    uint16_t ColorDepth;                //Number of bits in one pixel (1, 4, 8, 16, 24, 32)
    uint32_t Compression;               //Compression method being used (0 ==> no compression)
    uint32_t SizePixelMatrix;           //Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
    int32_t ResolutionX;                //Horizontal printing resolution in pixel/meter (39,3701 * DPI)
    int32_t ResolutionY;                //Vertical printing resolution in pixel/meter (39,3701 * DPI)
    uint32_t NumColorsInTable;          //Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
    uint32_t NumImportantColors;        //Number of important colors used
};

//Header structure for BMP image version: BITMAPV2INFOHEADER (52 bytes)
struct bmp_headerV2
{
    uint32_t SizeHeader;                //header_2 size in bytes
    int32_t Width;                      //Image width in pixel (left ==> right)
    int32_t Height;                     //Image height in pixel (positive low ==> high)
    uint16_t Planes;                    //Number of color planes used
    uint16_t ColorDepth;                //Number of bits in one pixel (1, 4, 8, 16, 24, 32)
    uint32_t Compression;               //Compression method being used (0 ==> no compression)
    uint32_t SizePixelMatrix;           //Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
    int32_t ResolutionX;                //Horizontal printing resolution in pixel/meter (39,3701 * DPI)
    int32_t ResolutionY;                //Vertical printing resolution in pixel/meter (39,3701 * DPI)
    uint32_t NumColorsInTable;          //Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
    uint32_t NumImportantColors;        //Number of important colors used
    
    //New additions compared to the latest version
    uint32_t RedMask;       //Color mask valid only if Compression is set
    uint32_t GreenMask;     //Color mask valid only if Compression is set
    uint32_t BlueMask;      //Color mask valid only if Compression is set
};

//Header structure for BMP image version: BITMAPV3INFOHEADER (56 bytes)
struct bmp_headerV3
{
    uint32_t SizeHeader;                //header_2 size in bytes
    int32_t Width;                      //Image width in pixel (left ==> right)
    int32_t Height;                     //Image height in pixel (positive low ==> high)
    uint16_t Planes;                    //Number of color planes used
    uint16_t ColorDepth;                //Number of bits in one pixel (1, 4, 8, 16, 24, 32)
    uint32_t Compression;               //Compression method being used (0 ==> no compression)
    uint32_t SizePixelMatrix;           //Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
    int32_t ResolutionX;                //Horizontal printing resolution in pixel/meter (39,3701 * DPI)
    int32_t ResolutionY;                //Vertical printing resolution in pixel/meter (39,3701 * DPI)
    uint32_t NumColorsInTable;          //Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
    uint32_t NumImportantColors;        //Number of important colors used
    
    uint32_t RedMask;       //Color mask valid only if Compression is set
    uint32_t GreenMask;     //Color mask valid only if Compression is set
    uint32_t BlueMask;      //Color mask valid only if Compression is set
    
    //New addition compared to the latest version
    uint32_t AlphaMask;     //Color mask that specifies the alpha component of each pixel
};

//Header structure for BMP image version: BITMAPV4HEADER (108 bytes)
struct bmp_headerV4
{
    uint32_t SizeHeader;                //header_2 size in bytes
    int32_t Width;                      //Image width in pixel (left ==> right)
    int32_t Height;                     //Image height in pixel (positive low ==> high)
    uint16_t Planes;                    //Number of color planes used
    uint16_t ColorDepth;                //Number of bits in one pixel (1, 4, 8, 16, 24, 32)
    uint32_t Compression;               //Compression method being used (0 ==> no compression)
    uint32_t SizePixelMatrix;           //Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
    int32_t ResolutionX;                //Horizontal printing resolution in pixel/meter (39,3701 * DPI)
    int32_t ResolutionY;                //Vertical printing resolution in pixel/meter (39,3701 * DPI)
    uint32_t NumColorsInTable;          //Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
    uint32_t NumImportantColors;        //Number of important colors used
    
    uint32_t RedMask;       //Color mask valid only if Compression is set
    uint32_t GreenMask;     //Color mask valid only if Compression is set
    uint32_t BlueMask;      //Color mask valid only if Compression is set
    
    uint32_t AlphaMask;     //Color mask that specifies the alpha component of each pixel
    
    //New additions compared to the latest version
    uint32_t CSType;                    //Define color space
    int32_t RedX;                       //X coordinate of red endpoint
    int32_t RedY;                       //Y coordinate of red endpoint
    int32_t RedZ;                       //Z coordinate of red endpoint
    int32_t GreenX;                     //X coordinate of green endpoint
    int32_t GreenY;                     //Y coordinate of green endpoint
    int32_t GreenZ;                     //Z coordinate of green endpoint
    int32_t BlueX;                      //X coordinate of blue endpoint
    int32_t BlueY;                      //Y coordinate of blue endpoint
    int32_t BlueZ;                      //Z coordinate of blue endpoint
    uint32_t GammaRed;                  //Toned response curve for red. Used if CDType is set
    uint32_t GammaGreen;                //Toned response curve for green. Used if CDType is set
    uint32_t GammaBlue;                 //Toned response curve for blue. Used if CDType is set
};

//Header structure for BMP image version: BITMAPV5HEADER (124 bytes)
struct bmp_headerV5
{
    uint32_t SizeHeader;                //header_2 size in bytes
    int32_t Width;                      //Image width in pixel (left ==> right)
    int32_t Height;                     //Image height in pixel (positive low ==> high)
    uint16_t Planes;                    //Number of color planes used
    uint16_t ColorDepth;                //Number of bits in one pixel (1, 4, 8, 16, 24, 32)
    uint32_t Compression;               //Compression method being used (0 ==> no compression)
    uint32_t SizePixelMatrix;           //Size of the raw bitmap data with padding (pixel matrix) in bytes (4 byte alignment)
    int32_t ResolutionX;                    //Horizontal printing resolution in pixel/meter (39,3701 * DPI)
    int32_t ResolutionY;                //Vertical printing resolution in pixel/meter (39,3701 * DPI)
    uint32_t NumColorsInTable;          //Number of colors on the color table (used when bpp<=8) (is '0' if all colors used)
    uint32_t NumImportantColors;        //Number of important colors used
    
    uint32_t RedMask;       //Color mask valid only if Compression is set
    uint32_t GreenMask;     //Color mask valid only if Compression is set
    uint32_t BlueMask;      //Color mask valid only if Compression is set
    
    uint32_t AlphaMask;     //Color mask that specifies the alpha component of each pixel
    
    uint32_t CSType;                    //Define color space
    int32_t RedX;                       //X coordinate of red endpoint
    int32_t RedY;                       //Y coordinate of red endpoint
    int32_t RedZ;                       //Z coordinate of red endpoint
    int32_t GreenX;                     //X coordinate of green endpoint
    int32_t GreenY;                     //Y coordinate of green endpoint
    int32_t GreenZ;                     //Z coordinate of green endpoint
    int32_t BlueX;                      //X coordinate of blue endpoint
    int32_t BlueY;                      //Y coordinate of blue endpoint
    int32_t BlueZ;                      //Z coordinate of blue endpoint
    uint32_t GammaRed;                  //Toned response curve for red. Used if CDType is set
    uint32_t GammaGreen;                //Toned response curve for green. Used if CDType is set
    uint32_t GammaBlue;                 //Toned response curve for blue. Used if CDType is set
    
    //New additions compared to the latest version
    uint32_t Intent;        //Rendering intent for bitmap
    uint32_t ProfileData;   //The offset, in bytes, from beginning of BITMAPV5HEADER to profile data
    uint32_t ProfileSize;   //Size, in bytes, of embedded profile data
    uint32_t Reserved;      //Should be 0
};
#pragma pack(pop)

struct img
{
    /* Image dimensions */
    int32_t Width;
    int32_t Height;

    /* Pixel map */
    struct  pixel_24bpp **Pixel24;  /* 3 channels with 8 bits (RGB) */
    uint8_t **Pixel8;               /* 1 channel with 8 bits (Grayscale) */
};

enum color_depth
{
    DEPTH24 = 24,
    DEPTH8 = 8
};

//bmp_headerV1_t ==> BITMAPINFOHEADER   (40 bytes)
typedef struct bmp_headerV1         bmp_headerV1_t;

//bmp_headerV2_t ==> BITMAPV2INFOHEADER (52 bytes)
typedef struct bmp_headerV2         bmp_headerV2_t;

//bmp_headerV3_t ==> BITMAPV3INFOHEADER (56 bytes)
typedef struct bmp_headerV3         bmp_headerV3_t;

//bmp_headerV4_t ==> BITMAPV4HEADER     (108 bytes)
typedef struct bmp_headerV4         bmp_headerV4_t;

//bmp_headerV5_t ==> BITMAPV5HEADER     (124 bytes)
typedef struct bmp_headerV5         bmp_headerV5_t;

typedef struct file_header          file_header_t; //(14 bytes)

typedef struct pixel_24bpp          pixel24_t;
typedef struct img                  img_t;


/*******************************************************************************
 *                                  FUNCTIONS                                  *
 *******************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Create BMP image file (header used: BITMAPINFOHEADER (V1))
 *   
 * Img        --> [Input]: pointer to image struct to be saved
 * Filename   --> [Input]: pointer to array of char that will be the name of saved image
 *
 * Return -1 if fail and 0 on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int save_BMP(img_t *Img, const char *Filename);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Create BMP image file (header used: BITMAPINFOHEADER (V1))
 * 
 * Filename   --> [Input]: pointer to array of char that will be the name of saved imag
 *
 * Return NULL if fail or pointer to image structure on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
img_t *read_BMP(const char *Filename);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Create new blank image struct (all pixels are 0)
 * 
 * Width      --> [Input]: image width in pixels
 * Height     --> [Input]: image height in pixels
 * ColorDepth --> [Input]: integer from enummeration tha especifies the color depth:
 *                         (0) DEPTH24 ==> Set 24 bits per pixel
 *                         (1) SEPTH8  ==> Set 8 bits per pixel
 *
 * Return NULL if fail or pointer to image structure on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
img_t *new_BMP(int32_t Width, int32_t Height, int32_t ColorDepth);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Create new blank image struct (all pixels are 0)
 * 
 * OriginalImmmage --> [Input]: pointer to image struct to use as reference to create
 *                              new blank image struct with samme spacial resolution
 * ColorDepth      --> [Input]: integer from enummeration tha especifies the color depth:
 *                              DEPTH24 ==> Set 24 bits per pixel
 *                              SEPTH8  ==> Set 8 bits per pixel
 *
 * Return NULL if fail or pointer to image structure on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
img_t *new_BMP_as_size(img_t *OriginalImage, int32_t ColorDepth);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Create a copy of the original image struct 
 * 
 * OriginalImmmage --> [Input]: pointer to image struct to be copied
 *
 * Return NULL if fail or pointer to image structure on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
img_t *copy_BMP(img_t *OriginalImage);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Deallocate the image struct 
 * 
 * Img --> [Input]: pointer to image struct to be deallocated
 *
 * Return -1 if fail or 0 on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int free_img(img_t *Img);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Display header information of .bpm iage file
 * 
 * Filename --> [Input]: pointer to string containing the namme of the file
 *                       to display info about.
 *
 * Does not return anything
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void display_header(const char *Filename);


#endif /* __BITMAP_H__ */

