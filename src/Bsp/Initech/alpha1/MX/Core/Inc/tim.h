/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */


/* USER CODE END Includes */

extern TIM_HandleTypeDef htim4;

extern TIM_HandleTypeDef htim10;

extern TIM_HandleTypeDef htim11;

/* USER CODE BEGIN Private defines */
#define PWM_LED_BLUE_BLOCK_PTR      (&htim4)
#define PWM_LED_BLUE_CHANNEL        TIM_CHANNEL_4
#define PWM_LED_GREEN_BLOCK_PTR     (&htim4)
#define PWM_LED_GREEN_CHANNEL       TIM_CHANNEL_3
#define PWM_LED_RED_BLOCK_PTR       (&htim11)
#define PWM_LED_RED_CHANNEL         TIM_CHANNEL_1

#define PWM_BACKLIGHT_BLOCK_PTR     (&htim10)
#define PWM_BACKLIGHT_CHANNEL       TIM_CHANNEL_1

#define PWM_HEATER_BLOCK_PTR        (&htim4)
#define PWM_HEATER_CHANNEL          TIM_CHANNEL_1
#define PWM_FAN_BLOCK_PTR           (&htim4)
#define PWM_FAN_CHANNEL             TIM_CHANNEL_2

/* USER CODE END Private defines */

void MX_TIM4_Init(void);
void MX_TIM10_Init(void);
void MX_TIM11_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

