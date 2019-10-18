//
//  iot_uart.c
//  IOT_RV
//
//  Created by Luke Neumann on 9/5/19.
//  Copyright © 2019 Luke Neumann. All rights reserved.
//

#include "iot_uart.h"
//#include <avr/io.h>


void USART_Init( unsigned int baud )
{
    /* Set baud rate */
    UBRR1H = (unsigned char)(baud>>8);
    UBRR1L = (unsigned char)baud;
    // enable the reciver interrupts
    //UCSR1A = 0;//(1<<(RXC1));
    /* Enable receiver and transmitter  and enable reciver interrupt*/
    UCSR1B = (1<<(RXEN1))|(1<<TXEN1|(1<<RXCIE1));
    /* Set frame format: 8data, 2stop bit */
    UCSR1C = (1<<USBS1)|(3<<UCSZ10);
    SREG = SREG | 0x80; // this turns on global interrupts in the status
}

void USART_Transmit( unsigned char data )
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR1A & (1<<UDRE1)) )
        ;
    /* Put data into buffer, sends the data */
    UDR1 = data;
}


unsigned char USART_Receive( )
{
   /* Wait for data to be received */
   while ( !(UCSR1A & (1<<RXC1)) )
       ;
   /* Get and return received data from buffer */
   return UDR1;
}

void uart_print_int(int16_t data){
    char dataString[20]; 
    uint8_t i = 0;
    sprintf(dataString, "%d", data); // converts the current count iteration to a string
    while(dataString[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(dataString[i]); // send the current count to the bluetooth module
        i++;
    }
}

void uart_print_HEX(int16_t data){
    char dataString[20];
    uint8_t i = 0;
    sprintf(dataString, "%02x", data); // converts the current count iteration to a string
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


void uart_print_string(char *text){
    while(*text != '\0'){ // loop till the null character is reached.
        USART_Transmit(*text); // send the current count to the bluetooth module
        text++;
    }

}
