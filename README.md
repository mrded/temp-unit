# temp-unit
Temperature and humidity reader unit on ESP8266 + DHT11

All together looks like this:

![94e9aa43e01dab39](https://user-images.githubusercontent.com/347098/83553224-cf9e5800-a502-11ea-8d09-b187b2867581.jpg)

It sends temperature and humidity readings over HTTP, so it can be collected and shown on a dashboard.

![Screenshot 2020-06-02 at 18 58 58](https://user-images.githubusercontent.com/347098/83553478-2a37b400-a503-11ea-8d5a-b22a5354f86d.png)

## What to buy:
### ESP8266 ESP-01 or ESP-01S
![ESP8266 ESP-01 or ESP-01S](https://user-images.githubusercontent.com/347098/83556426-c663ba00-a507-11ea-9a6f-1abed942dff8.jpg)

### DHT11 Temperature and humidity sensor
![DHT11](https://user-images.githubusercontent.com/347098/83556502-ef844a80-a507-11ea-8f1c-3e142b1d71a6.png)

### ESP8266 Programmer CH340G
![ESP8266 Programmer CH340G](https://user-images.githubusercontent.com/347098/83556336-99afa280-a507-11ea-8cce-bc25e2ddb423.jpg)

The programmer needs a jumper from `GPIO0` to `GND`. The jumper puts `ESP8266` to a program mode.

### 3.3v power
![3.3v power](https://user-images.githubusercontent.com/347098/83557074-e9429e00-a508-11ea-91c2-62bfef342909.jpg)

Everything can be found on aliexpress.

## How to install:
- [Install ESP8266 Boards](https://github.com/esp8266/Arduino#installing-with-boards-manager)
- [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)

Plug everything in, choose a ESP8266 board, connected port, compile and upload! 
