#include <logging.h>

const char *ansiGreen = "\u001b[32m";
const char *ansiYellow = "\u001b[33m";
const char *ansiRed = "\u001b[31m";
const char *ansiCyan = "\u001b[96m";
const char *ansiRst = "\u001b[0m";

void logSerialPretty(String inf){
    Serial.println(F("===================="));
    Serial.println(inf);
    Serial.println(F("===================="));
}

void logSerialPretty(String inf, String inf1){
    Serial.println(F("===================="));
    Serial.println(inf);
    Serial.println(F("--------------------"));
    Serial.println(inf1);
    Serial.println(F("===================="));
}

void logSerialPretty(String inf, String inf1, String inf2){
    Serial.println(F("===================="));
    Serial.println(inf);
    Serial.println(F("--------------------"));
    Serial.println(inf1);
    Serial.println(F("--------------------"));
    Serial.println(inf2);
    Serial.println(F("===================="));
}

void logSerial(String inf, int level){
    switch(level){
        case 0:
            Serial.print(ansiGreen);
            Serial.print(F("[DEBUG]"));
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            Serial.println(inf);
            break;
        case 1:
            Serial.print(ansiYellow);
            Serial.print(F("[WARN]"));
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            Serial.println(inf);
            break;
        case 2:
            Serial.print(ansiRed);
            Serial.print(F("[ERROR]")); //
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            Serial.println(inf);
            break;
        case 3:
            Serial.print(ansiCyan);
            Serial.print(F("[INFO]")); //
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            Serial.println(inf);
            break;
        case 4:
            Serial.print(ansiGreen);
            Serial.print(F("[DONE]")); //
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            Serial.println(inf);
            break;
    }
}

void logSerial(const char *inf, int level)
{
    switch(level){
        case 0:
            Serial.print(ansiGreen);
            Serial.print(F("[DEBUG]"));
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            Serial.println(inf);
            break;
        case 1:
            Serial.print(ansiYellow);
            Serial.print(F("[WARN]"));
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            Serial.println(inf);
            break;
        case 2:
            Serial.print(ansiRed);
            Serial.print(F("[ERROR]")); //
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            Serial.println(inf);
            break;
        case 3:
            Serial.print(ansiCyan);
            Serial.print(F("[INFO]")); //
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            Serial.println(inf);
            break;
        case 4:
            Serial.print(ansiGreen);
            Serial.print(F("[DONE]")); //
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            Serial.println(inf);
            break;
    }
}

//bool returndir: true -> \r is on the end
//bool returndir: false -> \r is on the beginning
void logSerial(String inf, int level, bool returndir){
    switch(level){
        case 0:
            Serial.print(ansiGreen);
            Serial.print(F("[DEBUG]"));
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            break;
        case 1:
            Serial.print(ansiYellow);
            Serial.print(F("[WARN]"));
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            break;
        case 2:
            Serial.print(ansiRed);
            Serial.print(F("[ERROR]")); //
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            break;
        case 3:
            Serial.print(ansiCyan);
            Serial.print(F("[INFO]")); //
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            break;
        case 4:
            Serial.print(ansiGreen);
            Serial.print(F("[DONE]")); //
            Serial.print(ansiRst);
            Serial.print(F("\t"));
            break;
    }
    Serial.print(inf);
    Serial.print(F("\r"));
}
void logSerial(String name, String color, String inf){

    Serial.print(color);
    Serial.print(F("["));
    Serial.print(name);
    Serial.print(F("]"));
    Serial.print(ansiRst);
    Serial.print(F("\t"));
    Serial.println(inf);
}