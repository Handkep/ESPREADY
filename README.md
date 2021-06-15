# ESPREADY
# Handke IoT
A Solution to controll all my RGB-LED-strips, collect Temperature data and Irrigate my Garden

## about my setup / hardware configuration
I am using NodeMCU dev boards and custom designed pcb's to build my LED "Driver". I got an Docker instance with Homeassistant, Octoprint, and PiHole (not yet) running on my Raspberry Pi 4B (4GB RAM)

## Features
- RGB and RGBW led strip 
- ws2812  (ONLY IN REV3)
- bme280
- PCF8574
- Wifi
- Mqtt
- configuration stored in Json files

## planned Features
- automatic updates via php script on a Raspberry pi


## Todo
- [x] implement PCF8574
- [ ] configuration file update (for configuring the PCF8574 and other little things) == mostly done ==
- [ ] reimplement RGBW support
- [ ] PCF8574 input button
- [ ] update via php on RPi



```c++
if(you_can_read_this){
    print("WOW, YOU`RE A HUMAN");
}
```

### About me / why dont i use Tasmota or ESP-Home?
back in 2018, i started (coding) a little bit HTML, CSS and Javascript. After that i got used to the whole microcontroller universe like the Arduino Nano or the NodeMCU. So i started to try some examples of the Arduino IDE. But i couldn´t code anything on my own. I decided to learn c++ (Arduino) to code my own IoT application. Now (some years later) i´m very proud of my IoT application. Currently im working on rev4, by the way: i had to rewrite the whole projekt 3 times because of my learning progress and my beginner-mistakes( odd coding, bad Variable-names and so on) :D. In the beginning it was more or less a project for learning purposes but became after some time very nice and usefull for my Room ^^