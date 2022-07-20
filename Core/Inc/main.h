/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

#define APP_USB_REPORT_SIZE 			4

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FUNKY_D_Pin GPIO_PIN_14
#define FUNKY_D_GPIO_Port GPIOC
#define FUNKY_EC1_Pin GPIO_PIN_15
#define FUNKY_EC1_GPIO_Port GPIOC
#define FUNKY_EC2_Pin GPIO_PIN_0
#define FUNKY_EC2_GPIO_Port GPIOA
#define FUNKY_B_Pin GPIO_PIN_1
#define FUNKY_B_GPIO_Port GPIOA
#define FUNKY_PB_Pin GPIO_PIN_2
#define FUNKY_PB_GPIO_Port GPIOA
#define FUNKY_A_Pin GPIO_PIN_3
#define FUNKY_A_GPIO_Port GPIOA
#define FUNKY_C_Pin GPIO_PIN_8
#define FUNKY_C_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
