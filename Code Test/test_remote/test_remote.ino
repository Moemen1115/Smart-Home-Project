#include <Arduino.h>
#include <IRremote.hpp> // Include the IRremote library

#define IR_RECEIVE_PIN 2 // Define the IR receiver pin
char reading = ' '; // Initialize with a space (empty)

void setup() {
    pinMode(13, OUTPUT);
//    digitalWrite(12, HIGH);
    Serial.begin(9600); // Start serial communication
    Serial.println(F("IR Receiver Initialized - Ready to receive signals"));

    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start IR receiver
}

void loop() {
    if (IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.protocol != UNKNOWN) {
            uint8_t command = IrReceiver.decodedIRData.command;

            // Map IR commands to corresponding numbers/arrows
            switch (command) {
                case 0x45: Serial.println(F("1")); reading = '1'; break;
                case 0x46: Serial.println(F("2")); reading = '2'; break;
                case 0x47: Serial.println(F("3")); reading = '3'; break;
                case 0x44: Serial.println(F("4")); reading = '4'; break;
                case 0x40: Serial.println(F("5")); reading = '5'; break;
                case 0x43: Serial.println(F("6")); reading = '6'; break;
                case 0x7: Serial.println(F("7")); reading = '7'; break;
                case 0x15: Serial.println(F("8")); reading = '8'; break;
                case 0x9: Serial.println(F("9")); reading = '9'; break;
                case 0x16: Serial.println(F("*")); reading = '*'; break;
                case 0xD: Serial.println(F("#")); reading = '#'; break;
                case 0x19: Serial.println(F("0")); reading = '0'; break;
                case 0x18: Serial.println(F("▲ UP")); reading = 'u'; break;
                case 0x52: Serial.println(F("▼ DOWN")); reading = 'd'; break;
                case 0x8: Serial.println(F("◄ LEFT")); reading = 'l'; break;
                case 0x5A: Serial.println(F("► RIGHT")); reading = 'r'; break;
                case 0x1C: Serial.println(F("OK")); reading = 'o'; break;
                default:
                    Serial.print(F("Unknown command: 0x"));
                    Serial.println(command, HEX);
//                    reading = ' '; // Reset if unknown
                    break;
            }

            Serial.print("Reading: ");
            Serial.println(reading);
        } else {
            Serial.println(F("Unknown Protocol or Noise"));
            reading = ' '; // Reset reading if noise
        }

        IrReceiver.resume(); // Resume receiving next signal
    }

    // LED Control
    if (reading == 'o') {
        digitalWrite(13, HIGH);
        Serial.println("LED ON");
    } else {
        digitalWrite(13, LOW);
    }
}
