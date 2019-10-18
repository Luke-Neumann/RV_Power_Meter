//
//  rn4870.h
//  two input adc test
//
//  Created by Luke Neumann on 10/5/19.
//

#ifndef rn4870_h
#define rn4870_h

#include <stdio.h>
#include "iot_uart.h"
#include <util/delay.h>
#include <string.h>

bool enter_command_mode(char * address);
//bool verify_command_mode(char * address);
void exit_command_mode();

void get_BLE_info();

void reboot_device();

void get_signal_strength();
void connect_to_device(char * address, char * type);

#endif /* rn4870_h */
