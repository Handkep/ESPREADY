#include <hiotMainlib.h>
#define PRINTJSON

void HiotDevice::loadConfig(){
    logSerial("Loading Config",4);
    if (LittleFS.begin()){
        logSerial("Loading Littlefs",4);
    }else{
        logSerial("Loading Littlefs",2);
    }

    File ipFile;
    ipFile = LittleFS.open("/ip.json","r");
    StaticJsonDocument<512> doc;
    if(ipFile){
        logSerial("Loading ipFile",4);
        DeserializationError error = deserializeJson(doc, ipFile);
        if(error)logSerial("deserialization",2);
        #ifdef PRINTJSON
            logSerial("printing ipfile serial",0);
            serializeJsonPretty(doc, Serial);
            Serial.println();
        #endif
        //Read ssid
        if(doc.containsKey("ssid")){
            strlcpy(conf.ssid,doc["ssid"],sizeof(conf.ssid));
            logSerial("reading ssid", 4);
        } else {
            logSerial("reading ssid",2);
        }
        //read wifi password
        if(doc.containsKey("wifi_password")){
            strlcpy(conf.wifiPassword,doc["wifi_password"],sizeof(conf.wifiPassword));
            logSerial("reading wifiPassword", 4);
        } else {
            logSerial("reading wifiPassword", 2);
        }
        // read broker ip
        if(doc.containsKey("broker")){
            strlcpy(conf.mqttBrokerIp,doc["broker"],sizeof(conf.mqttBrokerIp));
            logSerial("reading mqttBrokerIp", 4);
        }else{
            logSerial("reading mqttBrokerIp", 2);
        }
        // read broker Port
        if(doc.containsKey("broker_port")){

            conf.mqttBrokerPort = doc["broker_port"];
            logSerial("reading mqttBrokerPort", 4);
        } else {
            logSerial("reading mqttBrokerPort", 2);
        }
        // read mqtt user
        if(doc.containsKey("mqtt_user")){

            strlcpy(conf.mqttBrokerUser,doc["mqtt_user"],sizeof(conf.mqttBrokerUser));
            logSerial("reading mqtt_user", 4);
        } else {
            logSerial("reading mqtt_user", 2);
        }       
        // read mqtt password
        if(doc.containsKey("mqtt_password")){

            strlcpy(conf.mqttBrokerPassword,doc["mqtt_password"],sizeof(conf.mqttBrokerPassword));
            logSerial("reading mqtt_password", 4);
        } else {
            logSerial("reading mqtt_password", 2);
        }                
        ipFile.close();
        // doc.clear();
    } else {
        logSerial("Loading ipFile",2);
    }

    // loading hostname
    File hostnameFile = LittleFS.open("/hostname.json","r");

    if(hostnameFile){
        logSerial("Loading hostnameFile",0);
        DeserializationError error = deserializeJson(doc, hostnameFile);
        if(error)logSerial("deserialization",2);
        #ifdef PRINTJSON
            logSerial("printing hostnameFile serial",0);
            serializeJsonPretty(doc, Serial);
            Serial.println();
        #endif

        if(doc.containsKey("hostname")){
            strlcpy(conf.espHostname,doc["hostname"],sizeof(conf.espHostname));
            logSerial("reading hostname", 4);
        } else {
            logSerial("reading hostname",2);
        }
        hostnameFile.close();
    } else {
        logSerial("Loading hostnameFile",2);
    }

    // loading config
    File configFile = LittleFS.open("/config.json","r");

    if(configFile){
        logSerial("Loading configFile",0);
        DeserializationError error = deserializeJson(doc, configFile);
        if(error)logSerial("deserialization",2);
        #ifdef PRINTJSON
            logSerial("printing configFile serial",0);
            serializeJsonPretty(doc, Serial);
            Serial.println();
        #endif

        if(doc.containsKey("debug")){
            conf.debug = doc["debug"];
            logSerial("reading debug", 4);
        } else {
            logSerial("reading debug",2);
        }

        if(doc.containsKey("ws2812")){
            conf.useWS2812 = doc["ws2812"];
            logSerial("reading ws2812", 4);
        } else {
            logSerial("reading ws2812",2);
        }
    
        if(doc.containsKey("bme280")){
            conf.useBME280 = doc["bme280"];
            logSerial("reading bme280", 4);
        } else {
            logSerial("reading bme280",2);
        }

        if(doc.containsKey("PCF8574")){
            conf.usePCF8574 = doc["PCF8574"];
            logSerial("reading PCF8574", 4);
        } else {
            logSerial("reading PCF8574",2);
        }

        if(doc.containsKey("enableLEDs")){
            conf.enableLEDs = doc["enableLEDs"];
            logSerial("reading enableLEDs", 4);
        } else {
            logSerial("reading enableLEDs",2);
        }


        configFile.close();
    } else {
        logSerial("Loading configFile",2);
    }

    // loading effect colors
    // File effectFile = LittleFS.open("/effects.json","r");

    // if(effectFile){
    //     logSerial("Loading effectFile",0);
    //     DeserializationError error = deserializeJson(doc, effectFile);
    //     if(error)logSerial("deserialization",2);
    //     #ifdef PRINTJSON
    //         logSerial("printing effectFile serial",0);
    //         serializeJsonPretty(doc, Serial);
    //         Serial.println();
    //     #endif

    //     effectFile.close();
    // } else {
    //     logSerial("Loading effectFile",2);
    // }    

    

}

String HiotDevice::getESPStateJson(){
    String buf;
    StaticJsonDocument<512> doc;
    doc["uptimeMillis"] = millis();
    doc["IP"] = WiFi.localIP().toString();
    doc["MAC"] = WiFi.macAddress();
    doc["rgb"] = colors.getColorStringRGB(0);
    serializeJsonPretty(doc, buf);
    // Serial.println(buf);
    return buf;
}


