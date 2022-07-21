/*
 * debouncer.c
 *
 *  Created on: Jul 20, 2022
 *      Author: juanjosemarinelli
 */


#include "debouncer.h"

uint32_t DEB_lastUpdateTime[100];
uint32_t DEB_time[100] = { DEBOUNCE_TIME };


void DEB_SetTime(uint8_t array_index, uint32_t time)
{
	DEB_time[array_index] = time;
}

void DEB_DebounceArray(uint8_t * data_debounced, uint8_t * data_new, uint8_t array_length)
{
	uint32_t current_tick = HAL_GetTick();

	for(uint8_t i = 0; i < array_length; i++)
	{
		if(data_debounced[i] != data_new[i] && (current_tick - DEB_lastUpdateTime[i]) > DEB_time[i])
		{
			data_debounced[i] = data_new[i];
			DEB_lastUpdateTime[i] = current_tick;
		}
	}
}
