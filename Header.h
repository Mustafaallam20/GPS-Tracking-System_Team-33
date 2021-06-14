#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "Math.h"

#define PI 3.14159265358979323846

void PortF_Init();
void PortD_Init();
void PortE_Init();
double degtorad(double degree);
void PortB_Init();
void ledOn(double d);
double distance(double lati1, double long1, double lati2, double long2);

void delay(int time);
void delayMs(unsigned long t);
void delayUs(unsigned long t);