//
//  iot_twi.c
//  IOT_RV
//
//  Created by Luke Neumann on 9/5/19.
//  Copyright © 2019 Luke Neumann. All rights reserved.
//

/*
	the settings for this timer are pretty basic they run the timer on normal mode
	the TNC0 register will overflow and be reset by the hardware and when this happends
	the it will be reset but the TIMSK0 has enabled the interrupt for an overflow. 
	this makes it possible to count each time register has reached the 256 limit.
*/


#include "iot_timer.h"

unsigned int TIM16_ReadTCNT0( void )
{
	unsigned char sreg;
	unsigned int i;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
    SREG = SREG & 0x5F; // disable global interrupts by setting the highest pin in the status register to 0
	//TIMSK0 = 0x00; //disable interrupt
	/* Read TCNTn into i */
	i = TCNT0;
	/* Restore global interrupt flag */
	SREG = sreg;
	return i;
}

void TIM16_WriteTCNT0( unsigned int i )
{
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
    SREG = SREG & 0x5F; // disable global interrupts by setting the highest pin in the status register to 0
	//TIMSK0 = 0x00; //disable interrupt
	/* Set TCNTn to i */
	TCNT0 = i;
	/* Restore global interrupt flag */
	SREG = sreg;
}

void start_timer0(){
	SREG = SREG | 0x80; // this turns on global interrupts in the status register
	TIMSK0 = 0x01; // enables overflow interrupt for timer 0
	TCCR0A = 0x00; // timer/counter control register A
	TCCR0B = 0x02; // timer/counter control register B
}

void stop_timer0(){
	TCCR0B = 0x00; // timer/counter control register B
	SREG = SREG & 0x5F; // disable global interrupts by setting the highest pin in the status register to 0
}

void start_16_bit_timer1(){
    SREG = SREG | 0x80; // this turns on global interrupts in the status register
    TIMSK1 = 0x01; // enables overflow interrupt for timer 0
    TCCR1A = 0x00; // timer/counter control register A
    TCCR1B = 0x02; // timer/counter control register B
}

void stop_16_bit_timer1(){
    SREG = SREG & 0x5F; // disable global interrupts by setting the highest pin in the status register to 0
    TCCR1B = 0x00; // timer/counter control register B
    TIMSK1 = 0x00; //disable interrupt
}

uint16_t TIM16_ReadTCNT1()
{
    unsigned char sreg;
    uint16_t i = 0;
    uint16_t temp = 0;
    /* Save global interrupt flag */
    sreg = SREG;
    /* Disable interrupts */
    SREG = SREG & 0x5F; // disable global interrupts by setting the highest pin in the status register to 0
    /* Read TCNTn into i */
    i = TCNT1L; // read low byte first
    temp = TCNT1H; // read high byte second.
    i = (temp<<8) | i;
    /* Restore global interrupt flag */
    SREG = sreg;
    return i;
}

void TIM16_WriteTCNT1( uint16_t i )
{
    unsigned char sreg;
    /* Save global interrupt flag */
    sreg = SREG;
    /* Disable interrupts */
    SREG = SREG & 0x5F; // disable global interrupts by
    /* Set TCNTn to i */
    TCNT1H = (uint8_t)((i&0xff00)>>8); // write high byte first
    TCNT1L = (uint8_t)(i&0x00ff); // write low byte second.
    /* Restore global interrupt flag */
    SREG = sreg;
}

