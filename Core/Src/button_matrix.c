/*
 * button_matrix.c
 *
 *  Created on: Jul 20, 2022
 *      Author: Juanjo
 */

#include "button_matrix.h"

uint8_t* BMX_state_p;

uint32_t BMX_rowPins[BMX_ROWS] = {
	R1_Pin,
	R2_Pin,
	R3_Pin,
	R4_Pin,
	R5_Pin,
	R6_Pin
};

GPIO_TypeDef * BMX_rowGPIOs[BMX_ROWS] = {
	R1_GPIO_Port,
	R2_GPIO_Port,
	R3_GPIO_Port,
	R4_GPIO_Port,
	R5_GPIO_Port,
	R6_GPIO_Port
};

uint32_t BMX_colPins[BMX_COLS] = {
	C1_Pin,
	C2_Pin
};

GPIO_TypeDef * BMX_colGPIOs[BMX_COLS] = {
	C1_GPIO_Port,
	C2_GPIO_Port
};

void BMX_Init(uint8_t* state_array_pointer)
{
	BMX_state_p = state_array_pointer;
}

void BMX_Check()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    volatile uint8_t pinState;


    for(uint8_t col_i = 0; col_i < BMX_COLS; col_i++)
    {
		// Set current COL to output LOW
		HAL_GPIO_WritePin(BMX_colGPIOs[col_i], BMX_colPins[col_i], 0);
		GPIO_InitStruct.Pin = BMX_colPins[col_i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(BMX_colGPIOs[col_i], &GPIO_InitStruct);

		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");

		for(uint8_t row_i=0; row_i<BMX_ROWS; row_i++)
		{
			pinState = !HAL_GPIO_ReadPin(BMX_rowGPIOs[row_i], BMX_rowPins[row_i]);
			BMX_state_p[col_i * BMX_ROWS + row_i] = pinState;
		}

		// Set current row to HiZ
		HAL_GPIO_WritePin(BMX_colGPIOs[col_i], BMX_colPins[col_i], 1);
		GPIO_InitStruct.Pin = BMX_colPins[col_i];
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(BMX_colGPIOs[col_i], &GPIO_InitStruct);

		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");

    }
}
