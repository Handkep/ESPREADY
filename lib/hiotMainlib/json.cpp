#include <hiotMainlib.h>
#ifndef PRINTJSON
#define PRINTJSON

void HiotDevice::loadConfig(){
    logSerial(F("Loading Config"),4);
    if (LittleFS.begin()){
        logSerial(F("Loading Littlefs"), 4);
    }else{
        logSerial(F("Loading Littlefs"), 2);
    }

    File ipFile;
    ipFile = LittleFS.open("/ip.json","r");
    StaticJsonDocument<256> doc;
    
    if(ipFile){
        logSerial(F("Loading ipFile"), 4);
        DeserializationError error = deserializeJson(doc, ipFile);
        if(error)logSerial(F("deserialization"), 2);
#ifdef PRINTJSON
        logSerial(F("printing ipfile serial"), 0);
        serializeJsonPretty(doc, Serial);
        Serial.println();
#endif
        //Read ssid
        if(doc.containsKey(F("ssid"))){
            // strlcpy(conf.ssid,doc["ssid"],sizeof(conf.ssid));
            strcpy(conf.ssid,doc["ssid"]);
            // conf.ssid = *doc["ssid"].as<char*>();
            logSerial(F("reading ssid"), 4);
        } else {
            logSerial(F("reading ssid"), 2);
        }
        //read wifi password
        if(doc.containsKey(F("wifi_password"))){
            strcpy(conf.wifiPassword, doc["wifi_password"]);
            logSerial(F("reading wifiPassword"), 4);
        } else {
            logSerial(F("reading wifiPassword"), 2);
        }
        // read broker ip
        if(doc.containsKey(F("broker"))){
            strcpy(conf.mqttBrokerIp,doc["broker"]);
            logSerial(F("reading mqttBrokerIp"), 4);
        }else{
            logSerial(F("reading mqttBrokerIp"), 2);
        }
        // read broker Port
        if(doc.containsKey(F("broker_port"))){

            conf.mqttBrokerPort = doc["broker_port"];
            logSerial(F("reading mqttBrokerPort"), 4);
        } else {
            logSerial(F("reading mqttBrokerPort"), 2);
        }
        // read mqtt user
        if(doc.containsKey("mqtt_user")){

            strcpy(conf.mqttBrokerUser, doc["mqtt_user"]);
            logSerial(F("reading mqtt_user"), 4);
        } else {
            logSerial(F("reading mqtt_user"), 2);
        }       
        // read mqtt password
        if(doc.containsKey(F("mqtt_password"))){

            strcpy(conf.mqttBrokerPassword, doc["mqtt_password"]);
            logSerial(F("reading mqtt_password"), 4);
        } else {
            logSerial(F("reading mqtt_password"), 2);
        }                
        ipFile.close();
        // doc.clear();
    } else {
        logSerial(F("Loading ipFile, did you upload the filesystem image to esp8266?"), 2);
    }
    // loading hostname
    File hostnameFile = LittleFS.open(F("/hostname.json"), "r");

    if(hostnameFile){
        logSerial(F("Loading hostnameFile"), 0);
        DeserializationError error = deserializeJson(doc, hostnameFile);
        if(error)logSerial(F("deserialization"),2);
#ifdef PRINTJSON
        logSerial(F("printing hostnameFile serial"),0);
        serializeJsonPretty(doc, Serial);
        Serial.println();
        #endif

        if(doc.containsKey(F("hostname"))){
            strcpy(conf.espHostname, doc["hostname"]);
            logSerial(F("reading hostname"), 4);
        } else {
            logSerial(F("reading hostname"), 2);
        }
        hostnameFile.close();
    } else {
        logSerial(F("Loading hostnameFile, did you upload the filesystem image to esp8266?"), 2);
    }

    // loading config
    File configFile = LittleFS.open(F("/config.json"), "r");

    if(configFile){
        logSerial(F("Loading configFile"), 0);
        DeserializationError error = deserializeJson(doc, configFile);
        if(error)logSerial(F("deserialization"),2);
        #ifdef PRINTJSON
            logSerial(F("printing configFile serial"),0);
            serializeJsonPretty(doc, Serial);
            Serial.println();
        #endif

        if(doc.containsKey(F("debug"))){
            conf.debug = doc["debug"];
            logSerial(F("reading debug"), 4);
        } else {
            logSerial(F("reading debug"),2);
        }

        if(doc.containsKey(F("ws2812"))){
            conf.useWS2812 = doc["ws2812"];
            logSerial(F("reading ws2812"), 4);
        } else {
            logSerial(F("reading ws2812"), 2);
        }
    
        if(doc.containsKey(F("bme280"))){
            conf.useBME280 = doc["bme280"];
            logSerial(F("reading bme280"), 4);
        } else {
            logSerial(F("reading bme280"), 2);
        }

        if(doc.containsKey(F("PCF8574"))){
            conf.usePCF8574 = doc["PCF8574"];
            logSerial(F("reading PCF8574"), 4);
        } else {
            logSerial(F("reading PCF8574"), 2);
        }

        if(doc.containsKey(F("enableLEDs"))){
            conf.enableLEDs = doc["enableLEDs"];
            logSerial(F("reading enableLEDs"), 4);
        } else {
            logSerial(F("reading enableLEDs"), 2);
        }
        
        if(doc.containsKey(F("rgbw"))){
            conf.useRGBW = doc["rgbw"];
            logSerial(F("reading rgbw"), 4);
        } else {
            logSerial(F("reading rgbw"), 2);
        }


        configFile.close();
    } else {
        logSerial(F("Loading configFile, did you upload the filesystem image to esp8266?"), 2);
        logSerialPretty(F("NOW SCANNING FOR NETWORKS"));
        while(true){
            yield();
            digitalWrite(ONBORDLED, LOW);
            delay(1000);
            digitalWrite(ONBORDLED, HIGH);
            delay(1000);
            uint8_t networks = WiFi.scanNetworks();//Scan for total networks available
            
            Serial.write(27);       // ESC command
            Serial.print(F("[2J")); // clear screen command
            Serial.write(27);
            Serial.print(F("[H"));
            Serial.println(F("RSSI\tCHANNEL\tSSID"));
            for (int i = 0; i < networks; ++i){
                
                // logSerialPretty(String(WiFi.SSID(i)),String(WiFi.RSSI(i))+" dB",String(WiFi.encryptionType(i) == ENC_TYPE_NONE));
                // Serial.println(String(WiFi.RSSI(i))+ "\t"+ String(WiFi.channel(i)) +"\t"+ String(WiFi.SSID(i))); 
            }
        }
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
    StaticJsonDocument<128> doc;
    doc["uptimeMillis"] = millis();
    doc["IP"] = WiFi.localIP().toString();
    doc["MAC"] = WiFi.macAddress();
    doc["rssi"] = WiFi.RSSI();
    doc["version"]=VERSION;
    doc["color"]["r"] = colors.RGBW_write[0][0];
    doc["color"]["g"] = colors.RGBW_write[0][1];
    doc["color"]["b"] = colors.RGBW_write[0][2];
    serializeJson(doc, buf);
    // Serial.println(buf);
    return buf;
}
String HiotDevice::getESPColorJson(){
    String buf;
    StaticJsonDocument<128> doc;
    doc["r"] = colors.RGBW_write[0][0];
    doc["g"] = colors.RGBW_write[0][1];
    doc["b"] = colors.RGBW_write[0][2];
    serializeJson(doc, buf);
    // Serial.println(buf);
    return buf;
}


#endif