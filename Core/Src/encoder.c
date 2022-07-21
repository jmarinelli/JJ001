/*
 * encoder.c
 *
 *  Created on: Jul 20, 2022
 *      Author: juanjosemarinelli
 */


#include "encoder.h"

void ENC_Init(ENC_RotHandler *rot)
{
	// Reset
	rot->reportState = ROT_REP_IDLE;
	rot->hwState = ROT_IDLE;
	rot->msCounter = 0;
}

void ENC_Process(ENC_RotHandler *rot){
	uint8_t encA, encB;

	// Get encoder current state
	encA = *(rot->pinA);
	encB = *(rot->pinB);

	if(encA && !encB && rot->hwState == ROT_IDLE){
		rot->hwState  = ROT_CW;
	}else if(!encA && encB && rot->hwState  == ROT_IDLE){
		rot->hwState  = ROT_CCW;
	}else if(!encA && !encB && rot->hwState  == ROT_CW && rot->hwState != ROT_WAIT_TO_RESET){
		rot->toReport = ROT_CW;
		rot->hwState  = ROT_WAIT_TO_RESET;
	}else if(!encA && !encB && rot->hwState  == ROT_CCW && rot->hwState != ROT_WAIT_TO_RESET){
		rot->toReport = ROT_CCW;
		rot->hwState  = ROT_WAIT_TO_RESET;
	}else if(encA && encB){
		rot->hwState = ROT_IDLE;
	}

	if(rot->reportState == ROT_REP_IDLE && rot->toReport > 0){
		if(rot->toReport == ROT_CW) rot->reportState = ROT_REP_PUSH_CW;
		if(rot->toReport == ROT_CCW) rot->reportState = ROT_REP_PUSH_CCW;
		rot->toReport = 0;
		rot->msCounter = APP_ROT_REP_MS;
	}else if(rot->reportState > ROT_REP_IDLE){
		if(rot->msCounter > 0){
			rot->msCounter --;
		}else{
			if(rot->reportState == ROT_REP_PUSH_CW || rot->reportState == ROT_REP_PUSH_CCW){
				rot->reportState = ROT_REP_DELAY;
				rot->msCounter = APP_ROT_REP_MS;
			}else{
				rot->reportState = ROT_REP_IDLE;
			}
		}
	}
}

ENC_RotaryReportState ENC_GetState(ENC_RotHandler *rot)
{
	return rot->reportState;
}
