#include <logging.h>

void logSerialPretty(String inf){
    Serial.println("====================");
    Serial.println(inf);
    Serial.println("====================");
}

void logSerialPretty(String inf, String inf1){
    Serial.println("====================");
    Serial.println(inf);
    Serial.println("--------------------");
    Serial.println(inf1);
    Serial.println("====================");
}

void logSerialPretty(String inf, String inf1, String inf2){
    Serial.println("====================");
    Serial.println(inf);
    Serial.println("--------------------");
    Serial.println(inf1);
    Serial.println("--------------------");
    Serial.println(inf2);
    Serial.println("====================");
}

void logSerial(String inf, int level){
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

void logSerial(String name, String color, String inf){

    Serial.print(color);
    Serial.print("[");
    Serial.print(name);
    Serial.print("]");
    Serial.print(ANSI_RST);
    Serial.print("\t");
    Serial.println(inf);    
}