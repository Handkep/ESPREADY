#ifndef LOGGING_H
#define LOGGING_H
#include <Arduino.h>

// used for print colors on the console
#define ANSI_GREEN "\u001b[32m"
#define ANSI_YELLOW "\u001b[33m"
#define ANSI_RED "\u001b[31m"
#define ANSI_CYAN "\u001b[96m"
#define ANSI_RST "\u001b[0m"

void logSerialPretty(String inf);
void logSerialPretty(String inf, String inf1);
void logSerialPretty(String inf, String inf1, String inf2);
void logSerial(String inf, int level);
void logSerial(String name, String color, String inf);
#endif