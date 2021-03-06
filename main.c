#include "Header.h"

double d=0;
char lat[20];
char lg[20];       
double points_list[6][2] = {0};
int points_in_list = 0;
double cur_lat, cur_lon;
double prev_lat, prev_lon;
uint8_t start = 1;
char buf[12];
char disBuf[50] = {50};
int distnation = 0;

void SystemInit() {
	SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));
}

int main()
{
	
	PortF_Init();
	PortA_Init();
	PortB_Init();
	UART2_Init();
	LCD_init();
	
	while(1)
	{
		LCD_print_line("Distance: ");
		Receive_GPS_Data();
		//LCD_print_line("The Distance: ");
		distance(prev_lat, prev_lon, cur_lat, cur_lon);
		ftoa(d, buf, 3);
		LCD_print_line(buf);
		ledOn(d);
	}

}









/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////
////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////
////////////////////////////////////////////////////////////////////////////
void PortA_Init(){
  // Initializing Clock and wait until get stabilized
  SYSCTL_RCGCGPIO_R |= 0x01;
  while((SYSCTL_PRGPIO_R & 0x01) == 0);
  
  // Initializing Port A pins
  GPIO_PORTA_LOCK_R =0x4C4F434B;
  GPIO_PORTA_CR_R |= 0xE0;
  GPIO_PORTA_AMSEL_R &= ~0xE0;
  GPIO_PORTA_PCTL_R &= ~0xFFF00000;
  GPIO_PORTA_DIR_R |= 0xE0;
  GPIO_PORTA_AFSEL_R &= ~0xE0;
  GPIO_PORTA_DEN_R |= 0xE0;
}
	/////////////////////////////
////////////////////////////////////////////////////////////////////////////	/////////////////////////////
////////////////////////////////////////////////////////////////////////////
void PortB_Init(){
  // Initializing Clock and wait until get stablized
  SYSCTL_RCGCGPIO_R |= 0x02;
  while((SYSCTL_PRGPIO_R & 0x02) == 0);
  
  // Initializing Port B pins
  GPIO_PORTB_LOCK_R =0x4C4F434B;
  GPIO_PORTB_CR_R |= 0xFF;
  GPIO_PORTB_AMSEL_R = 0;
  GPIO_PORTB_PCTL_R =0;
  GPIO_PORTB_DIR_R |= 0xFF;
  GPIO_PORTB_AFSEL_R =0;
  GPIO_PORTB_DEN_R |= 0xFF;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


//from degree to radian
double degtorad(double degree){
  return (degree * PI / 180);
} 

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//function that turns on the LED when the distance exceeds 100 meters.
void ledOn(double d){
if( d <= 100)
	GPIO_PORTF_DATA_R |=0X02 ;    //red led is on 
else{
  GPIO_PORTF_DATA_R |=0X08 ;    //green led is on
	while(1);
}	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Receive_GPS_Data(){
    int Gpsdata; // for incoming serial data
    int finish = 0; // indicate end of message
    int pos_cnt = 0; // position counter
    int lat_cnt = 0; // latitude data counter
    int log_cnt = 0; // longitude data counter
    int flg = 0; // GPS flag
    int com_cnt = 0; // comma counter
    int flg_2 = 1;
	
		prev_lat = cur_lat;
		prev_lon = cur_lon;
	
    while(finish == 0){
      while((UART2_FR_R & 0x10) != 1>0){ // Check GPS data
        Gpsdata = UART2_DR_R;
        flg = 1;
        if( Gpsdata == '$' && pos_cnt == 0) pos_cnt++;
        if( Gpsdata == 'G' && pos_cnt == 1) pos_cnt++;
        if( Gpsdata == 'P' && pos_cnt == 2) pos_cnt++;
        if( Gpsdata == 'R' && pos_cnt == 3) pos_cnt++;
        if( Gpsdata == 'M' && pos_cnt == 4) pos_cnt++;
        if( Gpsdata == 'C' && pos_cnt == 5) pos_cnt++;
        if(pos_cnt == 6 &&  Gpsdata == ','){ // count commas in message
            com_cnt++;
            flg = 0;
       }

       if(com_cnt == 2 && Gpsdata == 'V'){
                finish = 1;
                flg = 0;
                flg_2 = 0;
        }
 
       if(com_cnt == 3 && flg == 1 && flg_2){
        lat[lat_cnt++] =  Gpsdata; // latitude
        flg = 0;
       }
 
       if(com_cnt == 5 && flg == 1 && flg_2){
         lg[log_cnt++] =  Gpsdata; // Longitude
         flg = 0;
       }
 
       if( Gpsdata == '*' && com_cnt >= 5){
         flg = 0;
         finish = 1;
         }
        }
    }
		
		cur_lat = deg_do_decimal_gps(lat);
		cur_lon = deg_do_decimal_gps(lg);

    points_list[points_in_list][0] = cur_lat;
    points_list[points_in_list][1] = cur_lon;
    points_in_list++;
		

    if(points_in_list == 5){
        uint8_t i;
        cur_lat = 0;
        cur_lon = 0;

        while(i<5){
            cur_lat += points_list[i][0];
            cur_lon += points_list[i][1];
            i++;
        }
        cur_lat /= 5;
        cur_lon /= 5;

        
      if(start){
        prev_lat = cur_lat;
        prev_lon = cur_lon;
        start = 0;
      } 
        points_in_list = 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

double deg_do_decimal_gps(char *str){
    char deg[4] = {0};
		uint8_t i;
		for(i = 0; i < 3 ; i++)
			deg[i] = str[i];
		
    return atof(deg) + (atof(&str[3]) / 60);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
		void delay_1msec(int n){
	int i, j;
	for(i = 0; i < n; i++)
	for(j = 0; j < 3180; j++){}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////		
		
void delayMs(unsigned long t)
{
	long i , j;
	   i = 0;
	   j = 0;
	  while(i<t){
       while(j<3180){
				 j++;
			 //Do Nothing
			 }
			 i++;
	  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////		
		

		
void delayUs(unsigned long t){
	long i , j;
	   i = 0;
	   j = 0;
	  while(i<t){
       while(j<3){
				 j++;
			 //Do Nothing
			 }
			 i++;
	  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////		
		
void LCD_init(void)
{
SYSCTL->RCGCGPIO |= 0x01; /* enable clock to GPIOA */
SYSCTL->RCGCGPIO |= 0x02; /* enable clock to GPIOB */
LCD_CTRL->DIR |= 0xE0; /* set PORTA pin 7-5 as output for control */
LCD_CTRL->DEN |= 0xE0; /* set PORTA pin 7-5 as digital pins */
LCD_DATA->DIR = 0xFF; /* set all PORTB pins as output for data */
LCD_DATA->DEN = 0xFF; /* set all PORTB pins as digital pins */
delayMs(20); /* initialization sequence */
LCD_command(0x30);
delayMs(5);
LCD_command(0x30);
delayUs(100);
LCD_command(0x30);
LCD_command(0x38); /* set 8-bit data, 2-line, 5x7 font */
LCD_command(0x06); /* move cursor right */
LCD_command(0x01); /* clear screen, move cursor to home */
LCD_command(0x0F); /* turn on display, cursor blinking */
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////		
void LCD_command(unsigned char command)
{
LCD_CTRL->DATA = 0; /* RS = 0, R/W = 0 */
LCD_DATA->DATA = command;
LCD_CTRL->DATA = EN; /* pulse E */
delayUs(0);
LCD_CTRL->DATA = 0;
if (command < 4)
delayMs(2); /* command 1 and 2 needs up to 1.64ms */
else
delayUs(40); /* all others 40 us */
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////		
void LCD_data(unsigned char data)
{
LCD_CTRL->DATA = RS; /* RS = 1, R/W = 0 */
LCD_DATA->DATA = data;
LCD_CTRL->DATA = EN | RS; /* pulse E */
delayUs(1000);
LCD_CTRL->DATA = 0;
delayUs(40);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void LCD_print_line(char* str){
	uint8_t i = 0;
	while(str[i]){
		LCD_data(str[i]);
		i++;
	}
}

// reverses a string 'str' of length 'len' 
void reverse(char *str, int len) 
{ 
    int i=0, j=len-1, temp; 
    while (i<j) 
    { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; 
    } 
} 
 
// reverses a string 'str' of length 'len' 
void rev(char *str, int len) 
{ 
    int i=0, j=len-1, temp; 
    while (i<j) 
    { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; 
    } 
} 
  
 // Converts a given integer x to string str[].  d is the number 
 // of digits required in output. If d is more than the number 
 // of digits in x, then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) 
    { 
        str[i++] = (x%10) + '0'; 
        x = x/10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    rev(str, i); 
    str[i] = '\0'; 
    return i; 
} 
  
// Converts a floating point number to string. 
void ftoa(float n, char *res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) 
    { 
        res[i] = '.';  // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter is needed 
        // to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 
