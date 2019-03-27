#include <msp430f2274.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <function.h>
//   PIN1 = P2.2     (/CS)
//   PIN2 = P2.1     (Clock)
//   PIN3 = P2.0     (Data)
//   PIN4 = VCC      (VREF)
//   PIN5 = OUTPUTA
//   PIN6 = VCC      (Power)
//   PIN7 = GND      (Ground)
//   PIN8 = OUTPUTB
//
//Sebastian Crespo
//DC module
//----------------------------------------
int adcValue=0;
float voltage=0;
int volatile delay=0;
int switch1=0;
int switch2=0;
char TOUCH[6]="Pet me";
char FLUSH[8]="Flush it";
char FEED[7]="Feed me";
char HURRY[5]="HURRY";

int l;
char sine[52]={0x80,0x8f,0x9f,0xae,0xbc,0xc9,0xd5,0xe0,
               0xea,0xf2,0xf8,0xfc,0xfe,0xff,0xfd,0xfa,
               0xf5,0xee,0xe5,0xdb,0xcf,0xc3,0xb5,0xa6,
               0x97,0x87,0x78,0x68,0x59,0x4a,0x3c,0x30,
               0x24,0x1a,0x11,0xa,0x5,0x2,0x0,0x1,
               0x3,0x7,0xd,0x15,0x1f,0x2a,0x36,0x43,
               0x51,0x60,0x70,0x80};
/**
 * main.c
 */
int main(void)
{   //Change internal frequency
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    if(CALBC1_16MHZ== 0xFF || CALDCO_16MHZ == 0xFF)
    {
        while(1);
    }
    BCSCTL1= CALBC1_16MHZ;// change the internal frequency to 16MHz
    DCOCTL = CALDCO_16MHZ;
    char y;
    input_PORTS();//configure Ports
    lcd_init();//initialize LCD
    init_ADC();//initialize ADC
    _delay_cycles(2000);
    while(1){
        //adc start convertion
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
               //__bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled
        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end
        adcValue = (long double)ADC10MEM;         // Read ADC Value
        if(adcValue>=400){//dark
            P1OUT |= 0x01;//make P1.0 an output port
            if((P3IN & BIT4)==BIT4){//digital switch outputs Vcc

                for(l=0; l<8; l++){//lcd shows Flush
                    lcd_char(FLUSH[l]);
                    }
                while((P3IN & BIT4) == BIT4){
                    __delay_cycles(2000);
                }
                lcd_command(0x01);//reset lcd
            }
            if((P3IN & BIT4) != BIT4){//digital switch outputs ground
                for(l=0; l<7; l++){//feed me is outputted in LCD
                    lcd_char(FEED[l]);
                    }
                while((P3IN & BIT4) != BIT4){
                for(y=0;y<52;y++){//dac output sine wave
                    dac_write(sine[y]/20);//output sine wave
                    __delay_cycles(1);
                           }
                }
                lcd_command(0x01);
            }
            }
        if(adcValue<200){//light
            P1OUT &= ~0x01;

            for(l=0; l<6; l++){//lcd outputs Pet me
                lcd_char(TOUCH[l]);
                __delay_cycles(4000000);
                                  }

            lcd_command(0x01);
        }
        else{//some light
            P1OUT ^= 0x01;
            for(l=0; l<5; l++){//lcd outputs Hurry
                lcd_char(HURRY[l]);
                __delay_cycles(4000000);
                }
            lcd_command(0x01);

        }
    }
}
