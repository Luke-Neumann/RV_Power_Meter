//
//  iot_uart.h
//  IOT_RV
//
//  Created by Luke Neumann on 9/5/19.
//  Copyright © 2019 Luke Neumann. All rights reserved.
//

#ifndef iot_uart_h
#define iot_uart_h

#include <stdio.h>
//#include <avr/io.h>
#include <stdbool.h>


void USART_Init( unsigned int baud );
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( );

#endif /* iot_uart_h */
