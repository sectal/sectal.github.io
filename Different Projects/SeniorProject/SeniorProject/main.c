//Final VOFAD Program
//Made by Kamal Nasif and Sebastian Crespo
/*
The following functions were provided by Parallax using the following link:
https://www.parallax.com/downloads/fingerprint-scanner-c-code

 
Paralax functions:

void fingerprint_sendCommand(char __fpCmd, char __fpParam1, char __fpParam2, char __fpParam3);
void fingerprint_readResponse(char *__fpResponse);
int fingerprint_allowOverwrite(char b);
int fingerprint_add(int userId, char userLevel, int scanNumber);
int fingerprint_deleteUser(int userId);
int fingerprint_scan(int userId, int *uid);
int fingerprint_countUsers();

 * @file fingerprint.c
 *
 * @author Matthew Matz
 *
 * @version 0.50
 *
 * @copyright
 * Copyright (C) Parallax, Inc. 2017. All Rights MIT Licensed.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

char capA = 0x41;
char capB = 0x42;
char capC = 0x43;
char capD = 0x44;
char capE = 0x45;
char capF = 0x46;
char capG = 0x47;
char capH = 0x48;
char capI = 0x49;
char capJ = 0x4A;
char capK = 0x4B;
char capL = 0x4C;
char capM = 0x4D;
char capN = 0x4E;
char capO = 0x4F;
char capP = 0x50;
char capQ = 0x51;
char capR = 0x52;
char capS = 0x53;
char capT = 0x54;
char capU = 0x55;
char capV = 0x56;
char capW = 0x57;
char capX = 0x58;
char capY = 0x59;
char capZ = 0x5A;

char lowA = 0x61;
char lowB = 0x62;
char lowC = 0x63;
char lowD = 0x64;
char lowE = 0x65;
char lowF = 0x66;
char lowG = 0x67;
char lowH = 0x68;
char lowI = 0x69;
char lowJ = 0x6A;
char lowK = 0x6B;
char lowL = 0x6C;
char lowM = 0x6D;
char lowN = 0x6E;
char lowO = 0x6F;
char lowP = 0x70;
char lowQ = 0x71;
char lowR = 0x72;
char lowS = 0x73;
char lowT = 0x74;
char lowU = 0x75;
char lowV = 0x76;
char lowW = 0x77;
char lowX = 0x78;
char lowY = 0x79;
char lowZ = 0x7A;

char zero = 0x30;
char one = 0x31;
char two = 0x32;
char three = 0x33;
char four = 0x34;
char five = 0x35;
char six = 0x36;
char seven = 0x37;
char eight = 0x38;
char nine = 0x39;
char space = 0x20;
char colon = 0x3A;
char slash = 0x2F;

int lengthOfINT = 0;
int temp = 0;
int i = 1;
int temp2 = 0;

int alarm_time = 0;
int rightFinger = 0;
int dis = 0;
int ultra1 = 10000;
int ultra2 = 10000;
int ultra3 = 10000;
int ultra4 = 10000;
int ultra5 = 10000;


//lcd_command(0x01); // clear display
//lcd_command(0x02); // cursor home
//lcd_command(0xC0); // shift cursor to second row
//lcd_char(capF);
//lcd_char(colon);
//lcd_char(space);
//numPrintLCD(check1);

//motor and encoder variables
int masterMotorPower = 0; //constant speed - change to make rover go faster or slower
int slaveMotorPower = 0; //variable speed to keep motors at same speed
int leftSlaveEncoder = 0;
int rightMasterEncoder = 0;
int error = 0;
int fixSpeed = 1; //how fast to fix speed
int backToLine = 0;

//IR, RFID, and ultrasonic variables
int leftIRHit = 0;
int rightIRHit = 0;
int rf= 0;
char RFID_bit;
int x = 0;
int RFID_trigger = 0;
char RX_interrupt;

//Amplifier variables
int point = 0;
int loop = 0;
int alarm = 0;
int sineTable[51] =
{
	125,141,156,171,185,198,211,221,
	231,238,244,248,250,250,248,244,
	238,231,221,211,198,185,171,156,
	141,125,109,94,79,65,52,39,
	29,19,12,6,2,0,0,2,
	6,12,19,29,39,52,65,79,
	94,109,125
};

int why = 0;
int why2 = 0;
int how = 0;
int remainder;
int RFIDreadCard = 0;
int rfidCard = 0;
int ultraSonic1 = 100;
int ultraSonic2 = 100;
int ultraSonic6 = 100;
int atReciever = 0;

//FingerPrint variables
#define CMD_SET_MODE                      0x2D
#define ACK_SUCCESS                       0x00 // Operation successfully
#define ACK_FAIL                          0x01 // Operation failed
#define ACK_FULL                          0x04 // Fingerprint database is full
#define ACK_NOUSER                        0x05 // No such user
#define ACK_USER_EXISTS                   0x07 // already exists
#define ACK_TIMEOUT                       0x08 // Acquisition timeout
#define CMD_SLEEP                         0x2C // Sleeps the device
#define CMD_ADD_FINGERPRINT_1             0x01
#define CMD_ADD_FINGERPRINT_2             0x02
#define CMD_ADD_FINGERPRINT_3             0x03
#define CMD_DELETE_USER                   0x04
#define CMD_DELETE_ALL_USERS              0x05
#define CMD_GET_USERS_COUNT               0x09
#define CMD_SCAN_COMPARE_1_TO_1           0x0B
#define CMD_SCAN_COMPARE_1_TO_N           0x0C
#define CMD_READ_USER_PRIVLAGE            0x0A
#define CMD_SENSITIVITY                   0x28
#define CMD_SCAN_GET_IMAGE                0x24
#define CMD_SCAN_GET_EIGENVALS            0x23
#define CMD_SCAN_PUT_EIGENVALS            0x44
#define CMD_PUT_EIGENVALS_COMPARE_1_TO_1  0x42
#define CMD_PUT_EIGENVALS_COMPARE_1_TO_N  0x43
#define CMD_GET_USER_EIGENVALS            0x31
#define CMD_PUT_USER_EIGENVALS            0x41
#define CMD_GET_USERS_INFO                0x2B
#define CMD_SET_SCAN_TIMEOUT              0x2E // How long to try scanning - multiples of ~0.25s
int addscan1 = -1;
int addscan2 = -1;
int addscan3 = -1;
int numUsers = 0;
int checkscan1 = -1;
int checkscan2 = -1;
int checkscan3 = -1;
int uid = 12;
int matchID = 0;
int *uidpoint = &matchID;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PORTC_OUT = 0x04; //yellow
//PORTC_OUT = 0x02; //red
//PORTC_OUT = 0x01; //green

//Amplifier functions
void DAC_INIT(void)
{
	//PORTA_DIR = 0XFF; //PORTA_PIN2CTRL setting pin2 to output
	
	DACA_CTRLA = 0x05; //enables DAC and channel 0
	DACA_CTRLB = 0x00;	//single channel operation on channel 0
	DACA_CTRLC = 0X18; //AREF on PORTB
	DACA_EVCTRL = 0X00; //Event channel 0 as input to DAC
}
// on_time = 115 = ~5sec 
void playAlarm(int on_time)
{
	while (on_time > 1)
	{
		DACA_CTRLA = 0x05;
		if (alarm == 0)
		{
			while(loop < 5000)
			{
				_delay_us(200); //change time to change frequency
				DACA_CH0DATA = sineTable[point];  // outputs 1.5V using formula [output voltage = (Data*2.5)/4095] 4095 because we are using 12bit and 2.5 because its the reference voltage
				point = point + 1; // go to next value from table
				//once point value reaches end of table then restart back at 0
				if (point == 51)
				{
					point = 0;
				}
				loop = loop + 1;
			}
			alarm = 1;
			loop = 0;
		}
		else
		{
			while(loop < 5000)
			{
				_delay_us(300); //change time to change frequency
				DACA_CH0DATA = sineTable[point];  // outputs 1.5V using formula [output voltage = (Data*2.5)/4095] 4095 because we are using 12bit and 2.5 because its the reference voltage
				point = point + 1; // go to next value from table
				//once point value reaches end of table then restart back at 0
				if (point == 51)
				{
					point = 0;
				}
				loop = loop + 1;
			}
			alarm = 0;
			loop = 0;
		}
	on_time = on_time - 1;
	}
}

void lcd_command(char cmd)
{
	char temp = cmd;
	PORTD_OUT = 0;
	_delay_ms(10);
	cmd = ( (cmd & 0xF0) >> 4) | 0x80; //Write Upper Nibble (RS=0) E --> 1
	PORTD_OUT = cmd;
	_delay_ms(10);
	cmd ^= 0x80; //E --> 0
	PORTD_OUT = cmd;
	_delay_ms(10);
	cmd = temp;
	cmd = ( (cmd & 0x0F) ) | 0x80; //Write Lower Nibble (RS=0) E --> 1
	PORTD_OUT = cmd;
	_delay_ms(10);
	cmd ^= 0x80; //E --> 0
	PORTD_OUT = cmd;
	_delay_ms(14);
}
void lcd_init()
{
	PORTD_DIR = 0xFF; //Change DDRA --> DDRB and PORTA --> PORTB if LCD attached to port B and so forth
	lcd_command(0x33); //Initialize LCD Driver
	lcd_command(0x32); //Four bit mode
	lcd_command(0x2C); //2 Line Mode
	lcd_command(0x0C); //Display On, Cursor Off, Blink Off Change to 0x0F if cursor is desired
	lcd_command(0x01); //Clear Screen, Cursor Home
}
void lcd_char(char data)
{
	char temp = data;
	PORTD_OUT = 0x40;
	_delay_ms(10);
	data = ( (data & 0xF0) >> 4) | 0xC0; //Write Upper Nibble (RS=1) E --> 1
	PORTD_OUT = data;
	_delay_ms(10);
	data ^= 0x80; // E --> 0
	PORTD_OUT = data;
	_delay_ms(10);
	data = temp;
	data = ( (data & 0x0F) ) | 0xC0; //Write Lower Nibble (RS=1) E --> 1
	PORTD_OUT = data;
	_delay_ms(10);
	data ^= 0x80; //E --> 0
	PORTD_OUT = data;
	_delay_ms(14);
}
char getChar(int x)
{
	char num = 0;
	if (x == 0)
	{
		num = zero;
	}
	if (x == 1)
	{
		num = one;
	}
	if (x == 2)
	{
		num = two;
	}
	if (x == 3)
	{
		num = three;
	}
	if (x == 4)
	{
		num = four;
	}
	if (x == 5)
	{
		num = five;
	}
	if (x == 6)
	{
		num = six;
	}
	if (x == 7)
	{
		num = seven;
	}
	if (x == 8)
	{
		num = eight;
	}
	if (x == 9)
	{
		num = nine;
	}
	return num;
}
void numPrintLCD(int number)
{
	temp = (long int)number;
	lengthOfINT = 0;
	if(temp == 0)
	{
		lcd_char(getChar(0));
	}
	else
	{
		while (temp>0)
		{
			lengthOfINT = lengthOfINT + 1;
			temp = temp / 10;
		}
		temp = (long int)number;
		temp2 = 0;
		i = lengthOfINT;
		while (i > 0)
		{
			temp2 = (int)(temp/pow(10,i-1)) % 10;
			lcd_char(getChar(temp2));
			temp = temp - ((float)temp2) * (pow(10,i-1));
			i = i - 1;
		}
	}
}
void ClockFreqSet(void)
{
	OSC_CTRL = 0x02; //set clock to be 32MHz

	while((OSC_STATUS &= 0x02) != 0x02)  //check if clock is ready at 32MHz if not loop until ready
	{ }

	CPU_CCP = 0xD8; //set as "IOREG" to protect important registers while the clock source is being switched
	CLK_CTRL = 0x01; //selecting the new source for the system to be clock32MHz internal oscillator
}
void pinSetup(void)
{
	PORTH_DIR = !PIN0_bm | !PIN1_bm | PIN2_bm; //PORTH0 and PORTH1 as input interrupts for the encoders pin2 for lock output
	PORTH.PIN0CTRL = PORT_OPC_PULLDOWN_gc | PORT_ISC_RISING_gc;
	PORTH.PIN1CTRL = PORT_OPC_PULLDOWN_gc | PORT_ISC_RISING_gc;
	PORTH.INT0MASK = PIN0_bm; //set portH interrupt 0 to work on pin 0
	PORTH.INT1MASK = PIN1_bm; //set portH interrupt 1 to work on pin 1
	PORTH.INTCTRL = 0x05; //setting the interrupts as low level
	
	/*
	PORTK_DIR = !PIN0_bm | !PIN1_bm | !PIN2_bm; //PORTK0 and PORTK1 as input interrupts for the left and right IR sensors and PORTK2 as middle IR
	PORTK.PIN0CTRL = PORT_OPC_PULLDOWN_gc | PORT_ISC_RISING_gc;
	PORTK.PIN1CTRL = PORT_OPC_PULLDOWN_gc | PORT_ISC_RISING_gc;
	PORTK.INT0MASK = PIN0_bm; //set portK interrupt 0 to work on pin 0
	PORTK.INT1MASK = PIN1_bm; //set portK interrupt 1 to work on pin 1
	PORTK.INTCTRL = 0x05; //setting the interrupts as low level
	PORTK.PIN2CTRL = (0x30);//Sets pullup on input
	PORTK.PIN3CTRL = (0x30);//Sets pullup on input
	PORTK.PIN4CTRL = (0x30);//Sets pullup on input
	PORTK.PIN5CTRL = (0x30);//Sets pullup on input
	PORTK.PIN6CTRL = (0x30);//Sets pullup on input
	PORTK.PIN7CTRL = (0x30);//Sets pullup on input
	*/
	
	PORTK_DIR =  !PIN0_bm | !PIN1_bm | !PIN2_bm | !PIN3_bm; //portK as input 
	PORTK.PIN0CTRL = (0x30);//Sets pullup on input
	PORTK.PIN1CTRL = (0x30);//Sets pullup on input
	PORTK.PIN2CTRL = (0x30);//Sets pullup on input
	PORTK.PIN3CTRL = (0x30);//Sets pullup on input
	PORTK.PIN4CTRL = (0x30);//Sets pullup on input
	PORTK.PIN5CTRL = (0x30);//Sets pullup on input
	PORTK.PIN6CTRL = (0x30);//Sets pullup on input
	PORTK.PIN7CTRL = (0x30);//Sets pullup on input
	
	PORTA_DIR =  !PIN0_bm | !PIN1_bm | PIN2_bm | !PIN3_bm | !PIN4_bm | !PIN5_bm | !PIN6_bm | !PIN7_bm; //portA as input for ultrasonic triggers and output for amplifier DAC pin2
	PORTA.PIN0CTRL = (0x30);//Sets pullup on input
	PORTA.PIN1CTRL = (0x30);//Sets pullup on input
	PORTA.PIN3CTRL = (0x30);//Sets pullup on input
	PORTA.PIN4CTRL = (0x30);//Sets pullup on input
	PORTA.PIN5CTRL = (0x30);//Sets pullup on input
	PORTA.PIN6CTRL = (0x30);//Sets pullup on input
	PORTA.PIN7CTRL = (0x30);//Sets pullup on input
	
	PORTB_DIR = 0xFF; //set port B as output
	PORTC_DIR = 0xFF; //Set PortC as output for power LED on pin 0
	
	PORTE_DIR = PIN0_bm | PIN4_bm | !PIN2_bm  | !PIN1_bm | !PIN3_bm | !PIN5_bm | !PIN6_bm | !PIN7_bm; //Set PortE pin0 and pin4 as outputs for the pwm and pin2 as RX for RFID USART
	PORTE.PIN1CTRL = (0x30);//Sets pullup on input
	PORTE.PIN3CTRL = (0x30);//Sets pullup on input
	PORTE.PIN5CTRL = (0x30);//Sets pullup on input
	PORTE.PIN6CTRL = (0x30);//Sets pullup on input
	PORTE.PIN7CTRL = (0x30);//Sets pullup on input
	
	PORTF_DIR = PIN0_bm | PIN4_bm; //Set PortF pin0 and pin4 as outputs for the pwm
	
	PORTJ_DIR = !PIN0_bm | !PIN1_bm | !PIN2_bm | !PIN3_bm; //Set PortJ pin0, pin1, pin2, and pin3 as inputs for the buttons
	PORTJ.PIN0CTRL = (0x30);//Sets pullup on input
	PORTJ.PIN1CTRL = (0x30);//Sets pullup on input
	PORTJ.PIN2CTRL = (0x30);//Sets pullup on input
	PORTJ.PIN3CTRL = (0x30);//Sets pullup on input
	PORTJ.PIN4CTRL = (0x30);//Sets pullup on input
	PORTJ.PIN5CTRL = (0x30);//Sets pullup on input
	PORTJ.PIN6CTRL = (0x30);//Sets pullup on input
	PORTJ.PIN7CTRL = (0x30);//Sets pullup on input
	
	//pin E0 = pin 35 make pin E slave which is left side
	TCE0_CTRLA = 0x00;      // Prescaler: clk/1 -> 500Hz
	TCE0_CTRLB = 0x13;      // CCAEN override, Frequency mode
	TCE0_CCA   = slaveMotorPower;     // CHANGE Duty cycle here 50000 = 75% duty cycle -> 10000 = 15% duty cycle
	
	//pin E1 = pin 39
	TCE1_CTRLA = 0x00;      // Prescaler: clk/1 -> 500Hz
	TCE1_CTRLB = 0x13;      // CCAEN override, Frequency mode
	TCE1_CCA   = slaveMotorPower;     // CHANGE Duty cycle here 50000 = 75% duty cycle -> 10000 = 15% duty cycle
	
	//pin F0 = pin 45 make pin F master which is right side
	TCF0_CTRLA = 0x00;      // Prescaler: clk/1 -> 500Hz
	TCF0_CTRLB = 0x13;      // CCAEN override, Frequency mode
	TCF0_CCA   = masterMotorPower;     // CHANGE Duty cycle here 50000 = 75% duty cycle -> 10000 = 15% duty cycle
	
	//pin F1 = pin 49
	TCF1_CTRLA = 0x00;      // Prescaler: clk/1 -> 500Hz
	TCF1_CTRLB = 0x13;      // CCAEN override, Frequency mode
	TCF1_CCA   = masterMotorPower;     // CHANGE Duty cycle here 50000 = 75% duty cycle -> 10000 = 15% duty cycle

	//pin C4 = pin 19 for RFID PWM
	TCC1_CTRLA = 0x01;      // Prescaler: clk/1
	TCC1_CTRLB = 0x11;      // CCAEN override, Frequency mode
	TCC1_CCA   = 130;     // fFRQ = fPER / (2N CCA+1) -> 125kHz = 32MHz / (2(1) * 130+1)
}

//motor functions
void rotate_right_90(int distanceInTicks, int masterPowerInput)
{
	int totalTicks = 0;
	masterMotorPower = masterPowerInput;
	slaveMotorPower = masterPowerInput - 5; //start with small error
	leftSlaveEncoder = 0; //reset encoder values
	rightMasterEncoder = 0;	//reset encoder values
	
	while (totalTicks < distanceInTicks) //if on_off = 1 then keep motors going
	{
		//turn on motors
		TCE0_CCA   = slaveMotorPower;
		TCF0_CCA   = masterMotorPower;
		TCF0_CTRLB = 0x13;
		TCE0_CTRLB = 0x13;
		_delay_us(1);
		TCE0_CTRLA = 0x01;
		TCF0_CTRLA = 0x01;
		//_delay_us(100);
		
		error = rightMasterEncoder - leftSlaveEncoder; //find motor speed error value
		slaveMotorPower += error / fixSpeed;
		
		leftSlaveEncoder = 0; //reset encoder values 
		rightMasterEncoder = 0;	//reset encoder values 
		_delay_us(300); //delay 10ms
		totalTicks += 1;
	}
	
	//turn off motors
	TCE0_CTRLA = 0x00;
	TCF0_CTRLA = 0x00;
	_delay_us(1);
	TCE0_CTRLB = 0x00;
	TCF0_CTRLB = 0x00;
}
void rotate_left_90(int distanceInTicks, int masterPowerInput)
{
	int totalTicks = 0;
	masterMotorPower = masterPowerInput;
	slaveMotorPower = masterPowerInput - 5; //start with small error
	leftSlaveEncoder = 0; //reset encoder values
	rightMasterEncoder = 0;	//reset encoder values
	
	while (totalTicks < distanceInTicks) //if on_off = 1 then keep motors going
	{
		//turn on motors
		TCE1_CCA   = slaveMotorPower;
		TCF1_CCA   = masterMotorPower;
		TCF1_CTRLB = 0x13;
		TCE1_CTRLB = 0x13;
		_delay_us(1);
		TCE1_CTRLA = 0x01;
		TCF1_CTRLA = 0x01;
		//_delay_us(100);
		
		error = rightMasterEncoder - leftSlaveEncoder; //find motor speed error value
		slaveMotorPower += error / fixSpeed;
		
		leftSlaveEncoder = 0; //reset encoder values
		rightMasterEncoder = 0;	//reset encoder values
		_delay_us(300); //delay 10ms
		totalTicks += 1;
	}
	
	//turn off motors
	TCE1_CTRLA = 0x00;
	TCF1_CTRLA = 0x00;
	_delay_us(1);
	TCE1_CTRLB = 0x00;
	TCF1_CTRLB = 0x00;
}
void right(int distanceInTicks, int masterPowerInput)
{
	int totalTicks = 0;
	masterMotorPower = masterPowerInput;
	slaveMotorPower = 0; //turn left by making left motor much slower
	leftSlaveEncoder = 0; //reset encoder values
	rightMasterEncoder = 0;	//reset encoder values
	
	//turn on motors
	TCE0_CCA   = slaveMotorPower;
	TCF0_CCA   = masterMotorPower;
	TCF0_CTRLB = 0x13;
	TCE0_CTRLB = 0x13;
	_delay_us(1);
	TCE0_CTRLA = 0x01;
	TCF0_CTRLA = 0x01;
	
	while (totalTicks < distanceInTicks) //if on_off = 1 then keep motors going
	{
		//_delay_us(100);
		
		leftSlaveEncoder = 0; //reset encoder values
		rightMasterEncoder = 0;	//reset encoder values
		_delay_us(300); //delay 10ms
		totalTicks += 1;
	}
	
	//turn off motors
	TCE0_CTRLA = 0x00;
	TCF0_CTRLA = 0x00;
	_delay_us(1);
	TCE0_CTRLB = 0x00;
	TCF0_CTRLB = 0x00;
}
void left(int distanceInTicks, int masterPowerInput)
{
	int totalTicks = 0;
	masterMotorPower = 0; //turn right by making right motor much slower
	slaveMotorPower = masterPowerInput; //start with small error
	leftSlaveEncoder = 0; //reset encoder values
	rightMasterEncoder = 0;	//reset encoder values
	
	//turn on motors
	TCE1_CCA   = slaveMotorPower;
	TCF1_CCA   = masterMotorPower;
	TCE1_CTRLB = 0x13;
	TCF1_CTRLB = 0x13;
	_delay_us(1);
	TCF1_CTRLA = 0x01;
	TCE1_CTRLA = 0x01;
	
	while (totalTicks < distanceInTicks) //if on_off = 1 then keep motors going
	{
		//_delay_us(100);
		
		leftSlaveEncoder = 0; //reset encoder values
		rightMasterEncoder = 0;	//reset encoder values
		_delay_us(300); //delay 10ms
		totalTicks += 1; //rightMasterEncoder
	}
	
	//turn off motors
	TCF1_CTRLA = 0x00;
	TCE1_CTRLA = 0x00;
	_delay_us(1);
	TCF1_CTRLB = 0x00;
	TCE1_CTRLB = 0x00;
}
void reverse(int distanceInTicks, int masterPowerInput)
{
	int totalTicks = 0;
	masterMotorPower = masterPowerInput;
	slaveMotorPower = masterPowerInput - 5; //start with small error
	leftSlaveEncoder = 0; //reset encoder values
	rightMasterEncoder = 0;	//reset encoder values
	
	while (totalTicks < distanceInTicks) //if on_off = 1 then keep motors going
	{
		//turn on motors
		TCE0_CCA   = slaveMotorPower;
		TCF1_CCA   = masterMotorPower;
		TCF1_CTRLB = 0x13;
		TCE0_CTRLB = 0x13;
		_delay_us(1);
		TCE0_CTRLA = 0x01;
		TCF1_CTRLA = 0x01;
		//_delay_us(100);
		
		error = rightMasterEncoder - leftSlaveEncoder; //find motor speed error value
		slaveMotorPower += error / fixSpeed;
		
		leftSlaveEncoder = 0; //reset encoder values
		rightMasterEncoder = 0;	//reset encoder values
		_delay_us(100); //delay 10ms
		totalTicks += 1;
	}
	
	//turn off motors
	TCE0_CTRLA = 0x00;
	TCF1_CTRLA = 0x00;
	_delay_us(1);
	TCE0_CTRLB = 0x00;
	TCF1_CTRLB = 0x00;
}
void forward(int distanceInTicks, int masterPowerInput)
{
	int totalTicks = 0;
	masterMotorPower = masterPowerInput;
	slaveMotorPower = masterPowerInput - 5; //start with small error
	leftSlaveEncoder = 0; //reset encoder values
	rightMasterEncoder = 0;	//reset encoder values
	
	while (totalTicks < distanceInTicks) //if on_off = 1 then keep motors going
	{
		//turn on motors
		TCE1_CCA   = slaveMotorPower;
		TCF0_CCA   = masterMotorPower;
		TCF0_CTRLB = 0x13;
		TCE1_CTRLB = 0x13;
		_delay_us(1);
		TCE1_CTRLA = 0x01;
		TCF0_CTRLA = 0x01;
		//_delay_us(100);
		
		error = rightMasterEncoder - leftSlaveEncoder; //find motor speed error value
		slaveMotorPower += error / fixSpeed;
		
		leftSlaveEncoder = 0; //reset encoder values
		rightMasterEncoder = 0;	//reset encoder values
		_delay_us(100); //delay 10ms
		totalTicks += 1;
	}
	
	//turn off motors
	TCE1_CTRLA = 0x00;
	TCF0_CTRLA = 0x00;
	_delay_us(1);
	TCE1_CTRLB = 0x00;
	TCF0_CTRLB = 0x00;
}
void stop(void)
{
	//turn off motors
	TCE0_CTRLA = 0x00;
	TCF0_CTRLA = 0x00;
	_delay_us(1);
	TCE0_CTRLB = 0x00;
	TCF0_CTRLB = 0x00;
	TCE1_CTRLA = 0x00;
	TCF1_CTRLA = 0x00;
	_delay_us(1);
	TCE1_CTRLB = 0x00;
	TCF1_CTRLB = 0x00;
}

//IR and ultrasonic functions
void obstacleDetected(void)
{
	lcd_command(0x01); // clear display
	lcd_command(0x02); // cursor home
	lcd_char(capO);
	lcd_char(capB);
	lcd_char(capS);
	lcd_char(capT);
	lcd_char(capA);
	lcd_char(capC);
	lcd_char(capL);
	lcd_char(capE);
	lcd_char(capD);
	lcd_command(0xC0); // shift cursor to second row
	lcd_char(capD);
	lcd_char(capE);
	lcd_char(capT);
	lcd_char(capE);
	lcd_char(capC);
	lcd_char(capT);
	lcd_char(capE);
	lcd_char(capD);
}
void backFromObstacle(void)
{
	if (atReciever == 0)
	{
		lcd_command(0x01); // clear display
		lcd_command(0x02); // cursor home
		lcd_char(capI);
		lcd_char(capN);
		lcd_char(space);
		lcd_char(capR);
		lcd_char(capO);
		lcd_char(capU);
		lcd_char(capT);
		lcd_char(capE);
		lcd_char(space);
		lcd_char(capT);
		lcd_char(capO);
		lcd_command(0xC0); // shift cursor to second row
		lcd_char(capR);
		lcd_char(capE);
		lcd_char(capC);
		lcd_char(capE);
		lcd_char(capI);
		lcd_char(capV);
		lcd_char(capE);
		lcd_char(capR);
	}
	if (atReciever == 1)
	{
		lcd_command(0x01); // clear display
		lcd_command(0x02); // cursor home
		lcd_char(capI);
		lcd_char(capN);
		lcd_char(space);
		lcd_char(capR);
		lcd_char(capO);
		lcd_char(capU);
		lcd_char(capT);
		lcd_char(capE);
		lcd_char(space);
		lcd_char(capT);
		lcd_char(capO);
		lcd_command(0xC0); // shift cursor to second row
		lcd_char(capS);
		lcd_char(capE);
		lcd_char(capN);
		lcd_char(capD);
		lcd_char(capE);
		lcd_char(capR);
	}
}
int distance(int sensor_number)
{
	int dur = 0;
	int check1 = 0;
	int counter = 0;

	if (sensor_number == 1)
	{
		_delay_us(320);
		PORTC_OUT = 0x00; //trig pin off
		_delay_us(64); // wait
		PORTC_OUT = PIN6_bm; //trig pin on
		_delay_us(480); // wait
		PORTC_OUT = 0x00; //trig pin off
		_delay_ms(32);
		while (check1 == 0)
		{
			if ((PORTA_IN & PIN0_bm) == 0)
			{
				check1 = 1;
				dur = counter;
			}
			_delay_us(32);
			counter = counter + 1;
			if (counter > 5000) // if no signal returns then return 999
			{
				return 999;
			}
		}
		dur = dur/15; //convert time to distance
		return dur;
	}
	if (sensor_number == 2)
	{
		_delay_us(320);
		PORTB_OUT = 0x00; //trig pin off
		_delay_us(64); // wait
		PORTB_OUT = PIN1_bm; //trig pin on
		_delay_us(480); // wait
		PORTB_OUT = 0x00; //trig pin off
		_delay_ms(32);
		while (check1 == 0)
		{
			if ((PORTA_IN & PIN1_bm) == 0)
			{
				check1 = 1;
				dur = counter;
			}
			_delay_us(32);
			counter = counter + 1;
			if (counter > 5000) // if no signal returns then return 999
			{
				return 999;
			}
		}
		dur = dur/15; //convert time to distance
		return dur;
	}
	if (sensor_number == 3)
	{
		_delay_us(320);
		PORTB_OUT = 0x00; //trig pin off
		_delay_us(64); // wait
		PORTB_OUT = PIN2_bm; //trig pin on
		_delay_us(480); // wait
		PORTB_OUT = 0x00; //trig pin off
		_delay_ms(32);
		while (check1 == 0)
		{
			if ((PORTA_IN & PIN5_bm) == 0)
			{
				check1 = 1;
				dur = counter;
			}
			_delay_us(32);
			counter = counter + 1;
			if (counter > 5000) // if no signal returns then return 999
			{
				return 999;
			}
		}
		dur = dur/15; //convert time to distance
		return dur;
	}
	if (sensor_number == 4)
	{
		_delay_us(320);
		PORTB_OUT = 0x00; //trig pin off
		_delay_us(64); // wait
		PORTB_OUT = PIN3_bm; //trig pin on
		_delay_us(480); // wait
		PORTB_OUT = 0x00; //trig pin off
		_delay_ms(32);
		while (check1 == 0)
		{
			if ((PORTA_IN & PIN3_bm) == 0)
			{
				check1 = 1;
				dur = counter;
			}
			_delay_us(32);
			counter = counter + 1;
			if (counter > 5000) // if no signal returns then return 999
			{
				return 999;
			}
		}
		dur = dur/15; //convert time to distance
		return dur;
	}
	if (sensor_number == 5)
	{
		_delay_us(320);
		PORTC_OUT = 0x00; //trig pin off
		_delay_us(64); // wait
		PORTC_OUT = PIN7_bm; //trig pin on
		_delay_us(480); // wait
		PORTC_OUT = 0x00; //trig pin off
		_delay_ms(32);
		while (check1 == 0)
		{
			if ((PORTA_IN & PIN4_bm) == 0)
			{
				check1 = 1;
				dur = counter;
			}
			_delay_us(32);
			counter = counter + 1;
			if (counter > 5000) // if no signal returns then return 999
			{
				return 999;
			}
		}
		dur = dur/15; //convert time to distance
		return dur;
	}
	if (sensor_number == 6)
	{
		_delay_us(320);
		PORTC_OUT = 0x00; //trig pin off
		_delay_us(64); // wait
		PORTC_OUT = PIN5_bm; //trig pin on
		_delay_us(480); // wait
		PORTC_OUT = 0x00; //trig pin off
		_delay_ms(32);
		while (check1 == 0)
		{
			if ((PORTA_IN & PIN6_bm) == 0)
			{
				check1 = 1;
				dur = counter;
			}
			_delay_us(32);
			counter = counter + 1;
			if (counter > 5000) // if no signal returns then return 999
			{
				return 999;
			}
		}
		dur = dur/15; //convert time to distance
		return dur;
	}

	//20cm=30, 19cm=27, 18cm=24, 17cm=21, 16cm=18
	//15cm=14, 14cm=13, 13cm=9, 12cm=8, 11cm=5, 10cm=4

}
void followLine(void)
{
	int turnMore = 0;
	
	//PORTC_OUT = 0x04; //yellow
	//while (!(PORTK_IN == 0x00)) //left=0, right=0, middle=0 -> at end of line when no sensor is touching the line
	//{
	if (PORTK_IN == 0x04) //left=0, right=0, middle=1 -> go forward
	{
		forward(1, 25000);
	}
	if (PORTK_IN == 0x01) //left=1, right=0, middle=0 -> turn right
	{
		right(1, 35000);
	}
	if (PORTK_IN == 0x02) //left=0, right=1, middle=0 -> turn left
	{
		left(1, 35000);
	}
	if (PORTK_IN == 0x05) //left=1, right=0, middle=1 -> rotate left
	{
		rotate_right_90(1, 50000);
	}
	if (PORTK_IN == 0x06) //left=0, right=1, middle=1 -> rotate right
	{
		rotate_left_90(1, 50000);
	}
	if (PORTK_IN == 0x03) //left=1, right=1, middle=0 -> go forward
	{
		forward(1, 25000);
	}
	if (PORTK_IN == 0x07) //left=1, right=1, middle=1 -> go forward
	{
		forward(1, 25000);
	}
	//}
	//PORTC_OUT = 0x07; //all on
}
void avoidObstacle(void)
{
	obstacleDetected();
	int u1 = 0;
	int u2 = 0;
	int u3 = 0;
	int u4 = 0;
	int u5 = 0;
	int how = 0;
	
	stop();
	_delay_ms(3200);
	rotate_left_90(20000, 40000);
	rotate_left_90(20000, 40000);
	rotate_left_90(20000, 40000);
	rotate_left_90(15000, 40000);
	_delay_ms(3200);
	
	//while side ultrasonics still see the box go forward
	u3 = distance(3);
	_delay_ms(200);
	u4 = distance(4);
	_delay_ms(200);
	u5 = distance(5);
	_delay_ms(200);
	while ((u3 < 40) || (u4 < 40) || (u5 < 40))
	{
		forward(3500, 25000);
		u3 = distance(3);
		_delay_ms(200);
		u4 = distance(4);
		_delay_ms(200);
		u5 = distance(5);
		_delay_ms(200);
	}
	forward(30000, 25000); //go a little more forward to make sure the rover is past the obstacle
	forward(30000, 25000); //go a little more forward to make sure the rover is past the obstacle
	forward(5000, 25000); //go a little more forward to make sure the rover is past the obstacle
	
	//turn right 90 degrees
	_delay_ms(3200);
	rotate_right_90(20000, 40000);
	rotate_right_90(20000, 40000);
	rotate_right_90(18000, 40000);
	rotate_right_90(15000, 40000);
	
	//after rotating right, go forward until u3 reaches box again
	u3 = distance(3);
	_delay_ms(200);
	u4 = distance(4);
	_delay_ms(200);
	while ((u3 > 30) || (u4 > 30))
	{
		forward(2000, 25000);
		u3 = distance(3);
		_delay_ms(200);
		u4 = distance(4);
		_delay_ms(200);
	}
	_delay_ms(100);

	//go forward until the end of the box is reached from the side
	u3 = distance(3);
	_delay_ms(200);
	u4 = distance(4);
	_delay_ms(200);
	u5 = distance(5);
	_delay_ms(200);
	while ((u3 < 40) || (u4 < 40) || (u5 < 40))
	{
		forward(2000, 25000);
		u3 = distance(3);
		_delay_ms(200);
		u4 = distance(4);
		_delay_ms(200);
		u5 = distance(5);
		_delay_ms(200);
	}
	forward(30000, 25000); //go a little more forward to make sure the rover is past the obstacle
	forward(30000, 25000); //go a little more forward to make sure the rover is past the obstacle
	forward(15000, 25000); //go a little more forward to make sure the rover is past the obstacle

	//turn right 90 degrees second time
	_delay_ms(3200);
	rotate_right_90(20000, 40000);
	rotate_right_90(20000, 40000);
	rotate_right_90(18000, 40000);
	rotate_right_90(15000, 40000);
	
	//after rotating right again, go forward until the path line is read by the any IR
	backToLine = 0;
	while (backToLine == 0)
	{
		if (PORTK_IN == 0x04) //left=0, right=0, middle=1
		{
			backToLine = 1;
		}
		if (PORTK_IN == 0x01) //left=1, right=0, middle=0 -> turn right
		{
			backToLine = 1;
		}
		if (PORTK_IN == 0x02) //left=0, right=1, middle=0 -> turn left
		{
			backToLine = 1;
		}
		forward(1, 25000); 
	}
	
	stop();
	_delay_ms(30000);
	
	//rotate left to hook up middle IR with path line
	backToLine = 0;
	while (backToLine == 0)
	{
		if (PORTK_IN == 0x04) //left=0, right=0, middle=1
		{
			how = 1;
		}
		if (PORTK_IN == 0x01) //left=1, right=0, middle=0 -> turn right
		{
			how = 1;
		}
		if (PORTK_IN == 0x02) //left=0, right=1, middle=0 -> turn left
		{
			how = 1;
		}
		if (how == 1)
		{
			if (PORTK_IN == 0x00) //left=0, right=0, middle=0
			{
				backToLine = 1;
			}
		}
		rotate_left_90(50, 30000);
	}
	
	reverse(30000, 25000); //go a little more forward to make sure the rover is past the obstacle
			
	//go forward to hook up middle IR with path line
	backToLine = 0;
	while (backToLine == 0)
	{
		if (PORTK_IN == 0x04) //left=0, right=0, middle=1
		{
			backToLine = 1;
		}
		forward(1, 12500);
	}
	backFromObstacle();
}

//RFID function
// 1 stop bit, 1 start bit, 2400Hz, no parity on port E2 pin 37
void RFIDsetupUSART(void)
{
	//PORTE.DIRCLR = PIN2_bm;//make into input RX
	
	//USARTE0_CTRLA = 0x10;//enable reciever interrupt as low level
	USARTE0_CTRLB |= USART_RXEN_bm;//enable transmitter and receiver
	
	USARTE0_CTRLC |= USART_CMODE_ASYNCHRONOUS_gc; //using as UART
	USARTE0_CTRLC |= USART_PMODE_DISABLED_gc;//no parity
	USARTE0_CTRLC &= ~(USART_SBMODE_bm);//1 stop bit
	USARTE0_CTRLC |= USART_CHSIZE_8BIT_gc;//8 bits
	USARTE0_CTRLB &= ~(USART_CLK2X_bm);//no 2x speed bit
	
	//setting usart to work at 2400Hz
	#define BSCALE    1
	#define BSEL      416
	
	USARTE0.BAUDCTRLA = (uint8_t)BSEL;// 1 MHz
	USARTE0.BAUDCTRLB = (uint8_t)( (BSCALE << 4)|(BSEL>>8) );
} //using 
char inChar(){
	while(!(USARTE0_STATUS & USART_RXCIF_bm))
	{
		continue;
	}
	return USARTE0_DATA;
} 
//readcard function will return 1 for black card and 0 for white card
int readCard(void)
{
	int card = 0;
	x = 0;
	rf = 0;
	
	lcd_command(0x01); // clear display
	lcd_command(0x02); // cursor home
	lcd_char(capR);
	lcd_char(capE);
	lcd_char(capA);
	lcd_char(capD);
	lcd_char(capI);
	lcd_char(capN);
	lcd_char(capG);
	lcd_char(space);
	lcd_char(capR);
	lcd_char(capF);
	lcd_char(capI);
	lcd_char(capD);
	
	while (card == 0)
	{
		while (x < 500)
		{
			RFID_bit = inChar();
			rf = rf + RFID_bit;
			x = x + 1;
		}
	
		if (rf > 22000)
		{
			PORTC_OUT = 0x04; //w yellow
			card = 1;
			return card;
		}
		if (rf < 15000)
		{
			PORTC_OUT = 0x01; //b green
			card = 2;
			return card;
		}
		rf = 0;
		x = 0;
	}
}

//Fingerprint Sensor functions
// 1 stop bit, 1 start bit, 19,200 Hz, no parity on port E
void fingerPrint_USART(){
	PORTF.DIRCLR = PIN2_bm;//make into input
	PORTF.DIRSET = PIN3_bm;//make into output
	
	USARTF0_CTRLB |= USART_RXEN_bm | USART_TXEN_bm;//enable transmitter and receiver
	
	USARTF0_CTRLC |= USART_CMODE_ASYNCHRONOUS_gc; //using as UART
	USARTF0_CTRLC |= USART_PMODE_DISABLED_gc;//no parity
	USARTF0_CTRLC &= ~(USART_SBMODE_bm);//1 stop bit
	USARTF0_CTRLC |= USART_CHSIZE_8BIT_gc;//8 bits
	USARTF0_CTRLB &= ~(USART_CLK2X_bm);//no 2x speed bit

	#define BSCALE    1//change to 19,200 Hz
	#define BSEL      51
	
	USARTF0.BAUDCTRLA = (uint8_t)BSEL;// 1 MHz
	USARTF0.BAUDCTRLB = (uint8_t)( (BSCALE << 4)|(BSEL>>8) );
}
char fingerPrint_inChar(){
	while(!(USARTF0_STATUS & USART_RXCIF_bm))
	{
		continue;
	}
	return USARTF0_DATA;
}
void outChar(char c){
	while((USARTF0_STATUS & 0x20) == 0);
	USARTF0_DATA = c;
}
void outString(char* string){
	uint8_t i = 0;
	while(string[i] != 0x0){
		outChar(string[i++]);    // Transmit the character
	}
}
void fingerprint_sendCommand(char __fpCmd, char __fpParam1, char __fpParam2, char __fpParam3)
{
	char __fpChk = __fpCmd;
	__fpChk ^= __fpParam1;
	__fpChk ^= __fpParam2;
	__fpChk ^= __fpParam3;
	__fpChk ^= 0;
	//send order
	outChar(0xF5);
	outChar(__fpCmd);//commands
	outChar(__fpParam1);//command parameters
	outChar(__fpParam2);
	outChar(__fpParam3);
	outChar(0);
	outChar(__fpChk);//Xor of variables
	outChar(0xF5);
	
}
//fp response is matrix of size 8 that
void fingerprint_readResponse(char *__fpResponse)
{
	char __read_fp[8];
	char __fpChk = 0;
	for(char idx = 0; idx < 8; idx++)
	{
		__read_fp[idx] = fingerPrint_inChar();//receive function;
		if(idx > 0 && idx < 6) __fpChk ^= __read_fp[idx];//
		if(idx > 1 && idx < 5) __fpResponse[idx-2] = __read_fp[idx];
	}
	__fpResponse[4] = 0;
	if(__read_fp[0] == 0xF5 && __read_fp[7] == 0xF5 && __read_fp[6] == __fpChk) __fpResponse[5] = 0xF5;
}
int fingerprint_allowOverwrite(char b)
{
	char __fpBuf[8];
	char __allowOverwrite = 0;
	if(b == 0) __allowOverwrite = 1;
	fingerprint_sendCommand(CMD_SET_MODE, 0, __allowOverwrite, 0);
	fingerprint_readResponse(__fpBuf);
	return (int) __fpBuf[2];
}
int fingerprint_add(int userId, char userLevel, int scanNumber)
{
	userLevel = 0x3 & userLevel;//set user level
	char __u2 = userId & 0xFF;//high bits of the user ID
	char __u1 = (userId >> 8) & 0xFF;//low bits of the user ID
	char __fpBuf[8];//matrix
	
	if(userId < 1) return ACK_NOUSER;//doesn't exist
	else {
		if(scanNumber < 1 || scanNumber > 3) {//adding all three scans of the finger
			fingerprint_sendCommand(CMD_ADD_FINGERPRINT_1, __u1, __u2, userLevel);//replacing fingerprint number 1
			fingerprint_readResponse(__fpBuf);//waiting for response
			
			if(__fpBuf[2] != ACK_SUCCESS) return (int) __fpBuf[2];//if successful move on
			_delay_ms(150000);         // delay 1.5s;//wait some time(need help on)
			fingerprint_sendCommand(CMD_ADD_FINGERPRINT_2, __u1, __u2, userLevel);//repeat
			fingerprint_readResponse(__fpBuf);
			if(__fpBuf[2] != ACK_SUCCESS) return (int) __fpBuf[2];
			_delay_ms(150000);         // delay 1.5s//need to wait some time
			fingerprint_sendCommand(CMD_ADD_FINGERPRINT_3, __u1, __u2, userLevel);
			fingerprint_readResponse(__fpBuf);
			} else if(scanNumber == 1) {//replace scan
			fingerprint_sendCommand(CMD_ADD_FINGERPRINT_1, __u1, __u2, userLevel);
			fingerprint_readResponse(__fpBuf);
			} else if(scanNumber == 2) {
			fingerprint_sendCommand(CMD_ADD_FINGERPRINT_2, __u1, __u2, userLevel);
			fingerprint_readResponse(__fpBuf);
			} else if(scanNumber == 3) {
			fingerprint_sendCommand(CMD_ADD_FINGERPRINT_3, __u1, __u2, userLevel);
			fingerprint_readResponse( __fpBuf);
		}
		
		return (int) __fpBuf[4];
	}
}
int fingerprint_deleteUser(int userId)
{
	char __u2 = userId & 0xFF;//getting user ID
	char __u1 = (userId >> 8) & 0xFF;
	char __fpBuf[8];
	
	if(userId < 1) {//delete all users
		fingerprint_sendCommand(CMD_DELETE_ALL_USERS, 0, 0, 0);
		fingerprint_readResponse(__fpBuf);
		} else {//delete one user
		fingerprint_sendCommand(CMD_DELETE_USER, __u1, __u2, 0);
		fingerprint_readResponse(__fpBuf);
	}
	
	return (int) __fpBuf[4];
}
int fingerprint_scan(int userId, int *uid)//use it to test
{
	char __u2 = userId & 0xFF;//user ID
	char __u1 = (userId >> 8) & 0xFF;
	char __fpBuf[8];
	int idTemp = 0;//temporary
	*uid = 0;
	
	if(userId < 1) {
		fingerprint_sendCommand(CMD_SCAN_COMPARE_1_TO_N, 0, 0, 0); //compare with the user who has 0 for their id(no negative numbers)
		fingerprint_readResponse( __fpBuf);
		*uid = ((((int) __fpBuf[0]) << 8) | ((int) __fpBuf[1]));//id of current user doesn't change
		} else {
		fingerprint_sendCommand(CMD_SCAN_COMPARE_1_TO_1, __u1, __u2, 0);//compare to user with some number
		fingerprint_readResponse(__fpBuf);
		if(__fpBuf[2] == ACK_SUCCESS) {//waiting for success
			*uid = userId;
			fingerprint_sendCommand(CMD_READ_USER_PRIVLAGE, __u1, __u2, 0);
			fingerprint_readResponse(__fpBuf);
		}
	}
	
	return (int) __fpBuf[2];
}
int fingerprint_countUsers()
{
	char __fpBuf[8];
	
	fingerprint_sendCommand(CMD_GET_USERS_COUNT, 0, 0, 0);
	fingerprint_readResponse( __fpBuf);
	
	if (__fpBuf[2] == 0x00) return ((((int) __fpBuf[0]) << 8) | ((int) __fpBuf[1]));
	else return -1;
}
void addFingerPrint(void)
{
	fingerprint_deleteUser(0); //zero means to delete all users
	
	//ON, OFF, ON, OFF, ON, OFF TO START CODE
	PORTC_OUT = 0x01;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x00;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x01;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x00;

	lcd_command(0x01); // clear display
	lcd_command(0x02); // cursor home
	lcd_char(capA);
	lcd_char(capD);
	lcd_char(capD);
	lcd_char(space);
	lcd_char(capF);
	lcd_char(capI);
	lcd_char(capN);
	lcd_char(capG);
	lcd_char(capE);
	lcd_char(capR);
	lcd_command(0xC0); // shift cursor to second row
	
	lcd_char(capS);
	lcd_char(capC);
	lcd_char(capA);
	lcd_char(capN);
	lcd_char(colon);
	lcd_char(space);
	lcd_char(one);
	lcd_char(slash);
	lcd_char(three);
	addscan1 = fingerprint_add(uid, 3, 1);
	
	PORTC_OUT = 0x01;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x00;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x01;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x00;

	lcd_command(0x01); // clear display
	lcd_command(0x02); // cursor home
	lcd_char(capA);
	lcd_char(capD);
	lcd_char(capD);
	lcd_char(space);
	lcd_char(capF);
	lcd_char(capI);
	lcd_char(capN);
	lcd_char(capG);
	lcd_char(capE);
	lcd_char(capR);
	lcd_command(0xC0); // shift cursor to second row
	
	lcd_char(capS);
	lcd_char(capC);
	lcd_char(capA);
	lcd_char(capN);
	lcd_char(colon);
	lcd_char(space);
	lcd_char(two);
	lcd_char(slash);
	lcd_char(three);
	addscan2 = fingerprint_add(uid, 3, 2);

	
	PORTC_OUT = 0x01;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x00;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x01;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x00;

	lcd_command(0x01); // clear display
	lcd_command(0x02); // cursor home
	lcd_char(capA);
	lcd_char(capD);
	lcd_char(capD);
	lcd_char(space);
	lcd_char(capF);
	lcd_char(capI);
	lcd_char(capN);
	lcd_char(capG);
	lcd_char(capE);
	lcd_char(capR);
	lcd_command(0xC0); // shift cursor to second row
	
	lcd_char(capS);
	lcd_char(capC);
	lcd_char(capA);
	lcd_char(capN);
	lcd_char(colon);
	lcd_char(space);
	lcd_char(three);
	lcd_char(slash);
	lcd_char(three);
	addscan3 = fingerprint_add(uid, 3, 3);	
	
	PORTC_OUT = 0x01;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x00;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x01;
	_delay_ms(5000);         // delay 250 ms
	PORTC_OUT = 0x00;
}
//check finger print, if correct return 1, if wrong return 0
int checkFingerPrint(void)
{
	checkscan1 = fingerprint_scan(uid, uidpoint);
	if (checkscan1 == 3)
	{
		return 1;
		//PORTC_OUT = 0x01; //green - unlock box
		//PORTH_OUT = PIN2_bm; //unlock
		//_delay_ms(40000);    //unlock time
		//PORTH_OUT = !PIN2_bm; //lock
	}
	else
	{
		//PORTC_OUT = 0x02; //red - wrong fingerprint
		return 0;
	}
}
void openSecurityBox(void)
{
	PORTH_OUT = PIN2_bm; //unlock
	_delay_ms(40000);    //unlock time
	PORTH_OUT = !PIN2_bm; //lock
}

//Every time the encoder hits a rising edge increment the encoder tick counters
ISR(PORTH_INT0_vect)
{
	leftSlaveEncoder = leftSlaveEncoder + 1;
}
ISR(PORTH_INT1_vect)
{
	rightMasterEncoder = rightMasterEncoder + 1;
}

void atSender(void)
{
	lcd_command(0x01); // clear display
	lcd_command(0x02); // cursor home
	lcd_char(capS);
	lcd_char(capE);
	lcd_char(capN);
	lcd_char(capD);
	lcd_char(capE);
	lcd_char(capR);
	lcd_char(space);
	lcd_char(capF);
	lcd_char(capO);
	lcd_char(capU);
	lcd_char(capN);
	lcd_char(capD);
	
	//returned to sender and finshed program
	lcd_command(0xC0); // shift cursor to second row
	lcd_char(capV);
	lcd_char(capO);
	lcd_char(capF);
	lcd_char(capA);
	lcd_char(capD);
	lcd_char(space);
	lcd_char(capC);
	lcd_char(capO);
	lcd_char(capM);
	lcd_char(capP);
	lcd_char(capL);
	lcd_char(capE);
	lcd_char(capT);
	lcd_char(capE);
	while(1){} //do nothing until the program restarts
}
void finalAlgorithmStart();
void finalAlgorithmReturn(void)
{
	int scanTires = 0; 

	while (scanTires < 3)
	{
		if (scanTires == 0)
		{
			PORTC_OUT = 0x04; //yellow - check fingerprint
			lcd_command(0x01); // clear display
			lcd_command(0x02); // cursor home
			lcd_char(capR);
			lcd_char(capE);
			lcd_char(capC);
			lcd_char(capE);
			lcd_char(capI);
			lcd_char(capV);
			lcd_char(capE);
			lcd_char(capR);
			lcd_char(space);
			lcd_char(capF);
			lcd_char(capO);
			lcd_char(capU);
			lcd_char(capN);
			lcd_char(capD);
			playAlarm(200);
			
			lcd_command(0x01); // clear display
			lcd_command(0x02); // cursor home
			lcd_char(capR);
			lcd_char(capE);
			lcd_char(capC);
			lcd_char(capE);
			lcd_char(capI);
			lcd_char(capV);
			lcd_char(capE);
			lcd_char(capR);
			lcd_char(space);
			lcd_char(capF);
			lcd_char(capO);
			lcd_char(capU);
			lcd_char(capN);
			lcd_char(capD);
			lcd_command(0xC0); // shift cursor to second row
			lcd_char(capS);
			lcd_char(capC);
			lcd_char(capA);
			lcd_char(capN);
			lcd_char(space);
			lcd_char(one);
			lcd_char(slash);
			lcd_char(three);
		}
		
		if (scanTires == 1)
		{
			PORTC_OUT = 0x04; //yellow - check fingerprint
			lcd_command(0x01); // clear display
			lcd_command(0x02); // cursor home
			lcd_char(capR);
			lcd_char(capE);
			lcd_char(capC);
			lcd_char(capE);
			lcd_char(capI);
			lcd_char(capV);
			lcd_char(capE);
			lcd_char(capR);
			lcd_char(space);
			lcd_char(capF);
			lcd_char(capO);
			lcd_char(capU);
			lcd_char(capN);
			lcd_char(capD);
			lcd_command(0xC0); // shift cursor to second row
			lcd_char(capS);
			lcd_char(capC);
			lcd_char(capA);
			lcd_char(capN);
			lcd_char(space);
			lcd_char(two);
			lcd_char(slash);
			lcd_char(three);
		}

		if (scanTires == 2)
		{
			PORTC_OUT = 0x04; //yellow - check fingerprint
			lcd_command(0x01); // clear display
			lcd_command(0x02); // cursor home
			lcd_char(capR);
			lcd_char(capE);
			lcd_char(capC);
			lcd_char(capE);
			lcd_char(capI);
			lcd_char(capV);
			lcd_char(capE);
			lcd_char(capR);
			lcd_char(space);
			lcd_char(capF);
			lcd_char(capO);
			lcd_char(capU);
			lcd_char(capN);
			lcd_char(capD);
			lcd_command(0xC0); // shift cursor to second row
			lcd_char(capS);
			lcd_char(capC);
			lcd_char(capA);
			lcd_char(capN);
			lcd_char(space);
			lcd_char(three);
			lcd_char(slash);
			lcd_char(three);
		}

		rightFinger = checkFingerPrint();
		if (rightFinger == 1)
		{
			PORTC_OUT = 0x01; //green - unlock box
			openSecurityBox();
			scanTires = 3;
		}
		else
		{
			PORTC_OUT = 0x02; //red - wrong fingerprint
			scanTires = scanTires + 1;
		}
		_delay_ms(30000);
	}
	
	lcd_command(0x01); // clear display
	lcd_command(0x02); // cursor home
	lcd_char(capB);
	lcd_char(capU);
	lcd_char(capT);
	lcd_char(capT);
	lcd_char(capO);
	lcd_char(capN);
	lcd_char(one);
	lcd_char(space);
	lcd_char(capT);
	lcd_char(capO);
	lcd_char(space);
	lcd_char(capS);
	lcd_char(capE);
	lcd_char(capN);
	lcd_char(capD);
	lcd_command(0xC0); // shift cursor to second row
	lcd_char(capV);
	lcd_char(capO);
	lcd_char(capF);
	lcd_char(capA);
	lcd_char(capD);
	lcd_char(space);
	lcd_char(capB);
	lcd_char(capA);
	lcd_char(capC);
	lcd_char(capK);
	
	while(!((PORTJ_IN & 0x10) == 16))
	{
		//add time delay if button is not pressed
	}
	lcd_command(0x01); // clear display
	lcd_command(0x02); // cursor home
	lcd_char(capI);
	lcd_char(capN);
	lcd_char(space);
	lcd_char(capR);
	lcd_char(capO);
	lcd_char(capU);
	lcd_char(capT);
	lcd_char(capE);
	lcd_char(space);
	lcd_char(capT);
	lcd_char(capO);
	lcd_command(0xC0); // shift cursor to second row
	lcd_char(capS);
	lcd_char(capE);
	lcd_char(capN);
	lcd_char(capD);
	lcd_char(capE);
	lcd_char(capR);
	
	reverse(30000, 25000); //reverse a little so the IRs dont hit the RFID card again
	rotate_left_90(20000, 40000);
	rotate_left_90(20000, 40000);
	rotate_left_90(20000, 40000);
	rotate_left_90(20000, 40000);
	rotate_left_90(20000, 40000);
	rotate_left_90(10000, 40000);
	//turn rover around until middle IR hits the line
	while (!(PORTK_IN == 0x04))
	{
		rotate_left_90(1, 30000);
	}
	
	//once middle IR hits, start following path again
	finalAlgorithmStart();
}
void startProgram(void)
{
	addFingerPrint();
	
	lcd_command(0x01); // clear display
	lcd_command(0x02); // cursor home
	lcd_char(capB);
	lcd_char(capU);
	lcd_char(capT);
	lcd_char(capT);
	lcd_char(capO);
	lcd_char(capN);
	lcd_char(one);
	lcd_char(space);
	lcd_char(capT);
	lcd_char(capO);
	lcd_char(space);
	lcd_char(capS);
	lcd_char(capE);
	lcd_char(capN);
	lcd_char(capD);
	lcd_command(0xC0); // shift cursor to second row
	lcd_char(capV);
	lcd_char(capO);
	lcd_char(capF);
	lcd_char(capA);
	lcd_char(capD);
	lcd_char(space);
	lcd_char(capT);
	lcd_char(capO);
	lcd_char(space);
	lcd_char(capP);
	lcd_char(capA);
	lcd_char(capT);
	lcd_char(capH);

}
void finalAlgorithmStart(void)
{
	PORTC_OUT = 0x01; //green
	while(1)
	{	
		//check for obstacle, if detected avoid obstacle with yellow on then back to line following with green on
		if (why == 3000)
		{
			ultraSonic1 = distance(1);
			ultraSonic2 = distance(2);
			ultraSonic6 = distance(6);
			if (((ultraSonic1 < 15) && (ultraSonic1 > 3)) || ((ultraSonic2 < 15) && (ultraSonic2 > 3)) || ((ultraSonic6 < 15) && (ultraSonic6 > 3))) //if less than 10cm then avoid obstacle
			{
				PORTC_OUT = 0x04; //yellow
				avoidObstacle();
				PORTC_OUT = 0x01; //green
			}
			why = 0;
		}
		
		//check for all white, which is RFID card 
		if (PORTK_IN == 0x00)
		{
			PORTC_OUT = 0x02; //red
			forward(17000, 30000); //go forward a little to get past white part of RFID card
			why2 = 0;
			while (why2 < 30000)
			{
				followLine();
				why2 = why2 + 1;
			}
		
			RFIDreadCard = readCard(); //read RFID Card
			if (RFIDreadCard == 2) // if receiver is reached then activate second half of program - Black card
			{
				atReciever = atReciever + 1;
				if (atReciever == 2)
				{
					atSender();
				}
				finalAlgorithmReturn();
			}
			
			if (RFIDreadCard == 1) //if sender card is reached exit program
			{
				atSender();
			}
	}
	
	followLine(); //continue on line
	why = why + 1;
	}
}

int main(void)
{
	ClockFreqSet();
	lcd_init();
	pinSetup();
	RFIDsetupUSART();
	fingerPrint_USART();
	DAC_INIT();
	PMIC.CTRL |= PMIC_LOLVLEN_bm; //enable low level interrupts
	sei(); //enable all interrupts
	
	lcd_command(0x01); // clear display
	lcd_command(0x02); // cursor home
	lcd_char(space);
	lcd_char(space);
	lcd_char(space);
	lcd_char(space);
	lcd_char(space);
	lcd_char(space);
	lcd_char(capV);
	lcd_char(capO);
	lcd_char(capF);
	lcd_char(capA);
	lcd_char(capD);
	lcd_command(0xC0); // shift cursor to second row
	lcd_char(capB);
	lcd_char(capU);
	lcd_char(capT);
	lcd_char(capT);
	lcd_char(capO);
	lcd_char(capN);
	lcd_char(one);
	lcd_char(space);
	lcd_char(capT);
	lcd_char(capO);
	lcd_char(space);
	lcd_char(capB);
	lcd_char(capE);
	lcd_char(capG);
	lcd_char(capI);
	lcd_char(capN);
	
	PORTC_OUT = 0x00;
	
	while (1)
	{
		_delay_ms(1);
		if ((PORTJ_IN & 0x10) == 16) //forward
		{
			startProgram();
			while(1)
			{
				if ((PORTJ_IN & 0x10) == 16) //forward
				{
					lcd_command(0x01); // clear display
					lcd_command(0x02); // cursor home
					lcd_char(capI);
					lcd_char(capN);
					lcd_char(space);
					lcd_char(capR);
					lcd_char(capO);
					lcd_char(capU);
					lcd_char(capT);
					lcd_char(capE);
					lcd_char(space);
					lcd_char(capT);
					lcd_char(capO);
					lcd_command(0xC0); // shift cursor to second row
					lcd_char(capR);
					lcd_char(capE);
					lcd_char(capC);
					lcd_char(capE);
					lcd_char(capI);
					lcd_char(capV);
					lcd_char(capE);
					lcd_char(capR);
					finalAlgorithmStart();
				}
			}
		}
		
		if ((PORTJ_IN & 0x02) == 2) //play alarm
		{
			playAlarm(115);
		}
		
		if ((PORTJ_IN & 0x04) == 4)
		{
			int sens1 = distance(3);
			int sens2 = distance(4);
			lcd_command(0x01); // clear display
			lcd_command(0x02); // cursor home
			lcd_char(capS);
			lcd_char(capE);
			lcd_char(capN);
			lcd_char(capS);
			lcd_char(one);
			lcd_char(colon);
			numPrintLCD(sens1);
			lcd_command(0xC0); // shift cursor to second row
			lcd_char(capS);
			lcd_char(capE);
			lcd_char(capN);
			lcd_char(capS);
			lcd_char(two);
			lcd_char(colon);
			numPrintLCD(sens2);
		}
		
		if ((PORTJ_IN & 0x08) == 8) //resets encoder counters
		{
						int sens6 = distance(6);
						lcd_command(0x01); // clear display
						lcd_command(0x02); // cursor home
						lcd_char(capS);
						lcd_char(capE);
						lcd_char(capN);
						lcd_char(capS);
						lcd_char(six);
						lcd_char(colon);
						numPrintLCD(sens6);
		}
	}
}


