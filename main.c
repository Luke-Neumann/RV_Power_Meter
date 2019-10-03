/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
#define F_CPU 8000000    // AVR clock frequency in Hz, used by util/delay.h
#define ACUTAL_CLK_F 7333333.0
#define BAUD_RATE 115200
#define BAUD_RATE_REGISTER (F_CPU/(16*BAUD_RATE))-1 // formula to set the baud rate register

#define GAIN 5.1
#define R1 150000 // ideally should be 150k
#define R2 11090 // ideally should be 11k
#define VOLTAGE_DIVIDER R2/(R1+R2) // the goal is t0 get at divition of 2.048/36 which is about 0.056
#define SHUNT_VOLTAGE_LIMIT 0.05 // 500 amps gives 50 mv
#define SHUNT_AMP_LIMIT 200
#define BIAS 0 // for grounded shunt

#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>
#include "iot_uart.h"
#include "iot_twi.h"
#include "iot_timer.h"
#include "iot_ads1115.h"
#include <avr/interrupt.h>
#include "iot_external_interrupts.h"


const float VPS = PROG_GAIN_AMP_CONFIG_3/ (32768.0*VOLTAGE_DIVIDER); // volts per step. Use this conversion in place of Amps per step.
const float MVPS = PROG_GAIN_AMP_CONFIG_0*1000/ (32768.0*GAIN); // mili volts per step. Use this conversion in place of Amps per step.
const float APS = PROG_GAIN_AMP_CONFIG_0*SHUNT_AMP_LIMIT / (32768.0*SHUNT_VOLTAGE_LIMIT*GAIN); // volts per step then multiplied by converion factor to get out amps per step (7.8125uV/step)*(200amps/(gain*0.05V))


uint16_t overFlowCount = 0;

ISR(INT5_vect) // this is for the external interrupt pin PB0
{
    fast_mode();
}


ISR(TIMER0_OVF_vect)
{
    ++overFlowCount;
}


void stopAndPrintTimer0(){
	uint8_t endTime = 0;
	int16_t upper_bits = 0;
    int16_t lower_bits = 0;
    float Time = 0.0;
    
    char text[30] = "\t\t\t\t\tTimer: ";
    char text1[30] = " ms\r";


    stop_timer0();


    endTime = TIM16_ReadTCNT0();
    TIM16_WriteTCNT0(0x00); // reset the timer to 0


    Time = 8.0*(overFlowCount*256.0+endTime)*1000.0/ACUTAL_CLK_F;

    uart_print_string(text);
    //uart_print_int(endTime);
    uart_print_float(Time);
    uart_print_string(text1);


    overFlowCount = 0;


}


int16_t process1(){
	int16_t data = 0;
    //start_timer0();
    set_config_register(0x8a, 0x83); // reads the data from a0 and a1
    //stopAndPrintTimer0();
    data=get_conversion_register();
    

    return data;
}

int16_t process2(){
    set_config_register(0xb4, 0x83); // reads the data from a2
    return get_conversion_register();
}

int16_t process3(){

    return get_conversion_register();
}



void generate_message(){
    char text0[8] = "Shunt: ";
    char text1[15] = " mV, Battery: ";
    char text2[5] = " V\r";
    float shunt_voltage;
    float battery_voltage;

    
    shunt_voltage = (float)process1();
	//start_timer0();
    battery_voltage = (float)process2();
//stopAndPrintTimer0();
    shunt_voltage *= MVPS;
    battery_voltage *= VPS;

    uart_print_string(text0);
    uart_print_float(shunt_voltage);
    uart_print_string(text1);
    uart_print_float(battery_voltage);
    uart_print_string(text2);

}


void fast_mode(){

//    uart_print_int(process3());
//    uart_print_string("\r");
    char text0[8] = "Shunt: ";
    char text2[6] = " mV\r";
    float shunt_voltage;

    shunt_voltage = (float)process3();


    shunt_voltage *= MVPS;

    uart_print_string(text0);
    uart_print_float(shunt_voltage);
    uart_print_string(text2);


}





int main(void)
{

    USART_Init(BAUD_RATE_REGISTER); // initialize the baudrate of the uart


    set_Hi_thresh_register(0x80, 0x00); 
    set_Lo_thresh_register(0x00, 0x00); 
    set_config_register(0x8c, 0x00);

    enable_interrupt_INT5();

    DDRD = 1 << 4;  /* make the LED pin an output */
    PORTD ^= 1 << 4;
    //DDRD = 0 << 5;  /* make the LED pin an output */

    /* insert your hardware initialization here */
    while(1){
        /* insert your main loop code here */
        _delay_ms(40);  /* max is 262.14 ms / F_CPU in MHz */
        //generate_message();
        PORTD ^= 1 << 4;    /* toggle the LED */
        //start_timer0();
        //generate_message();
        // while ((PORTD & (1<<5)))
        // {

        //     uart_print_string("test\r");

        // }
        //fast_mode();

        

        _delay_ms(40);  /* max is 262.14 ms / F_CPU in MHz */
		//stopAndPrintTimer0();
        PORTD ^= 1 << 4; // flip the state of the 4th pin of the D register. this turns an led on and off.
    }
    return 0;   /* never reached */
}
