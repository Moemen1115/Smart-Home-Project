#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Firebase Helper Libraries
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi Credentials
#define WIFI_SSID "MOE"
#define WIFI_PASSWORD "j9m02822"

// Firebase Credentials
#define API_KEY "AIzaSyCtz1TbWDvtTgIe5kI-LQvCUV6XUsmJ06g"
#define DATABASE_URL "https://smarthome-6c527-default-rtdb.firebaseio.com/"  // Ensure it ends with "/"

// Firebase Objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variables
unsigned long readDataPrevMillis = 0;
bool firebaseConnected = false;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Connect to Firebase
  connectToFirebase();
}

void loop() {
  // Read data from Firebase every 5 seconds
  if (Firebase.ready() && firebaseConnected && millis() - readDataPrevMillis > 5000) {
    readDataPrevMillis = millis();
    readDataFromFirebase();
  }
}

// Function to connect to Firebase
void connectToFirebase() {
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

// Function to read data from Firebase
void readDataFromFirebase() {
  int ldrValue;
  float voltage;

  if (Firebase.RTDB.getInt(&fbdo, "Sensor/ldr_data")) {
    ldrValue = fbdo.intData();
    Serial.print("LDR Data: ");
    Serial.println(ldrValue);
  } else {
    Serial.println("Failed to read LDR Data: " + fbdo.errorReason());
  }

  if (Firebase.RTDB.getFloat(&fbdo, "Sensor/voltage")) {
    voltage = fbdo.floatData();
    Serial.print("Voltage: ");
    Serial.println(voltage);
  } else {
    Serial.println("Failed to read Voltage: " + fbdo.errorReason());
  }
}
