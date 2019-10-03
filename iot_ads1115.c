//
//  iot_ads1115.c
//  IOT_RV
//
//  Created by Luke Neumann on 9/24/19.
//  Copyright © 2019 Luke Neumann. All rights reserved.
//

#include "iot_ads1115.h"
//#include <avr/io.h>

void set_config_register(int8_t upper_bits, int8_t lower_bits){
    TWI_beginTransmission(ADS1115, true);
    TWI_write(CONFIG_REGISTER); // address of the config register
    TWI_write(upper_bits);
    TWI_write(lower_bits);
    TWI_endTransmission();
    _delay_ms(15);
}

void set_Hi_thresh_register(int8_t upper_bits, int8_t lower_bits){
    TWI_beginTransmission(ADS1115, true);
    TWI_write(HI_THRESH_REGISTER); // address of the config register
    TWI_write(upper_bits);
    TWI_write(lower_bits);
    TWI_endTransmission();
    _delay_ms(15);
}


void set_Lo_thresh_register(int8_t upper_bits, int8_t lower_bits){
    TWI_beginTransmission(ADS1115, true);
    TWI_write(LO_THRESH_REGISTER); // address of the config register
    TWI_write(upper_bits);
    TWI_write(lower_bits);
    TWI_endTransmission();
    _delay_ms(15);
}




int16_t get_conversion_register(){

    int16_t upper_bits = 0;
    int16_t lower_bits = 0;

    // prepares to read the data by pointing to the conversion register.
    TWI_beginTransmission(ADS1115, true);
    TWI_write(CONVERSION_REGISTER);
    TWI_endTransmission();
    //start_timer0();
    TWI_beginTransmission(ADS1115, false);
    upper_bits = TWI_read(false) << 8;
    lower_bits = TWI_read(true);
    TWI_endTransmission();
    //topAndPrintTimer0();
    upper_bits |= lower_bits; // This masks bits 8 to 15 and adds the rest of the data to bits 0 through 7.
    return upper_bits;
}