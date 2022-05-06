#ifndef ARRAYOPERATIONS_H
#define ARRAYOPERATIONS_H
#include <Arduino.h>
#define LENGTHOF2NDARRAY 4
// used for print colors on the console
void reversearr(int (*arr)[LENGTHOF2NDARRAY], size_t len);

void rotatearrleft(int (*arr)[LENGTHOF2NDARRAY], size_t n, size_t len);
void rotatearrright(int (*arr)[LENGTHOF2NDARRAY], size_t n, size_t len);
#endif