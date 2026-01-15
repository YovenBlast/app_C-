#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

/* WiFi Credentials */
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

/* Firebase Credentials */
#define FIREBASE_HOST "your-project-id.firebaseio.com"
#define FIREBASE_AUTH "YOUR_DATABASE_SECRET"

/* Pins */
#define MQ4_PIN A0
#define LED_PIN D5

FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // WiFi Connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  // Firebase Initialization
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  int gasValue = analogRead(MQ4_PIN);
  Serial.print("Gas Level: ");
  Serial.println(gasValue);

  // Threshold value (adjust after calibration)
  if (gasValue > 400) {
    digitalWrite(LED_PIN, HIGH);

    Firebase.setInt(firebaseData, "/ButaneSensor/GasValue", gasValue);
    Firebase.setString(firebaseData, "/ButaneSensor/Status", "Gas Detected");
  } else {
    digitalWrite(LED_PIN, LOW);

    Firebase.setInt(firebaseData, "/ButaneSensor/GasValue", gasValue);
    Firebase.setString(firebaseData, "/ButaneSensor/Status", "Normal");
  }

  delay(2000);
}
