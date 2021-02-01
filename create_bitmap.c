/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Library for creating and manipulating bitmap images             *
 *                                                                 *
 * Author: Vitor Henrique Andrade Helfensteller Straggiotti Silva  *
 * Created on: 01/02/2021 (DD/MM/YYYY)                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include<stdlib.h>
#include<stdio.h>
#include"create_bitmap.h"

/****************************************************************************************************/
/* Returns the header of the bitmap image file */
header_t header_bmp(u_int16_t precision, u_int32_t width, u_int32_t height, u_int32_t numTableColors)
{
    header_t header;
    u_int32_t tableBytes;  /* Number of bytes of the color table */

    
}
