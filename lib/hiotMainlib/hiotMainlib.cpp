#include <hiotMainlib.h>




WiFiClient espClient;
PubSubClient esp(espClient);
Config conf;
PCF8574 PCF(0x20);



String topic_debug = "/debug";
// String topic_color = "/cmd/color";
String topic_cmd_json = "/cmd/json";
// String topic_animationspeed = "/cmd/speed" ;
String topic_fadespeed = "/state/fadespeed";
// String topic_mode = "/cmd/mode";
String topic_power = "/cmd/power";
String topic_irrigation_Zone1 = "/cmd/irrigation/zone1";
String topic_irrigation_Zone2 = "/cmd/irrigation/zone2";
String topic_irrigation_Zone3 = "/cmd/irrigation/zone3";
String topic_irrigation_Zone4 = "/cmd/irrigation/zone4";
String topic_irrigation_Zone5 = "/cmd/irrigation/zone5";
String topic_irrigation_Zone6 = "/cmd/irrigation/zone6";
String topic_irrigation_Zone7 = "/cmd/irrigation/zone7";
String topic_irrigation_Zone8 = "/cmd/irrigation/zone8";
// String topic_reset = "/reset";
String topic_power_state = "/state/power";
String topic_state_json = "/state/json";
String topic_effect_state = "/state/mode";
String topic_color_state = "/state/color";
String topic_relais_state = "/state/relais";
String topic_bme_temperature = "/bme/temp"; 
String topic_bme_humidity = "/bme/hum";
String topic_bme_pressure = "/bme/pres"; 
String topic_bme_altitude = "/bme/alt";
String topic_interrupt = "/cmd/interrupt";
String topic_pull_update = "/pull_update";
String topic_LWT = "/LWT";
String topic_STATE = "/STATE";

unsigned long _interval_publishBMETemp = 15000;


HiotDevice::HiotDevice(){

}

// funcloop
void HiotDevice::loop(){
    esp.loop();
    ArduinoOTA.handle();
    connectToWifi();
    connectToMQTT();
    publishESPStateJsonRepetitive();
    if(conf.enableLEDs)colors.loop();
    if(conf.useBME280)publishBMETemp();
    // checkforPins();
    // delay(100);
}

// funcsetup
void HiotDevice::setup(){
    Serial.begin(115200);
    Serial.flush();
    pinMode(ONBORDLED, OUTPUT);
    colors.isColorOnOrOff = 1;
    // colors.setColorString("255,255,255");
    backupColor = "255,255,255";
    alertBlink(5, 30,5);
    logSerial("START",0);
    logSerial("TEST",1);
    logSerial("TEST",2);
    logSerial("TEST",3);
    Serial.println("HH   HH   AAA   NN   NN DDDDD   KK  KK EEEEEEE    IIIII  OOOOO  TTTTTTT \nHH   HH  AAAAA  NNN  NN DD  DD  KK KK  EE          III  OO   OO   TTT \nHHHHHHH AA   AA NN N NN DD   DD KKKK   EEEEE       III  OO   OO   TTT \nHH   HH AAAAAAA NN  NNN DD   DD KK KK  EE          III  OO   OO   TTT \nHH   HH AA   AA NN   NN DDDDDD  KK  KK EEEEEEE    IIIII  OOOO0    TTT ");
    delay(1000);
    colors.setup();
    loadConfig();
    // topic_color = insertHostnameintoVariable(topic_color);
    topic_cmd_json = insertHostnameintoVariable(topic_cmd_json);
    // topic_mode = insertHostnameintoVariable(topic_mode);
    // topic_effect_state = insertHostnameintoVariable(topic_effect_state);
    // topic_power = insertHostnameintoVariable(topic_power);
    topic_LWT = insertHostnameintoVariable(topic_LWT);
    // topic_color_state = insertHostnameintoVariable(topic_color_state);
    topic_state_json = insertHostnameintoVariable(topic_state_json);
    // topic_power_state = insertHostnameintoVariable(topic_power_state);
    topic_STATE = insertHostnameintoVariable(topic_STATE);
    topic_interrupt = insertHostnameintoVariable(topic_interrupt);
    topic_fadespeed = insertHostnameintoVariable(topic_fadespeed);

    if(conf.useBME280){
        topic_bme_temperature = insertHostnameintoVariable(topic_bme_temperature);
        topic_bme_humidity = insertHostnameintoVariable(topic_bme_humidity);
        topic_bme_pressure = insertHostnameintoVariable(topic_bme_pressure);
        topic_bme_altitude = insertHostnameintoVariable(topic_bme_altitude);
    }
    
    if(conf.usePCF8574){

        topic_irrigation_Zone1 = insertHostnameintoVariable(topic_irrigation_Zone1);
        topic_irrigation_Zone2 = insertHostnameintoVariable(topic_irrigation_Zone2);
        topic_irrigation_Zone3 = insertHostnameintoVariable(topic_irrigation_Zone3);
        topic_irrigation_Zone4 = insertHostnameintoVariable(topic_irrigation_Zone4);
        topic_irrigation_Zone5 = insertHostnameintoVariable(topic_irrigation_Zone5);
        topic_irrigation_Zone6 = insertHostnameintoVariable(topic_irrigation_Zone6);
        topic_irrigation_Zone7 = insertHostnameintoVariable(topic_irrigation_Zone7);
        topic_irrigation_Zone8 = insertHostnameintoVariable(topic_irrigation_Zone8);
        
        PCF.begin();
        logSerial("Enabling PCF",3);
        // for(int i = 0; i<8;i++){
        //     PCF.write(i,LOW);
        //     delay(100);
        //     PCF.write(i,HIGH);
        //     delay(100);
        // }
        logSerial("Tested Relays",0);
    }
    connectToWifi();
    connectToMQTT();
    esp.publish(topic_STATE.c_str(),getESPStateJson().c_str());

    ArduinoOTA.begin(WiFi.localIP());
    ArduinoOTA.setHostname(conf.espHostname);
    // pinMode(16,INPUT);
    // attachInterrupt(digitalPinToInterrupt(16), checkforPins, CHANGE);
}

// funcalertBlink
void HiotDevice::alertBlink(int lengthon, int lengthoff, int count){
        for (int i = 0; i < count; i++)
        {
            digitalWrite(ONBORDLED, LOW);
            delay(lengthon);
            digitalWrite(ONBORDLED, HIGH);
            delay(lengthoff);
        }
}

// funclogSerialPretty
void HiotDevice::logSerialPretty(String inf){
    Serial.println("====================");
    Serial.println(inf);
    Serial.println("====================");
}

// funclogSerialPretty
void HiotDevice::logSerialPretty(String inf, String inf1){
    Serial.println("====================");
    Serial.println(inf);
    Serial.println("--------------------");
    Serial.println(inf1);
    Serial.println("====================");
}

// funclogSerialPretty
void HiotDevice::logSerialPretty(String inf, String inf1, String inf2){
    Serial.println("====================");
    Serial.println(inf);
    Serial.println("--------------------");
    Serial.println(inf1);
    Serial.println("--------------------");
    Serial.println(inf2);
    Serial.println("====================");
}

// funclogSerial
// level 0 = debug
// level 1 = warn
// level 2 = error
// level 3 = info
void HiotDevice::logSerial(String inf, int level){
    switch(level){
        case 0:
            Serial.print(ANSI_GREEN);
            Serial.print("[DEBUG]");
            Serial.print(ANSI_RST);
            Serial.print("\t");
            Serial.println(inf);
            break;
        case 1:
            Serial.print(ANSI_YELLOW);
            Serial.print("[WARN]");
            Serial.print(ANSI_RST);
            Serial.print("\t");
            Serial.println(inf);
            break;
        case 2:
            Serial.print(ANSI_RED);
            Serial.print("[ERROR]"); //
            Serial.print(ANSI_RST);
            Serial.print("\t");
            Serial.println(inf);
            break;
        case 3:
            Serial.print(ANSI_CYAN);
            Serial.print("[INFO]"); //
            Serial.print(ANSI_RST);
            Serial.print("\t");
            Serial.println(inf);
            break;
        case 4:
            Serial.print(ANSI_GREEN);
            Serial.print("[DONE]"); //
            Serial.print(ANSI_RST);
            Serial.print("\t");
            Serial.println(inf);
            break;
    }
}

// funclogSerial
void HiotDevice::logSerial(String name, String color, String inf){

    Serial.print(color);
    Serial.print("[");
    Serial.print(name);
    Serial.print("]");
    Serial.print(ANSI_RST);
    Serial.print("\t");
    Serial.println(inf);    
}

// §setLEDPins
// set
void HiotDevice::setLEDPins(int R, int G, int B){

    colors.pinAmount = 3;    
	pinMode(R,OUTPUT);
	pinMode(G,OUTPUT);
	pinMode(B,OUTPUT);
    analogWrite(R,255);
    analogWrite(G,255);
    analogWrite(B,255);
    colors.RGBW[0][0] = 255;
    colors.RGBW[0][1] = 255;
    colors.RGBW[0][2] = 255;
    colors.RGBW_write[0][0] = 255;
    colors.RGBW_write[0][1] = 255;
    colors.RGBW_write[0][2] = 255;
    colors.RGBW_Pin[0] = R;
    colors.RGBW_Pin[1] = G;
    colors.RGBW_Pin[2] = B;

}

void HiotDevice::setLEDPins(int R, int G, int B, int W){

    colors.pinAmount = 4;    
	pinMode(R,OUTPUT);
	pinMode(G,OUTPUT);
	pinMode(B,OUTPUT);
	pinMode(W,OUTPUT);
    analogWrite(W,255);
    colors.RGBW[0][3] = 255;
    colors.RGBW_write[0][3] = 255;
    colors.RGBW_Pin[0] = R;
    colors.RGBW_Pin[1] = G;
    colors.RGBW_Pin[2] = B;
    colors.RGBW_Pin[3] = W;

}

// funcsetLEDPins
void HiotDevice::setLEDPinsRGBW(int R, int G, int B, int W){

    colors.pinAmount = 4;    
	pinMode(R,OUTPUT);
	pinMode(G,OUTPUT);
	pinMode(B,OUTPUT);
	pinMode(W,OUTPUT);
    analogWrite(W,255);
    colors.RGBW[0][3] = 255;
    colors.RGBW_write[0][3] = 255;
    colors.RGBW_Pin[0] = R;
    colors.RGBW_Pin[1] = G;
    colors.RGBW_Pin[2] = B;
    colors.RGBW_Pin[3] = W;

}

// funcinsertHostnameintoVariable
String HiotDevice::insertHostnameintoVariable(String topic)
{
    String buf(String(conf.espHostname) + topic);
    logSerial(buf,0);
    return buf;
}

// funcconnectToWifi
void HiotDevice::connectToWifi(){
    if(WiFi.status() != WL_CONNECTED){
        delay(100);
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        WiFi.hostname(conf.espHostname);
        WiFi.begin(conf.ssid, conf.wifiPassword);
        logSerial(String("(Re)Connectig to Wifi at ")+String(conf.ssid),3);
        while (WiFi.status() != WL_CONNECTED)
        {                                            //loop while Connecting to WiFi
            Serial.print(".");
            alertBlink(5, 500,1);
        }
        // Serial.println();
        logSerial(String("Connected to Wifi at ")+String(conf.ssid),4);
        logSerialPretty("Wifi-Info",String("IP: ") + String(WiFi.localIP().toString()) + String("\n") + 
                        String("Hostname: ") + WiFi.hostname() + String("\n") + 
                        String("MAC: ") + WiFi.macAddress());
        alertBlink(5, 80,3);

        // hostname is not working
        // MDNS.setHostname(conf.espHostname);
        if (MDNS.begin(conf.espHostname)) {  //Start mDNS (Bonjourname)
            // MDNS.addService("http","tcp", 80);
            logSerial("Started MDNS: "+String(WiFi.getHostname()) ,4);
        }else{
            logSerial("MDNS", ERROR);
        }
    }
}

// funcmqttCallback
void HiotDevice::mqttCallback(char* topic, byte* payload, int length){

    String recvPayload;
    String recvTopic(topic);
    String buf = "Message Arrived in Topic: " + String(ANSI_CYAN) + recvTopic + String(ANSI_RST) + String(" Payload: \"") + String(ANSI_CYAN);
    
    for (int i = 0; i < length; i++) {
        recvPayload += (char)payload[i];
    }
    logSerial("MQTT", ANSI_CYAN,buf + recvPayload + String(ANSI_RST));

    // for json mqtt
    if(recvTopic == topic_cmd_json){
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, recvPayload);
        if(error)logSerial("deserialization",2);
        // logSerial("printing json payload serial",0);
        // serializeJsonPretty(doc, Serial);
        // Serial.println();
        //Read sent power state, logic is the same as |power|
        if(doc.containsKey("state")){
            String jsonbuf = doc["state"];
            if(colors.isColorOnOrOff){
                if(jsonbuf == "OFF"){
                    backupColor = colors.getColorStringRGBW(0);
                    backupEffect = colors.currentEffect;
                    colors.isColorOnOrOff = 0;
                    colors.setColorString("0,0,0");
                    // esp.publish(topic_power_state.c_str(),recvPayload.c_str());
                    logSerial("ON to OFF",0);
                    esp.publish(topic_state_json.c_str(),"{\"state\": \"OFF\"}");
                }
            }else{
                if(jsonbuf == "ON"){
                    colors.isColorOnOrOff = 1;
                    colors.setColorString(backupColor);
                    colors.currentEffect = backupEffect;
                    // esp.publish(topic_power_state.c_str(),recvPayload.c_str());
                    logSerial(String(colors.currentEffect),0);
                    logSerial(String(backupEffect),0);
                    logSerial("OFF to ON",0);
                    esp.publish(topic_state_json.c_str(),"{\"state\": \"ON\"}");
                }
            }    
        } 
        if(doc.containsKey("color")) {
            StaticJsonDocument<512> colordoc;
            String jsonbuf = doc["color"];
            DeserializationError error = deserializeJson(colordoc, jsonbuf);
            if(error)logSerial("deserialization",2);
            if(colordoc.containsKey("r") && colordoc.containsKey("g") && colordoc.containsKey("b")){
                String colorbuf = String(colordoc["r"]) + "," + String(colordoc["g"]) + "," + String(colordoc["b"]);
                // String colorbuf = colordoc["r"] + "," + colordoc["g"] + "," + colordoc["b"];
                colors.setColorString(colorbuf);
                String colorJsonbuff = "{\"state\": \"ON\", \"color_mode\": \"rgb\", \"effect\": \"none\", \"color\":" + String(doc["color"]) + "}";
                esp.publish(topic_state_json.c_str(),colorJsonbuff.c_str());
            }
        }
        if(doc.containsKey("fadespeed")){
            colors.fadespeed = doc["fadespeed"];
            esp.publish(topic_fadespeed.c_str(),String(colors.fadespeed).c_str());
            colors.fadespeed = pow(colors.fadespeed,3)/50;
        }
        if(doc.containsKey("effect")){
            if(doc["effect"] == "JUMP" || doc["effect"] == "1") colors.currentEffect = 1;
            if(doc["effect"] == "FADE" || doc["effect"] == "2") colors.currentEffect = 2;
            if(doc["effect"] == "STROBE" || doc["effect"] == "3") colors.currentEffect = 3;
        }
    // not in use \/
    // }else if(recvTopic == topic_color){
    //     colors.setColorString(recvPayload);
    //     esp.publish(topic_color_state.c_str(),recvPayload.c_str());
    //     esp.publish(topic_effect_state.c_str(),"COLOR");

    // }else if(recvTopic == topic_mode){
    //     if(recvPayload == "JUMP") colors.currentEffect = 1;
    //     if(recvPayload == "FADE") colors.currentEffect = 2;
    //     if(recvPayload == "STROBE") colors.currentEffect = 3;
    //     esp.publish(topic_effect_state.c_str(),recvPayload.c_str());

        // |power|
    }else if(recvTopic == topic_power){



    // ESPhttpUpdate.update(espClient, "192.168.2.109", 5000, "/esp");


    t_httpUpdate_return ret = ESPhttpUpdate.update(espClient, "192.168.2.115", 5000, "/esp", "v1.0.1");
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            logSerial("Update failed", ERROR);
            break;
        case HTTP_UPDATE_NO_UPDATES:
            logSerial("no update", INFO);
            break;
        case HTTP_UPDATE_OK:
            Serial.println("[update] Update ok."); // may not be called since we reboot the ESP
            break;
    }



        if(colors.isColorOnOrOff){
            if(recvPayload == "OFF"){
                backupColor = colors.getColorStringRGB(0);
                backupEffect = colors.currentEffect;
                colors.isColorOnOrOff = 0;
                colors.setColorString("0,0,0");
                esp.publish(topic_power_state.c_str(),recvPayload.c_str());
                logSerial("ON to OFF",0);
            }   
        }else{
            if(recvPayload == "ON"){
                colors.isColorOnOrOff = 1;
                colors.setColorString(backupColor);
                colors.currentEffect = backupEffect;
                esp.publish(topic_power_state.c_str(),recvPayload.c_str());
                logSerial("OFF to ON",0);
            }
        }
    }else if(recvTopic == topic_interrupt){
            if(recvPayload == "0,0,0"){
                colors.setColorString(interruptColor);
                colors.currentEffect = interruptEffect.toInt();
                esp.publish(topic_power_state.c_str(),recvPayload.c_str());
            }     
            if(recvPayload != "0,0,0"){
                interruptColor = colors.getColorStringRGB(0);
                interruptEffect = colors.currentEffect;
                colors.setColorString(recvPayload);
                esp.publish(topic_power_state.c_str(),recvPayload.c_str());
            }   
    }else if(recvTopic.substring(0, recvTopic.length()-1) == topic_irrigation_Zone1.substring(0, topic_irrigation_Zone1.length()-1) && conf.usePCF8574){
        logSerial("TRIGERED RELAYYY YAYY",0);
        if(recvTopic == topic_irrigation_Zone1){
            if(recvPayload == "ON"){
                PCF.write(0,LOW);
            }else{
                PCF.write(0,HIGH);
            }
        }else if(recvTopic == topic_irrigation_Zone2){
            if(recvPayload == "ON"){
                PCF.write(1,LOW);
            }else{
                PCF.write(1,HIGH);
            }
        }else if(recvTopic == topic_irrigation_Zone3){
            if(recvPayload == "ON"){
                PCF.write(2,LOW);
            }else{
                PCF.write(2,HIGH);
            }
        }else if(recvTopic == topic_irrigation_Zone4){
            if(recvPayload == "ON"){
                PCF.write(3,LOW);
            }else{
                PCF.write(3,HIGH);
            }
        }else if(recvTopic == topic_irrigation_Zone5){
            if(recvPayload == "ON"){
                PCF.write(4,LOW);
            }else{
                PCF.write(4,HIGH);
            }
        }else if(recvTopic == topic_irrigation_Zone6){
            if(recvPayload == "ON"){
                PCF.write(5,LOW);
            }else{
                PCF.write(5,HIGH);
            }
        }else if(recvTopic == topic_irrigation_Zone7){
            if(recvPayload == "ON"){
                PCF.write(6,LOW);
            }else{
                PCF.write(6,HIGH);
            }
        }else if(recvTopic == topic_irrigation_Zone8){
            if(recvPayload == "ON"){
                PCF.write(7,LOW);
            }else{
                PCF.write(7,HIGH);
            }
        }
    }
}

// funcconnectToMQTT
void HiotDevice::connectToMQTT(){
    if(!esp.connected()){
        using std::placeholders::_1;
        using std::placeholders::_2;
        using std::placeholders::_3;
        esp.setServer(conf.mqttBrokerIp,conf.mqttBrokerPort);
        esp.setCallback(std::bind( &HiotDevice::mqttCallback, this, _1,_2,_3));

        logSerial(String("(Re)Connectig to MQTT at ")+String(conf.mqttBrokerIp)+ "  "+String(esp.state()),3);
        // logSerial(String(esp.state()),2);
        // if(esp.connect(conf.espHostname, topic_LWT.c_str(),2,true,"offline")){
        if(esp.connect(conf.espHostname, conf.mqttBrokerUser , conf.mqttBrokerPassword ,topic_LWT.c_str(),2,true,"offline")){
            
            logSerial("Connected to MQTT",4);
            alertBlink(10,100,4);
            esp.publish(topic_LWT.c_str(),"online",true);
            if(conf.enableLEDs){
            
                // esp.subscribe(topic_color.c_str());
                esp.subscribe(topic_cmd_json.c_str());
                // esp.subscribe(topic_mode.c_str());
                // esp.subscribe(topic_power.c_str());
                esp.subscribe(topic_interrupt.c_str());
                esp.publish(topic_fadespeed.c_str(),"45");
                String colorJsonbuff = "{\"state\": \"ON\", \"color_mode\": \"rgb\", \"color\":" + getESPColorJson() + "}";
                // String colorJsonbuff = "{\"state\": \"ON\", \"color_mode\": \"rgb\", \"brightness\": 255, \"color\":{ \"r\": 255, \"g\": 255, \"b\":255}";
                esp.publish(topic_state_json.c_str(),colorJsonbuff.c_str());
                esp.publish(topic_power_state.c_str(),"ON",true);
                esp.publish(topic_color_state.c_str(),"255,255,255");
            }
            if(conf.usePCF8574){
                logSerial(topic_irrigation_Zone1, DEBUG);
                esp.subscribe(topic_irrigation_Zone1.c_str());
                esp.subscribe(topic_irrigation_Zone2.c_str());
                esp.subscribe(topic_irrigation_Zone3.c_str());
                esp.subscribe(topic_irrigation_Zone4.c_str());
                esp.subscribe(topic_irrigation_Zone5.c_str());
                esp.subscribe(topic_irrigation_Zone6.c_str());
                esp.subscribe(topic_irrigation_Zone7.c_str());
                esp.subscribe(topic_irrigation_Zone8.c_str());

            }

        }
        // else{
            
        //     logSerial("Connecting to MQTT",4);
        //     delay(500);
        // }
    }
}



// funcpublishESPStateJsonRepetitive
void HiotDevice::publishESPStateJsonRepetitive(){
    if(micros() - _millis_publishESPStateJsonRepetitive >= 30*1000000){
        _millis_publishESPStateJsonRepetitive = micros();
        esp.publish(topic_STATE.c_str(),getESPStateJson().c_str());
    }
    if(conf.enableLEDs){
        if(colors.currentEffect > 0){
            if(micros() - _millis_publishESPColor >= 0.5*1000000){
                _millis_publishESPColor = micros();
                String effectbuff = "";
                if(colors.currentEffect == 1) effectbuff = "JUMP";
                if(colors.currentEffect == 2) effectbuff = "FADE";
                if(colors.currentEffect == 3) effectbuff = "STROBE";
                String colorJsonbuff = "{\"state\": \"ON\", \"effect\":\"" + effectbuff + "\", \"color_mode\": \"rgb\", \"color\":" + getESPColorJson() + "}";
                esp.publish(topic_state_json.c_str(),colorJsonbuff.c_str());
            }
        }
    }
}


void HiotDevice::publishBMETemp(){
    if(millis() - _millis_publishBMETemp >=  _interval_publishBMETemp)
    {
        _millis_publishBMETemp = millis();
        if (! sensor.begin(0x76, &Wire)) {
            logSerial("Could not find a valid BME280 sensor, check wiring!",2);
        }else{
            char buffer[15];
            String buf = String(sensor.readTemperature()) + " ; " + String(sensor.readHumidity());
            logSerial(buf,0);
            esp.publish(topic_bme_temperature.c_str(), dtostrf(sensor.readTemperature(),8,2,buffer));
            esp.publish(topic_bme_humidity.c_str(), dtostrf(sensor.readHumidity(),8,2,buffer));
            esp.publish(topic_bme_pressure.c_str(), dtostrf(sensor.readPressure(),8,2,buffer));
            esp.publish(topic_bme_altitude.c_str(), dtostrf(sensor.readAltitude(1013.25),8,2,buffer));
        }
    }
}

void checkforPins(){
    // if(enableReadPin1 && digitalRead(16) == HIGH){
        // PCF.write(0,HIGH);
    //     delay(100);
        // pin1 ^= 1;
    //     enableWritePin1 = 1;
    //     enableReadPin1 = 0;
        // Serial.println("ON");
    // }else{
    //     enableReadPin1 = 1;
    //     Serial.println("OFF");
    // }
    // if(pin1 && enableWritePin1){
    //     PCF.write(0,pin1);
    //     enableWritePin1 = 0;
    // }
}