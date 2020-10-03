/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "rtc.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "string.h"
#include "TM1637.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum MODE {
  DISP_TIME = 0,
  DISP_DATE,
  MODIFY_SECOND,
  MODIFY_MINUTE,
  MODIFY_HOUR,
  MODIFY_DATE,
  MODIFY_MONTH,
  MODIFY_YEAR,
  DISP_ADC1,
  //INSERT BEFORE
  CNT_RTC_MODE,
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define ADC_ARRAY_SIZE 128

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t mode = 0;
TM1637_InstDef tm1637 = {GPIOA, GPIO_PIN_6, GPIO_PIN_7, 6};
char buffer[64] = {0};
uint32_t adc1_array[ADC_ARRAY_SIZE] = {0};
uint8_t adc1_index = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
uint32_t ConvertADC(ADC_HandleTypeDef* hadc) {
  HAL_ADC_Start(hadc);
  HAL_ADC_PollForConversion(hadc, 0xffff);
  uint32_t value = HAL_ADC_GetValue(hadc);
  HAL_ADC_Stop(hadc);
  return value;
}

void UpdateDisplay(uint8_t mode, RTC_DateTypeDef date, RTC_TimeTypeDef time){
  int blink = (HAL_GetTick() / 200) % 2 > 0 ? 0xFF : 0;
  char raw[6] = {0};
  adc1_index %= ADC_ARRAY_SIZE;
  adc1_array[adc1_index++] = ConvertADC(&hadc1);
  uint32_t adc1_avg = 0;
  for (uint8_t i = 0;i<ADC_ARRAY_SIZE;i++){
      adc1_avg += adc1_array[i];
  }
  adc1_avg /= ADC_ARRAY_SIZE;

  switch (mode)
  {
  case DISP_TIME:
  case MODIFY_SECOND:
  case MODIFY_MINUTE:
  case MODIFY_HOUR:
    raw[0] = mode != MODIFY_SECOND ? segmentMap[time.Seconds%10] : segmentMap[time.Seconds%10] & blink;
    raw[1] = mode != MODIFY_SECOND ? segmentMap[time.Seconds/10] : segmentMap[time.Seconds/10] & blink;
    raw[2] = mode != MODIFY_MINUTE ? segmentMap[time.Minutes%10] | (1<<7) : segmentMap[time.Minutes%10] & blink | (1<<7);
    raw[3] = mode != MODIFY_MINUTE ? segmentMap[time.Minutes/10] : segmentMap[time.Minutes/10] & blink;
    raw[4] = mode != MODIFY_HOUR ? segmentMap[time.Hours%10] | (1<<7) : segmentMap[time.Hours%10] & blink | (1<<7);
    raw[5] = mode != MODIFY_HOUR ? segmentMap[time.Hours/10] : segmentMap[time.Hours/10] & blink;
    break;
  case DISP_DATE:
  case MODIFY_DATE:
  case MODIFY_MONTH:
  case MODIFY_YEAR:
    raw[0] = mode != MODIFY_DATE ? segmentMap[date.Date%10] : segmentMap[date.Date%10] & blink;
    raw[1] = mode != MODIFY_DATE ? segmentMap[date.Date/10] : segmentMap[date.Date/10] & blink;
    raw[2] = mode != MODIFY_MONTH ? segmentMap[date.Month%10] | (1<<7) : segmentMap[date.Month%10] & blink | (1<<7);
    raw[3] = mode != MODIFY_MONTH ? segmentMap[date.Month/10] : segmentMap[date.Month/10] & blink;
    raw[4] = mode != MODIFY_YEAR ? segmentMap[date.Year%10] : segmentMap[date.Year%10] & blink | (1<<7);
    raw[5] = mode != MODIFY_YEAR ? segmentMap[date.Year/10] : segmentMap[date.Year/10] & blink;
    break;
  case DISP_ADC1:
    raw[0] = segmentMap[adc1_avg % 10];
    raw[1] = segmentMap[adc1_avg / 10 % 10];
    raw[2] = segmentMap[adc1_avg / 100  % 10];
    raw[3] = segmentMap[adc1_avg / 1000  % 10];
    raw[4] = segmentMap[adc1_avg / 10000  % 10];
    raw[5] = segmentMap[adc1_avg / 100000  % 10];
    break;
  default:
    break;
  }
  tm1637_DisplayRaw(&tm1637, raw, sizeof(raw));
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_USB_DEVICE_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  /* USER CODE BEGIN 2 */
  RTC_First_Init(&hrtc);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

  RTC_TimeTypeDef time = {0};
  RTC_DateTypeDef date = {0};

  tm1637_Init(&tm1637);

  GPIO_PinState tim3_sw_last_state = HAL_GPIO_ReadPin(TIM3_SW_GPIO_Port, TIM3_SW_Pin);

  int16_t diff_last = 0;

  HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    uint32_t tick = HAL_GetTick();
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, tick / 100 % 2);
    HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, !(tick / 100 % 2));

    GPIO_PinState tim3_sw = HAL_GPIO_ReadPin(TIM3_SW_GPIO_Port, TIM3_SW_Pin);
    if (tim3_sw != tim3_sw_last_state) {
      if (tim3_sw == GPIO_PIN_SET)
      {
        htim3.Instance->CNT = 0;
        diff_last = htim3.Instance->CNT;
        mode++;
        mode %= CNT_RTC_MODE;
        sprintf(buffer, "mode = %d\r\n", mode);
        CDC_Transmit_FS(buffer, strlen(buffer));
      }
      tim3_sw_last_state = tim3_sw;
    }
    
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);

    int16_t diff = htim3.Instance->CNT;
    diff /= 4;
    if (diff != diff_last) {
      sprintf(buffer, "CNT = %d, DIFF= %d\r\n", htim3.Instance->CNT, diff);
      CDC_Transmit_FS(buffer, strlen(buffer));
      switch (mode)
      {
      case MODIFY_SECOND:
        time.Seconds = (time.Seconds + 60 + diff) % 60;
        break;
      case MODIFY_MINUTE:
        time.Minutes = (time.Minutes + 60 + diff) % 60;
        break;
      case MODIFY_HOUR:
        time.Hours = (time.Hours + 24 + diff) % 24;
        break;
      case MODIFY_DATE:
        date.Date = (date.Date + 31 + diff) % 31;
        break;
      case MODIFY_MONTH:
        date.Month = (date.Month + 12 + diff) % 12;
        break;
      case MODIFY_YEAR:
        date.Year = (date.Year + 100 + diff) % 100;
        break;
      default:
        break;
      }
      htim3.Instance->CNT = 0;
      HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
      HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
      diff_last = htim3.Instance->CNT;
    }
    
    UpdateDisplay(mode, date, time);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
