#include <Arduino.h>
#include <hiotMainlib.h>


// test


HiotDevice device;



void setup() {
	device.setLEDPins(13,15,12);
	device.setup();
	
}

void loop() {
	device.loop();
}