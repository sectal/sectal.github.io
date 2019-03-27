/*
 * function.h
 *
 *  Created on: Jul 20, 2018
 *      Author: sebas
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

void lcd_command(char);
void lcd_char(char);
void dac_write(char data);
char digitToChar(int digit);
int n_tu(int number, int count);
void float_to_string(float f, char r[]);
int n_tu(int number, int count);
void oscill_init();
void init_ADC();
void input_PORTS();

#endif /* FUNCTION_H_ */
