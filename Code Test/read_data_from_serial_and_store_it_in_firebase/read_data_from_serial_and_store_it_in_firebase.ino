#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Firebase Libraries
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi Credentials
#define WIFI_SSID "Jo#123"
#define WIFI_PASSWORD "98765432199"

// #define WIFI_SSID "MOE"
// #define WIFI_PASSWORD "j9m02822"

// Firebase Credentials
#define API_KEY "AIzaSyCtz1TbWDvtTgIe5kI-LQvCUV6XUsmJ06g"
#define DATABASE_URL "https://smarthome-6c527-default-rtdb.firebaseio.com/"

// Firebase Objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Pin Definitions
#define LED_PIN 2   // GPIO2 (D4 on ESP8266)
int sensorValue = 0;
unsigned long lastMillis = 0;
bool firebaseConnected = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWi-Fi Connected!");

  // Connect to Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase SignUp successful!");
    firebaseConnected = true;
  } else {
    Serial.println("Firebase SignUp failed: " + String(config.signer.signupError.message.c_str()));
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && firebaseConnected) {
    // **1. Read sensor data from Serial**
    if (Serial.available()) {
      sensorValue = Serial.parseInt();
      Serial.print("Read from Serial: ");
      Serial.println(sensorValue);

      // **2. Store sensor data in Firebase**
      if (Firebase.RTDB.setInt(&fbdo, "Sensor/reading", sensorValue)) {
        Serial.println("Sensor data sent to Firebase.");
      } else {
        Serial.println("Failed to send sensor data: " + fbdo.errorReason());
      }
    }

    // **3. Read LED state from Firebase**
    int ledState = 0;
    if (Firebase.RTDB.getInt(&fbdo, "Control/LED")) {
      ledState = fbdo.intData();
      Serial.print("LED State from Firebase: ");
      Serial.println(ledState);
      
      // **4. Control LED**
      digitalWrite(LED_PIN, ledState);
    } else {
      Serial.println("Failed to read LED state: " + fbdo.errorReason());
    }
  }
  
  delay(1000);
}
