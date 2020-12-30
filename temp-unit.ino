#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "your-ssid";
const char* password = "your-wifi-password";
#include <DS18B20.h>


DS18B20 ds(2);

ESP8266WebServer server(80);

float temperature = 0;

String macAddress;

void setup(void) {
  Serial.begin(115200);

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
  float newValue = ds.getTempC();

  return (!isnan(newValue) && newValue > 0) 
    ? newValue : temperature;
}

void handleMetrics() {
  String metrics;

  temperature = getTemperature();

  String labels = "{device_mac_address=\"" + macAddress + "\"}";
 
  metrics += "ds18b20_temperature_celsius_raw_value" + labels + " " + String(temperature) + "\n";

  server.send(200, "text/plain", metrics);
}
