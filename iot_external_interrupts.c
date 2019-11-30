//
//  iot_external_interrupts.c
//  two input adc test
//
//  Created by Luke Neumann on 10/2/19.
//

#include "iot_external_interrupts.h"

void enable_interrupt_INT0() {
    EICRA = EICRA | 0x00; //set the interrupt to trigger on a falling edge.
    EIMSK = EIMSK | 0x01; // this enables the int0 interrupt pin.
    SREG = SREG | 0x80; // this turns on global interrupts in the status register
}

void disable_interrupt_INT0() {
    SREG = SREG & 0x5F; // disable global interrupts by setting the highest pin in the status
    EIMSK = EIMSK & 0xFE; // this disables the int0 interrupt pin.
}

void enable_interrupt_INT5() {
    EICRB = EICRB | 0x08; //set the interrupt to trigger on a falling edge.
    EIMSK = EIMSK | 0x20; // this enables the int5 interrupt pin.
    SREG = SREG | 0x80; // this turns on global interrupts in the status register
}

void disable_interrupt_INT5() {
    SREG = SREG & 0x5F; // disable global interrupts by setting the highest pin in the status
    EIMSK = EIMSK & 0xFE; // this disables the int0 interrupt pin.
}

void enable_interrupt_PCINT0() {
    // not finished yet
    PCICR = PCICR | 0x01; // this enables the int0 interrupt pin.
    SREG = SREG | 0x80; // this turns on global interrupts in the status register
}

void disable_interrupt_PCINT0() {
    SREG = SREG & 0x5F; // disable global interrupts by setting the highest pin in the status
    PCICR = 0x00; // this enables the int0 interrupt pin.
}
