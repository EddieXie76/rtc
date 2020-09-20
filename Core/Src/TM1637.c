#include "stm32f1xx_hal.h"
#include "TM1637.h"

void _tm1637_Start(TM1637_InstDef *htm1637);
void _tm1637_Stop(TM1637_InstDef *htm1637);
void _tm1637_ReadResult(TM1637_InstDef *htm1637);
void _tm1637_WriteByte(TM1637_InstDef *htm1637, unsigned char b);
void _tm1637_DelayUsec(unsigned int i);
void _tm1637_ClkHigh(TM1637_InstDef *htm1637);
void _tm1637_ClkLow(TM1637_InstDef *htm1637);
void _tm1637_DioHigh(TM1637_InstDef *htm1637);
void _tm1637_DioLow(TM1637_InstDef *htm1637);

const char segmentMap[] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, // 0-7
	0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, // 8-9, A-F
	0x00
};


void tm1637_Init(TM1637_InstDef *htm1637)
{
    tm1637_SetBrightness(htm1637, 2);
    unsigned char raw[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    tm1637_DisplayRaw(htm1637, raw, sizeof(raw));
}

void tm1637_DisplayRaw(TM1637_InstDef *htm1637, unsigned char raw[], int len)
{
    _tm1637_Start(htm1637);
    _tm1637_WriteByte(htm1637, 0x40);
    _tm1637_ReadResult(htm1637);
    _tm1637_Stop(htm1637);

    _tm1637_Start(htm1637);
    _tm1637_WriteByte(htm1637, 0xc0);
    _tm1637_ReadResult(htm1637);

    for (int i = len; i > 0; --i) {
        _tm1637_WriteByte(htm1637, raw[i - 1]);
        _tm1637_ReadResult(htm1637);
    }

    _tm1637_Stop(htm1637);
}

void tm1637_DisplayNumber(TM1637_InstDef *htm1637, int v, int displaySeparator, int base)
{
    unsigned char digitArr[htm1637->grid];
    for (int i = 0; i < htm1637->grid; ++i) {
        digitArr[i] = segmentMap[v % base];
        if (i == displaySeparator) {
            digitArr[i] |= 1 << 7;
        }
        v /= base;
    }

    tm1637_DisplayRaw(htm1637, digitArr, htm1637->grid);
}

void tm1637_DisplayDec(TM1637_InstDef *htm1637, int v, int displaySeparator)
{
	tm1637_DisplayNumber(htm1637, v, displaySeparator, 10);
}


void tm1637_DisplayHex(TM1637_InstDef *htm1637, int v, int displaySeparator)
{
	tm1637_DisplayNumber(htm1637, v, displaySeparator, 16);
}

void tm1637_DisplayBin(TM1637_InstDef *htm1637, int v, int displaySeparator)
{
	tm1637_DisplayNumber(htm1637, v, displaySeparator, 2);
}



// Valid brightness values: 0 - 8.
// 0 = display off.
void tm1637_SetBrightness(TM1637_InstDef *htm1637, char brightness)
{
    // Brightness command:
    // 1000 0XXX = display off
    // 1000 1BBB = display on, brightness 0-7
    // X = don't care
    // B = brightness
    _tm1637_Start(htm1637);
    _tm1637_WriteByte(htm1637, 0x87 + brightness);
    _tm1637_ReadResult(htm1637);
    _tm1637_Stop(htm1637);
}

void _tm1637_Start(TM1637_InstDef *htm1637)
{
    _tm1637_ClkHigh(htm1637);
    _tm1637_DioHigh(htm1637);
    _tm1637_DelayUsec(2);
    _tm1637_DioLow(htm1637);
}

void _tm1637_Stop(TM1637_InstDef *htm1637)
{
    _tm1637_ClkLow(htm1637);
    _tm1637_DelayUsec(2);
    _tm1637_DioLow(htm1637);
    _tm1637_DelayUsec(2);
    _tm1637_ClkHigh(htm1637);
    _tm1637_DelayUsec(2);
    _tm1637_DioHigh(htm1637);
}

void _tm1637_ReadResult(TM1637_InstDef *htm1637)
{
    _tm1637_ClkLow(htm1637);
    _tm1637_DelayUsec(5);
    // while (dio); // We're cheating here and not actually reading back the response.
    _tm1637_ClkHigh(htm1637);
    _tm1637_DelayUsec(2);
    _tm1637_ClkLow(htm1637);
}

void _tm1637_WriteByte(TM1637_InstDef *htm1637, unsigned char b)
{
    for (int i = 0; i < 8; ++i) {
        _tm1637_ClkLow(htm1637);
        if (b & 0x01) {
            _tm1637_DioHigh(htm1637);
        }
        else {
            _tm1637_DioLow(htm1637);
        }
        _tm1637_DelayUsec(3);
        b >>= 1;
        _tm1637_ClkHigh(htm1637);
        _tm1637_DelayUsec(3);
    }
}

void _tm1637_DelayUsec(unsigned int i)
{
    for (; i>0; i--) {
        for (int j = 0; j < 10; ++j) {
            __asm__ __volatile__("nop\n\t":::"memory");
        }
    }
}

void _tm1637_ClkHigh(TM1637_InstDef *htm1637)
{
    HAL_GPIO_WritePin(htm1637->port, htm1637->clk_pin, GPIO_PIN_SET);
}

void _tm1637_ClkLow(TM1637_InstDef *htm1637)
{
    HAL_GPIO_WritePin(htm1637->port, htm1637->clk_pin, GPIO_PIN_RESET);
}

void _tm1637_DioHigh(TM1637_InstDef *htm1637)
{
    HAL_GPIO_WritePin(htm1637->port, htm1637->dio_pin, GPIO_PIN_SET);
}

void _tm1637_DioLow(TM1637_InstDef *htm1637)
{
    HAL_GPIO_WritePin(htm1637->port, htm1637->dio_pin, GPIO_PIN_RESET);
}
