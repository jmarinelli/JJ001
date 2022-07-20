/*
 * app.c
 *
 *  Created on: Jul 20, 2022
 *      Author: juanjosemarinelli
 */


#include "app.h"
#include "debouncer.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

uint8_t APP_usbFailsCounter = 0;
uint8_t APP_usbReportTimer = 0;
uint8_t APP_usbReportOk = 0;

#define 	TOTAL_INPUTS 32
#define 	APP_INPUTS	20	// simple direct input switches (no matrix)
#define		APP_MATRIX_OFFSET			0
#define 	APP_SIMPLE_INPUT_OFFSET		12

uint32_t APP_inputPins[APP_INPUTS] = {
	EC1_Pin,
	EC2_Pin,
	EC3_Pin,
	EC4_Pin,
	EC5_Pin,
	EC6_Pin,
	EC7_Pin,
	EC8_Pin,
	EC9_Pin,
	EC10_Pin,
	SW1_Pin,
	SW2_Pin,
	FUNKY_A_Pin,
	FUNKY_B_Pin,
	FUNKY_C_Pin,
	FUNKY_D_Pin,
	FUNKY_EC1_Pin,
	FUNKY_EC2_Pin,
	FUNKY_PB_Pin,
	EP_Pin
};

GPIO_TypeDef * APP_inputGPIOs[APP_INPUTS] = {
	EC1_GPIO_Port,
	EC2_GPIO_Port,
	EC3_GPIO_Port,
	EC4_GPIO_Port,
	EC5_GPIO_Port,
	EC6_GPIO_Port,
	EC7_GPIO_Port,
	EC8_GPIO_Port,
	EC9_GPIO_Port,
	EC10_GPIO_Port,
	SW1_GPIO_Port,
	SW2_GPIO_Port,
	FUNKY_A_GPIO_Port,
	FUNKY_B_GPIO_Port,
	FUNKY_C_GPIO_Port,
	FUNKY_D_GPIO_Port,
	FUNKY_EC1_GPIO_Port,
	FUNKY_EC2_GPIO_Port,
	FUNKY_PB_GPIO_Port,
	EP_GPIO_Port
};

uint8_t APP_inputState[TOTAL_INPUTS];
uint8_t APP_inputDebounced[TOTAL_INPUTS];

// Encoders
ENC_RotHandler APP_encoderA = { .pinA = &(APP_inputDebounced[12]), .pinB = &(APP_inputDebounced[13]), .stepDivider = 1  };
ENC_RotHandler APP_encoderB = { .pinA = &(APP_inputDebounced[14]), .pinB = &(APP_inputDebounced[15]), .stepDivider = 1  };
ENC_RotHandler APP_encoderC = { .pinA = &(APP_inputDebounced[16]), .pinB = &(APP_inputDebounced[17]), .stepDivider = 1  };
ENC_RotHandler APP_encoderD = { .pinA = &(APP_inputDebounced[18]), .pinB = &(APP_inputDebounced[19]), .stepDivider = 1  };
ENC_RotHandler APP_encoderE = { .pinA = &(APP_inputDebounced[20]), .pinB = &(APP_inputDebounced[21]), .stepDivider = 1  };
ENC_RotHandler APP_encoderF = { .pinA = &(APP_inputDebounced[28]), .pinB = &(APP_inputDebounced[29]), .stepDivider = 1  };


uint8_t APP_RotState[16];
uint8_t APP_AdjustRotState[8];
uint8_t APP_AdjustRotState_last[8];

uint8_t APP_EnableTask = 0;

void APP_ReadInputs(void);
void APP_UsbReport(void);

void APP_Init(void)
{
    // Light up leds
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);

	// Init encoders
	DEB_SetTime(12,50);
	DEB_SetTime(13,50);
	DEB_SetTime(14,50);
	DEB_SetTime(15,50);
	DEB_SetTime(16,50);
	DEB_SetTime(17,50);
	DEB_SetTime(18,50);
	DEB_SetTime(19,50);
	DEB_SetTime(20,50);
	DEB_SetTime(21,50);
	DEB_SetTime(28,50);
	DEB_SetTime(29,50);
	ENC_Init(&APP_encoderA);
	ENC_Init(&APP_encoderB);
	ENC_Init(&APP_encoderC);
	ENC_Init(&APP_encoderD);
	ENC_Init(&APP_encoderE);
	ENC_Init(&APP_encoderF);

    // Enable reporting
    APP_EnableTask = 1;
}

void APP_ReadInputs(void)
{
	for(uint8_t i=0; i<APP_INPUTS; i++)
	{
		APP_inputState[APP_SIMPLE_INPUT_OFFSET+i] = !HAL_GPIO_ReadPin(APP_inputGPIOs[i], APP_inputPins[i]);
	}
}

void APP_1msTask(void)
{

	if(APP_EnableTask)
	{
		APP_usbReportTimer++;
		if(APP_usbReportTimer >= APP_USB_REPORT_INTERVAL)
		{
			APP_usbReportTimer = 0;
			APP_UsbReport();
		}


		APP_ReadInputs();

		DEB_DebounceArray(APP_inputDebounced, APP_inputState, TOTAL_INPUTS);

		ENC_Process(&APP_encoderA);
		ENC_Process(&APP_encoderB);
		ENC_Process(&APP_encoderC);
		ENC_Process(&APP_encoderD);
		ENC_Process(&APP_encoderE);
		ENC_Process(&APP_encoderF);

	}

}


void APP_UsbReport(void)
{
	static uint8_t usbReportBuff[APP_USB_REPORT_SIZE];

	usbReportBuff[0] = 	APP_inputDebounced[0]  << 0 |
						APP_inputDebounced[1]  << 1 |
						APP_inputDebounced[2]  << 2 |
						APP_inputDebounced[3]  << 3 |
						APP_inputDebounced[4]  << 4 |
						APP_inputDebounced[5]  << 5 |
						APP_inputDebounced[6]  << 6 |
						APP_inputDebounced[7]  << 7 ;

	usbReportBuff[1] = 	APP_inputDebounced[8]  << 0 |
						APP_inputDebounced[9]  << 1 |
						APP_inputDebounced[10]  << 2 |
						APP_inputDebounced[11]  << 3 |
						(ENC_GetState(&APP_encoderA) == ROT_REP_PUSH_CW)   << 4 | // APP_inputState[12]  << 4 |
						(ENC_GetState(&APP_encoderA) == ROT_REP_PUSH_CCW)  << 5 | // APP_inputState[13]  << 5 |
						(ENC_GetState(&APP_encoderB) == ROT_REP_PUSH_CW)   << 6 | // APP_inputState[14]  << 6 |
						(ENC_GetState(&APP_encoderB) == ROT_REP_PUSH_CCW)  << 7 ; // APP_inputState[15]  << 7 |

	usbReportBuff[2] = 	(ENC_GetState(&APP_encoderC) == ROT_REP_PUSH_CW)   << 0 | // APP_inputState[16]  << 5 |
						(ENC_GetState(&APP_encoderC) == ROT_REP_PUSH_CCW)  << 1 | // APP_inputState[17]  << 5 |
						(ENC_GetState(&APP_encoderD) == ROT_REP_PUSH_CW)   << 2 |
						(ENC_GetState(&APP_encoderD) == ROT_REP_PUSH_CCW)  << 3 |
						(ENC_GetState(&APP_encoderE) == ROT_REP_PUSH_CW)   << 4 |
						(ENC_GetState(&APP_encoderE) == ROT_REP_PUSH_CCW)  << 5 |
						APP_inputDebounced[22]  << 6 |
						APP_inputDebounced[23]  << 7;

	usbReportBuff[3] = 	APP_inputDebounced[24]  << 0 |
						APP_inputDebounced[25]  << 1 |
						APP_inputDebounced[26]  << 2 |
						APP_inputDebounced[27]  << 3 |
						(ENC_GetState(&APP_encoderF) == ROT_REP_PUSH_CW)   << 4 | // APP_inputState[28]  << 5 |
						(ENC_GetState(&APP_encoderF) == ROT_REP_PUSH_CCW)  << 5 | // APP_inputState[29]  << 5 |
						APP_inputDebounced[30]  << 6 |
						APP_inputDebounced[31]  << 7 ;


	// Send report
	if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t*)usbReportBuff, APP_USB_REPORT_SIZE) != USBD_OK)
	{
		APP_usbFailsCounter++;
	}
	else
	{
		APP_usbFailsCounter = 0;
		APP_usbReportOk = 1;
	}

	// Check if USB report failed to many times
	if(APP_usbFailsCounter > APP_USB_FAIL_LIMIT)
	{
		USBD_CUSTOM_HID_ResetState(&hUsbDeviceFS);
		APP_usbFailsCounter = 0;
	}
}
