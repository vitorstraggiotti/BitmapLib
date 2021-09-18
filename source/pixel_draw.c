/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * Source code for pixel drawing												*
 *																				*
 * Author: Vitor Henrique Andrade Helfensteller Satraggiotti Silva				*
 * Start date: 06/06/2021 (DD/MM/YYYY)											*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/bitmap.h"
#include "../include/pixel_draw.h"

/******************************************************************************/
//Draw a circle on the pixel matrix [OK]
void circle(dimensions_t Dimension, pixel_t **PixelMatrix, int CenterX, int CenterY, int Radius, pixel_t Color)
{
	int CircumferenceX, CircumferenceY, StepX, StepY;
	
	//Verifying coordinates boundaries
	if((CenterX < 0) || (CenterX >= Dimension.Width))
	{
		printf("Error: circle center X coordinate are out of the screen. Drawing terminated\n\n");
		exit(EXIT_FAILURE);
	}else if((CenterY < 0) || (CenterY >= Dimension.Height))
	{
		printf("Error: circle center Y coordinate are out of the screen. Drawing terminated\n\n");
		exit(EXIT_FAILURE);
	}
	
	
	//Drawing circumference
	for(StepX = 0; StepX <= Radius; StepX++)
	{
		StepY = (int)((float)Radius * sqrt(1.0 - pow((float)StepX/(float)Radius, 2)));
		
		//1° quadrant
		CircumferenceX = CenterX + StepX;
		CircumferenceY = CenterY + StepY;
		for(int tmp = CircumferenceY; tmp >= CenterY; tmp--)
		{
			if((CircumferenceX < Dimension.Width) && (tmp < Dimension.Height))
			{
				PixelMatrix[tmp][CircumferenceX].Red = Color.Red;
				PixelMatrix[tmp][CircumferenceX].Green = Color.Green;
				PixelMatrix[tmp][CircumferenceX].Blue = Color.Blue;
			}
		}
		
		//2° quadrant
		CircumferenceX = CenterX - StepX;
		CircumferenceY = CenterY + StepY;
		for(int tmp = CircumferenceY; tmp >= CenterY; tmp--)
		{
			if((CircumferenceX >= 0) && (tmp < Dimension.Height))
			{
				PixelMatrix[tmp][CircumferenceX].Red = Color.Red;
				PixelMatrix[tmp][CircumferenceX].Green = Color.Green;
				PixelMatrix[tmp][CircumferenceX].Blue = Color.Blue;
			}
		}
		
		//3° quadrant
		CircumferenceX = CenterX - StepX;
		CircumferenceY = CenterY - StepY;
		for(int tmp = CircumferenceY; tmp <= CenterY; tmp++)
		{
			if((CircumferenceX >= 0) && (tmp >= 0))
			{
				PixelMatrix[tmp][CircumferenceX].Red = Color.Red;
				PixelMatrix[tmp][CircumferenceX].Green = Color.Green;
				PixelMatrix[tmp][CircumferenceX].Blue = Color.Blue;
			}
		}

		//4° quadrant
		CircumferenceX = CenterX + StepX;
		CircumferenceY = CenterY - StepY;
		for(int tmp = CircumferenceY; tmp <= CenterY; tmp++)
		{
			if((CircumferenceX < Dimension.Width) && (tmp >= 0))
			{
				PixelMatrix[tmp][CircumferenceX].Red = Color.Red;
				PixelMatrix[tmp][CircumferenceX].Green = Color.Green;
				PixelMatrix[tmp][CircumferenceX].Blue = Color.Blue;
			}
		}
		
	}

}

/******************************************************************************/
//Draw a circumference on the pixel matrix [OK]
void circumference(dimensions_t Dimension, pixel_t **PixelMatrix, int CenterX, int CenterY, int Radius, pixel_t Color)
{
	int RowMin, RowMax, ColumnMin, ColumnMax;
	int CircumferenceX, CircumferenceY, StepX, StepY;
	int PreviousStepY;
	
	//Verifying coordinates boundaries
	if((CenterX < 0) || (CenterX >= Dimension.Width))
	{
		printf("Error: circumference center X coordinate are out of the screen. Drawing terminated\n\n");
		exit(EXIT_FAILURE);
	}else if((CenterY < 0) || (CenterY >= Dimension.Height))
	{
		printf("Error: circumference center Y coordinate are out of the screen. Drawing terminated\n\n");
		exit(EXIT_FAILURE);
	}
	
	
	//Drawing circumference
	for(StepX = 0; StepX <= Radius; StepX++)
	{
		StepY = (int)((float)Radius * sqrt(1.0 - pow((float)StepX/(float)Radius, 2)));
		
		//1° quadrant
		CircumferenceX = CenterX + StepX;
		CircumferenceY = CenterY + StepY;
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
		
		//2° quadrant
		CircumferenceX = CenterX - StepX;
		CircumferenceY = CenterY + StepY;
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
		
		//3° quadrant
		CircumferenceX = CenterX - StepX;
		CircumferenceY = CenterY - StepY;
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
		
		//4° quadrant
		CircumferenceX = CenterX + StepX;
		CircumferenceY = CenterY - StepY;
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

