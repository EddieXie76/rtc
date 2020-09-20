/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */
void RTC_First_Init(RTC_HandleTypeDef* hrtc) {
  if (HAL_RTCEx_BKUPRead(hrtc, RTC_BKP_DR1)==FIRST_INIT)
  {
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};
    time.Hours = 12;
    time.Minutes = 0;
    time.Seconds = 0;

    if (HAL_RTC_SetTime(hrtc, &time, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }
    date.WeekDay = RTC_WEEKDAY_MONDAY;
    date.Month = RTC_MONTH_JANUARY;
    date.Date = 1;
    date.Year = 20;

    if (HAL_RTC_SetDate(hrtc, &date, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }

    HAL_RTCEx_BKUPWrite(hrtc, RTC_BKP_DR1, FIRST_INIT);
  }
}

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{
  RTC_TamperTypeDef sTamper = {0};

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the RTC Tamper
  */
  sTamper.Tamper = RTC_TAMPER_1;
  sTamper.Trigger = RTC_TAMPERTRIGGER_LOWLEVEL;
  if (HAL_RTCEx_SetTamper(&hrtc, &sTamper) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/