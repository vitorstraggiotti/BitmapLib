/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Header file of the library for pixel drawing							*
 *																		*
 * Author: Vitor Henrique Andrade Helfensteller Straggiotti Silva		*
 * Created on: 06/06/2021 (DD/MM/YYYY)									*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

#ifndef __PIXEL_DRAW_H__
#define __PIXEL_DRAW_H__


/*******************************************************************************
 *                            MACROS AND TYPEDEF                               *
 *******************************************************************************/
 
 
/*******************************************************************************
 *                                  FUNCTIONS                                  *
 *******************************************************************************/

//========================================= GEOMETRY DRAWING ===================

//------------------------------------------------------------------------------
//Draw a circle on the pixel matrix
void circle(dimensions_t Dimension,
			pixel_t **PixelMatrix,
			int Pos_x,
			int Pos_y,
			int Radius,
			pixel_t Color);
//------------------------------------------------------------------------------
//Draw a circumference on the pixel matrix
void circumference(dimensions_t Dimension,
					pixel_t **PixelMatrix,
					int Pos_x,
					int Pos_y,
					int Radius,
					pixel_t Color);


/*******************************************************************************
 *                                   STRUCTURES                                *
 *******************************************************************************/

#endif
