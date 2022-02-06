#include <Arduino.h>
#include <hiotMainlib.h>

HiotDevice device;

void setup() {
	device.setLEDPins(15,12,13); 
	// device.setLEDPins(14,12,13); // pcb
	// device.setLEDPinsRGBW(14,12,13,15);
	// device.setLEDPinsRGBW(13,15,12,14); // fenstertreppe
	device.setup();
}

void loop() {
	device.loop();
}