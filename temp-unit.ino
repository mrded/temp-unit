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
  
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  
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

void handleMetrics() {
  String metrics;

  float temperature = dht.readTemperature(false);
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity) || temperature < 0 || humidity < 0) {
    server.send(500, "text/plain", "Problem with readings");
    return;
  }
  
  metrics += "dht_temperature_celsius_raw_value " + String(temperature) + "\n";
  metrics += "dht_humidity_percentage_raw_value " + String(humidity) + "\n";

  server.send(200, "text/plain", metrics);
}
