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
void seven_segment(int one, int two, int three);
char UART2_read();
void UART2_Init();
uint8_t UART2_Available(void);
uint8_t UART2_Read(void);
void UART2_Write(uint8_t data);

double deg_do_decimal_gps(char *str);
void Receive_GPS_Data();
