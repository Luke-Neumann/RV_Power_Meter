//
//  iot_twi.h
//  IOT_RV
//
//  Created by Luke Neumann on 9/5/19.
//  Copyright © 2019 Luke Neumann. All rights reserved.
//

#ifndef iot_twi_h
#define iot_twi_h

#include <stdio.h>
#include <util/twi.h>
#include <stdbool.h>
#include <avr/io.h>





void TWI_masterInit();
bool TWI_start();
bool TWI_repeated_start();
bool TWI_SLA(int8_t address, bool write);
bool TWI_beginTransmission(int8_t address, bool write);
int16_t TWI_read(bool last_byte);
bool TWI_write(int8_t DATA);
void TWI_endTransmission();





#endif /* iot_twi_h */