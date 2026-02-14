/* ESP32 IoT Environmental Monitor (Beta)
Version 0.2
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

// ================= CONFIG =================
const char* ssid = "SSID_01";
const char* password = "PASSWORD_SSID1";
String serverURL = "http://192.168.1.105:5000/receive_sensor";

#define SAMPLE_INTERVAL 120000  
#define HTTP_TIMEOUT    5000    

#define DHTPIN 4              
#define DHTTYPE DHT11
#define MQ135_PIN 34          

// ==========================================

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastSendTime = 0;

// ================= WIFI CONNECT =================
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Failed!");
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  dht.begin();

  analogSetAttenuation(ADC_11db); 

  connectWiFi();
}

// ================= SEND DATA =================
void sendSensorData() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal baca DHT!");
    return;
  }

  int airRaw = analogRead(MQ135_PIN);
  float airVoltage = (airRaw / 4095.0) * 3.3;

  String jsonPayload = "{";
  jsonPayload += "\"temperature\":" + String(t, 2) + ",";
  jsonPayload += "\"humidity\":" + String(h, 2) + ",";
  jsonPayload += "\"air_quality\":" + String(airVoltage, 3);
  jsonPayload += "}";

  WiFiClient client;
  HTTPClient http;

  http.setTimeout(HTTP_TIMEOUT);
  http.begin(client, serverURL);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    Serial.println("HTTP Code: " + String(httpResponseCode));
    Serial.println("Response: " + http.getString());
  } else {
    Serial.println("HTTP Error: " + String(httpResponseCode));
  }

  http.end();
}

// ================= LOOP =================
void loop() {

  // Auto reconnect WiFi jika terputus
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Lost. Reconnecting...");
    connectWiFi();
  }

  unsigned long currentMillis = millis();

  if (currentMillis - lastSendTime >= SAMPLE_INTERVAL) {
    lastSendTime = currentMillis;
    sendSensorData();
  }
}
