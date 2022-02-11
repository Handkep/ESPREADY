#ifndef HIOTMAINLIB_H
#define HIOTMAINLIB_H

#include <Arduino.h>
#include <Arduinojson.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <PubSubClient.h>
#include <Adafruit_BME280.h>
#include <PCF8574.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <NeoPixelBus.h>

// used for print colors on the console
#define ANSI_GREEN "\u001b[32m"
#define ANSI_YELLOW "\u001b[33m"
#define ANSI_RED "\u001b[31m"
#define ANSI_CYAN "\u001b[96m"
#define ANSI_RST "\u001b[0m"
// status led pin
#define ONBORDLED 16

// bool pin1;

// #define PRINTJSON

// storing config information
struct Config{
    // network configuration
    char ssid[50];
    char wifiPassword[50];
    char mqttBrokerIp[50];
    int  mqttBrokerPort;
    char mqttBrokerUser[50];
    char mqttBrokerPassword[50];
    char espHostname[50];
    // device configuration
    bool debug;
    bool useRGBW;
    bool useWS2812;
    bool useBME280;
    bool usePCF8574;
    bool enableLEDs;
};

extern Config conf;

class ColorObj
{
    public:
        // TODO clear up / combine both pin-setup functions 
        void setRGBPins(int R, int G, int B);
        void setRGBWPins(int R, int G, int B, int W);
        void setup();
        bool convert();
        void setColorString(String color);
        String getColorStringRGB(int i);
        String getColorStringRGBW(int i);
        void loop();
        int calculatems(int speed, int current_colors, int next_colors);
        #define LEDAMMOUNT 1
        const int ledAmmount = LEDAMMOUNT;
        int RGB_Pin[3];  
        int RGBW_Pin[4];
        int RGB[LEDAMMOUNT][3];
        int RGBW[LEDAMMOUNT][4];
        int RGB_write[LEDAMMOUNT][3];
        int RGBW_write[LEDAMMOUNT][4];
        unsigned long fade_micros[4];
        unsigned long fade_millis; //could be deleted
        int pinAmount;
        int jumpcolors[3][3] = {{255,0,0},{0,255,0},{0,0,255}};
        int strobecolors[2][3] = {{255,255,255},{0,0,0}};
        int jumplen = 3;
        int strobelen = 2;
        int currentEffect;
        unsigned long fadespeed = 2000;
        int ledIndex;
        bool isColorOnOrOff;
    private:
        // effects:

        void jump();
        void strobe();
        void fade();
        void effect();
        unsigned long _millis_Effect;
        int effectIndex;
        void animateColor();
        void writeColor();
        unsigned long _lastMillis_writeColor_neopixelbus;
        int _countAnimateColor;
        int _countWriteColor;
        unsigned long _millis_printcolors;
        unsigned long _lastMillis_AnimateColors[LEDAMMOUNT][4];
        unsigned long fadems[LEDAMMOUNT][4];
};
        

class HiotDevice{
    public:
        HiotDevice();
        // main loop 
        void loop();
        void setup();
        void setLEDPins(int R, int G, int B);
    private:
        String backupColor;
        int backupEffect;
        String interruptColor;
        String interruptEffect;
        ColorObj colors;
        bool enableReadPin1 = 1;
        bool enableWritePin1;
        // PCF8574 PCF();
        // NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
        int version;
        void printVersionSerial();
        //blocking func to blink onbord LED on AMICA NODEMCU
        void alertBlink(int lengthon, int lengthoff, int count);
        void logSerialPretty(String inf);
        void logSerialPretty(String inf, String inf1);
        void logSerialPretty(String inf, String inf1, String inf2);
        
        // level 0 = debug
        // level 1 = warn
        // level 2 = error
        // level 3 = info
        void logSerial(String inf, int level);
        void logSerial(String name, String color, String inf);
        String insertHostnameintoVariable(String topic);
        // loading config
        void loadConfig();
        String getESPStateJson();
        String getESPColorJson();
        void publishESPStateJsonRepetitive();
            unsigned long _millis_publishESPStateJsonRepetitive = 0;
            unsigned long _millis_publishESPColor = 0;
        void publishBMETemp();
            Adafruit_BME280 sensor;
            unsigned long _millis_publishBMETemp = 0;
        
        void connectToWifi();
        void mqttCallback(char* topic, byte* payld, int length);
        void connectToMQTT();

};
void checkforPins();
        

#endif
