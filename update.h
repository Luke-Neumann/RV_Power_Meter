//
//  update.h
//  two input adc test
//
//  Created by Luke Neumann on 1/8/20.
//

#ifndef update_h
#define update_h

#include <stdio.h>
#include "iot_uart.h"
#include <util/delay.h>
#include "rn4870.h"

bool set_server_address(char * server_address, char * received);
bool set_server_address_type(char * server_address_type, char * received);
bool check_characteristic_server_address(char * received);
bool check_characteristic_server_address_type(char * received);


#endif /* update_h */
