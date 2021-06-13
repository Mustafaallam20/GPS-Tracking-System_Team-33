#include "Header.h"

float d=0;


int main()
{
	while(1)
	{
			
	}
}



// Initialization of Port F pins
void PortF_Init(void)
{
volatile unsigned long delay;
SYSCTL_RCGCGPIO_R|=0x20;
while((SYSCTL_PRGPIO_R&0x20)==0){};
  
  GPIO_PORTF_LOCK_R=0x4C4F434B;
  GPIO_PORTF_CR_R |= 0x0E;               
  GPIO_PORTF_AMSEL_R &= ~0x0E;        
  GPIO_PORTF_PCTL_R &= ~0x0000FFF0;
  GPIO_PORTF_DIR_R |= 0x0E;            
  GPIO_PORTF_AFSEL_R &= ~0x0E;               
  GPIO_PORTF_DEN_R |= 0x0E;
  GPIO_PORTF_DATA_R &= ~0x0E;
	
	
}
// Initialization of Port D pins
void PortD_Init(void)
{
volatile unsigned long delay;
SYSCTL_RCGCGPIO_R|=0x08;
while((SYSCTL_PRGPIO_R&0x08)==0){};
  
  GPIO_PORTD_LOCK_R=0x4C4F434B;
  GPIO_PORTD_CR_R |= 0x0F;
	GPIO_PORTD_AMSEL_R &= ~0x0F;
	GPIO_PORTD_PCTL_R &= ~0x0F;
	GPIO_PORTD_DIR_R |=  0x0F;
	GPIO_PORTD_AFSEL_R &= ~0x0F;
	GPIO_PORTD_DEN_R |= 0x0F;
}


// Initialization of Port E pins
void PortE_Init(void)
{
volatile unsigned long delay;
SYSCTL_RCGCGPIO_R|=0x10;
while((SYSCTL_PRGPIO_R&0x010)==0){};
  
GPIO_PORTB_LOCK_R=0x4C4F434B;
GPIO_PORTB_CR_R|= 0xEf;
GPIO_PORTB_DIR_R|= 0xEF;
GPIO_PORTB_DEN_R|= 0xEF;
GPIO_PORTB_AMSEL_R&=~ 0xEf;
GPIO_PORTB_AFSEL_R&=~ 0xEf;
GPIO_PORTB_PCTL_R&=~ 0xEf;
}


//from degree to radian
double degtorad(double degree){
  return (degree * PI / 180);
} 


// Initialization of Port B pins
void PortB_Init(void)
{
volatile unsigned long delay;
SYSCTL_RCGCGPIO_R|=0x02;
while((SYSCTL_PRGPIO_R&0x02)==0){};
  
GPIO_PORTB_LOCK_R=0x4C4F434B;
GPIO_PORTB_CR_R|=0x1F;
GPIO_PORTB_DIR_R|=0xFE;
GPIO_PORTB_DEN_R|=0x1F;
GPIO_PORTB_AMSEL_R&=~ 0x1F;
GPIO_PORTB_AFSEL_R|=0x03;
GPIO_PORTB_PCTL_R|=0x000000FF;
}



//function that turns on the LED when the distance exceeds 100 meters.
void ledOn(double d)
{
if( d <= 100)
{ 
	GPIO_PORTF_DATA_R |=0X02 ;    //red led is on 
}
else
  GPIO_PORTF_DATA_R |=0X08 ;    //green led is on 
}




//Calculating Distance between two point
double distance(double lati1, double long1, double lati2, double long2){
  const int ROE = 6371; //Radius of earth in (km)
  double phi1 = degtorad(lati1);
  double phi2 = degtorad(lati2);
  double del1 = degtorad(lati2 - lati1);
  double del2 = degtorad(long2 - long1);

  double x = sin(del1 / 2) * sin(del1 / 2) + cos(phi1) * cos(phi2) * sin(del2 / 2) * sin(del2 / 2);
  double y = 2 * atan2(sqrt(x), sqrt(1 - x));
  double z = ROE * y;

  return d += z;
}




//Time delay function
void delay(int time){
  int tmp = time*3180;
  while(tmp--);
}




//function that will display the output distance on the 3-digit 7 segments.
 void seven_segment(int one, int two, int three){

  GPIO_PORTB_DATA_R |= 0x1c; // turn off all digits
  GPIO_PORTB_DATA_R &= ~0x10;
  GPIO_PORTD_DATA_R = one;
  delay(5);
  
  GPIO_PORTB_DATA_R |= 0x1c; // turn off all digits
  GPIO_PORTB_DATA_R &= ~0x08;
  GPIO_PORTD_DATA_R = two;
  delay(5);

  GPIO_PORTB_DATA_R |= 0x1c; // turn off all digits
  GPIO_PORTB_DATA_R &= ~0x04;
  GPIO_PORTD_DATA_R = three;
  delay(5);
  
}

void UART2_Init(void){ 
	
	
    SYSCTL_RCGCUART_R |=0x0004; 
    SYSCTL_RCGCGPIO_R |=0x0008; 

   UART2_CTL_R &= ~0x0001; 
   UART2_IBRD_R = 104; 
   UART2_FBRD_R = 11; 
   UART2_LCRH_R = 0x0070; 
   UART2_CTL_R |= 0x0301; 

    GPIO_PORTD_LOCK_R = 0x4C4F434B;
    GPIO_PORTD_CR_R |= 0xC0;
    GPIO_PORTD_AMSEL_R &= ~0xC0;
    GPIO_PORTD_PCTL_R &= ~0x0FF000000;
    GPIO_PORTD_DIR_R |= 0x80; 
    GPIO_PORTD_DIR_R &= ~0x40;
    GPIO_PORTD_AFSEL_R |= 0xC0;
    GPIO_PORTD_DEN_R |= 0xC0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t UART2_Available(void){ 
return ((UART2_FR_R&0x0010) == 0x0010) ?  0 : 1; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t UART2_Read(void){
  while(UART2_Available() != 1); 
  return (uint8_t)(UART2_DR_R&0xFF); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void UART2_Write(uint8_t data){
  while((UART2_FR_R&0x0020) != 0); 
  UART2_DR_R = data; 
}


