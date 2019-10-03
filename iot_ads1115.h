//
//  iot_ads1115.h
//  IOT_RV
//
//  Created by Luke Neumann on 9/24/19.
//  Copyright © 2019 Luke Neumann. All rights reserved.
//

#ifndef iot_ads1115_h
#define iot_ads1115_h


#define PROG_GAIN_AMP_CONFIG_0 0.256
#define PROG_GAIN_AMP_CONFIG_1 0.512
#define PROG_GAIN_AMP_CONFIG_2 1.024
#define PROG_GAIN_AMP_CONFIG_3 2.048
#define PROG_GAIN_AMP_CONFIG_4 4.096
#define PROG_GAIN_AMP_CONFIG_5 6.144

#define ADS1115 0x48 // address of the module it is 0x48 because the address pin is set to ground. There are 3 other options
#define CONVERSION_REGISTER 0x00 // address for the conversion register on the ADS1115.
#define CONFIG_REGISTER 0x01 // address for the configuration register on the ADS1115
#define LO_THRESH_REGISTER 0x02 // address for the configuration register on the ADS1115
#define HI_THRESH_REGISTER 0x03 // address for the configuration register on the ADS1115


#include <stdio.h>
//#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "iot_twi.h"

void set_config_register(int8_t upper_bits, int8_t lower_bits);
int16_t get_conversion_register();
void set_Hi_thresh_register(int8_t upper_bits, int8_t lower_bits);
void set_Lo_thresh_register(int8_t upper_bits, int8_t lower_bits);


#endif /* iot_ads1115_h */
