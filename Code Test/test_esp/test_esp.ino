#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi Credentials
#define WIFI_SSID "MOE"
#define WIFI_PASSWORD "j9m02822"

// Firebase Credentials
#define API_KEY "AIzaSyCtz1TbWDvtTgIe5kI-LQvCUV6XUsmJ06g"
#define DATABASE_URL "https://smarthome-6c527-default-rtdb.firebaseio.com/"

// Hardware Pins
#define LED1_PIN 12
#define LED2_PIN 14
#define LDR_PIN 36

// Firebase Objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int ldrData = 0;
float voltage = 0.0;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected with IP: " + WiFi.localIP().toString());

  // Firebase Configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase signUp OK");
    signupOK = true;
  } else {
    Serial.printf("Firebase signUp FAILED: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Read LDR sensor
    ldrData = analogRead(LDR_PIN);
    voltage = ldrData * (3.3 / 4095.0);  // Convert to voltage

    // Store LDR Data
    if (Firebase.RTDB.setInt(&fbdo, "/Sensor/ldr_data", ldrData)) {
      Serial.println("LDR Data Saved: " + String(ldrData));
    } else {
      Serial.println("FAILED to Save LDR Data: " + fbdo.errorReason());
    }

    // Store Voltage
    if (Firebase.RTDB.setFloat(&fbdo, "/Sensor/voltage", voltage)) {
      Serial.println("Voltage Saved: " + String(voltage));
    } else {
      Serial.println("FAILED to Save Voltage: " + fbdo.errorReason());
    }
  }
}
