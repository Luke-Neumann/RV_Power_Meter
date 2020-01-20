//
//  update.c
//  two input adc test
//
//  Created by Luke Neumann on 1/8/20.
//

#include "update.h"




int hex_to_int(char c){
        int first = c / 16 - 3;
        int second = c % 16;
        int result = first*10 + second;
        if(result > 9) result--;
        return result;
}

int hex_to_ascii(char c, char d){
        int high = hex_to_int(c) * 16;
        int low = hex_to_int(d);
        return high+low;
}

void convert_hex_to_char(char * Hex_address){
    
    int length = strlen(Hex_address);
    char address[50] = "";
    int i;
    int temp;
    char value_holder[5];
    char buf = 0;

    for(i = 0; i < length; i++){
        if(i % 2 != 0){
            temp = hex_to_ascii(buf, Hex_address[i]);
            sprintf(value_holder, "%c", temp);
            strncat(address, value_holder, strlen(value_holder));
        }else{
            buf = Hex_address[i];
        }
    }
    memset(Hex_address, 0, strlen(Hex_address)); // clear the old address
    memcpy(Hex_address, address, strlen(address)); // copy the converted value to Hex address
    
}


bool set_server_address(char * server_address, char * received){
    int count = 0;
    int count1 = 0;
    char * address_holder1 = received;
    char * address_holder2 = server_address;
    bool result = false;
    char expected[15] = "N/A\r\n\r\nCMD> ";
    char cmd[10] = "SHR,0072\r";

    uart_print_string(cmd);
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
        result = false;
    }
    else {
        memset(server_address, 0, strlen(server_address)); // clear the old address
        _delay_ms(10);
        while ((*received != '\r')&&(*received != '\0')) {
            *server_address = *received;
            server_address++;
            received++;
        }
        received = address_holder1;
        server_address = address_holder2;
        convert_hex_to_char(server_address);

        result = true;
    }
    
    return result;
    
}

bool set_server_address_type(char * server_address_type, char * received){
    int count = 0;
    int count1 = 0;
    char * address_holder1 = received;
    char * address_holder2 = server_address_type;
    bool result = false;
    char expected[15] = "N/A\r\n\r\nCMD> ";
    char cmd[10] = "SHR,0074\r";
    
    _delay_ms(100);
    uart_print_string(cmd);
    _delay_ms(100);
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
        result = false;
    }
    else {
        memset(server_address_type, 0, strlen(server_address_type)); // clear the old address
        while ((*received != '\r')&&(*received != '\0')) {
            *server_address_type = *received;
            server_address_type++;
            received++;
        }
        received = address_holder1;
        server_address_type = address_holder2;
        convert_hex_to_char(server_address_type);

        result = true;
    }
    return result;
}


void set_password(char * password, char * received){
    char cmd[10] = "SHR,0078\r";
    _delay_ms(100);
    uart_print_string(cmd);
}

void set_sample_interval(int32_t * sample_interval, char * received){
    char cmd[10] = "SHR,007A\r";
    _delay_ms(100);
    uart_print_string(cmd);
}

void set_number_of_samples_per_interval(int32_t * sample_interval, char * received){
    char cmd[10] = "SHR,007C\r";
    _delay_ms(100);
    uart_print_string(cmd);
}

