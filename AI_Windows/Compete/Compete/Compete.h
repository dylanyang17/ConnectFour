#ifndef COMPETE_H_
#define COMPETE_H_

#include "Data.h"

#define MAX_TIME INFINITE

extern unsigned long timeA;
extern unsigned long timeB;

int compete(char strategyA[], char strategyB[], bool Afirst, Data* data);

#endif