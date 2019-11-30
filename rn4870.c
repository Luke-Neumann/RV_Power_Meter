//
//  rn4870.c
//  two input adc test
//
//  Created by Luke Neumann on 10/5/19.
//

#include "rn4870.h"



bool enter_command_mode(char * address){
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[10] = "CMD> ";
    char cmd[5] = "$";
    uart_print_string(cmd);
    _delay_ms(50);
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    _delay_ms(50);  // wait for things to settle.
    
    //uart_print_string(address);
    
    
     //now verify command mode.

    while (count2<10) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            if(address[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            memset(address, 0, sizeof(address)); // clear the buffer
            return true;
        }
    }
    return false; // unable to match the input data.

    //return true;
}

bool exit_command_mode(char * address){
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[10] = "END\r";
    
    char cmd[3] = "-";
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string("\r");
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    
   // uart_print_string(address);
    
    while (count2<10) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            if(address[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            memset(address, 0, sizeof(address)); // clear the buffer
            return true;
        }
    }
    return false; // unable to match the input data.
    //return true;
    
    
}

void get_BLE_info(){
    char cmd[5] = "D\r";
    uart_print_string(cmd);
    _delay_ms(10);  /* max is 262.14 ms / F_CPU in MHz */
}

void reboot_device(){
    char cmd[10] = "R,1\r";
    uart_print_string(cmd);
    _delay_ms(200);  /* max is 262.14 ms / F_CPU in MHz */
}



void get_signal_strength(char * address, char * type){ // the signal strength in dBm.
    
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    //char expected[10] = "\r";
    char error[10] = "Err\r";
    
    
    char cmd[10] = "M\r";
    uart_print_string(cmd);
    _delay_ms(10);  // wait for the return signal.
    
    count = 0;
    count1 = 0;
    _delay_ms(10);
    while(count<strlen(error)){ // check if there was an error
        if(address[count]==error[count]){ // compare what is inside of the recieved character to the expected character
            ++count1;
        }
        ++count;
    }
    count = 0;
    if (count1 == strlen(error)) { // check if there was an error
        memset(type, 0, sizeof(type)); // clear the buffer
        memset(address, 0, sizeof(address)); // clear the buffer
    }
    else{
        memset(type, 0, sizeof(type)); // clear the buffer
        while(count < 3){ // copy the signal strength.
            type[count] = address[count];
//            type++;
//            address++;
            count++;
        }
        memset(address, 0, sizeof(address)); // clear the buffer
    }

    
}

//char * get_signal_strength(){ // the signal strength in dBm.
//    char cmd[10] = "M\r";
//    return cmd;
//    //_delay_ms(10);  /* max is 262.14 ms / F_CPU in MHz */
//}

// connects to a BLE device with the given address and address type
void connect_to_device(char * address, char * type){
    char beginning[5] = "C,"; // This is the connect command
    char middle[5] = ","; // This is the connect command
    char ending[5] = "\r";
    char total[40];
    // build the string using memcpy
    memcpy(total, beginning, strlen(beginning));
    memcpy(total + strlen(beginning), type, strlen(type));
    memcpy(total + strlen(beginning)+strlen(type), middle, strlen(middle));
    memcpy(total + strlen(beginning)+strlen(type)+strlen(middle), address, strlen(address));
    memcpy(total + strlen(beginning)+strlen(type)+strlen(middle)+strlen(address), ending, strlen(ending));
    uart_print_string(total);
    _delay_ms(1000);  /* max is 262.14 ms / F_CPU in MHz */

}
