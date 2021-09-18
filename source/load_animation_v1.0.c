/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Library to render progress bar on terminal
	
	Author: Vitor Henrique Andrade Helfensteller Straggiotti Silva
	Date: 20/06/2021  (DD/MM/YYYY)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
#include <stdio.h>
#include "../include/load_animation_v1.0.h"


/******************************************************************************/
void print_progress(long CurrState, long Min, long Max, unsigned int BarSize)
{
	unsigned int BarPosition;
	float Percentage;
	
	//Calculating progress
	BarPosition = (unsigned int)(((long)BarSize * (CurrState - Min)) / (Max - Min));
	BarPosition++;
	Percentage = 100.0 * ((float)(CurrState - Min) / (float)(Max - Min));
	
	//Drawing progress bar
	fputs("\r[", stdout);
	for(unsigned int i = 0; i < BarSize; i++)
	{
		if(i < BarPosition)
		{
			fputc('=', stdout);
		}else
		{
			fputc('.', stdout);
		}
	}
	
	printf("] %.3f %%", Percentage);
	fflush(stdout);

}

