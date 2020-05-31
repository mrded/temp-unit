#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 2

const char* ssid = "your-ssid";
const char* password = "your-wifi-password";

//@see: https://io.adafruit.com
const char* IO_KEY = "ADAFRUIT IO KEY";
const char* IO_USERNAME = "USERNAME";
const char* IO_TEMPERATURE_KEY = "temp";
const char* IO_HUMIDITY_KEY = "humidity";

DHT dht(DHTPIN, DHTTYPE, 11);

void setup(void) {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("DHT Weather Reading Unit");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(void) {
  if ((WiFi.status() == WL_CONNECTED)) {
    float temp_c = dht.readTemperature(false);
    float humidity = dht.readHumidity();   

    sendTemperature(temp_c);
    sendHumidity(humidity);
  }

  // 1 hour delay
  delay(60UL * 60UL * 1000UL);
}

void sendTemperature(float temp_c) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://io.adafruit.com/api/v2/" + String(IO_USERNAME) + "/feeds/" + String(IO_TEMPERATURE_KEY) +"/data");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-AIO-Key", IO_KEY);
  http.POST("{\"datum\":{\"value\":\"" + String((int)temp_c) + "\"}}");
  http.end();
}

void sendHumidity(float humidity) {
  WiFiClient client;
  HTTPClient http; 

  http.begin(client, "http://io.adafruit.com/api/v2/" + String(IO_USERNAME) + "/feeds/" + String(IO_HUMIDITY_KEY) + "/data");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-AIO-Key", IO_KEY);
  http.POST("{\"datum\":{\"value\":\"" + String((int)humidity) + "\"}}");
  http.end();
}
