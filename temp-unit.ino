#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#define DHTTYPE DHT22 // DHT11 | DHT22
#define DHTPIN 2

const char* ssid = "your-ssid";
const char* password = "your-wifi-password";

DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80);

float temperature = 0;
float humidity = 0;

bool splitFlag = true;

String macAddress;

void setup(void) {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  WiFi.softAPdisconnect(true);

  macAddress = String(WiFi.macAddress());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("OK");
  
  Serial.print("MAC address: ");
  Serial.println(macAddress);
  
  Serial.print("Connected to: ");
  Serial.println(ssid);
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleMetrics);
  server.on("/metrics", handleMetrics);
  server.onNotFound(handleNotFound);
  server.begin();
  
  Serial.println("HTTP server is ready!");
}

void loop(void) {
  if (WiFi.status() != WL_CONNECTED) return;

  server.handleClient();
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

float getTemperature() {
  float newValue = dht.readTemperature(false);

  return (!isnan(newValue) && newValue > 0) 
    ? newValue : temperature;
}

float getHumidity() {
  float newValue = dht.readHumidity();

  return (!isnan(newValue) && newValue > 0) 
    ? newValue : humidity;
}

void handleMetrics() {
  String metrics;

  if (splitFlag) {
    temperature = getTemperature();
  }
  else {
    humidity = getHumidity();
  }

  splitFlag = !splitFlag;

  String labels = "{device_mac_address=\"" + macAddress + "\"}";
 
  metrics += "dht_temperature_celsius_raw_value" + labels + " " + String(temperature) + "\n";
  metrics += "dht_humidity_percentage_raw_value" + labels + " " + String(humidity) + "\n";

  server.send(200, "text/plain", metrics);
}
