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

// Operational status or single-shot conversion start
#define OS_LOW 0x00 // When writing: No effect, When reading: Device is currently performing a conversion
#define OS_HIGH 0x01 // When writing: Start a single conversion (when in power-down state), When reading: Device is not currently performing a conversion

// input multiplexer configuration
#define MUX_0 0x00 // AINP = AIN0 and AINN = AIN1 (default)
#define MUX_1 0x01 // AINP = AIN0 and AINN = AIN3
#define MUX_2 0x02 // AINP = AIN1 and AINN = AIN3
#define MUX_3 0x03 // AINP = AIN2 and AINN = AIN3
#define MUX_4 0x04 // AINP = AIN0 and AINN = GND
#define MUX_5 0x05 // AINP = AIN1 and AINN = GND
#define MUX_6 0x06 // AINP = AIN2 and AINN = GND
#define MUX_7 0x07 // AINP = AIN3 and AINN = GND

// Programmable gain amplifier configuration
#define FSR_0 0x00 // ±6.144 V
#define FSR_1 0x01 // ±4.096 V
#define FSR_2 0x02 // ±2.048 V (default)
#define FSR_3 0x03 // ±1.024 V
#define FSR_4 0x04 // ±0.512 V
#define FSR_5 0x05 // ±0.256 V
#define FSR_6 0x06 // ±0.256 V
#define FSR_7 0x07 // ±0.256 V

// Device operating mode
#define MODE_0 0x00 // Continuous-conversion mode
#define MODE_1 0x01 // Single-shot mode or power-down state (default)

// Data rate
#define SPS_0 0x00 // 8 SPS
#define SPS_1 0x01 // 16 SPS
#define SPS_2 0x02 // 32 SPS
#define SPS_3 0x03 // 64 SPS
#define SPS_4 0x04 // 128 SPS (default)
#define SPS_5 0x05 // 250 SPS
#define SPS_6 0x06 // 475 SPS
#define SPS_7 0x07 // 860 SPS

// Comparator mode (ADS1114 and ADS1115 only)
#define COMP_MODE_0 0x00 //  Traditional comparator (default)
#define COMP_MODE_1 0x01 // Window comparator

// Comparator polarity (ADS1114 and ADS1115 only)
#define COMP_POL_0 0x00 //  Active low (default)
#define COMP_POL_1 0x01 // Active high

// Latching comparator (ADS1114 and ADS1115 only)
#define COMP_LAT_0 0x00 //  Nonlatching comparator
#define COMP_LAT_1 0x01 // Latching comparator

// Comparator queue and disable (ADS1114 and ADS1115 only)
#define COMP_QUE_0 0x00 //  Assert after one conversion
#define COMP_QUE_1 0x01 //  Assert after two conversions
#define COMP_QUE_2 0x02 //  Assert after four conversions
#define COMP_QUE_3 0x03 //  Disable comparator and set ALERT/RDY pin to high-impedance (default)

#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>
#include "iot_twi.h"

void set_config_register(int8_t upper_bits, int8_t lower_bits);
int16_t get_conversion_register();
void set_Hi_thresh_register(int8_t upper_bits, int8_t lower_bits);
void set_Lo_thresh_register(int8_t upper_bits, int8_t lower_bits);
void run_ads1115_test();
uint8_t generate_upper_config_param(uint8_t OS, uint8_t MUX, uint8_t PGA, uint8_t MODE);
uint8_t generate_lower_config_param(uint8_t DR, uint8_t COMP_MODE,uint8_t COMP_POL, uint8_t COMP_LAT, uint8_t COMP_QUE);

#endif /* iot_ads1115_h */
