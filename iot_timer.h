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



#endif /* iot_timer_h */
