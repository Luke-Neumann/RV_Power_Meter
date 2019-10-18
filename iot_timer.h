//
//  iot_twi.h
//  IOT_RV
//
//  Created by Luke Neumann on 9/5/19.
//  Copyright © 2019 Luke Neumann. All rights reserved.
//

#ifndef iot_timer_h
#define iot_timer_h


#include <stdio.h>

#include <stdbool.h>
#include <avr/io.h>



unsigned int TIM16_ReadTCNT0( void );
void TIM16_WriteTCNT0( unsigned int i );
void start_timer0();
void stop_timer0();
void start_16_bit_timer1();
void stop_16_bit_timer1();
uint16_t TIM16_ReadTCNT1();
void TIM16_WriteTCNT1( uint16_t i );
//void PrintTimer1();
//void stopAndPrintTimer0();
//void stopAndPrintTimer1();







#endif /* iot_timer_h */
