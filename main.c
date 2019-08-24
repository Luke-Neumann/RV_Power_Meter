/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
#define F_CPU 8000000    // AVR clock frequency in Hz, used by util/delay.h
#define BAUD_RATE 115200
#define BAUD_RATE_REGISTER (F_CPU/(16*BAUD_RATE))-1 // formula to set the baud rate register

#define GAIN 5.1
#define SHUNT_VOLTAGE_LIMIT 0.05 // 500 amps gives 50 mv
#define SHUNT_AMP_LIMIT 200
#define PROG_GAIN_AMP_CONFIG 0.256
#define ADS1115 0x48 // address of the module it is 0x48 because the address pin is set to ground. There are 3 other options
#define BIAS 0 // for grounded shunt



#include <stdbool.h>
#include <util/twi.h>
#include <util/delay.h>
#include <avr/io.h>





const float MVPS = PROG_GAIN_AMP_CONFIG*1000/ (32768.0*GAIN); // mili volts per step. Use this conversion in place of Amps per step.
const float APS = PROG_GAIN_AMP_CONFIG*SHUNT_AMP_LIMIT / (32768.0*SHUNT_VOLTAGE_LIMIT*GAIN); // volts per step then multiplied by converion factor to get out amps per step (7.8125uV/step)*(200amps/(gain*0.05V))

char success[30] = "init success"; // holds some text
char error[30] = "ERROR!"; // holds some text
char data_array[30] = "Testing "; // holds some text
char str[10]; // holds the count
char str1[50]; // holds the count
uint16_t count = 0;




//bool check_if_device_is_connected(){
//    // use command d and parse the data and return the result
//}
//
//
//bool connect_BLE_device( char & address){
//    //check if device is already connected to this address then return true
//    // if device is connected to another address then disconnect from that address k,1
//
//    // use the c,<addr> command via uart.
//
//    // listen for correct response the AOK response
//    // if response is bad (Err) try again. use command z to give up.
//    // if response is good then return true
//
//}
//
//bool bond_devices(){
//    // check if device is connected
//    // if device is not connected then return false
//    // if device is connected then send the command b to bond.
//    // if response is AOK then return true else false.
//}
//
//void scan_and_search_for_address(){
//    // use command f and parse the data
//}
//
//void set_default_services(char & hex_data){
//    // use command ss to set the services
//}
//
//void set_private_service(char & uuid){
//    // use command ps with a custom uuid
//}
//
//void set_private_characteristics(char & uuid){
//    // use command pc
//}




void USART_Init( unsigned int baud )
{
    /* Set baud rate */
    UBRR1H = (unsigned char)(baud>>8);
    UBRR1L = (unsigned char)baud;
    /* Enable receiver and transmitter */
    UCSR1B = (1<<(RXEN1))|(1<<TXEN1);
    /* Set frame format: 8data, 2stop bit */
    UCSR1C = (1<<USBS1)|(3<<UCSZ10);
}

// sends data to the uart register
void USART_Transmit( unsigned char data )
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR1A & (1<<UDRE1)) )
        ;
    /* Put data into buffer, sends the data */
    UDR1 = data;
}

unsigned char USART_Receive( )
{
    /* Wait for data to be received */
    while ( !(UCSR1A & (1<<RXC1)) )
        ;
    /* Get and return received data from buffer */
    return UDR1;
}

void ERROR(char idc){
    int8_t i = 0; // reset the index
    while(error[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(error[i]); // send the text to the bluetooth module
        i++;
    }
    USART_Transmit(' '); // send carrage return to the bluetooth module
    USART_Transmit(idc); // send carrage return to the bluetooth module
    USART_Transmit('\r'); // send carrage return to the bluetooth module
}
void successful(char id){
    int8_t i = 0; // reset the index
    while(success[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(success[i]); // send the text to the bluetooth module
        i++;
    }
    USART_Transmit(' '); // send carrage return to the bluetooth module
    USART_Transmit(id); // send carrage return to the bluetooth module
    USART_Transmit('\r'); // send carrage return to the bluetooth module
}

void print_test(){
    uint8_t i = 0;
    
    i = 0; // reset the index
    while(data_array[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(data_array[i]); // send the text to the bluetooth module
        i++;
    }
    i = 0;
    while(str[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(str[i]); // send the current count to the bluetooth module
        i++;
    }
    
    USART_Transmit('\r'); // send carrage return to the bluetooth module
    
    
}









bool TWI_start(){
    
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // Send START condition
    
    while (!(TWCR & (1<<TWINT))); // Wait for TWINT Flag set. This indicates that the START condition has been transmitted
    
    if ((TWSR & 0xF8) != TW_START){ //Check value of TWI Status Register. Mask prescaler bits. If status different from START go to ERROR
        ERROR('A');
        return false;
    }

    return true;
    
}

bool TWI_repeated_start(){
    
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // Send START condition
    
    while (!(TWCR & (1<<TWINT))); // Wait for TWINT Flag set. This indicates that the START condition has been transmitted
    
    if ((TWSR & 0xF8) != TW_REP_START){ //Check value of TWI Status Register. Mask prescaler bits. If status different from START go to ERROR
        ERROR('G');
        return false;
    }
    
    return true;
    
}

bool TWI_SLA(int8_t address, bool write){
    
    if (write) {
        TWDR = address<<1; //Load SLA_W into TWDR Register.
    }
    else {
        TWDR = ((address<<1) | 1);//~((~address)<<1); //Load SLA_R into TWDR Register.
    }
    
    TWCR = (1<<TWINT) | (1<<TWEN); // Clear TWINT bit in TWCR to start transmission of address
    
    while (!(TWCR & (1<<TWINT))); //Wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
    
    if (write) {
        if ((TWSR & 0xF8) != TW_MT_SLA_ACK){ //Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
            ERROR('B');
            return false;
        }
    }
    else {
        if ((TWSR & 0xF8) != TW_MR_SLA_ACK){ //Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
            ERROR('B');
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
    
    //data = TWDR;
    if (last_byte) {
        TWCR = (1<<TWINT) | (1<<TWEN); //Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
    }
    else {
        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); //Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
    }
    
    while (!(TWCR & (1<<TWINT))); //Wait for TWINT Flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received.

//    if ((TWSR & 0xF8) != TW_MR_SLA_ACK){ // Check value of TWI Status Register. Mask prescaler bits. If status different from MT_DATA_ACK go to ERROR
//        ERROR('l');
//
//    }

    
    if (last_byte) {
        if(TW_STATUS == TW_MR_DATA_NACK){
            data = TWDR;
            ERROR('N');
        }

    }
    else {
        if(TW_STATUS == TW_MR_DATA_ACK){
            ERROR('M');
            data = TWDR;
        }

    }
    
    
//
//    TWCR = (1<<TWINT) | (1<<TWEN); //Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
//    while (!(TWCR & (1<<TWINT)));
//

    return data;
}

bool TWI_write(int8_t DATA){
    TWDR = DATA;
    TWCR = (1<<TWINT) | (1<<TWEN); //Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
    
    while (!(TWCR & (1<<TWINT))); //Wait for TWINT Flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received.
    
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK){ // Check value of TWI Status Register. Mask prescaler bits. If status different from MT_DATA_ACK go to ERROR
        ERROR('C');
        return false;
    }
    return true;
}



void TWI_endTransmission(){
    
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //Transmit STOP condition
    
}



void Init_adc(){
     // this tells the device with the given address to get ready for the subsequent bytes.
    if (TWI_beginTransmission(ADS1115, true)) {
        successful('1');
    }
    if (TWI_write(0x01)) {
        successful('2');
    }
    
    
//    if (TWI_repeated_start()) {
//        successful('a');
//    }
//    if (TWI_SLA(ADS1115, true)) {
//        successful('b');
//    }
    
    
//    if (TWI_beginTransmission(ADS1115, 1)) {
//        successful('3');
//    }
    if (TWI_write(0x8a)) {
        successful('3');
    }
    
    
//    if (TWI_repeated_start()) {
//        successful('c');
//    }
//    if (TWI_SLA(ADS1115, true)) {
//        successful('d');
//    }
    
    
//    if (TWI_beginTransmission(ADS1115, 1)) {
//        successful('5');
//    }
    if (TWI_write(0x83)) {
        successful('4');
    }
    TWI_endTransmission();
    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
    
    
    if (TWI_beginTransmission(ADS1115, true)) {
        //successful('5');
    }
    if (TWI_write(0x00)) {
        //successful('6');
    }
    
    
    //    TWI_repeated_start();
    //    TWI_SLA(ADS1115, false);
    TWI_endTransmission();
    

}

void read_and_transmit_ADC_data(){
    uint8_t i = 0;
    int16_t measured_value = 0;
    float measured_value_f = 0;
    int16_t test_value_1 = 0;
    int8_t test_value_2 = 0;
    
    // This is this to point to the conversion register and put an inital value into the measured value.
//    if (TWI_beginTransmission(ADS1115, true)) {
//        //successful('5');
//    }
//    if (TWI_write(0x00)) {
//        //successful('6');
//    }
//
//
////    TWI_repeated_start();
////    TWI_SLA(ADS1115, false);
//    TWI_endTransmission();
    //_delay_ms(40);
    TWI_beginTransmission(ADS1115, false);
//    if (TWI_repeated_start()) {
//        successful('a');
//    }
//    if (TWI_SLA(ADS1115, false)) {
//        successful('b');
//    }

    
//    TWI_endTransmission();
//
//
//    // now that we are pointing to the conversion register we can read the data from it.
//    // the msb will be given first so shift it over 8 bits then use the or operator to add the lsb to measured value.
//     // When using request from it is important to use the begin and end transmission functions.
//    if (TWI_beginTransmission(ADS1115, 0)) {
//        //successful('7');
//    }
    
    test_value_1 = TWI_read(false) << 8;
    measured_value = test_value_1;//TWI_read(false) << 8; // This shifts the bits over 8 places to the left.

//    TWI_repeated_start();
//    TWI_SLA(ADS1115, false);
    
    
    
    test_value_2 = TWI_read(true);
    measured_value |= test_value_2;//TWI_read(true); // This masks bits 8 to 15 and adds the rest of the data to bits 0 through 7.
//    TWI_nack(TW_MR_SLA_NACK);
    //TWI_read(true);
    TWI_endTransmission();
    
    // account for bias if any
    //measured_value += BIAS;
    

    
    sprintf(str1, "%x", test_value_1); // converts the current count iteration to a string

    i = 0;
    while(str1[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(str1[i]); // send the current count to the bluetooth module
        i++;
    }
    USART_Transmit('\r'); // send the text to the bluetooth module


    sprintf(str1, "%x", test_value_2); // converts the current count iteration to a string

    i = 0;
    while(str1[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(str1[i]); // send the current count to the bluetooth module
        i++;
    }
    USART_Transmit('\r'); // send the text to the bluetooth module
    
    sprintf(str1, "Data: %d", measured_value); // converts the current count iteration to a string

    i = 0;
    while(str1[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(str1[i]); // send the current count to the bluetooth module
        i++;
    }
    USART_Transmit('\r'); // send the text to the bluetooth module
    
    
    
    measured_value_f = measured_value;
    measured_value_f *= MVPS;
    
    
    char *tmpSign = (measured_value_f < 0) ? "-" : "";
    float tmpVal = (measured_value_f < 0) ? -measured_value_f : measured_value_f;
    
    int tmpInt1 = tmpVal;                  // Get the integer (678).
    float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
    int tmpInt2 = trunc(tmpFrac * 10000);  // Turn into integer (123).
    
    // Print as parts, note that you need 0-padding for fractional bit.
    
    sprintf (str1, "adc_read = %s%d.%04d mV\n", tmpSign, tmpInt1, tmpInt2);
    
    
    
    
    
    //sprintf(str1, "Data: %f", measured_value_f); // converts the current count iteration to a string
    
    i = 0;
    while(str1[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(str1[i]); // send the current count to the bluetooth module
        i++;
    }
    USART_Transmit('\r'); // send the text to the bluetooth module

}

read_config_register(){
    
    uint8_t i = 0;
    int8_t test_value_1 = 0;
    int8_t test_value_2 = 0;
    
    TWI_beginTransmission(ADS1115, 1);
    TWI_write(0x01); // point to config register
    
    
    
    
    
    
    
    
    TWI_repeated_start();
    TWI_SLA(ADS1115, false);
    

    
    
    
    
    
    
    
    
    
//    TWI_endTransmission();
//
//
//    TWI_beginTransmission(ADS1115, 0);

    test_value_1 = TWI_read(false);
    
    test_value_2 = TWI_read(true);

    TWI_endTransmission();
    
    sprintf(str1, "%x", test_value_1); // converts the current count iteration to a string
    
    i = 0;
    while(str1[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(str1[i]); // send the current count to the bluetooth module
        i++;
    }
    USART_Transmit('\r'); // send the text to the bluetooth module
    
    
    sprintf(str1, "%x", test_value_2); // converts the current count iteration to a string
    
    i = 0;
    while(str1[i] != '\0'){ // loop till the null character is reached.
        USART_Transmit(str1[i]); // send the current count to the bluetooth module
        i++;
    }
    USART_Transmit('\r'); // send the text to the bluetooth module
    
    
    
    
    

    
    
    
}


















int main(void)
{
    USART_Init(BAUD_RATE_REGISTER); // initialize the baudrate of the uart
    
    Init_adc();
    
    //read_config_register();
    
    
    DDRD = 1 << 4;           /* make the LED pin an output */
    PORTD ^= 1 << 4;
    /* insert your hardware initialization here */
    
    while(1){
        
        //sprintf(str, "%d", count); // converts the current count iteration to a string
        
        /* insert your main loop code here */
        _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
        PORTD ^= 1 << 4;    /* toggle the LED */
        _delay_ms(250); // add a 1 second delay

        read_and_transmit_ADC_data();
        //read_config_register();
        
        
        //print_test();
        PORTD ^= 1 << 4; // flip the state of the 4th pin of the D register. this turns an led on and off.
        
        //count++;
    }
    return 0;   /* never reached */
}
