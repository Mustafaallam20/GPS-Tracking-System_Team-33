#include "tm4c123gh6pm.h"
#include "TM4C123GH6PM_2.h"
#include "stdint.h"
#include <stdlib.h>
#include "stdio.h"
#include "Math.h"

#define LCD_DATA 	 GPIOB
#define LCD_CTRL   GPIOA
#define RS				 0x20
#define RW				 0x40
#define EN				 0x80
#define LCD_H
#define PI 3.14159265358979323846
#define GPIO_PA10_M 0xC0

void PortF_Init();
void PortA_Init();
void PortB_Init();

double degtorad(double degree);
void ledOn(double d);
double distance(double lati1, double long1, double lati2, double long2);
void delay(int time);


void delay_1msec(int n);
void LCD_init();
void delayMs(unsigned long t);
void LCD_data (unsigned char data);
void LCD_command (unsigned char command);
void delayUs(unsigned long t);


double deg_do_decimal_gps(char *str);
void Receive_GPS_Data();
char UART2_read();
void UART2_Init();
uint8_t UART2_Available(void);
uint8_t UART2_Read(void);
void UART2_Write(uint8_t data);
void LCD_print_line(char* str);
void ftoa(float n, char *res, int afterpoint) ;
