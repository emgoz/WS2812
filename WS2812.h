/*
* another light weight WS2812 lib
*
* Controls WS2811/WS2812/WS2812B RGB-LEDs
*
* Based on the code by Matthias Riegler, Windell H. Oskay and Freezy
* Added HSV conversion from FastLED lib by Daniel Garcia and Mark Kriegsman; see fastled.io
*
* April 2, 2015
* Marv aka eMGoz or MGOS
*
*/

#ifndef WS2812_H_
#define WS2812_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#ifndef F_CPU
#define  F_CPU 16000000UL
#endif
#include <util/delay.h>
#include <stdint.h>

#ifdef ARDUINO
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif
#endif

// If you want to use the setColorOrder functions, enable this line:
// #define RGB_ORDER_ON_RUNTIME

// If you don't want to use the setBrightness-function in order
// to save program space and faster execution (esp. on ATtiny) disable this line:
#define USE_GLOBAL_BRIGHTNESS

#ifdef RGB_ORDER_ON_RUNTIME
#define OFFSET_R(r) r+offsetRed
#define OFFSET_G(g) g+offsetGreen
#define OFFSET_B(b) b+offsetBlue
#else
// CHANGE YOUR STATIC RGB ORDER HERE
#define OFFSET_R(r) r+1
#define OFFSET_G(g) g
#define OFFSET_B(b) b+2
#endif

const byte dim_curve[] =
{
    0, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6,
    6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8,
    8, 8, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11,
    11, 11, 12, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15,
    15, 15, 16, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20,
    20, 20, 21, 21, 22, 22, 22, 23, 23, 24, 24, 25, 25, 25, 26, 26,
    27, 27, 28, 28, 29, 29, 30, 30, 31, 32, 32, 33, 33, 34, 35, 35,
    36, 36, 37, 38, 38, 39, 40, 40, 41, 42, 43, 43, 44, 45, 46, 47,
    48, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
    63, 64, 65, 66, 68, 69, 70, 71, 73, 74, 75, 76, 78, 79, 81, 82,
    83, 85, 86, 88, 90, 91, 93, 94, 96, 98, 99, 101, 103, 105, 107, 109,
    110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
    146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
    193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255,
};



class WS2812
{
public:
    WS2812(uint16_t num_led);
    ~WS2812();

#ifndef ARDUINO
    void setOutput(const volatile uint8_t* port, volatile uint8_t* reg, uint8_t pin);
#else
    void setOutput(uint8_t pin);
#endif


    uint32_t getRGB(uint16_t index);
    void setRGB(uint16_t index, uint32_t rgb);
    void setRGB(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
    void setHSV(uint16_t index, uint8_t hue, uint8_t sat, uint8_t val);  // hue between 0 and 191
#ifdef USE_GLOBAL_BRIGHTNESS
    void setBrightnessLinear(uint8_t _brightness)
    {
        brightness = dim_curve[_brightness];
    }
    void setBrightness(uint8_t _brightness)
    {
        brightness = _brightness;
    }
    uint8_t getBrightness()
    {
        return brightness;
    }
#endif


    void sync();

#ifdef RGB_ORDER_ON_RUNTIME
    void setColorOrderRGB();
    void setColorOrderGRB();
    void setColorOrderBRG();
#endif

private:
    uint16_t count_led;
    uint8_t *pixels;
#ifdef USE_GLOBAL_BRIGHTNESS
    uint8_t brightness;
#endif
    void ws2812_sendarray_mask(uint8_t *array,uint16_t length, uint8_t pinmask,uint8_t *port, uint8_t *portreg);

    const volatile uint8_t *ws2812_port;
    volatile uint8_t *ws2812_port_reg;
    uint8_t pinMask;

#ifdef RGB_ORDER_ON_RUNTIME
    uint8_t offsetRed;
    uint8_t offsetGreen;
    uint8_t offsetBlue;
#endif
};



#endif /* WS2812_H_ */
