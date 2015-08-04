#include <WS2812.h>

#define LEDCount 10
#define outputPin 3

WS2812 LED(LEDCount); 
  
void setup() {
  LED.setOutput(outputPin);
  LED.setBrightness(127);  //half
}

void loop() {
  static byte t;
  for (int i = 0; i < LEDCount; i++){
    byte hue = LEDCount+t;
    LED.setHSV(i, hue, 255,255);
  
  }
  
  LED.sync();
  
  
  t++;
  
  
  delay(10);
}