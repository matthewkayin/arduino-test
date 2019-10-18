#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>

std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789. ";

unsigned char as_byte(std::string bitstring);
std::string to_binary(int value, int segment_length);
int to_keynum(char value);
std::string* to_segments(std::string message);

int main(){

    std::string base_location_name = "/dev/ttyACM";

    #ifdef _WIN32
        base_location_name = "COM";
    #endif

    std::string arduino_location = "";
    int attempts = 0;

    while(arduino_location == "" && attempts < 5){

        attempts++;
        std::vector<std::string> possible_names;
        for(int i = 0; i < 256; i++){

            std::string check_name = base_location_name + std::to_string(i);
            std::ifstream arduino_check(check_name.c_str());
            if(arduino_check.good()){

                arduino_location = check_name;
            }
        }
    }

    if(arduino_location == ""){

        std::cout << "Error, could not detect device!" << std::endl;
        return 0;
    }

    std::cout << "Device detected at " << arduino_location << std::endl;

    std::string input = "";
    system(("stty -F " + arduino_location + " -hupcl").c_str());
    while(input != "exit"){

        std::cout << "Enter your input: ";
        std::getline(std::cin, input);

        if(input == "exit"){

            break;
        }

        std::ofstream arduino_out;
        std::string* segments = to_segments(input);
        arduino_out.open(arduino_location);
        std::cout << "Outputing: ";

        for(unsigned int i = 0; i < input.length(); i++){

            std::cout << segments[i] << " ";
            arduino_out << as_byte(segments[i]);
        }

        arduino_out.close();

        std::cout << std::endl;
    }

    system(("stty -F " + arduino_location + " hupcl").c_str());

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
