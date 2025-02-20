#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  // SDA pin
#define RST_PIN 9  // Reset pin

MFRC522 rfid(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
    Serial.begin(9600);
    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522 module
    Serial.println("RFID Scanner Ready");
}

void loop() {
    if (!rfid.PICC_IsNewCardPresent()) {
        return;
    }
    if (!rfid.PICC_ReadCardSerial()) {
        return;
    }
    Serial.print("Card UID: ");
    String uidString = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
//        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
//        Serial.print(rfid.uid.uidByte[i], HEX);
        uidString += String(rfid.uid.uidByte[i], HEX);
        if (i < rfid.uid.size - 1) {
            uidString += " ";
        }
    }
//    Serial.println();
    
    if (uidString.equalsIgnoreCase("33 FC EB F6"))
    {
        Serial.println("Welcome in your Home!");
    }
    else {
        Serial.println("You are Burglar");
    }
    
    rfid.PICC_HaltA(); // Stop reading
}
