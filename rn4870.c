//
//  rn4870.c
//  two input adc test
//
//  Created by Luke Neumann on 10/5/19.
//

#include "rn4870.h"



bool enter_command_mode(char * address){
    int count = 0;
    char expected[10] = "CMD> ";
    char cmd[5] = "$";
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    _delay_ms(50);  // wait for things to settle.
//    while(!((expected[0]==address[strlen(address)])&&
//           (expected[1]==address[strlen(address)-1])&&
//           (expected[2]==address[strlen(address)-2])&&
//           (expected[3]==address[strlen(address)-3]))){
//        if(count>100){
//            return false;
//        }
//        count++;
//    }
//    while(){
//        if (expected[0]==address[strlen(address)]) {
//            if (<#condition#>) {
//                if (<#condition#>) {
//                    <#statements#>
//                }
//            }
//        }
//        if(count>100){
//            return false;
//        }
//        count++;
//    }
    
    return true;
}

//bool verify_command_mode(char * address){
//    char command[10] = "CMD>";
//    char extract[10] = "";
//
//    While(0){
//        address;
//    }
//
//
//}

void exit_command_mode(){
    
    char cmd[10] = "-";
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string("\r");
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    
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

void get_signal_strength(){ // the signal strength in dBm.
    
    char cmd[10] = "M\r";
    uart_print_string(cmd);
    //_delay_ms(10);  /* max is 262.14 ms / F_CPU in MHz */

}

void connect_to_device(char * address, char * type){ // the signal strength in dBm.
    
    char beginning[5] = "C,"; // This is the connect command
    char middle[5] = ","; // This is the connect command
    char ending[5] = "\r";
    char total[40];
    memcpy(total, beginning, strlen(beginning));
    memcpy(total + strlen(beginning), type, strlen(type));
    memcpy(total + strlen(beginning)+strlen(type), middle, strlen(middle));
    memcpy(total + strlen(beginning)+strlen(type)+strlen(middle), address, strlen(address));
    memcpy(total + strlen(beginning)+strlen(type)+strlen(middle)+strlen(address), ending, strlen(ending));
    uart_print_string(total);
    _delay_ms(10);  /* max is 262.14 ms / F_CPU in MHz */

}
