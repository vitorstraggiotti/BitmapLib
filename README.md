# BitmapLib
 
A simple library to read, write and seek info of .bmp files

## Functions available

```C
//------------------------------------------------------------------------------
/* Create BMP image file (header used: BITMAPINFOHEADER (V1)) [OK]
   Return -1 if fail and 0 on success */
int save_BMP(img_t *Img, const char *Filename);
//------------------------------------------------------------------------------
/* Read BMP image to a pixel matrix [OK]
   Return NULL if fail */
img_t *read_BMP(const char *Filename);
//------------------------------------------------------------------------------
//Create new empty image with given size
img_t *new_BMP(int32_t Width, int32_t Height);
//------------------------------------------------------------------------------
//Display header information
void display_header(const char *Filename);
//------------------------------------------------------------------------------
//Frees space occupied by PixelMatrix
void free_img(img_t *Img);
```
