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
#define DATABASE_URL "https://smarthome-6c527-default-rtdb.firebaseio.com/"  // Ensure URL ends with "/"

// Firebase Objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variables
unsigned long sendDataPrevMillis = 0;
bool firebaseConnected = false;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20) {  // Max 10 seconds timeout
    Serial.print(".");
    delay(500);
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Check Internet Connection
    if (checkInternetConnection()) {
      Serial.println("Internet is accessible!");
      
      // Connect to Firebase
      connectToFirebase();
    } else {
      Serial.println("Connected to Wi-Fi but NO internet access.");
    }
  } else {
    Serial.println("\nFailed to connect to Wi-Fi.");
  }
}

void loop() {
  // Check connection every 10 seconds
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 10000) {
    lastCheck = millis();
    Serial.println("\nChecking internet connection...");
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Wi-Fi disconnected! Reconnecting...");
      WiFi.reconnect();
    } else if (!checkInternetConnection()) {
      Serial.println("No internet access!");
    } else {
      Serial.println("Internet is accessible!");
    }
  }

  // Send data to Firebase every 5 seconds
  if (Firebase.ready() && firebaseConnected && millis() - sendDataPrevMillis > 5000) {
    sendDataPrevMillis = millis();
    sendDataToFirebase();
  }
}

// Function to check internet access
bool checkInternetConnection() {
  WiFiClient client;
  if (client.connect("www.google.com", 80)) {
    client.stop();
    return true;  // Successfully connected to Google
  }
  return false;  // Connection failed
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

// Function to send data to Firebase
void sendDataToFirebase() {
  int sensorValue = analogRead(A0);  // Read a sample sensor value (LDR or any analog sensor)

  if (Firebase.RTDB.setInt(&fbdo, "Sensor/ldr_data", sensorValue)) {
    Serial.print("LDR Data ");
    Serial.print(sensorValue);
    Serial.println(" - Successfully saved to Firebase!");
  } else {
    Serial.println("Firebase write FAILED: " + fbdo.errorReason());
  }
}
