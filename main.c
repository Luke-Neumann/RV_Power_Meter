/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
#define F_CPU 8000000    // AVR clock frequency in Hz, used by util/delay.h
#define BAUD_RATE 115200
#define BAUD_RATE_REGISTER (F_CPU/(16*BAUD_RATE))-1 // formula to set the baud rate register

#define GAIN 5.1
#define R1 55060 // ideally should be 56k
#define R2 3259 // ideally should be 3.3k
#define VOLTAGE_DIVIDER R2/(R1+R2) // the goal is t0 get at divition of 2.048/36 which is about 0.056
#define SHUNT_VOLTAGE_LIMIT 0.05 // 500 amps gives 50 mv
#define SHUNT_AMP_LIMIT 200
#define PROG_GAIN_AMP_CONFIG_0 0.256
#define PROG_GAIN_AMP_CONFIG_1 0.512
#define PROG_GAIN_AMP_CONFIG_2 1.024
#define PROG_GAIN_AMP_CONFIG_3 2.048
#define PROG_GAIN_AMP_CONFIG_4 4.096
#define PROG_GAIN_AMP_CONFIG_5 6.144
#define ADS1115 0x48 // address of the module it is 0x48 because the address pin is set to ground. There are 3 other options
#define BIAS 0 // for grounded shunt

#define CONVERSION_REGISTER 0x00 // address for the conversion register on the ADS1115.
#define CONFIG_REGISTER 0x01 // address for the configuration register on the ADS1115


#include <stdbool.h>
#include <util/twi.h>
#include <util/delay.h>
#include <avr/io.h>
#include "iot_uart.h"
#include "iot_twi.h"




const float VPS = PROG_GAIN_AMP_CONFIG_3/ (32768.0*VOLTAGE_DIVIDER); // volts per step. Use this conversion in place of Amps per step.
const float MVPS = PROG_GAIN_AMP_CONFIG_0*1000/ (32768.0*GAIN); // mili volts per step. Use this conversion in place of Amps per step.
const float APS = PROG_GAIN_AMP_CONFIG_0*SHUNT_AMP_LIMIT / (32768.0*SHUNT_VOLTAGE_LIMIT*GAIN); // volts per step then multiplied by converion factor to get out amps per step (7.8125uV/step)*(200amps/(gain*0.05V))

char success[30] = "init success"; // holds some text
char error[30] = "ERROR!"; // holds some text
char data_array[30] = "Testing "; // holds some text
char str[10]; // holds the count
char str1[50]; // holds the count
uint16_t count = 0;



void set_config_register(int8_t upper_bits, int8_t lower_bits){
    TWI_beginTransmission(ADS1115, true);
    TWI_write(CONFIG_REGISTER); // address of the config register
    TWI_write(upper_bits);
    TWI_write(lower_bits);
    TWI_endTransmission();
    _delay_ms(50);
}


int16_t read_ADC(){

    int16_t upper_bits = 0;
    int16_t lower_bits = 0;

    // prepares to read the data by pointing to the conversion register.
    TWI_beginTransmission(ADS1115, true);
    TWI_write(CONVERSION_REGISTER);
    TWI_endTransmission();

    TWI_beginTransmission(ADS1115, false);
    upper_bits = TWI_read(false) << 8;
    lower_bits = TWI_read(true);
    TWI_endTransmission();

    upper_bits |= lower_bits; // This masks bits 8 to 15 and adds the rest of the data to bits 0 through 7.
    return upper_bits;
}

void uart_print_int(int16_t data){
    char dataString[20]; 
    sprintf(dataString, "%d", data); // converts the current count iteration to a string

    uint8_t i = 0;
    while(dataString[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(dataString[i]); // send the current count to the bluetooth module
        i++;
    }
}

void uart_print_float(float data){
    char dataString[20];

    char *tmpSign = (data < 0) ? "-" : "";
    float tmpVal = (data < 0) ? -data : data;
    int tmpInt1 = tmpVal;                  // Get the integer (678).
    float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
    int tmpInt2 = trunc(tmpFrac * 10000);  // Turn into integer (123).
    
    // Print as parts, note that you need 0-padding for fractional bit.
    
    sprintf (dataString, "%s%d.%04d", tmpSign, tmpInt1, tmpInt2);

    uint8_t i = 0;
    while(dataString[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(dataString[i]); // send the current count to the bluetooth module
        i++;
    }
}

void uart_print_string(char *text){//, int var2){
    uint8_t i = 0;
    while(*text != '\0'){ // loop till the null character is reached.
        USART_Transmit(*text); // send the current count to the bluetooth module
        //i++;
        text++;
    }

}

int16_t process1(){
    set_config_register(0x8a, 0x83); // reads the data from a0 and a1
    return read_ADC();
}

int16_t process2(){
    set_config_register(0xb4, 0x83); // reads the data from a2
    return read_ADC();
}

void generate_message(){
    char text0[8] = "Shunt: ";
    char text1[15] = " mV, Battery: ";
    char text2[5] = " V\r";
    float shunt_voltage;
    float battery_voltage;

    shunt_voltage = (float)process1();
    battery_voltage = (float)process2();

    shunt_voltage *= MVPS;
    battery_voltage *= VPS;

    uart_print_string(text0);
    uart_print_float(shunt_voltage);
    uart_print_string(text1);
    uart_print_float(battery_voltage);
    uart_print_string(text2);

}



int main(void)
{
    USART_Init(BAUD_RATE_REGISTER); // initialize the baudrate of the uart
    
    DDRD = 1 << 4;  /* make the LED pin an output */
    PORTD ^= 1 << 4;
    /* insert your hardware initialization here */
    while(1){
        /* insert your main loop code here */
        _delay_ms(40);  /* max is 262.14 ms / F_CPU in MHz */
        PORTD ^= 1 << 4;    /* toggle the LED */
        _delay_ms(40); // add a 1 second delay
        generate_message();
        PORTD ^= 1 << 4; // flip the state of the 4th pin of the D register. this turns an led on and off.
    }
    return 0;   /* never reached */
}
