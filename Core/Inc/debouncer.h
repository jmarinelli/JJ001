/*
 * debouncer.h
 *
 *  Created on: Jul 20, 2022
 *      Author: juanjosemarinelli
 */

#ifndef INC_DEBOUNCER_H_
#define INC_DEBOUNCER_H_

#include "stm32f1xx_hal.h"

#define DEBOUNCE_TIME	10

void DEB_SetTime(uint8_t array_index, uint8_t time);
void DEB_DebounceArray(uint8_t * data_debounced, uint8_t * data_new, uint8_t array_length);

#endif /* INC_DEBOUNCER_H_ */
