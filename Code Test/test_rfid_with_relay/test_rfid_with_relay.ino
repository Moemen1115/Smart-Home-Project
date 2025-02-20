#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

#define in1 8 

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 to 0x3F if needed

void setup() {
  Serial.begin(9600);
  SPI.begin(); 
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Scan your card...");

  pinMode(8 , OUTPUT);

  pinMode(7 , OUTPUT);
  digitalWrite(7, 1 );
  
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String tagID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagID += String(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.print("RFID Tag: ");
  Serial.println(tagID);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID: ");
  lcd.setCursor(4, 0);
  lcd.print(tagID);

  delay(2000); // Show for 2 seconds
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your card...");

  Serial.println(tagID);
if (tagID == "33fcebf6" ) {
      digitalWrite(8, 1 );
      Serial.println("on");
      delay(1000);
   }
   else {
   digitalWrite(8, 0 );
   Serial.println("off");
   }
}
