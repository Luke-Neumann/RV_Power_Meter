//
//  rn4870.c
//  two input adc test
//
//  Created by Luke Neumann on 10/5/19.
//

#include "rn4870.h"

#include <string.h>
#include <avr/io.h>

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

    
    while (count2<500) { // check every 10 seconds
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
        count2++;
    }
    return false; // unable to match the input data.

    //return true;
}

bool exit_command_mode(char * address){
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[10] = "END\r\n";
    
    char cmd[3] = "-";
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string("\r");
    //_delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
    
   // uart_print_string(address);
    
    while (count2<500) {
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
        count2++;
    }
    return false; // unable to match the input data.
    //return true;
    
    
}

void exit_command_mode_robustly(char * received){
    int count = 0; // used for keeping track of which element of received we are on.
    int count1 = 0; // counts through the elements of expected.
    int count2 = 0; // counts how many times we have looped through the received data.
    int temp = 0;
    int16_t count3 = 0; // counts the matching characters between expected and received.
    bool finished = false;
    bool false_alarm = false;
    bool found_expected = false;
    char expected[10] = "END\r\n";
    char cmd[3] = "-";
    
    memset(received, 0, sizeof(received)); // clear the buffer
    //_delay_ms(50);
    while ((!finished)) {
        count2 = 0;
        //_delay_ms(50);
        uart_print_string(cmd);
        _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
        uart_print_string(cmd);
        _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
        uart_print_string(cmd);
        _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
        uart_print_string("\r");
        _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */
//        uart_print_string("abc\r");
//        uart_print_string(received);
        //uart_print_string("HEY2\r");
        uart_print_string(received);
         
        while ((count2<50) && (!finished)) {
            count = 0;
            count3 = 0;
            
            //            found_expected = false;
            //            false_alarm = false;
            
            while((count < strlen(received)) && (!finished)){ //} && !found_expected && !false_alarm){
                count1 = 0;
                
                if(received[(count+count1)]==expected[count1]){ // compare what is inside of the recieved character to the expected character

                    //                    uart_print_string("HEY3\r");
                    //                    USART_Transmit(received[count]);
                    //uart_print_string("HEY3\r");
                    //                    count3++; // because there was a match increment count3
                    //                    count1++; // increment because we need to check for the next match
                    //                    if ((count+count1)>strlen(received)) {
                    //                        uart_print_string("Fail\r");
                    //                    }
                    while (count1 < strlen(expected)) {
                        //                        USART_Transmit(received[count+count1]);
                        //uart_print_string("HEY4\r");
                        temp = count + count1;
                        USART_Transmit(received[temp]);
                        if (received[(count+count1)]==expected[count1]) { // check for a match
                            //uart_print_int((count+count1));
//                            uart_print_string("HEY0\r");
//                            uart_print_int(count1);
//                            uart_print_string("HEY1\r");
//                            uart_print_int(count);
//                            uart_print_string("HEY2\r");
//                            temp = count + count1;
//                            USART_Transmit(received[temp]);
                            //uart_print_string("HEY3\r");
                            
                            count3++;
                        }
                        //                        else{
                        //                            //false_alarm = true;
                        //                            count3 = 0;
                        //                            break;
                        //                        }
                        count1++;
                    }
//                    uart_print_int(count3);
//                    uart_print_int(strlen(expected));
                    if (count3 == strlen(expected)) {
                        memset(received, 0, sizeof(received)); // clear the buffer
                        uart_print_string("HEY3\r");
                        finished = true;
                        //break;
                    }
                    else{
                        count3 = 0;
                        //break;
                    }
                    //uart_print_string("\r");
                    //found_expected = true;
                }
                
                ++count;
            }
            //            if (count3 == strlen(expected)) {
            //                memset(received, 0, sizeof(received)); // clear the buffer
            //                finished = true;
            //                break;
            //            }
            //            else{
            //                count3 = 0;
            //            }
            _delay_ms(10);
            count2++;
        }
        //finished = false;
    }
    memset(received, 0, sizeof(received)); // clear the buffer
}

void get_BLE_info(){
    char cmd[5] = "D\r";
    uart_print_string(cmd);
    _delay_ms(10);  /* max is 262.14 ms / F_CPU in MHz */
}

bool reboot_device(char * received){
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[30] = "Rebooting\r\n%REBOOT%";
    char cmd[10] = "R,1\r";
    uart_print_string(cmd);
    _delay_ms(300);  /* max is 262.14 ms / F_CPU in MHz */
    while (count2<1000) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            if(received[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            memset(received, 0, sizeof(received)); // clear the buffer
            return true;
        }
        count2++;
    }
    
    return false; // unable to match the input data.
     //return true;
    
}

bool disconnect_device(char * received){
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[30] = "AOK\r\nCMD> %DISCONNECT%";
    char cmd[10] = "K,1\r";
    uart_print_string(cmd);
    _delay_ms(300);  /* max is 262.14 ms / F_CPU in MHz */
    while (count2<1000) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            if(received[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            memset(received, 0, sizeof(received)); // clear the buffer
            return true;
        }
        count2++;
    }
    
    return false; // unable to match the input data.
     //return true;
    
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


bool start_advertisement(char * address){ // the signal strength in dBm.
    
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    char expected[15] = "AOK\r";
    char error[10] = "Err\r";
    
    
    char cmd[10] = "A,0014\r";
    uart_print_string(cmd);
    _delay_ms(10);  // wait for the return signal.
    

    
    while(count3<50){
        _delay_ms(10);
        count = 0;
        count1 = 0;
        count2 = 0;
        while(count<strlen(expected)){ // check if there was an error
            if(address[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        count = 0;
//        while(count<strlen(error)){ // check if there was an error
//            if(address[count]==error[count]){ // compare what is inside of the recieved character to the expected character
//                ++count2;
//            }
//            ++count;
//        }
        if (count1 == strlen(expected)) { // check if there was an error
            memset(address, 0, sizeof(address)); // clear the buffer
            return true;
        }
//        if (count2 == strlen(error)) { // check if there was an error
//            memset(type, 0, sizeof(type)); // clear the buffer
//            memset(address, 0, sizeof(address)); // clear the buffer
//        }
        count3++;

    }
    
    return false;


    
}

bool stop_advertisement(char * address){
    
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    char expected[15] = "AOK\r";
    char error[10] = "Err\r";
    
    
    char cmd[10] = "Y\r";
    uart_print_string(cmd);
    _delay_ms(10);  // wait for the return signal.
    

    
    while(count3<50){
        _delay_ms(10);
        count = 0;
        count1 = 0;
        count2 = 0;
        while(count<strlen(expected)){ // check if there was an error
            if(address[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        count = 0;
//        while(count<strlen(error)){ // check if there was an error
//            if(address[count]==error[count]){ // compare what is inside of the recieved character to the expected character
//                ++count2;
//            }
//            ++count;
//        }
        if (count1 == strlen(expected)) { // check if there was an error
            memset(address, 0, sizeof(address)); // clear the buffer
            return true;
        }
//        if (count2 == strlen(error)) { // check if there was an error
//            memset(type, 0, sizeof(type)); // clear the buffer
//            memset(address, 0, sizeof(address)); // clear the buffer
//        }
        count3++;

    }
    
    return false;


    
}


// connects to a BLE device with the given address and address type
bool connect_to_device(char * address, char * type, char * received){
//    char beginning[5] = "C,"; // This is the connect command
//    char middle[5] = ","; // This is the connect command
//    char ending[5] = "\r";
//    char total[40];
//    // build the string using memcpy
//    memcpy(total, beginning, strlen(beginning));
//    memcpy(total + strlen(beginning), type, strlen(type));
//    memcpy(total + strlen(beginning)+strlen(type), middle, strlen(middle));
//    memcpy(total + strlen(beginning)+strlen(type)+strlen(middle), address, strlen(address));
//    memcpy(total + strlen(beginning)+strlen(type)+strlen(middle)+strlen(address), ending, strlen(ending));
//    uart_print_string(total);
//    _delay_ms(500);  /* max is 262.14 ms / F_CPU in MHz */
    
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    bool successful = false;
    int unsuccessful = 0;
    char expected[100] = "";
    char unexpected[100] = "";
    char exp0[30] = "Trying\r\n%CONNECT,";
    char exp1[5] = ",";
    char exp2[10] = "%%";
    char exp3[15] = "STREAM_OPEN%";
    char unexp0[30] = "DISCONNECT%Err\r";
    //char unexp1[5] = "Err\r";

    memcpy(expected, exp0, strlen(exp0));
    memcpy(expected + strlen(exp0), type, strlen(type));
    memcpy(expected + strlen(exp0)+strlen(type), exp1, strlen(exp1));
    memcpy(expected + strlen(exp0)+strlen(type)+strlen(exp1), address, strlen(address));
//    memcpy(expected + strlen(exp0)+strlen(type)+strlen(exp1)+strlen(address),exp2, strlen(exp2));
//    memcpy(unexpected,expected,strlen(expected));
//    memcpy(unexpected+strlen(expected),unexp0,strlen(unexp0));
//    memcpy(expected + strlen(exp0)+strlen(type)+strlen(exp1)+strlen(address)+strlen(exp2),exp3,strlen(exp3));

        //    uart_print_string(expected);
        //    uart_print_string("\r");
        //    uart_print_string(unexpected);


    char beginning[5] = "C,"; // This is the connect command
    char middle[5] = ","; // This is the connect command
    char ending[5] = "\r";
    char total[40]="";
            // build the string using memcpy
    memcpy(total, beginning, strlen(beginning));
    memcpy(total + strlen(beginning), type, strlen(type));
    memcpy(total + strlen(beginning)+strlen(type), middle, strlen(middle));
    memcpy(total + strlen(beginning)+strlen(type)+strlen(middle), address, strlen(address));
    memcpy(total + strlen(beginning)+strlen(type)+strlen(middle)+strlen(address), ending, strlen(ending));
    uart_print_string(total);
    //_delay_ms(500);  /* max is 262.14 ms / F_CPU in MHz */


    while (count2<60) {
        count = 0;
        count1 = 0;
        _delay_ms(100);
        while(count<strlen(expected)){
            if(received[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        count = 0;
        while(count<strlen(unexpected)){
            if(received[count]==unexpected[count]){ // compare what is inside of the recieved character to the expected character
                ++count3;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            memset(received, 0, sizeof(received)); // clear the buffer
            return true;
        }
        count2++;

    //            if (count3 == strlen(unexpected)) {
    //                memset(received, 0, sizeof(received)); // clear the buffer
    //                unsuccessful++;
    //                break;
    //            }

    }
    return false;
}

bool create_BLE_service(char * received, char * UUID){
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[10] = "AOK\r\n";
     
    char cmd[5] = "PS,";
    char endline[5] = "\r";
    char total[256] = "";
    
    
    memcpy(total, cmd, strlen(cmd));
    memcpy(total + strlen(cmd), UUID, strlen(UUID));
    memcpy(total + strlen(cmd) + strlen(UUID), endline, strlen(endline));

    uart_print_string(total);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */

     
    // uart_print_string(address);
     
    while (count2<50) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            if(received[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            memset(received, 0, sizeof(received)); // clear the buffer
            return true;
        }
        count2++;
    }
    return false; // unable to match the input data.
     //return true;
    
}

bool create_BLE_characteristic(char * received, char * UUID,char * property_bitmap, char * data_size){
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[10] = "AOK\r\n";
     
    char cmd[5] = "PC,";
    char cmd1[5] = ",";
    char cmd2[5] = ",";
    char endline[5] = "\r";
    char total[256] = "";
    
    
    memcpy(total, cmd, strlen(cmd));
    memcpy(total + strlen(cmd), UUID, strlen(UUID));
    memcpy(total + strlen(cmd) + strlen(UUID), cmd1, strlen(cmd1));
    memcpy(total + strlen(cmd) + strlen(UUID) + strlen(cmd1), property_bitmap, strlen(property_bitmap));
    memcpy(total + strlen(cmd) + strlen(UUID) + strlen(cmd1) + strlen(property_bitmap), cmd2, strlen(cmd2));
    memcpy(total + strlen(cmd) + strlen(UUID) + strlen(cmd1) + strlen(property_bitmap)+strlen(cmd2), data_size, strlen(data_size));
    memcpy(total + strlen(cmd) + strlen(UUID) + strlen(cmd1) + strlen(property_bitmap)+strlen(cmd2)+strlen(data_size), endline, strlen(endline));

    uart_print_string(total);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */

     
    // uart_print_string(address);
     
    while (count2<50) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            if(received[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            memset(received, 0, sizeof(received)); // clear the buffer
            return true;
        }
        count2++;
    }
    return false; // unable to match the input data.
     //return true;
    
}

bool clear_services_and_characteristics(char * received){
    //memset(received, 0, sizeof(received)); // clear the buffer
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[15] = "AOK\r\nCMD> ";
     
    char cmd[10] = "PZ\r";
    
    //_delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */

    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    

     
    // uart_print_string(address);
     
    while (count2<500) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            //uart_print_string(received);
            if(received[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            //uart_print_string("test\r\n");
            memset(received, 0, sizeof(received)); // clear the buffer
            return true;
            
        }
        count2++;
    }
    return false; // unable to match the input data.
     //return true;
    
}

void list_services_and_characteristics(){
    char cmd[10] = "LS\r";
    uart_print_string(cmd);
    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
}

bool set_default_services(char * received){
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[15] = "AOK\r\nCMD> ";
     
    char cmd[10] = "SS,F0\r";
    
    //_delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */

    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd);
    

     
    // uart_print_string(address);
     
    while (count2<1000) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            //uart_print_string(received);
            if(received[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            //uart_print_string("test\r\n");
            memset(received, 0, sizeof(received)); // clear the buffer
            return true;
        }
        count2++;
    }
    return false; // unable to match the input data.
     //return true;
}

void server_handel_read(){
    char cmd1[10] = "SHR,0072\r";
    char cmd2[10] = "SHR,0074\r";
    char cmd3[10] = "SHR,0076\r";
    char cmd4[10] = "SHR,0078\r";
    char cmd5[10] = "SHR,007A\r";
    char cmd6[10] = "SHR,007C\r";
    
    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd1);
    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd2);
    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd3);
    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd4);
    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd5);
    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(cmd6);
    _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
}

bool check_BLE_characteristic_value(char * received, char * handle){
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[10] = "N/A\r\n\r\nCMD> ";
    char unexpected[10] = "N/A\r\n\r\nCMD> ";
    char cmd[5] = "SHR,";

    char endline[5] = "\r";
    char total[20] = "";
    
    
    memcpy(total, cmd, strlen(cmd));
    memcpy(total + strlen(cmd), handle, strlen(handle));
    memcpy(total + strlen(cmd) + strlen(handle), endline, strlen(endline));

    uart_print_string(total);
    _delay_ms(50);  /* max is 262.14 ms / F_CPU in MHz */

     
    // uart_print_string(address);
     
    while (count2<50) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            if(received[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            memset(received, 0, sizeof(received)); // clear the buffer
            return true;
        }
        count2++;
    }
    return false; // unable to match the input data.
     //return true;
    
    
    
}

void write_BLE_characteristic(){
    
}

bool set_device_name(char * received, char * name){
    //memset(received, 0, sizeof(received)); // clear the buffer
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[15] = "AOK\r\nCMD> ";
     
    char cmd[10] = "SN,";
    char end[5] = "\r";
    char total[30] = "";
    
    memcpy(total, cmd, strlen(cmd));
    memcpy(total + strlen(cmd), name, strlen(name));
    memcpy(total + strlen(cmd) + strlen(name), end, strlen(end));

    
    //_delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */

    //_delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
    uart_print_string(total);
    

     
    // uart_print_string(address);
     
    while (count2<500) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            //uart_print_string(received);
            if(received[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            //uart_print_string("test\r\n");
            memset(received, 0, sizeof(received)); // clear the buffer
            return true;
            
        }
        count2++;
    }
    return false; // unable to match the input data.
     //return true;
}

bool get_connection_status(char * received){
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    char expected[15] = "none\r\nCMD> ";
    char cmd[10] = "GK\r";
    
    uart_print_string(cmd);
    while (count2<500) {
        count = 0;
        count1 = 0;
        _delay_ms(10);
        while(count<strlen(expected)){
            if(received[count]==expected[count]){ // compare what is inside of the recieved character to the expected character
                ++count1;
            }
            ++count;
        }
        if (count1 == strlen(expected)) {
            memset(received, 0, sizeof(received)); // clear the buffer
            return true;
        }
        count2++;
    }
    return false; // unable to match the input data.
}




