#include <hiotMainlib.h>




WiFiClient espClient;
PubSubClient esp(espClient);
Config conf;
PCF8574 PCF(0x20);
WiFiUDP Udp;
char packet[30];

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

unsigned long _interval_publishBMETemp = 30000;


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

    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        // Serial.print("Received packet! Size: ");
        // Serial.println(packetSize);
        int len = Udp.read(packet, 255);
        if (len > 0)
        {
            packet[len] = '\0';
        }
        // Serial.print("Packet received: ");
        // Serial.println(packet);
        String buf = packet;
        // buf.trim(" ");
        // Serial.println(buf);
        // int fi =buf.indexOf(",");
        // int si =buf.indexOf(",",fi+1);
        // int ti =buf.indexOf(",",si+1);
        // Serial.println(fi);
        // Serial.println(si);
        // Serial.println(ti);
        // char r[3] = "";
        // for(size_t i=0;i< packetSize;i++)
        // {
        //     if(packet[i]=='['){
        //         // Serial.println(packet[i]);
        //         continue;
        //     }
        //     else if (packet[i] == ')')
        //     {
        //         break;
        //     }
        //     if(i<fi)
        //     {
        //         r[i]=packet[i];
        //         Serial.println("Asdf");
        //     }
        // }
        // Serial.println(r);
        StaticJsonDocument<80> doc;
        deserializeJson(doc,packet);
        // serializeJsonPretty(doc, Serial);+
        uint8_t r = doc["r"];
        uint8_t g = doc["g"];
        uint8_t b = doc["b"];
        uint8_t i = doc["i"];
        // colors.RGBW_write[i][0] = r;
        // colors.RGBW_write[i][1] = g;
        // colors.RGBW_write[i][2] = b;
        // Serial.println(doc["r"]);
        // Serial.printf("r%d g%d b%d i%d\n\n",r,g,b,i);
    }
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
    logSerial(F("START"), 0);
    logSerial(F("TEST"), 1);
    logSerial(F("TEST"), 2);
    logSerial(F("TEST"), 3);
    // delay(1000);
    // Serial.write(27);       // ESC command
    // Serial.print("[2J");    // clear screen command
    // Serial.write(27);
    // Serial.print("[H");
    Serial.println(F("HH   HH   AAA   NN   NN DDDDD   KK  KK EEEEEEE    IIIII  OOOOO  TTTTTTT \nHH   HH  AAAAA  NNN  NN DD  DD  KK KK  EE          III  OO   OO   TTT \nHHHHHHH AA   AA NN N NN DD   DD KKKK   EEEEE       III  OO   OO   TTT \nHH   HH AAAAAAA NN  NNN DD   DD KK KK  EE          III  OO   OO   TTT \nHH   HH AA   AA NN   NN DDDDDD  KK  KK EEEEEEE    IIIII  OOOO0    TTT "));
    colors.setup();
    logSerial(F("colors.setup"), 3);
    loadConfig();
    logSerial(F("config"), 0);
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
    logSerial(F("topics"), 0);

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
        logSerial(F("Enabling PCF"), 3);
        // for(int i = 0; i<8;i++){
        //     PCF.write(i,LOW);
        //     delay(100);
        //     PCF.write(i,HIGH);
        //     delay(100);
        // }
        logSerial(F("Tested Relays"), 0);
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
            // analogWrite(ONBORDLED,0);
            delay(lengthon);
            digitalWrite(ONBORDLED, HIGH);
            // analogWrite(ONBORDLED,2);
            delay(lengthoff);
        }
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
    for(int i = 0; i<LEDAMMOUNT;i++){
        colors.RGBW[i][0] = 255;
        colors.RGBW[i][1] = 255;
        colors.RGBW[i][2] = 255;

        colors.RGBW_write[i][0] = 255;
        colors.RGBW_write[i][1] = 255;
        colors.RGBW_write[i][2] = 255;
        RgbColor red(colors.RGBW_write[i][0], colors.RGBW_write[i][1], colors.RGBW_write[i][2]);
        // strip.SetPixelColor(j,red);
        colors.leds[i].setRGB(colors.RGBW_write[i][0], colors.RGBW_write[i][1], colors.RGBW_write[i][2]);
        Serial.println(colors.leds[i]);
    }
    // strip.Show();
    FastLED.show();

    colors.RGBW_Pin[0] = R;
    colors.RGBW_Pin[1] = G;
    colors.RGBW_Pin[2] = B;

}

void HiotDevice::setLEDPins(int R, int G, int B, int W){

    colors.pinAmount = 4;    
    colors.ledAmmount=1;
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
    // colors.ledAmmount=1;
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
    // const char* buffer = conf.espHostname + topic.c_str()
    // char* buffer;
    // strncpy(buffer, topic.c_str(), sizeof(topic.c_str()));
    logSerial(buf,0);
    return buf;
}

// funcconnectToWifi
void HiotDevice::connectToWifi(){
    if(WiFi.status() != WL_CONNECTED){
        // if(conf.ssid == "" || conf.wifiPassword == ""){
        //     logSerial("NO WIFI FOUND IN CONFIG!", ERROR);
        //     return;
        // }
        _millis_connectToWifi = millis();
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
            if(millis() - _millis_connectToWifi >= WIFI_TIMEOUT){
                // Serial.print("\r");
                Serial.print(F("\r                                                    \r"));
                // Serial.print("\r");
                logSerial(F("no Wifi found, try again in 20s\r"), ERROR, false);
                delay(20000);
                Serial.print(F("\r                                                    \r"));
                _millis_connectToWifi = millis();
            }
        }
        // Serial.println();
        Serial.print(F("\r                                                    \r"));
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
        Udp.begin(UDP_PORT);
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
        if(error)logSerial(F("deserialization"),2);
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
                    logSerial(F("OFF to ON"), 0);
                    esp.publish(topic_state_json.c_str(),"{\"state\": \"ON\"}");
                }
            }    
        } 
        if(doc.containsKey(F("color"))) {
            StaticJsonDocument<512> colordoc;
            String jsonbuf = doc["color"];
            DeserializationError error = deserializeJson(colordoc, jsonbuf);
            if(error)
                logSerial(F("deserialization"), 2);
            if(colordoc.containsKey("r") && colordoc.containsKey("g") && colordoc.containsKey("b")){
                // String colorBuffer = String(colordoc["r"]) + "," + String(colordoc["g"]) + "," + String(colordoc["b"]); 36296 36312

                char colorBuffer[15];
                sprintf(colorBuffer, "%d,%d,%d", colordoc["r"].as<uint8_t>(), colordoc["g"].as<uint8_t>(), colordoc["b"].as<uint8_t>());

                logSerial(colorBuffer, 2);
                colors.setColorString(String(colorBuffer));
                // colors.adjustBrightnessColor();
                // Serial.println(colors.currentBrightness);
                String colorJsonbuff = F("{\"state\": \"ON\", \"color_mode\": \"rgb\", \"effect\": \"none\", \"color\":") + String(doc["color"]) + "}";

                esp.publish(topic_state_json.c_str(),colorJsonbuff.c_str());
            }
        }
        if (doc.containsKey(F("brightness")))
        {
            Serial.print("Setting Brightness: ");
            Serial.println(colors.currentBrightness);
            colors.currentBrightness = doc["brightness"].as<uint8_t>();
            char brightnessBuffer[40];
            sprintf(brightnessBuffer, "{\"state\": \"ON\",  \"brightness\":%d}", colors.currentBrightness);
            colors.adjustBrightnessColor();
            esp.publish(topic_state_json.c_str(), brightnessBuffer);
            logSerial(String(brightnessBuffer), 2);
        }
        if(doc.containsKey(F("fadespeed"))){
            colors.fadespeed = doc["fadespeed"];
            esp.publish(topic_fadespeed.c_str(),String(colors.fadespeed).c_str());
            colors.fadespeed = pow(colors.fadespeed,3)/50;
            Serial.println(colors.fadespeed);
        }
        if(doc.containsKey(F("effect"))){
            if(doc[F("effect")] == F("JUMP") || doc[F("effect")] == F("1")) colors.currentEffect = 1;
            if(doc[F("effect")] == F("FADE") || doc[F("effect")] == F("2")) colors.currentEffect = 2;
            if(doc[F("effect")] == F("STROBE") || doc[F("effect")] == F("3")) colors.currentEffect = 3;
            if(doc[F("effect")] == F("RAINBOW") || doc[F("effect")] == F("4")) colors.currentEffect = 4;
            if(doc[F("effect")] == F("EFFECTTEST") || doc[F("effect")] == F("5")) colors.currentEffect = 5;

            String colorJsonbuff = "{\"state\": \"ON\", \"effect\":\"" + String(doc[F("effect")]) + "}";
            esp.publish(topic_state_json.c_str(), colorJsonbuff.c_str());
        }
        if (doc.containsKey(F("pixels")))
        {
            // Serial.println(String(doc));
        }

        // |power|
    }else if(recvTopic == topic_power){

    // ESPhttpUpdate.update(espClient, "192.168.2.109", 5000, "/esp");

    t_httpUpdate_return ret = ESPhttpUpdate.update(espClient, "192.168.2.115", 5000, "/esp", "v1.0.1");
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            logSerial(F("Update failed"), ERROR);
            break;
        case HTTP_UPDATE_NO_UPDATES:
            logSerial(F("no update"), INFO);
            break;
        case HTTP_UPDATE_OK:
            Serial.println(F("[update] Update ok.")); // may not be called since we reboot the ESP
            break;
    }

        if(colors.isColorOnOrOff){
            if(recvPayload == "OFF"){
                backupColor = colors.getColorStringRGB(0);
                backupEffect = colors.currentEffect;
                colors.isColorOnOrOff = 0;
                colors.setColorString("0,0,0");
                esp.publish(topic_power_state.c_str(),recvPayload.c_str());
                logSerial(F("ON to OFF"), 0);
            }   
        }else{
            if(recvPayload == "ON"){
                colors.isColorOnOrOff = 1;
                colors.setColorString(backupColor);
                colors.currentEffect = backupEffect;
                esp.publish(topic_power_state.c_str(),recvPayload.c_str());
                logSerial(F("OFF to ON"), 0);
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
        logSerial(F("TRIGERED RELAYYY YAYY"), 0);
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
    colors._StrobeIndex = 0;
}

// funcconnectToMQTT
void HiotDevice::connectToMQTT(){
    if(!esp.connected()){
        using std::placeholders::_1;
        using std::placeholders::_2;
        using std::placeholders::_3;
        esp.setServer(conf.mqttBrokerIp,conf.mqttBrokerPort);
        esp.setCallback(std::bind( &HiotDevice::mqttCallback, this, _1,_2,_3));

        logSerial(String(F("(Re)Connectig to MQTT at ")) + String(conf.mqttBrokerIp) + "  " + String(esp.state()), 3);
        // logSerial(String(esp.state()),2);
        // if(esp.connect(conf.espHostname, topic_LWT.c_str(),2,true,"offline")){
        if(esp.connect(conf.espHostname, conf.mqttBrokerUser , conf.mqttBrokerPassword ,topic_LWT.c_str(),2,true,"offline")){

            logSerial(F("Connected to MQTT"), 4);
            alertBlink(10,100,4);
            esp.publish(topic_LWT.c_str(),"online",true);
            if(conf.enableLEDs){
            
                // esp.subscribe(topic_color.c_str());
                esp.subscribe(topic_cmd_json.c_str());
                // esp.subscribe(topic_mode.c_str());
                // esp.subscribe(topic_power.c_str());
                esp.subscribe(topic_interrupt.c_str());
                esp.publish(topic_fadespeed.c_str(),"45");
                String colorJsonbuff = "{\"state\": \"ON\", \"color_mode\": \"rgb\", \" brightness \": 255, \"color\":" + getESPColorJson() + "}";
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
                if(colors.currentEffect == 4) effectbuff = "RAINBOW";
                if(colors.currentEffect == 5) effectbuff = "EFFECTTEST";
                // String colorJsonbuff = "{\"state\": \"ON\", \"effect\":\"" + effectbuff + "\", \"color_mode\": \"rgb\", \"color\":" + getESPColorJson() + "}";
                String colorJsonbuff = "{\"state\": \"ON\", \"effect\":\"" + effectbuff + "\", \"color\":" + getESPColorJson() + "}";
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
            logSerial(F("Could not find a valid BME280 sensor, check wiring!"), 2);
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