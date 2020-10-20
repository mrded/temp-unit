#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#define DHTTYPE DHT22 // DHT11 | DHT22
#define DHTPIN 2

const char* ssid = "your-ssid";
const char* password = "your-wifi-password";

bool splitFlag = true;

DHT dht(DHTPIN, DHTTYPE);

void setup(void) {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  WiFi.softAPdisconnect(true);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("OK");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(void) {
  if (WiFi.status() != WL_CONNECTED) return;

  if (splitFlag) {
    float temperature = dht.readTemperature(false);
    sendData("temperature", temperature);
  }
  else {
    float humidity = dht.readHumidity();
    sendData("humidity", humidity);
  }

  splitFlag = !splitFlag;

  // 30 sec delay
  delay(30UL * 1000UL);
}

void sendData(String measurement, float value) {
  if (isnan(value) || value < 0) return;

  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://node-red.local/readings");
  http.addHeader("Content-Type", "application/json");

  http.addHeader("measurement", String(measurement));
  http.addHeader("value", String(value));
  http.addHeader("unit", String(WiFi.macAddress()));
  http.POST("");
  http.end();
}
