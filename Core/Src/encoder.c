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
	rot->stepCounter = 0;
	rot->stepTimeout = 0;

	if(rot->stepDivider != 2 && rot->stepDivider != 4) rot->stepDivider = 1;
}

void ENC_Process(ENC_RotHandler *rot){
	uint8_t encA, encB;
	ENC_RotaryState dir = 0;


	// Get encoder current state
	encA = *(rot->pinA);
	encB = *(rot->pinB);

	// Check step
	if(encA != rot->lastStateA || encB != rot->lastStateB)
	{
		if(rot->lastStateA == 0 && rot->lastStateB == 0)
		{
			if(encA == 1 && encB == 0)
			{
				dir = ROT_CW;
			}
			else
			{
				dir = ROT_CCW;
			}
		}
		else if(rot->lastStateA == 0 && rot->lastStateB == 1)
		{
			if(encA == 0 && encB == 0)
			{
				dir = ROT_CW;
			}
			else
			{
				dir = ROT_CCW;
			}
		}
		else if(rot->lastStateA == 1 && rot->lastStateB == 1)
		{
			if(encA == 0 && encB == 1)
			{
				dir = ROT_CW;
			}
			else
			{
				dir = ROT_CCW;
			}
		}
		else
		{
			if(encA == 1 && encB == 1)
			{
				dir = ROT_CW;
			}
			else
			{
				dir = ROT_CCW;
			}
		}
	}

	// If there was a step
	if(dir > 0)
	{
		if(rot->stepCounter == 0 || rot->lastStep == dir)	// First step or same direction
		{
			rot->stepCounter++;
			if(rot->stepCounter == rot->stepDivider)
			{
				// Reset values
				rot->stepCounter = 0;
				rot->lastStep = 0;
				rot->stepTimeout = 0;

				// Ready to report
				rot->toReport = dir;
			}
			else
			{
				rot->stepTimeout = APP_ROT_STEP_TIMEOUT_MS;
				rot->lastStep = dir;
			}
		}
		else // Other direction than last step
		{
			rot->lastStep = dir;
			rot->stepCounter = 1;
		}
	}

	//Check step timeout
	if(rot->stepTimeout > 0)
	{
		rot->stepTimeout--;

		// Reset step if timeout reached
		if(rot->stepTimeout == 0)
		{
			rot->stepCounter = 0;
			rot->lastStep = 0;
		}
	}

	// Reporting
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

	// Save current pin state
	rot->lastStateA = encA;
	rot->lastStateB = encB;
}

ENC_RotaryReportState ENC_GetState(ENC_RotHandler *rot)
{
	return rot->reportState;
}
