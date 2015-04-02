#include <WS2812.h>

WS2812 LED(1); // 1 LED
	
void setup() {
	LED.setOutput(3); // Digital Pin 3
	LED.setRGB(0, 255,0,0); // Set LED at index 0 red
}

void loop() {
	static byte brightness = 0;
	LED.setBrightnessLinear(brightness++);
	LED.sync();
	delay(10); 
}

