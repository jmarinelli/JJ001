/*
 * encoder.h
 *
 *  Created on: Jul 20, 2022
 *      Author: juanjosemarinelli
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "stm32f1xx_hal.h"

#define APP_ROT_STEP_TIMEOUT_MS			1000
#define APP_ROT_REP_MS					50
#define APP_ROT_DEL_MS					50

typedef enum
{
	ROT_IDLE    = 0x00U,
  	ROT_CW    	= 0x01U,
	ROT_CCW     = 0x02U,
	ROT_WAIT_TO_RESET = 0x03U
} ENC_RotaryState;

typedef enum
{
	ROT_REP_IDLE    		= 0x00U,
  	ROT_REP_PUSH_CW    		= 0x01U,
	ROT_REP_PUSH_CCW     	= 0x02U,
	ROT_REP_DELAY			= 0x04U
} ENC_RotaryReportState;

typedef struct
{
	ENC_RotaryState hwState;
	ENC_RotaryState toReport;
	ENC_RotaryReportState reportState;
	uint8_t msCounter;
	uint8_t* pinA;
	uint8_t* pinB;
	uint8_t lastStateA;
	uint8_t lastStateB;

	ENC_RotaryState lastStep;
	uint8_t stepDivider;
	uint8_t stepCounter;
	uint16_t stepTimeout;
} ENC_RotHandler;


void ENC_Init(ENC_RotHandler *rot);
void ENC_Process(ENC_RotHandler *rot);
ENC_RotaryReportState ENC_GetState(ENC_RotHandler *rot);

#endif /* INC_ENCODER_H_ */
