#include <WS2812.h>

WS2812 LED(1); // 1 LED
	
void setup() {
	LED.setOutput(3); // Digital Pin 3
	LED.setRGB(0, 255, 255, 255); // Set LED at index 0 white
}

void loop() {
	LED.setBrightness(255);	//full brightness
	LED.sync();		//refresh the LED
	delay(500);
	
	LED.setBrightness(0);	//off
	LED.sync(); 	//refresh the LED
	delay(500); 
}

