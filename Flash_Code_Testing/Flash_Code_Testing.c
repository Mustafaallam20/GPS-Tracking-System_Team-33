#include "Header.h"

int main(void)
{

    //init port F
    SYSCTL_RCGCGPIO_R |= 0x20;
    GPIO_PORTF_DEN_R = 0x0E;
    GPIO_PORTF_DIR_R = 0x0E;

    while (1)
    {
        GPIO_PORTF_DATA_R = 0x0E;     //Test leds
    }

}