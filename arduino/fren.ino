int led_green = 13;
int led_blue = 12;
int flash_time = 34;

void set_lights(bool green, bool blue){

    if(green){

        digitalWrite(led_green, HIGH);

    }else{

        digitalWrite(led_green, LOW);
    }

    if(blue){

        digitalWrite(led_blue, HIGH);

    }else{

        digitalWrite(led_blue, LOW);
    }
}

void flash_blue(){

    set_lights(false, true);
    delay(flash_time);
    set_lights(false, false);
    // delay(50);
}

void flash_green(){

    set_lights(true, false);
    delay(flash_time);
    set_lights(false, false);
    // delay(50);
}

void display_byte(unsigned char data){

    bool buffer[8];
    for(int i = 0; i < 8; i++){

        buffer[i] = (data >> i) & 1;
    }

    for(int i = 0; i < 8; i++){

        if(buffer[7 - i]){

            flash_blue();

        }else{

            flash_green();
        }
    }
}

void setup(){

    pinMode(led_green, OUTPUT);
    pinMode(led_blue, OUTPUT);
    Serial.begin(9600);

    set_lights(true, true);
    delay(600);
    set_lights(false, false);
}

void loop(){

    if(Serial.available() > 0){

        char buffer = Serial.read();
        display_byte(buffer);
    }
}
