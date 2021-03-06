/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SEG1_CLK_Pin GPIO_PIN_6
#define SEG1_CLK_GPIO_Port GPIOA
#define SEG1_DIO_Pin GPIO_PIN_7
#define SEG1_DIO_GPIO_Port GPIOA
#define SEG4_CLK_Pin GPIO_PIN_4
#define SEG4_CLK_GPIO_Port GPIOC
#define SEG4_DIO_Pin GPIO_PIN_5
#define SEG4_DIO_GPIO_Port GPIOC
#define SEG2_CLK_Pin GPIO_PIN_0
#define SEG2_CLK_GPIO_Port GPIOB
#define SEG2_DIO_Pin GPIO_PIN_1
#define SEG2_DIO_GPIO_Port GPIOB
#define SEG3_CLK_Pin GPIO_PIN_10
#define SEG3_CLK_GPIO_Port GPIOB
#define SEG3_DIO_Pin GPIO_PIN_11
#define SEG3_DIO_GPIO_Port GPIOB
#define TIM3_SW_Pin GPIO_PIN_8
#define TIM3_SW_GPIO_Port GPIOC
#define TIM5_SW_Pin GPIO_PIN_9
#define TIM5_SW_GPIO_Port GPIOA
#define RED_Pin GPIO_PIN_10
#define RED_GPIO_Port GPIOC
#define YELLOW_Pin GPIO_PIN_11
#define YELLOW_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
