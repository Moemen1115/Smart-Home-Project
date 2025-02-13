#include <ESP8266WiFi.h>

// Wi-Fi Credentials
// THis Will Change
#define WIFI_SSID "MOE"
#define WIFI_PASSWORD "j9m02822"

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20) { // Max 10 seconds timeout
    Serial.print(".");
    delay(500);
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Check internet connection
    if (checkInternetConnection()) {
      Serial.println("Internet is accessible!");
    } else {
      Serial.println("Connected to Wi-Fi but NO internet access.");
    }
  } else {
    Serial.println("\nFailed to connect to Wi-Fi.");
  }
}

void loop() {
  // Periodically check internet connection
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 10000) { // Check every 10 seconds
    lastCheck = millis();
    Serial.println("\nChecking internet connection...");
    if (checkInternetConnection()) {
      Serial.println("Internet is accessible!");
    } else {
      Serial.println("NO internet access!");
    }
  }
}

// Function to check internet access
bool checkInternetConnection() {
  WiFiClient client;
  if (client.connect("www.google.com", 80)) {
    client.stop();
    return true; // Successfully connected to Google
  }
  return false; // Connection failed
}
