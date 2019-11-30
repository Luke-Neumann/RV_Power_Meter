/* Name: main.c
 * Author: <Luke Neumann>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
#define F_CPU 8000000    // AVR clock frequency in Hz, used by util/delay.h
#define ACUTAL_CLK_F 8000000 //7300000.0 // This is used for getting the correct frequency for the timer formula.
#define BAUD_RATE 115200 // This can be any other baud rate.
#define BAUD_RATE_REGISTER 3//(F_CPU/(16*BAUD_RATE))-1 // formula to set the baud rate register

// battery sensing info
#define R1 150000 // ideally should be 150k. Used for dividing VCC
#define R2 11090 // ideally should be 11k. Used for dividing VCC
// the goal is to get a voltage division ratio of 2.048/30 which is about 0.068323
// This will allow for VCC to go up to 30 volts without going out of the range of the ADC.
#define VOLTAGE_DIVIDER 0.068171//R2/(R1+R2)

// current sensing info
#define GAIN 5.1 // This is the gain of the op amp that feeds into the shunt. It is used to calculate the voltage
#define SHUNT_VOLTAGE_LIMIT 0.05 // for example 200 amps gives 50 mv
#define SHUNT_AMP_LIMIT 200 // This is the shunts rated amperage.
#define BIAS 0.0 //0.16677 // Can be used to correct biased data.


#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>
#include "iot_uart.h"
#include "iot_twi.h"
#include "iot_timer.h"
#include "iot_ads1115.h"
#include <avr/interrupt.h>
#include "iot_external_interrupts.h"
#include "rn4870.h"
#include <string.h>

uint16_t overFlowCount = 0;
uint16_t overFlowCount1 = 0;

char received[512] = "";
char device_address[20] = "801F12B4BDBF";
char server_address[20] = "801F12B4BDCE";
char server_address_type[5] = "0";

const float VPS = PROG_GAIN_AMP_CONFIG_3/ (32768.0*VOLTAGE_DIVIDER); // volts per step. Use this conversion in place of Amps per step.
const float MVPS = PROG_GAIN_AMP_CONFIG_0*1000/ (32768.0*GAIN); // mili volts per step. Use this conversion in place of Amps per step.
const float APS = PROG_GAIN_AMP_CONFIG_0*SHUNT_AMP_LIMIT / (32768.0*SHUNT_VOLTAGE_LIMIT*GAIN); // Amps per step (7.8125uV/step)*(200amps/(gain*0.05V))


void fast_mode();


ISR(INT5_vect) // this is for the external interrupt pin PE5s
{
    fast_mode(); // run a simple test function to read and print ADC data quickly
}
ISR(USART1_RX_vect) // not working yet!
{
    //uart_print_string("TRUE");
    char temp[5] = "";
    temp[0] = USART_Receive();
    memcpy(received+strlen(received), temp, strlen(temp));
}
ISR(TIMER0_OVF_vect) // interrupt for timer 0
{
    ++overFlowCount; // increment value when the timer register overflows and is reset to zero.
}
ISR(TIMER1_OVF_vect) // interrupt for timer 1
{
    ++overFlowCount1; // increment value when the timer register overflows and is reset to zero.
}

// This function will simplify the process to read and print data from timer 1
void PrintTimer1(){
    uint16_t endTime = 0; // This is the final value read from the clock register when the timer is stopped.
    double Time = 0.0; // This is for holding the completed time calculation.
    char text[30] = "\tTimer1: "; // text for printing before the time.
    char text1[30] = " s\r"; // text for printing after the time.
    endTime = TIM16_ReadTCNT1(); // read from the time register.
    Time = 8.0*(overFlowCount1*65536.0+endTime)/ACUTAL_CLK_F; // convert the number into seconds.
    uart_print_string(text);
    uart_print_float(Time);
    uart_print_string(text1);
}
// This function will simplify the process to read and print data from timer 0
void stopAndPrintTimer0(){
    stop_timer0(); // stop timer zero from running
    uint8_t endTime = 0; // This is the final value read from the clock register when the timer is stopped.
//    int16_t upper_bits = 0;
//    int16_t lower_bits = 0;
    float Time = 0.0; // This is for holding the completed time calculation.
    char text[30] = "\t\t\t\t\tTimer0: ";// text for printing before the time.
    char text1[30] = " ms\r"; // text for printing after the time.

    endTime = TIM16_ReadTCNT0(); // read from the time register.
    TIM16_WriteTCNT0(0x00); // reset the timer register to 0
    Time = 8.0*(overFlowCount*256.0+endTime)*1000.0/ACUTAL_CLK_F; // convert the number into seconds.
    uart_print_string(text); // print messages.
    uart_print_float(Time);
    uart_print_string(text1);
    overFlowCount = 0; // reset the overflow count.
}

void stopAndPrintTimer1(){
    stop_16_bit_timer1(); // stop timer 1 from running
    uint16_t endTime = 0; // This is the final value read from the clock register when the timer is stopped.
//    int16_t upper_bits = 0;
//    int16_t lower_bits = 0;
    float Time = 0.0; // This is for holding the completed time calculation.
    char text2[30] = " end time ";
    char text3[30] = " overflow ";
    char text[30] = "\tTimer1: ";
    char text1[30] = " ms\r";

    endTime = TIM16_ReadTCNT1();// read from the time register.
    TIM16_WriteTCNT1(0); // reset the timer to 0
    Time = 8.0*(overFlowCount1*65536.0+endTime)*1000.0/ACUTAL_CLK_F; // convert the number into seconds.
    uart_print_string(text2); // print messages
    uart_print_int(endTime);
    uart_print_string(text3);
    uart_print_int(overFlowCount1);
    uart_print_string(text);
    uart_print_float(Time);
    uart_print_string(text1);
    overFlowCount1 = 0; // reset the overflow count.
}

int16_t process3(){ // because this is used for fast mode there is no need to reset the config register
    return get_conversion_register(); // return the data.
}


// prints by reading only 1 input of ADC which makes it possible
// to sample faster than a 16 ms period.
void fast_mode(){
    char text0[8] = "Shunt: ";
    char text2[6] = " mV\r";
    float shunt_voltage = 0.0;
    shunt_voltage = process3();  // (float)process3(); // read in a value and cast it to a float
    shunt_voltage *= APS; // convert to milivolts
    shunt_voltage += BIAS; // convert to milivolts
    uart_print_string(text0); // print message
    uart_print_float(shunt_voltage);
    uart_print_string(text2);
}



void start_fast_mode(){ // initiate a fast mode.
    uint8_t upper_bits = generate_upper_config_param(OS_HIGH, MUX_0, FSR_5, MODE_0); // enter config values
    uint8_t lower_bits = generate_lower_config_param(SPS_0, COMP_MODE_0, COMP_POL_0, COMP_LAT_0, COMP_QUE_0);
    set_Hi_thresh_register(0x80, 0x00); // set the MSB high to activate the config ready alert pin
    set_Lo_thresh_register(0x00, 0x00); // set the MSB Low to activate the config ready alert pin
    set_config_register(upper_bits, lower_bits); // set the config register
    enable_interrupt_INT5(); // turn on the external interrupt pin.
}

void stop_fast_mode(){
    disable_interrupt_INT5(); // disable the external interrupt
}

int16_t process1(){
    set_config_register(0x8a, 0x83); // sets ADC to read the data from pins a0 and a1
    return get_conversion_register(); // read and return values
}

int16_t process2(){
    set_config_register(0xb4, 0x83); // sets ADC to read the data from pins a2 and a3
    return get_conversion_register(); // read and return values
}



void generate_message(){ // reads both values from the ADC and prints them.
    char text0[8] = "Shunt: ";
    char text1[15] = " A, Battery: ";
    char text2[5] = " V\r";
    float shunt_voltage;
    float battery_voltage;
    shunt_voltage = process1(); //(float)process1();
    battery_voltage = process2(); // (float)process2();
    shunt_voltage *= APS; // convert value to milivolts
    shunt_voltage += BIAS; // convert value to milivolts
    battery_voltage *= VPS; // convert value to volts
    uart_print_string(text0); // print message.
    uart_print_float(shunt_voltage);
    uart_print_string(text1);
    uart_print_float(battery_voltage);
    uart_print_string(text2);
}



void read_pin_test(){ // used for testing the pin inputs from the BLE module to get the status of the module
    char text1[15] = "\t\tst1: ";
    char text2[15] = ":";
    char text3[15] = "\r";
    uart_print_string(text1);
    uart_print_int((PIND & 0x20)>>5); // reads pd5 and shifts the bit to the right
    uart_print_string(text2);
    uart_print_int((PIND & 0x40)>>6);
    uart_print_string(text3);
}


//int8_t convert_hex_to_db(char * signal_strength){
//
//
//    if(signal_strength[0] == "-"){
//        char *tmpSign = "-";
//    }
//    else{
//        char *tmpSign = "";
//    }
//    float tmpVal = (data < 0) ? -data : data;
//    int tmpInt1 = tmpVal;                  // Get the integer (678).
//    float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
//    int tmpInt2 = trunc(tmpFrac * 10000);  // Turn into integer (123).
//
//    // Print as parts, note that you need 0-padding for fractional bit.
//
//    sprintf (dataString, "%s%d", tmpSign, tmpInt1);
//}

int main(void)
{
    char signal_strength[10] = "";
    
    
    
    OSCCAL = 0x57; // this determines the frequency of the rc ocilator
    _delay_ms(1000);  /* max is 262.14 ms / F_CPU in MHz */
    

    //char text[50] = "\r\rCommand\rMode\rSuccessful\r\r\r";
    //char incoming_data[10];
    bool  success = 0;
    bool  success1 = 0;
    int count_test = 0;

    //string text1 = "TEST\r\r\r\r";

    USART_Init(BAUD_RATE_REGISTER); // initialize the baudrate of the uart
    
    uart_print_string("\r\r\r\r\r\rbooting up\r\r\r");

    //_delay_ms(3000);  /* max is 262.14 ms / F_CPU in MHz */
    //start_fast_mode();
    //start_16_bit_timer1();

    DDRD = 1 << 4;  /* make the LED pin an output */
//PORTD = (1 << 5)||(1<<6);
    PORTD ^= 1 << 4;
    PORTE = (1<<5); // this activates the internal pull up resistor for pe5
    //DDRD = 0 << 5;  /* make the LED pin an output */
    
    enable_interrupt_PCINT0();
    
    _delay_ms(500);  /* max is 262.14 ms / F_CPU in MHz */
    
    //uart_print_string("\r\r\r\r\r\r\r\rStarting1\r");
    
    
    
    while(!enter_command_mode(received)){
        memset(received, 0, sizeof(received)); // clear the buffer
    }
    memset(received, 0, sizeof(received));
    _delay_ms(200);
//    uart_print_string(received);
//    uart_print_string("\r");


    connect_to_device(server_address, server_address_type);
    
    
    
    _delay_ms(6000);  /* max is 262.14 ms / F_CPU in MHz */
    memset(received, 0, sizeof(received));
//    while(!exit_command_mode(received)){
//        memset(received, 0, sizeof(received)); // clear the buffer
//    }
//    memset(received, 0, sizeof(received));
//

    
    
    
    //uart_print_string("TEST2\r");
    //start_fast_mode();
    
    //char * pointer = 0;
    //success = enter_command_mode(received);

    /* insert your hardware initialization here */
    while(1){
        /* insert your main loop code here */
//        _delay_ms(300);  /* max is 262.14 ms / F_CPU in MHz */
//        PORTD ^= 1 << 4;    /* toggle the LED */
        //read_pin_test();

       // generate_message();
        
        uart_print_string("Count: ");
        uart_print_int(count_test);
        uart_print_string("\r");
        _delay_ms(500);
        while(!enter_command_mode(received)){
            memset(received, 0, sizeof(received)); // clear the buffer
        }
        memset(received, 0, sizeof(received));
        //success = true;
//////
//        //incoming_data[0] = USART_Receive();
        //_delay_ms(500);  /* max is 262.14 ms / F_CPU in MHz */
////        //get_BLE_info();
        get_signal_strength(received, signal_strength);
        
////////
////////        //incoming_data[1] = USART_Receive();
////
////        _delay_ms(100);
//////
        _delay_ms(500);  /* max is 262.14 ms / F_CPU in MHz */
        memset(received, 0, sizeof(received));
        while(!exit_command_mode(received)){
            memset(received, 0, sizeof(received)); // clear the buffer
        }
        memset(received, 0, sizeof(received));
        //success1=true;
        
        count_test++;
        
        
          /* max is 262.14 ms / F_CPU in MHz */

        //uart_print_string(pointer);
        uart_print_string("received: ");
        uart_print_string(received);
        uart_print_string("\r");
        uart_print_string("signal strength: ");
        uart_print_string(signal_strength);
        uart_print_string(" db\r");
        
//        if (success) {
//            success = false;
//            uart_print_string(text);
//        }
        
        //connect_to_device(server_address, server_address_type);
		
        //PORTD ^= 1 << 4; // flip the state of the 4th pin of the D register. this turns an led on and off.
    }
    return 0;   /* never reached */
}
