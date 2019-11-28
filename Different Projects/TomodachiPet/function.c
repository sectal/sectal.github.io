/*
 * function.c
 *
 *  Created on: Jul 20, 2018
 *      Author: sebas
 */

#include <msp430f2274.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

char uf_lcd_temp;
char uf_lcd_temp2;
char uf_lcd_x;

int n_tu(int number, int count)
{
    int result = 1;
    while(count-- > 0)
        result *= number;

    return result;
}

/*** Convert float to string ***/
void float_to_string(float f, char r[])
{
    long long int length, length2, i, number, position, sign;
    float number2;

    sign = -1;   // -1 == positive number
    if (f < 0)
    {
        sign = '-';
        f *= -1;
    }

    number2 = f;
    number = f;
    length = 0;  // Size of decimal part
    length2 = 0; // Size of tenth

    /* Calculate length2 tenth part */
    while( (number2 - (float)number) != 0.0 && !((number2 - (float)number) < 0.0) )
    {
         number2 = f * (n_tu(10.0, length2 + 1));
         number = number2;

         length2++;
    }

    /* Calculate length decimal part */
    for (length = (f > 1) ? 0 : 1; f > 1; length++)
        f /= 10;

    position = length;
    length = length + 1 + length2;
    number = number2;
    if (sign == '-')
    {
        length++;
        position++;
    }

    for (i = length; i >= 0 ; i--)
    {
        if (i == (length))
            r[i] = '\0';
        else if(i == (position))
            r[i] = '.';
        else if(sign == '-' && i == 0)
            r[i] = '-';
        else
        {
            r[i] = (number % 10) + '0';
            number /=10;
        }
    }
}
char digitToChar(int digit){
        return ((char) (digit + '0'));//ASCII values we want start at 0x30 so we add the digits
    }
void lcd_command(char uf_lcd_x){
    P4DIR = 0xFF;
    uf_lcd_temp = uf_lcd_x;
    P4OUT = 0x00;
    __delay_cycles(22000);
    uf_lcd_x = uf_lcd_x >> 4;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x20;
    P4OUT = uf_lcd_x;
    __delay_cycles(22000);
    uf_lcd_x = uf_lcd_x & 0x0F;
    P4OUT = uf_lcd_x;
    __delay_cycles(22000);
    P4OUT = 0x00;
    __delay_cycles(22000);
    uf_lcd_x = uf_lcd_temp;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x20;
    P4OUT = uf_lcd_x;
    __delay_cycles(22000);
    uf_lcd_x = uf_lcd_x & 0x0F;
    P4OUT = uf_lcd_x;
    __delay_cycles(22000);
    }
void lcd_init(void){
    lcd_command(0x33);
    lcd_command(0x32);
    lcd_command(0x2C);
    lcd_command(0x0C);
    lcd_command(0x01);
}
void lcd_char(char uf_lcd_x){
    P4DIR = 0xFF;
    uf_lcd_temp = uf_lcd_x;
    P4OUT = 0x10;
    __delay_cycles(22000);
    uf_lcd_x = uf_lcd_x >> 4;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x30;
    P4OUT = uf_lcd_x;
    __delay_cycles(22000);
    uf_lcd_x = uf_lcd_x & 0x1F;
    P4OUT = uf_lcd_x;
    __delay_cycles(22000);
    P4OUT = 0x10;
    __delay_cycles(22000);
    uf_lcd_x = uf_lcd_temp;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x30;
    P4OUT = uf_lcd_x;
    __delay_cycles(22000);
    uf_lcd_x = uf_lcd_x & 0x1F;
    P4OUT = uf_lcd_x;
    __delay_cycles(22000);
    }
void dac_write(char data){//write to dac
    int output = 0x0000;
    char temp;
    signed char x;

    P2SEL &= 0xF8; //select digital I/0
    P2DIR |= 0x07; //set directon register P2.0 - P2.2
    P2OUT &= 0xF8; // clear data, clock, and enable
    P2OUT |= 0x04; // set enable high

    temp = data; // format data
    data >>= 4;
    data &= 0x0F;
    data |= 0xF0;
    output = data;
    output <<= 8;
    data = temp;
    data <<= 4;
    data &= 0xF0;
    output |= data;

    P2OUT &= 0xFB; //set enable low
    for (x = 15; x > -1; x--){  //send out 16 bits of data
        P2OUT |= (output >> x) & 0x01;
        P2OUT |= 0x02;
        P2OUT &= 0xFD;
        P2OUT &= 0xFE;
    }
    P2OUT |= 0x04; // set enable high
}
void oscill_init(){
    if(CALBC1_16MHZ== 0xFF || CALDCO_16MHZ == 0xFF)
      {
          while(1);
      }
      BCSCTL1= CALBC1_16MHZ;// change the internal frequency to 16MHz(1us)
      DCOCTL = CALDCO_16MHZ;
}
void init_ADC(){
    //adc enabled
    ADC10AE0 |= BIT7;  // A7
    ADC10CTL1 = INCH_7;                         // ADC Channel -> 7
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;//+ ADC10IE;  // Ref -> Vcc, 64 CLK S&H, interrupt enabled
}
void input_PORTS(){
    //ports
    P1DIR |= BIT0;                   // Set LED pin -> Output
    P3DIR &= ~0x10;// P3.4 Input
    P3REN |= 0x10;// Enable Resistor for P3.4
    P3OUT |= 0x10;// Select Pull Up for  P3.4
}
