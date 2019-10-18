//
//  iot_uart.h
//  IOT_RV
//
//  Created by Luke Neumann on 9/5/19.
//  Copyright © 2019 Luke Neumann. All rights reserved.
//

// This code handles the uart communication for the at90usb1286 avr microcontroller

#ifndef iot_uart_h
#define iot_uart_h

#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>


void USART_Init( unsigned int baud );
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( );
void uart_print_int(int16_t data);
void uart_print_float(float data);
void uart_print_string(char * text);
void uart_print_HEX(int16_t data);

#endif /* iot_uart_h */
