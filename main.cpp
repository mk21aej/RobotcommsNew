#include "mbed.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>

#define MSG_BUFF_LENGTH 32
#define BLINKING_RATE 10ms

UnbufferedSerial pc(USBTX, USBRX, 115200);  
char MSG[MSG_BUFF_LENGTH];  
char buf[32] = {0};  
int length;
DigitalOut led(LED1); 
DigitalOut led1(D2);  

//Function to generate random sensor data
int generate_sensor_data() {
    return rand() % 100;  
}

// Main() runs in its own thread in the OS
int main() {
    length = snprintf(MSG, MSG_BUFF_LENGTH, "\r\nProgram Starting\r\n");
    pc.write(MSG, length);

    while (true) {
        int sensor_data = generate_sensor_data();
        length = snprintf(MSG, MSG_BUFF_LENGTH, "{Status: %d}", sensor_data);
        pc.write(MSG, length);

        if (uint32_t num = pc.read(buf, sizeof(buf))) {
            if (strcmp(buf, "1") == 0) {
                // Toggle LED1
                led1 = !led1;
                length = snprintf(MSG, MSG_BUFF_LENGTH, "Received Command LED1\r\n");
                pc.write(MSG, length);
            } else if (strcmp(buf, "s") == 0) {
                // Command "s" sends the LED1 status in JSON format
                length = snprintf(MSG, MSG_BUFF_LENGTH, "{ \"status\": { \"LED1\": %d } }\r\n", led1.read());
                pc.write(MSG, length);
            }
        }
        // Toggle LED1 to indicate MCU is running
        led = !led;

        // Sleep for the defined blinking rate
        ThisThread::sleep_for(BLINKING_RATE);
    }
}
