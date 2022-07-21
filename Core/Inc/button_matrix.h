/*
 * button_matrix.h
 *
 *  Created on: Jul 20, 2022
 *      Author: Juanjo
 */

#ifndef INC_BUTTON_MATRIX_H_
#define INC_BUTTON_MATRIX_H_

#include "main.h"

#define BMX_ROWS		6
#define BMX_COLS		2

#define BMX_DEBOUNCE_TIME	10

void BMX_Init(uint8_t* state_array_pointer);
void BMX_Check(void);

#endif /* INC_BUTTON_MATRIX_H_ */
