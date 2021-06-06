#include "Header.h"



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
