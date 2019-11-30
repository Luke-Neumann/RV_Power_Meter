//
//  iot_external_interrupts.h
//  two input adc test
//
//  Created by Luke Neumann on 10/2/19.
//

#ifndef iot_external_interrupts_h
#define iot_external_interrupts_h

#include <stdio.h>
#include <avr/io.h>

void enable_interrupt_INT0();
void disable_interrupt_INT0();
void enable_interrupt_INT5();

void disable_interrupt_INT5();

void enable_interrupt_PCINT0();

void disable_interrupt_PCINT0();

#endif /* iot_external_interrupts_h */
