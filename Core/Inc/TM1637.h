#ifndef __TM1637_H
#define __TM1637_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f1xx_hal.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t clk_pin;
    uint16_t dio_pin;
    uint8_t grid;
} TM1637_InstDef;

void tm1637_Init(TM1637_InstDef *htm1637);
void tm1637_DisplayDec(TM1637_InstDef *htm1637, int v, int displaySeparator);
void tm1637_DisplayHex(TM1637_InstDef *htm1637, int v, int displaySeparator);
void tm1637_DisplayBin(TM1637_InstDef *htm1637, int v, int displaySeparator);
void tm1637_SetBrightness(TM1637_InstDef *htm1637, char brightness);
void tm1637_DisplayRaw(TM1637_InstDef *htm1637, unsigned char raw[], int len);

const char segmentMap[];

#ifdef __cplusplus
  }
#endif /* __cplusplus */
  
#endif /* __STM32F101x6_H */