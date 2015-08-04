/*
* another light weight WS2812 lib
*
* Controls WS2811/WS2812/WS2812B RGB-LEDs
*
* Based on the code by Matthias Riegler, Windell H. Oskay and Freezy
* Added HSV conversion from FastLED lib by Daniel Garcia and Mark Kriegsman; see fastled.io
*
* Marv aka eMGoz or MGOS
*
*/

#include "WS2812.h"
#include <stdlib.h>


WS2812::WS2812(uint16_t num_leds)
{
    count_led = num_leds;

    pixels = (uint8_t*)malloc(count_led*3);
#ifdef RGB_ORDER_ON_RUNTIME
    offsetGreen = 0;
    offsetRed = 1;
    offsetBlue = 2;
#endif
#ifdef USE_GLOBAL_BRIGHTNESS
	brightness = 255;
#endif
}

void WS2812::setRGB(uint16_t index, uint8_t r, uint8_t g, uint8_t b)
{
    if(index < count_led)
    {
        uint16_t tmp = index * 3;
        pixels[OFFSET_R(tmp)] = r;
        pixels[OFFSET_G(tmp)] = g;
        pixels[OFFSET_B(tmp)] = b;
    }
}
void WS2812::setRGB(uint16_t index, uint32_t rgb)
{
    if(index < count_led)
    {
        uint16_t tmp = index * 3;
        pixels[OFFSET_R(tmp)] = (rgb >> 16) & 0xFF;
        pixels[OFFSET_G(tmp)] = (rgb >> 8) & 0xFF;
        pixels[OFFSET_B(tmp)] = rgb & 0xFF;
    }
}
uint32_t WS2812::getRGB(uint16_t index)
{
    if(index < count_led)
    {
        uint16_t tmp = index * 3;
        return ((uint32_t)(pixels[OFFSET_R(tmp)])<<16) |  (pixels[OFFSET_G(tmp)] << 8) | pixels[OFFSET_B(tmp)];
    }
    return 0;
}

#define HSV_SECTION_3 (64)
void WS2812::setHSV(uint16_t index, uint8_t hue, uint8_t sat, uint8_t val)
{
    uint8_t r,g,b;
    uint8_t value = dim_curve[ val ];
    uint8_t invsat = dim_curve[ 255 - sat ];
    uint8_t brightness_floor = (value * invsat) / 256;
    uint8_t color_amplitude = value - brightness_floor;
    uint8_t section = hue >> 6; // / HSV_SECTION_3; // 0..2
    uint8_t offset = hue & 0x3F ; // % HSV_SECTION_3;  // 0..63
    uint8_t rampup = offset; // 0..63
    uint8_t rampdown = (HSV_SECTION_3 - 1) - offset; // 63..0
    uint8_t rampup_amp_adj   = (rampup   * color_amplitude *4) / (256);
    uint8_t rampdown_amp_adj = (rampdown * color_amplitude *4) / (256);
    uint8_t rampup_adj_with_floor   = rampup_amp_adj   + brightness_floor;
    uint8_t rampdown_adj_with_floor = rampdown_amp_adj + brightness_floor;

    if( section )
    {
        if( section == 1)
        {
            // section 1: 0x40..0x7F
            r = brightness_floor;
            g = rampdown_adj_with_floor;
            b = rampup_adj_with_floor;
        }
        else
        {
            // section 2; 0x80..0xBF
            r = rampup_adj_with_floor;
            g = brightness_floor;
            b = rampdown_adj_with_floor;
        }
    }
    else
    {
        // section 0: 0x00..0x3F
        r = rampdown_adj_with_floor;
        g = rampup_adj_with_floor;
        b = brightness_floor;
    }
    setRGB(index,r,g,b);
}

void WS2812::sync()
{
    *ws2812_port_reg |= pinMask; // Enable DDR
    ws2812_sendarray_mask(pixels,3*count_led,pinMask,(uint8_t*) ws2812_port,(uint8_t*) ws2812_port_reg);
}

#ifdef RGB_ORDER_ON_RUNTIME
void WS2812::setColorOrderGRB()   // Default color order
{
    offsetGreen = 0;
    offsetRed = 1;
    offsetBlue = 2;
}

void WS2812::setColorOrderRGB()
{
    offsetRed = 0;
    offsetGreen = 1;
    offsetBlue = 2;
}

void WS2812::setColorOrderBRG()
{
    offsetBlue = 0;
    offsetRed = 1;
    offsetGreen = 2;
}
#endif

WS2812::~WS2812()
{
}

#ifndef ARDUINO
void WS2812::setOutput(const volatile uint8_t* port, volatile uint8_t* reg, uint8_t pin)
{
    pinMask = (1<<pin);
    ws2812_port = port;
    ws2812_port_reg = reg;
}
#else
void WS2812::setOutput(uint8_t pin)
{
    pinMask = digitalPinToBitMask(pin);
    ws2812_port = portOutputRegister(digitalPinToPort(pin));
    ws2812_port_reg = portModeRegister(digitalPinToPort(pin));
}
#endif


