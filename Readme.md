This Arduino library is based on the code by Matthias Riegler, Windell H. Oskay and Freezy;
see https://github.com/cpldcpu/light_ws2812
The low level bit banging is unmodified, originally by Tim aka cpldcpu (cpldcpu@gmail.com)

The HSV conversion is from the FastLED lib by Daniel Garcia and Mark Kriegsman;
see http://fastled.io/

I simplified the use of the original light_ws2812 by removing the cRGB datatype and added direct RGB and HSV access to each pixel.
Also a global brightness control has been implemented, similar to the one used in FastLED.
Added keywords for syntax highlighting in the Arduino IDE.
April 2nd, 2015
- Marv aka eMGoz or MGOS
http://emgoz.blogspot.de/
