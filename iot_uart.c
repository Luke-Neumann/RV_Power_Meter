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
    /* Enable receiver and transmitter */
    UCSR1B = (1<<(RXEN1))|(1<<TXEN1);
    /* Set frame format: 8data, 2stop bit */
    UCSR1C = (1<<USBS1)|(3<<UCSZ10);
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
