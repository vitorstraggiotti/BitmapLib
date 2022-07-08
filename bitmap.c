/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Source code to manage bitmap image                                           *
 * Image creation characteristics:                                              *
 * 24 bit color depth, no color table, no compression                           *
 *                                                                              *
 * Autor: Vitor Henrique Andrade Helfensteller Satraggiotti Silva               *
 * Start date: 28/05/2021   (DD/MM/YYYY)                                        *
 * Version: 1.2.0  ([major].[minor].[bugs])                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
 
 // 1) Adicionar suporte para imagens 8 bits (escala de cinza)
 //    1.1) expandir extrutura de imagem para suportar pixel 8 bits
 //    1.2) Adicionar suporte na função de salvamento para pixel 8 bits
 
 
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>     /* To use precise data types (uint8_t, uint16_t ...) */

#include "bitmap.h"

/*******************************************************************************
 *                                 HELPER FUNCTIONS                            *
 *******************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * HELPER function that check if the input of function "save_BMP" is valid
 * Used on: save_BMP()
 * 
 * Img      --> [Input]: pointer to image struct
 * Filename --> [Input]: pointer to array of characters that hold the filename
 *  
 * Return 1 if arguments ar valid and 0 if not
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static inline int args_for_save_BMP_is_not_valid(img_t *Img, const char *Filename)
{
    if((Img == NULL) || (Filename == NULL))
    {
        printf("Error: [args_for_save_BMP_is_not_valid()] --> Invalid arguments.\n");
        return 1;
    }
    else if((Img->Pixel24 == NULL) && (Img->Pixel8 == NULL))
    {
        printf("Error: [args_for_save_BMP_is_not_valid()] --> Image argument is empty.\n");
        return 1;
    }
    else if((Img->Width > 20000)||(Img->Height > 20000))
    {
        printf("Error: [args_for_save_BMP_is_not_valid()] --> Dimensions too big. Limit: 20000 x 20000.\n");
        return 1;
    }
    else if((Img->Width < 2)||(Img->Height < 2))
    {
        printf("Error: [args_for_save_BMP_is_not_valid()] --> Dimensions too small. Limit: 2 x 2.\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * HELPER function that write pixel matrix to file
 * Used on: save_BMP()
 *
 * Img        --> [Input]: pointer to image struct
 * ColorDepth --> [Input]: integer from enummeration tha especifies the color depth:
 *                         (0) DEPTH24 ==> Set 24 bits per pixel
 *                         (1) SEPTH8  ==> Set 8 bits per pixel
 * ImageFile  --> [Output]: pointer to file where pixel matrix will be saved
 * 
 * Return -1 if fail and 0 on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static inline int write_pixel_matrix_to_file(img_t * Img, int32_t ColorDepth,\
                                             FILE * ImageFile)
{
    uint8_t ByteZero = 0;

    int32_t TotalWidthMod4 = -1; /* pixel matrix width in bytes modulus 4*/

    TotalWidthMod4 = (Img->Width * 3) % 4;

    switch (ColorDepth)
    {
        case DEPTH24:
            TotalWidthMod4 = (Img->Width * 3) % 4;
            break;

        case DEPTH8:
            TotalWidthMod4 = Img->Width % 4;
            break;

        default:
            printf("Error: [write_pixel_matrix_to_file()] --> Invalid color depth.\n");
            return -1;
    }

    for (int32_t row = Img->Height - 1; row >= 0; row--)
    {
        for (int32_t column = 0; column < Img->Width; column++)
        {
            /* Save pixel value */
            switch (ColorDepth)
            {
                case DEPTH24:
                    if (fwrite(&Img->Pixel24[row][column], sizeof(pixel24_t), 1, ImageFile) != 1)
                    {
                        printf("Error: [write_pixel_matrix_to_file()] --> Could not write pixel to file.\n");
                        return -1;
                    }
                    break;

                case DEPTH8:
                    if (fwrite(&Img->Pixel8[row][column], sizeof(uint8_t), 1, ImageFile) != 1)
                    {
                        printf("Error: [write_pixel_matrix_to_file()] --> Could not write pixel to file.\n");
                        return -1;
                    }
                    break;

                default:
                    printf("Error: [write_pixel_matrix_to_file()] --> Invalid color depth\n");
                    return -1;
            }

            /* Add padding to row to make 4 byte alligned */
            if (column == (Img->Width - 1))
            {
                switch (TotalWidthMod4)
                {
                    case 0:
                        /* DO NOTHING */
                        break;
                    
                    case 1:
                        if(fwrite(&ByteZero, sizeof(uint8_t), 3, ImageFile) != 1)
                        {
                            printf("Error: [write_pixel_matrix_to_file()] --> Could not write padding to file.\n");
                            return -1;
                        }
                        break;
                    
                    case 2:
                        if (fwrite(&ByteZero, sizeof(uint8_t), 2, ImageFile) != 1)
                        {
                            printf("Error: [write_pixel_matrix_to_file()] --> Could not write padding to file.\n");
                            return -1;
                        }
                        break;
                    
                    case 3:
                        if (fwrite(&ByteZero, sizeof(uint8_t), 1, ImageFile) != 1)
                        {
                            printf("Error: [write_pixel_matrix_to_file()] --> Could not write padding to file.\n");
                            return -1;
                        }
                        break;
                    
                    default:
                        printf("Error: [write_pixel_matrix_to_file()] --> Invalid \"TotalWidthMod4\"\n");
                        return -1;
                } /* end padding switch */
            } /* last column if */

        }/* column for loop */
    }/* row for loop */

    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * HELPER function that write headers to file
 * Used on: saved_BMP()
 * 
 * FileHeader --> [Input]: pointer to file header to be written to file
 * BMPHeader  --> [Input]: pointer to BMP info header to be written to file
 * ImageFile  --> [Output]: pointer to file where the headers will be saved
 * 
 * Return -1 if fail or 0 on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static inline int write_headers_to_file(file_header_t * FileHeader,\
                                        bmp_headerV1_t * BMPHeaderV1,\
                                        FILE * ImageFile)
{
    if(fwrite(FileHeader, sizeof(file_header_t), 1, ImageFile) != 1)
    {
        printf("Error: [write_headers_to_file()] --> Could not write \"File Header\" to file.\n");
        return -1;
    }
    if(fwrite(BMPHeaderV1, sizeof(bmp_headerV1_t), 1, ImageFile) != 1)
    {
        printf("Error: [write_headers_to_file()] --> Could not write \"BMP Header\" to file.\n");
        return -1;
    }

    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * HELPER function that read pixel matrix from file and put it on an image struct
 * 
 * ImageFile  --> [Input]: pointer to file where the pixel matrix will be read
 * ColorDepth --> [Input]: integer from enummeration tha especifies the color depth:
 *                         (0) DEPTH24 ==> Set 24 bits per pixel
 *                         (1) SEPTH8  ==> Set 8 bits per pixel
 * Img        --> [Output]: pointer to image struct where the pixel matrix will be written
 * 
 * Return -1 if fail or 0 on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static inline int read_pixel_matrix_from_file(FILE * ImageFile, int32_t ColorDepth, img_t * Img)
{
    uint8_t Trash;
    
    /* Allocate space for pixel matrix */
    switch (ColorDepth)
    {
        case DEPTH24:
            
            Img->Pixel24 = malloc(Img->Height * sizeof(pixel24_t*));
            if (Img->Pixel24 == NULL)
            {
                printf("Error: [read_pixel_matrix_from_file()] --> Could not allocate memory for pixel matrix (rows; 24bpp)\n");
                return -1;
            }

            for(int32_t Row = 0; Row < Img->Height; Row++)
            {
                Img->Pixel24[Row] = malloc(Img->Width * sizeof(pixel24_t));
                if (Img->Pixel24[Row] == NULL)
                {
                    printf("Error: [read_pixel_matrix_from_file()] --> Could not allocate memory for pixel matrix (columns; 24bpp)\n");
                    return -1;
                }
            }
    
            /* Reading image and copying to pixel matrix */
            for(int32_t Row = 0; Row < Img->Height; Row++)
            {
                for(int32_t Column = 0; Column < Img->Width; Column++)
                {
                    if(fread(&Img->Pixel24[Row][Column], sizeof(pixel24_t), 1, ImageFile) != 1)
                    {
                        printf("Error: [read_pixel_matrix_from_file()] --> Could not read pixel values from file. (24bpp)\n");
                        return -1;
                    }
                    
                    if(Column == (Img->Width - 1))
                    {
                        if(((Img->Width * 3) % 4) == 1)
                        {
                            if(fread(&Trash, sizeof(uint8_t), 3, ImageFile) != 3)
                            {
                                printf("Error: [read_pixel_mmatrix_from_file()] --> Could not read 3 padding bytes. (24bpp)\n");
                                return -1;
                            }
                            
                        }else if(((Img->Width * 3) % 4) == 2)
                        {
                            if(fread(&Trash, sizeof(uint8_t), 2, ImageFile) != 2)
                            {
                                printf("Error: [read_pixel_mmatrix_from_file()] --> Could not read 2 padding bytes. (24bpp)\n");
                                return -1;
                            }
                            
                        }else if(((Img->Width * 3) % 4) == 3)
                        {
                            if(fread(&Trash, sizeof(uint8_t), 1, ImageFile) != 1)
                            {
                                printf("Error: [read_pixel_mmatrix_from_file()] --> Could not read 1 padding byte. (24bpp)\n");
                                return -1;
                            }
                        }
                    }
                }
            }

            break;

        case DEPTH8:
            
            Img->Pixel8 = malloc(Img->Height * sizeof(uint8_t*));
            if (Img->Pixel8 == NULL)
            {
                printf("Error: [read_pixel_matrix_from_file()] --> Could not allocate memory for pixel matrix (rows; 8bpp)\n");
                return -1;
            }

            for(int32_t Row = 0; Row < Img->Height; Row++)
            {
                Img->Pixel8[Row] = malloc(Img->Width * sizeof(uint8_t));
                if (Img->Pixel8[Row] == NULL)
                {
                    printf("Error: [read_pixel_matrix_from_file()] --> Could not allocate memory for pixel matrix (columns; 8bpp)\n");
                    return -1;
                }
            }

            /* Reading image and copying to pixel matrix */
            for(int32_t Row = 0; Row < Img->Height; Row++)
            {
                for(int32_t Column = 0; Column < Img->Width; Column++)
                {
                    if(fread(&Img->Pixel8[Row][Column], sizeof(uint8_t), 1, ImageFile) != 1)
                    {
                        printf("Error: [read_pixel_matrix_from_file()] --> Could not read pixel values from file.\n");
                        return -1;
                    }
                    
                    if(Column == (Img->Width - 1))
                    {
                        if(((Img->Width * 3) % 4) == 1)
                        {
                            if(fread(&Trash, sizeof(uint8_t), 3, ImageFile) != 3)
                            {
                                printf("Error: [read_pixel_matrix_from_file()] --> Could not read 3 padding bytes.\n");
                                return -1;
                            }
                            
                        }else if(((Img->Width * 3) % 4) == 2)
                        {
                            if(fread(&Trash, sizeof(uint8_t), 2, ImageFile) != 2)
                            {
                                printf("Error: [read_pixel_matrix_from_file()] --> Could not read 2 padding bytes.\n");
                                return -1;
                            }
                            
                        }else if(((Img->Width * 3) % 4) == 3)
                        {
                            if(fread(&Trash, sizeof(uint8_t), 1, ImageFile) != 1)
                            {
                                printf("Error: [read_pixel_matrix_from_file()] --> Could not read 1 padding byte.\n");
                                return -1;
                            }
                        }
                    }
                }
            }

            break;
        
        default:
            printf("Error: [read_pixel_matrix_from_file()] --> Invalid Color depth\n");
            return -1;
    }

    return 0;
}


/*******************************************************************************
 *                              FUNCTION DEFINITIONS                           *
 *******************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Create BMP image file (header used: BITMAPINFOHEADER (V1))
 *   
 * Img        --> [Input]: pointer to image struct to be saved
 * Filename   --> [Input]: pointer to array of char that will be the name of saved image
 *
 * Return -1 if fail and 0 on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int save_BMP(img_t *Img, const char *Filename)
{
    file_header_t   FileHeader;
    bmp_headerV1_t  BMPHeaderV1;

    int32_t         SizeWidthByte;
    int32_t         TotalWidthMod4;
    int32_t         ColorDepth = 0;

    FILE *ImageFile;

    /* Validate arguments */
    if (args_for_save_BMP_is_not_valid(Img, Filename))
    {
        return -1;
    }

    /* Acquire color depth */
    if ((Img->Pixel24 != NULL) && (Img->Pixel8 == NULL))
    {
        ColorDepth = DEPTH24;
    }
    else if ((Img->Pixel8 != NULL) && (Img->Pixel24 == NULL))
    {
        ColorDepth = DEPTH8;
    }
    else
    {
        printf("Error: [save_BMP()] --> Failed color depth validation.\n");
        return -1;
    }
    

    FileHeader.CharID_1 = 0x42;
    FileHeader.CharID_2 = 0x4D;
    FileHeader.Reserved_1 = 0;
    FileHeader.Reserved_2 = 0;
    FileHeader.OffsetPixelMatrix = 54;

    BMPHeaderV1.SizeHeader = 40;
    BMPHeaderV1.Width = Img->Width;
    BMPHeaderV1.Height = Img->Height;
    BMPHeaderV1.Planes = 1;
    BMPHeaderV1.Compression = 0;
    BMPHeaderV1.ResolutionX = RESOLUTION_X;
    BMPHeaderV1.ResolutionY = RESOLUTION_Y;


    switch (ColorDepth)
    {
        case DEPTH24: /*#####################################################*/

            /* Check for pixel matrix */
            if (Img->Pixel24 == NULL)
            {
                printf("Error: [save_BMP()] --> Could not find pixel matrix for color depth of 24");
                return -1;
            }

            /* Finding pixel matrix size and adding padding */
            SizeWidthByte = Img->Width * 3;         /* size of one line in bytes */
            TotalWidthMod4 = SizeWidthByte % 4;
            
            if(TotalWidthMod4 != 0)
            {
                SizeWidthByte = SizeWidthByte + 4 - TotalWidthMod4;
            }
            
            BMPHeaderV1.ColorDepth = DEPTH24;       /* Bits per pixel */
            BMPHeaderV1.NumColorsInTable = 0;  /* Number of colors used in color table */
            BMPHeaderV1.NumImportantColors = 0;
            BMPHeaderV1.SizePixelMatrix = SizeWidthByte * Img->Height;

            /* Finding total image file size */
            FileHeader.FileSize = 54 + BMPHeaderV1.SizePixelMatrix;
            
            /* Opening image file */            
            ImageFile = fopen(Filename, "wb");
            if(ImageFile == NULL)
            {
                printf("Error: [save_BMP()] --> Problem ocurred while creating image file.\n\n");
                return -1;
            }

            /* Writing headers */
            if (write_headers_to_file(&FileHeader, &BMPHeaderV1, ImageFile) == -1)
            {
                return -1;
            }
            
            /* Writing image */
            if (write_pixel_matrix_to_file(Img, DEPTH24, ImageFile) == -1)
            {
                return -1;
            }

            fclose(ImageFile);

            break;
        
        case DEPTH8: /*######################################################*/

            /* Check for pixel matrix */
            if (Img->Pixel8 == NULL)
            {
                printf("Error: [save_BMP()] --> Could not find pixel matrix for color depth of 8");
                return -1;
            }

            /* Finding pixel matrix size and adding padding */
            SizeWidthByte = Img->Width;         /* size of one line in bytes */
            TotalWidthMod4 = SizeWidthByte % 4;
            
            if(TotalWidthMod4 != 0)
            {
                SizeWidthByte = SizeWidthByte + 4 - TotalWidthMod4;
            }
            
            BMPHeaderV1.ColorDepth = DEPTH8;        /* Bits per pixel */
            BMPHeaderV1.NumColorsInTable = 0;  /* Number of colors used in color table */
            BMPHeaderV1.NumImportantColors = 0;
            BMPHeaderV1.SizePixelMatrix = SizeWidthByte * Img->Height;

            /* Finding total image file size */
            FileHeader.FileSize = 54 + BMPHeaderV1.SizePixelMatrix;
            
            /* Opening image file */
            ImageFile = fopen(Filename, "wb");
            if(ImageFile == NULL)
            {
                printf("Error: [save_BMP()] --> Problem ocurred while creating image file.\n\n");
                return -1;
            }

            /* Writing headers */
            if (write_headers_to_file(&FileHeader, &BMPHeaderV1, ImageFile) == -1)
            {
                return -1;
            }
            
            /* Writing image */
            if (write_pixel_matrix_to_file(Img, DEPTH8, ImageFile) == -1)
            {
                return -1;
            }
            
            fclose(ImageFile);

            break;
        
        default:
            printf("Error: [save_BMP()] --> Invalid color depth while saving.");
            return -1;
    }

    return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Read BMP image file into image struct
 * 
 * Filename   --> [Input]: pointer to array of char that will be the name of saved imag
 *
 * Return NULL if fail or pointer to image structure on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
img_t *read_BMP(const char *Filename)
{
    file_header_t   FileHeader;
    bmp_headerV1_t  BMPHeaderV1;
    bmp_headerV2_t  BMPHeaderV2;
    bmp_headerV3_t  BMPHeaderV3;
    bmp_headerV4_t  BMPHeaderV4;
    bmp_headerV5_t  BMPHeaderV5;

    int32_t ColorDepth;

    img_t           *Img;
    FILE            *ImageFile;

    /* Allocate space for image struct */
    Img = malloc(sizeof(img_t));
    if (Img == NULL)
    {
        printf("Error: [read_BMP()] --> Could not allocate memory for image struct.\n");
        return NULL;
    }
    /* Initialise image struct */
    Img->Height = 0;
    Img->Width = 0;
    Img->Pixel24 = NULL;
    Img->Pixel8 = NULL;
    
    /* Open image */
    ImageFile = fopen(Filename, "rb");
    if(ImageFile == NULL)
    {
        printf("Error: [read_BMP()] --> Could not open file for pixel matrix extraction.\n");
        return NULL;
    }
    
    /* Acquire file header and verify if valid */
    if(fread(&FileHeader, sizeof(file_header_t), 1, ImageFile) != 1)
    {
        printf("Error: [read_BMP()] --> Could not read \"File Header\".\n");
        return NULL;
    }
    
    if((FileHeader.CharID_1 != 0x42) || (FileHeader.CharID_2 != 0x4D))
    {
        printf("Error: [read_BMP()] --> Input file was not recognized as a BMP image.\n");
        return NULL;
    }
    

    /* Finding out BMP header version and reading it */
    switch(FileHeader.OffsetPixelMatrix - sizeof(file_header_t))
    {
        case BITMAP_V1_INFOHEADER :
            if(fread(&BMPHeaderV1, sizeof(bmp_headerV1_t), 1, ImageFile) != 1)
            {
                printf("Error: [read_BMP()] --> Could not read \"Header V1\".\n");
                return NULL;
            }
            Img->Width = BMPHeaderV1.Width;
            Img->Height = BMPHeaderV1.Height;
            ColorDepth = BMPHeaderV1.ColorDepth;
            break;
            
        case BITMAP_V2_INFOHEADER :
            if(fread(&BMPHeaderV2, sizeof(bmp_headerV2_t), 1, ImageFile) != 1)
            {
                printf("Error: [read_BMP()] --> Could not read \"Header V2\". \n");
                return NULL;
            }
            Img->Width = BMPHeaderV2.Width;
            Img->Height = BMPHeaderV2.Height;
            ColorDepth = BMPHeaderV2.ColorDepth;
            break;
        
        case BITMAP_V3_INFOHEADER :
            if(fread(&BMPHeaderV3, sizeof(bmp_headerV3_t), 1, ImageFile) != 1)
            {
                printf("Error: [read_BMP()] --> Could not read \"Header V3\".\n");
                return NULL;
            }
            Img->Width = BMPHeaderV3.Width;
            Img->Height = BMPHeaderV3.Height;
            ColorDepth = BMPHeaderV3.ColorDepth;
            break;
        
        case BITMAP_V4_INFOHEADER :
            if(fread(&BMPHeaderV4, sizeof(bmp_headerV4_t), 1, ImageFile) != 1)
            {
                printf("Error: [read_BMP()] --> Could not read \"Header V4\".\n");
                return NULL;
            }
            Img->Width = BMPHeaderV4.Width;
            Img->Height = BMPHeaderV4.Height;
            ColorDepth = BMPHeaderV4.ColorDepth;
            break;
            
        case BITMAP_V5_INFOHEADER :
            if(fread(&BMPHeaderV5, sizeof(bmp_headerV5_t), 1, ImageFile) != 1)
            {
                printf("Error: [read_BMP()] --> Could not read \"Header V5\".\n");
                return NULL;
            }
            Img->Width = BMPHeaderV5.Width;
            Img->Height = BMPHeaderV5.Height;
            ColorDepth = BMPHeaderV5.ColorDepth;
            break;
            
        default :
            printf("Error: [read_BMP()] --> Bitmap header is not supported. Suported bitmap headers:\n");
            printf("       - BITMAPIFOHEADER     (V1)\n");
            printf("       - BITMAPV2INFOHEADER  (V2)\n");
            printf("       - BITMAPV3INFOHEADER  (V3)\n");
            printf("       - BITMAPV4HEADER      (V4)\n");
            printf("       - BITMAPV5HEADER      (V5)\n");
            return NULL;
    }

    if (read_pixel_matrix_from_file(ImageFile, ColorDepth, Img) == -1)
    {
        printf("Error: [read_BMP()] --> \"read_pixel_matrix_from_file()\" failed.\n");
        return NULL;
    }
    
    fclose(ImageFile);
    
    return Img;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Create new blank image struct (all pixels are 0)
 * 
 * Width      --> [Input]: image width in pixels
 * Height     --> [Input]: image height in pixels
 * ColorDepth --> [Input]: integer from enummeration tha especifies the color depth:
 *                         DEPTH24 ==> Set 24 bits per pixel
 *                         SEPTH8  ==> Set 8 bits per pixel
 *
 * Return NULL if fail or pointer to image structure on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
img_t *new_BMP(int32_t Width, int32_t Height, int32_t ColorDepth)
{
    img_t   *BlankImg = NULL;

    switch(ColorDepth)
    {
        case DEPTH24:

            BlankImg = (img_t *)malloc(sizeof(img_t));

            BlankImg->Width = Width;
            BlankImg->Height = Height;

            BlankImg->Pixel8 = NULL;

            BlankImg->Pixel24 = (pixel24_t **)malloc(Height * sizeof(pixel24_t *));
            if (BlankImg->Pixel24 == NULL)
            {
                printf("Error: [new_BMP()] --> Could not allocate memory. (row; 24bpp)\n");
                return NULL;
            }

            for(int32_t Row = 0; Row < Height; Row++)
            {
                BlankImg->Pixel24[Row] = (pixel24_t *)malloc(Width * sizeof(pixel24_t));
                if(BlankImg->Pixel24[Row] == NULL)
                {
                    printf("Error: [new_BMP()] --> Could not allocate memmory. (column; 24bpp)\n");
                    return NULL;
                }

                for(int32_t Column = 0; Column < Width; Column++)
                {
                    BlankImg->Pixel24[Row][Column].Red   = 0;
                    BlankImg->Pixel24[Row][Column].Green = 0;
                    BlankImg->Pixel24[Row][Column].Blue  = 0;
                }
            }
            break;

        case DEPTH8:

            BlankImg = (img_t *)malloc(sizeof(img_t));

            BlankImg->Width = Width;
            BlankImg->Height = Height;

            BlankImg->Pixel24 = NULL;

            BlankImg->Pixel8 = (uint8_t **)malloc(Height * sizeof(uint8_t *));
            if (BlankImg->Pixel8 == NULL)
            {
                printf("Error: [new_BMP()] --> Could not allocate memmory. (row; 8bpp)\n");
                return NULL;
            }

            for(int32_t Row = 0; Row < Height; Row++)
            {
                BlankImg->Pixel8[Row] = (uint8_t *)malloc(Width * sizeof(uint8_t));
                if (BlankImg->Pixel8[Row] == NULL)
                {
                    printf("Error: [new_BMP()] --> Could not allocate mmemory. (column; 8bpp)\n");
                    return NULL;
                }

                for(int32_t Column = 0; Column < Width; Column++)
                {
                    BlankImg->Pixel8[Row][Column] = 0;
                }
            }
            break;

        default:
            printf("Error: [new_BMP()] --> Invalid color depth.\n");
            return NULL;
    }
    
    return BlankImg;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Create new blank image struct (all pixels are 0) with the same spacial resolution
 * as the original image
 * 
 * OriginalImmmage --> [Input]: pointer to image struct to use as reference to create
 *                              new blank image struct with samme spacial resolution
 * ColorDepth      --> [Input]: integer from enummeration tha especifies the color depth:
 *                              DEPTH24 ==> Set 24 bits per pixel
 *                              SEPTH8  ==> Set 8 bits per pixel
 *
 * Return NULL if fail or pointer to image structure on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
img_t *new_BMP_as_size(img_t *OriginalImage, int32_t ColorDepth)
{
    img_t *BlankImage;
    
    BlankImage = new_BMP(OriginalImage->Width, OriginalImage->Height, ColorDepth);

    return BlankImage;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Create a copy of the original image struct 
 * 
 * OriginalImmmage --> [Input]: pointer to image struct to be copied
 *
 * Return NULL if fail or pointer to image structure on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
img_t *copy_BMP(img_t *OriginalImage)
{
    img_t   *CopyImg;
    uint8_t ColorDepth = 0;

    /* Validate input */
    if (OriginalImage == NULL)
    {
        printf("Error: [copy_BMP()] --> No input.\n");
        return NULL;
    }

    /* Validate and find color depth */
    if ((OriginalImage->Pixel24 != NULL) && (OriginalImage->Pixel8 == NULL))
    {
        ColorDepth = DEPTH24;
    }
    else if ((OriginalImage->Pixel8 != NULL) && (OriginalImage->Pixel24 == NULL))
    {
        ColorDepth = DEPTH8;
    }
    else
    {
        printf("Error: [copy_BMP()] --> Invalid pointer cobination on image struct.\n");
        return NULL;
    }
    
    /* Allocate image struct */
    CopyImg = (img_t *) malloc(sizeof(img_t));
    if (CopyImg == NULL)
    {
        printf("Error: [copy_BMP()] --> Could not allocate memory for image struct.\n");
        return NULL;
    }    

    CopyImg->Height = OriginalImage->Height;
    CopyImg->Width = OriginalImage->Width;
    CopyImg->Pixel24 = NULL;
    CopyImg->Pixel8 = NULL;

    switch (ColorDepth)
    {
        case DEPTH24:

            CopyImg->Pixel24 = (pixel24_t **) malloc(OriginalImage->Height * sizeof(pixel24_t *));
            if (CopyImg->Pixel24 == NULL)
            {
                printf("Error: [copy_BMP()] --> Could not allocate memory. (row; 24bpp)\n");
                return NULL;
            }

            for (int32_t Row = 0; Row < OriginalImage->Height; Row++)
            {
                CopyImg->Pixel24[Row] = (pixel24_t *)malloc(OriginalImage->Width * sizeof(pixel24_t));
                if (CopyImg->Pixel24[Row] == NULL)
                {
                    printf("Error: [copy_BMP()] --> Could not allocate memory. (column; 24bpp)\n");
                    return NULL;
                }

                for (int32_t Column = 0; Column < OriginalImage->Width; Column++)
                {
                    CopyImg->Pixel24[Row][Column].Red   = OriginalImage->Pixel24[Row][Column].Red;
                    CopyImg->Pixel24[Row][Column].Green = OriginalImage->Pixel24[Row][Column].Green;
                    CopyImg->Pixel24[Row][Column].Blue  = OriginalImage->Pixel24[Row][Column].Blue;
                }
            }

            break;
        
        case DEPTH8:

            CopyImg->Pixel8 = (uint8_t **) malloc(OriginalImage->Height * sizeof(uint8_t *));
            if (CopyImg->Pixel8 == NULL)
            {
                printf("Error: [copy_BMP()] --> Could not allocate memory. (row; 8bpp)\n");
                return NULL;
            }

            for (int32_t Row = 0; Row < OriginalImage->Height; Row++)
            {
                CopyImg->Pixel8[Row] = (uint8_t *)malloc(OriginalImage->Width * sizeof(uint8_t));
                if (CopyImg->Pixel8[Row] == NULL)
                {
                    printf("Error: [copy_BMP()] --> Could not allocate memory. (column; 8bpp)\n");
                    return NULL;
                }

                for (int32_t Column = 0; Column < OriginalImage->Width; Column++)
                {
                    CopyImg->Pixel8[Row][Column] = OriginalImage->Pixel8[Row][Column];
                }
            }

            break;
        
        default:
            printf("Error: [copy_BMP()] --> Invalid color depth.\n");
            return NULL;
    }

    return CopyImg;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Deallocate the image struct 
 * 
 * Img --> [Input]: pointer to image struct to be deallocated
 *
 * Return -1 if fail or 0 on success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int free_img(img_t *Img)
{
    if(Img == NULL)
    {
        printf("Error: [free_img()] --> No input.\n");
        return -1;
    }

    if(Img->Pixel24 != NULL)
    {
        for (int32_t Row = 0; Row < Img->Height; Row++)
        {
            free(Img->Pixel24[Row]);
        }
        free(Img->Pixel24);     
    }

    if(Img->Pixel8 != NULL)
    {
        for (int32_t Row = 0; Row < Img->Height; Row++)
        {
            free(Img->Pixel8[Row]);
        }
        free(Img->Pixel8);
    }

    free(Img);

    return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Display header information of .bpm iage file
 * 
 * Filename --> [Input]: pointer to string containing the namme of the file
 *                       to display info about.
 *
 * Does not return anything
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void display_header(const char *Filename)
{       
    file_header_t FileHeader;
    bmp_headerV1_t BMPHeaderV1;
    bmp_headerV2_t BMPHeaderV2;
    bmp_headerV3_t BMPHeaderV3;
    bmp_headerV4_t BMPHeaderV4;
    bmp_headerV5_t BMPHeaderV5;
    
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
    
    //Print Windows BMP header information
    switch(FileHeader.OffsetPixelMatrix - sizeof(file_header_t))
    {
        //----------------------------------------------------------------------
        case BITMAP_V1_INFOHEADER :
            
            fread(&BMPHeaderV1, sizeof(bmp_headerV1_t), 1, File);
            printf("BMP header type: BITMAPINFOHEADER (V1)\n");
            printf("BMP header size ............... %u bytes\n", BMPHeaderV1.SizeHeader);
            printf("Image width ................... %d pixels\n", BMPHeaderV1.Width);
            printf("Image Height .................. %d pixels\n", BMPHeaderV1.Height);
            printf("Number of color plnaes ........ %u\n", BMPHeaderV1.Planes);
            printf("Bits per pixel ................ %u\n", BMPHeaderV1.ColorDepth);
            printf("Image size .................... %u\n", BMPHeaderV1.SizePixelMatrix);
            printf("X axis printing resolution .... %d\n", BMPHeaderV1.ResolutionX);
            printf("Y axis printing resolution .... %d\n", BMPHeaderV1.ResolutionY);
            printf("Number of palette colors ...... %u\n", BMPHeaderV1.NumColorsInTable);
            printf("Number of important colors .... %u\n", BMPHeaderV1.NumImportantColors);
            
            //Print compression method
            printf("Compression method ............ ");
            switch(BMPHeaderV1.Compression)
            {
                case 0 :
                    printf("none (ID code: BI_RGB)\n\n");
                    break;
                    
                case 1 :
                    printf("RLE 8-bit/pixel (ID code: BI_RLE8)\n\n");
                    break;
                    
                case 2 :
                    printf("RLE 4-bit/pixel (ID code: BI_RLE4)\n\n");
                    break;
                    
                case 3 :
                    printf("OS22XBITMAPHEADER: Huffman 1D (ID code: BI_BITFIELDS)\n\n");
                    break;
                    
                case 4 :
                    printf("OS22XBITMAPHEADER: RLE-24 (ID code: BI_JPEG)\n\n");
                    break;
                    
                case 5 :
                    printf("Undefined (ID code: BI_PNG)\n\n");
                    break;
                    
                case 6 :
                    printf("RGBA bit field masks (ID code: BI_ALPHABITFIELDS)\n\n");
                    break;
                    
                case 11 :
                    printf("none (ID code: BI_CMYK)\n\n");
                    break;
                    
                case 12 :
                    printf("RLE-8 (ID code: BI_CMYKRLE8)\n\n");
                    break;
                    
                case 13 :
                    printf("RLE-4 (ID code: BI_CMYKRLE4)\n\n");
                    break;
                    
                default :
                    printf("Error! invalid value\n\n");
            }
            
            break;

        //----------------------------------------------------------------------
        case BITMAP_V2_INFOHEADER :
            
            fread(&BMPHeaderV2, sizeof(bmp_headerV2_t), 1, File);
            printf("BMP header type: BITMAPV2INFOHEADER (V2)\n");
            printf("BMP header size ............... %u bytes\n", BMPHeaderV2.SizeHeader);
            printf("Image width ................... %d pixels\n", BMPHeaderV2.Width);
            printf("Image Height .................. %d pixels\n", BMPHeaderV2.Height);
            printf("Number of color plnaes ........ %u\n", BMPHeaderV2.Planes);
            printf("Bits per pixel ................ %u\n", BMPHeaderV2.ColorDepth);
            printf("Image size .................... %u\n", BMPHeaderV2.SizePixelMatrix);
            printf("X axis printing resolution .... %d\n", BMPHeaderV2.ResolutionX);
            printf("Y axis printing resolution .... %d\n", BMPHeaderV2.ResolutionY);
            printf("Number of palette colors ...... %u\n", BMPHeaderV2.NumColorsInTable);
            printf("Number of important colors .... %u\n", BMPHeaderV2.NumImportantColors);
            
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
                    printf("RLE-8 (ID code: BI_CMYKRLE8)\n");
                    break;
                    
                case 13 :
                    printf("RLE-4 (ID code: BI_CMYKRLE4)\n");
                    break;
                    
                default :
                    printf("Error! invalid value\n");
            }
            
            printf("Red mask ...................... %u\n", BMPHeaderV2.RedMask);
            printf("Green mask .................... %u\n", BMPHeaderV2.GreenMask);
            printf("Blue mask ..................... %u\n\n", BMPHeaderV2.BlueMask);
            
            break;
    
        //----------------------------------------------------------------------
        case BITMAP_V3_INFOHEADER :
            
            fread(&BMPHeaderV3, sizeof(bmp_headerV3_t), 1, File);
            printf("BMP header type: BITMAPV3INFOHEADER (V3)\n");
            printf("BMP header size ............... %u bytes\n", BMPHeaderV3.SizeHeader);
            printf("Image width ................... %d pixels\n", BMPHeaderV3.Width);
            printf("Image Height .................. %d pixels\n", BMPHeaderV3.Height);
            printf("Number of color plnaes ........ %u\n", BMPHeaderV3.Planes);
            printf("Bits per pixel ................ %u\n", BMPHeaderV3.ColorDepth);
            printf("Image size .................... %u\n", BMPHeaderV3.SizePixelMatrix);
            printf("X axis printing resolution .... %d\n", BMPHeaderV3.ResolutionX);
            printf("Y axis printing resolution .... %d\n", BMPHeaderV3.ResolutionY);
            printf("Number of palette colors ...... %u\n", BMPHeaderV3.NumColorsInTable);
            printf("Number of important colors .... %u\n", BMPHeaderV3.NumImportantColors);
            
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
                    printf("RLE-8 (ID code: BI_CMYKRLE8)\n");
                    break;
                    
                case 13 :
                    printf("RLE-4 (ID code: BI_CMYKRLE4)\n");
                    break;
                    
                default :
                    printf("Error! invalid value\n");
            }
            
            printf("Red mask ...................... %u\n", BMPHeaderV3.RedMask);
            printf("Green mask .................... %u\n", BMPHeaderV3.GreenMask);
            printf("Blue mask ..................... %u\n", BMPHeaderV3.BlueMask);
            
            printf("Alpha mask .................... %u\n\n", BMPHeaderV3.AlphaMask);

            break;
    
        //----------------------------------------------------------------------
        case BITMAP_V4_INFOHEADER :

            fread(&BMPHeaderV4, sizeof(bmp_headerV4_t), 1, File);
            printf("BMP header type: BITMAPV4HEADER (V4)\n");
            printf("BMP header size ............... %u bytes\n", BMPHeaderV4.SizeHeader);
            printf("Image width ................... %d pixels\n", BMPHeaderV4.Width);
            printf("Image Height .................. %d pixels\n", BMPHeaderV4.Height);
            printf("Number of color plnaes ........ %u\n", BMPHeaderV4.Planes);
            printf("Bits per pixel ................ %u\n", BMPHeaderV4.ColorDepth);
            printf("Image size .................... %u\n", BMPHeaderV4.SizePixelMatrix);
            printf("X axis printing resolution .... %d\n", BMPHeaderV4.ResolutionX);
            printf("Y axis printing resolution .... %d\n", BMPHeaderV4.ResolutionY);
            printf("Number of palette colors ...... %u\n", BMPHeaderV4.NumColorsInTable);
            printf("Number of important colors .... %u\n", BMPHeaderV4.NumImportantColors);
            
            //Print compression method
            printf("Compression method ............ ");
            switch(BMPHeaderV4.Compression)
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
                    printf("RLE-8 (ID code: BI_CMYKRLE8)\n");
                    break;
                    
                case 13 :
                    printf("RLE-4 (ID code: BI_CMYKRLE4)\n");
                    break;
                    
                default :
                    printf("Error! invalid value\n");
            }
            
            printf("Red mask ...................... %u\n", BMPHeaderV4.RedMask);
            printf("Green mask .................... %u\n", BMPHeaderV4.GreenMask);
            printf("Blue mask ..................... %u\n", BMPHeaderV4.BlueMask);
            
            printf("Alpha mask .................... %u\n", BMPHeaderV4.AlphaMask);

            printf("Color space ................... ");
            switch(BMPHeaderV4.CSType)
            {
                case 0x0:
                    printf("LCS_CALIBRATED_RGB\n");
                    break;
                
                case 0x73524742 : //"sRGB"
                    printf("LCS_sRGB\n");
                    break;
                
                case 0x57696E20 :
                    printf("LCS_WINDOWS_COLOR_SPACE\n");
                    break;
                
                default :
                    printf("PROFILE_LINKED or PROFILE_EMBEDDED\n");
            }
            printf("X red endpoint ................ %d\n", BMPHeaderV4.RedX);
            printf("Y red endpoint ................ %d\n", BMPHeaderV4.RedY);
            printf("Z red endpoint ................ %d\n", BMPHeaderV4.RedZ);
            printf("X green endpoint .............. %d\n", BMPHeaderV4.GreenX);
            printf("Y green endpoint .............. %d\n", BMPHeaderV4.GreenY);
            printf("Z green endpoint .............. %d\n", BMPHeaderV4.GreenZ);
            printf("X Blue endpoint ............... %d\n", BMPHeaderV4.BlueX);
            printf("Y Blue endpoint ............... %d\n", BMPHeaderV4.BlueY);
            printf("Z Blue endpoint ............... %d\n", BMPHeaderV4.BlueZ);
            printf("Toned response for red ........ %X\n", BMPHeaderV4.GammaRed);
            printf("Toned response for green ...... %X\n", BMPHeaderV4.GammaGreen);
            printf("Toned response for blue ....... %X\n\n", BMPHeaderV4.GammaBlue);

            break;

        //----------------------------------------------------------------------
        case BITMAP_V5_INFOHEADER :

            fread(&BMPHeaderV5, sizeof(bmp_headerV4_t), 1, File);
            printf("BMP header type: BITMAPV5HEADER (V5)\n");
            printf("BMP header size ............... %u bytes\n", BMPHeaderV5.SizeHeader);
            printf("Image width ................... %d pixels\n", BMPHeaderV5.Width);
            printf("Image Height .................. %d pixels\n", BMPHeaderV5.Height);
            printf("Number of color plnaes ........ %u\n", BMPHeaderV5.Planes);
            printf("Bits per pixel ................ %u\n", BMPHeaderV5.ColorDepth);
            printf("Image size .................... %u\n", BMPHeaderV5.SizePixelMatrix);
            printf("X axis printing resolution .... %d\n", BMPHeaderV5.ResolutionX);
            printf("Y axis printing resolution .... %d\n", BMPHeaderV5.ResolutionY);
            printf("Number of palette colors ...... %u\n", BMPHeaderV5.NumColorsInTable);
            printf("Number of important colors .... %u\n", BMPHeaderV5.NumImportantColors);
            
            //Print compression method
            printf("Compression method ............ ");
            switch(BMPHeaderV5.Compression)
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
                    printf("RLE-8 (ID code: BI_CMYKRLE8)\n");
                    break;
                    
                case 13 :
                    printf("RLE-4 (ID code: BI_CMYKRLE4)\n");
                    break;
                    
                default :
                    printf("Error! invalid value\n");
            }
            
            printf("Red mask ...................... %u\n", BMPHeaderV5.RedMask);
            printf("Green mask .................... %u\n", BMPHeaderV5.GreenMask);
            printf("Blue mask ..................... %u\n", BMPHeaderV5.BlueMask);
            
            printf("Alpha mask .................... %u\n", BMPHeaderV5.AlphaMask);

            printf("Color space ................... ");
            switch(BMPHeaderV5.CSType)
            {
                case 0x0:
                    printf("LCS_CALIBRATED_RGB\n");
                    break;
                
                case 0x73524742 : //"sRGB"
                    printf("LCS_sRGB\n");
                    break;
                
                case 0x57696E20 :
                    printf("LCS_WINDOWS_COLOR_SPACE\n");
                    break;
                
                default :
                    printf("PROFILE_LINKED or PROFILE_EMBEDDED\n");
            }
            printf("X red endpoint ................ %d\n", BMPHeaderV5.RedX);
            printf("Y red endpoint ................ %d\n", BMPHeaderV5.RedY);
            printf("Z red endpoint ................ %d\n", BMPHeaderV5.RedZ);
            printf("X green endpoint .............. %d\n", BMPHeaderV5.GreenX);
            printf("Y green endpoint .............. %d\n", BMPHeaderV5.GreenY);
            printf("Z green endpoint .............. %d\n", BMPHeaderV5.GreenZ);
            printf("X Blue endpoint ............... %d\n", BMPHeaderV5.BlueX);
            printf("Y Blue endpoint ............... %d\n", BMPHeaderV5.BlueY);
            printf("Z Blue endpoint ............... %d\n", BMPHeaderV5.BlueZ);
            printf("Toned response for red ........ %X\n", BMPHeaderV5.GammaRed);
            printf("Toned response for green ...... %X\n", BMPHeaderV5.GammaGreen);
            printf("Toned response for blue ....... %X\n", BMPHeaderV5.GammaBlue);

            printf("Rendering intent .............. ");
            switch(BMPHeaderV5.Intent)
            {
                case 0x8 :
                    printf("LCS_GM_ABS_COLORIMETRIC\n");
                    break;
                
                case 0x1 :
                    printf("LCS_GM_BUSINESS\n");
                    break;
                    
                case 0x2 :
                    printf("LCS_GM_GRAPHICS\n");
                    break;
                    
                case 0x4 :
                    printf("LCS_GM_IMAGES\n");
                    break;
                    
                default :
                    printf("Error: invalid value\n");
            }           
            printf("Profile data .................. %u\n", BMPHeaderV5.ProfileData);
            printf("Profile size .................. %u\n", BMPHeaderV5.ProfileSize);
            printf("Reserved ...................... %u\n\n", BMPHeaderV5.Reserved);

            break;

        //----------------------------------------------------------------------
        default :
        
        printf("Error: wrong size of BMP header");
        exit(EXIT_FAILURE);

    }

    fclose(File);

}
