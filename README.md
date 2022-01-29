# BitmapLib
 
A simple library to read, write and seek info of .bmp files

## Functions available

```C
/* Create BMP image file (header used: BITMAPINFOHEADER (V1))
   Return -1 if fail and 0 on success */
int save_BMP(img_t *Img, const char *Filename);


/* Read BMP image to a pixel matrix.
   Return NULL if fail */
img_t *read_BMP(const char *Filename);


/* Create new empty image with given size.
   Return NULL if fail.
   Type --> RGB_24BITS
            GREY_8BITS */
img_t *new_BMP(int32_t Width, int32_t Height, int Type);


/* Create new empty image with same size as given image.
   Return NULL if fail.
   Type --> RGB_24BITS
            GREY_8BITS */
img_t *new_BMP_as_size(img_t *OriginalImage, int Type);


/* Create a copy of given image.
   Return NULL if fail */
img_t *copy_BMP(img_t *OriginalImage);


/* Frees space occupied by PixelMatrix.
   Does not return anything */
void free_img(img_t *Img);


/* Display header information.
   Does not return anything */
void display_header(const char *Filename);
```
