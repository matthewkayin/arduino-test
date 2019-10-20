#include "serial.hpp"
#include <iostream>
#include <string>
#include <bitset>

std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789. ";

unsigned char as_byte(std::string bitstring);
std::string to_binary(int value, int segment_length);
int to_keynum(char value);
std::string* to_segments(std::string message);

int main(){

    std::string input = "";
    Serial arduino_out = Serial();
    arduino_out.open();
    while(input != "exit"){

        std::cout << "Enter your input: ";
        std::getline(std::cin, input);

        if(input == "exit"){

            break;
        }

        std::cout << "Outputing: ";

        std::string* segments = to_segments(input);
        char out_buffer[input.length()];
        for(unsigned int i = 0; i < input.length(); i++){

            std::cout << segments[i] << " ";
            out_buffer[i] = as_byte(segments[i]);
        }
        arduino_out.write(out_buffer, input.length());

        std::cout << std::endl;
    }

    arduino_out.close();

    return 0;
}

unsigned char as_byte(std::string bitstring){

    std::bitset<8> b(bitstring);
    return (b.to_ulong() & 0xFF);
}

std::string to_binary(int value, int bitlength){

    int count = 0;
    std::string bitstring = "";

    int quantity = value;
    int mod;
    while(quantity != 0){

        mod = quantity % 2;
        if(mod == 0){

            quantity = (quantity / 2);
            bitstring = "0" + bitstring;

        }else{

            quantity = ((quantity - 1) / 2);
            bitstring = "1" + bitstring;
        }

        count++;
    }

    if(count > bitlength){

        std::cout << "Warning! Bitstring is longer than allocated length!" << std::endl;

    }else if(count < bitlength){

        int extra = bitlength - count;
        for(int i = 0; i < extra; i++){

            bitstring = "0" + bitstring;
        }
    }

    return bitstring;
}

int to_keynum(char value){

    int index = -1;
    for(unsigned int i = 0; i < alphabet.length(); i++){

        if(alphabet[i] == value){

            index = i;
            break;
        }
    }

    if(index == -1){

        std::cout << "Error! Character requested was not in alphabet!" << std::endl;
    }

    return index;
}

std::string* to_segments(std::string message){

    std::string* segments = new std::string[message.length()];
    for(unsigned int i = 0; i < message.length(); i++){

        char current_char = message.at(i);
        int keynum = to_keynum(current_char);
        std::string bitstring = to_binary(keynum, 8);
        segments[i] = bitstring;
    }

    return segments;
}
