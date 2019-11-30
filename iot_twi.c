//
//  iot_twi.c
//  IOT_RV
//
//  Created by Luke Neumann on 9/5/19.
//  Copyright © 2019 Luke Neumann. All rights reserved.
//
//#include <util/twi.h>
#include "iot_twi.h"
// #include <avr/io.h>
// #include <stdbool.h>
// #include <util/delay.h>

void TWI_masterInit()
{
    TWBR = 10;
    TWSR = 0x00;
//    TWAR = 0x02;
//    TWAMR = 0x00;
    TWCR = (1 << TWEA) | (1 << TWEN);
}

bool TWI_start(){
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // Send START condition
    while (!(TWCR & (1<<TWINT))); // Wait for TWINT Flag set. This indicates that the START condition has been transmitted
    if ((TWSR & 0xF8) != TW_START){ //Check value of TWI Status Register. Mask prescaler bits. If status different from START go to ERROR
        return false;
    }
    return true;
}

bool TWI_repeated_start(){
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // Send START condition
    while (!(TWCR & (1<<TWINT))); // Wait for TWINT Flag set. This indicates that the START condition has been transmitted
    if ((TWSR & 0xF8) != TW_REP_START){ //Check value of TWI Status Register. Mask prescaler bits. If status different from START go to ERROR
        return false;
    }
    return true;
}

bool TWI_SLA(int8_t address, bool write){
    if (write) { // check whether we are reading or writing.
        TWDR = address<<1; //Load SLA_W into TWDR Register.
    }
    else {
        TWDR = ((address<<1) | 0x01); //Load SLA_R into TWDR Register.
    }
    TWCR = (1<<TWINT) | (1<<TWEN); // Clear TWINT bit in TWCR to start transmission of address
    while (!(TWCR & (1<<TWINT))); //Wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
    if (write) {
        if ((TWSR & 0xF8) != TW_MT_SLA_ACK){ //Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
            return false;
        }
    }
    else {
        if ((TWSR & 0xF8) != TW_MR_SLA_ACK){ //Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
            return false;
        }
    }  
    return true;
}

bool TWI_beginTransmission(int8_t address, bool write){
    if (!TWI_start()) { // check for successful start
        return false;
    }
    if (!TWI_SLA(address, write)) {
        return false;
    }
    return true;
}

int16_t TWI_read(bool last_byte){
    int16_t data = 0;
    if (last_byte) {
        TWCR = (1<<TWINT) | (1<<TWEN); //Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
    }
    else {
        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); //Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
    }
    
    while (!(TWCR & (1<<TWINT))); //Wait for TWINT Flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received.
    
    if (last_byte) {
        if(TW_STATUS == TW_MR_DATA_NACK){
            data = TWDR;
        }
    }
    else {
        if(TW_STATUS == TW_MR_DATA_ACK){
            data = TWDR;
        }

    }
    
    return data;
}

bool TWI_write(int8_t DATA){
    TWDR = DATA;
    TWCR = (1<<TWINT) | (1<<TWEN); //Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
    
    while (!(TWCR & (1<<TWINT))); //Wait for TWINT Flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received.
    
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK){ // Check value of TWI Status Register. Mask prescaler bits. If status different from MT_DATA_ACK go to ERROR
        return false;
    }
    return true;
}

void TWI_endTransmission(){
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //Transmit STOP condition
}


