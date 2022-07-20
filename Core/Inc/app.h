/*
 * app.h
 *
 *  Created on: Jul 20, 2022
 *      Author: juanjosemarinelli
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include "main.h"

#define APP_REFRESH_RATE 				30		//ms
#define APP_DELTA_REFRESH_RATE 			200		//ms


#define APP_USB_FAIL_LIMIT				50
#define APP_USB_REPORT_SIZE 			4
#define APP_USB_REPORT_INTERVAL			5		//ms


void APP_Init(void);

void APP_1msTask(void);

#endif /* INC_APP_H_ */
