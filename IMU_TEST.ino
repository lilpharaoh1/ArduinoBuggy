#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

float x, y, z;
float a, b, c;

float pose = 90;

//Networking
char ssid[] = "GROUPZ777";
char pass[] = "GROUPZ777";
WiFiServer server(907);
IPAddress ip;
int wifi_counter = 0;


void setup() {
  
if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

delay(1000);

WiFi.beginAP(ssid, pass);
ip = WiFi.localIP();
server.begin();

}

void loop() {

  if (wifi_counter >= 200) {
    WiFiClient client = server.available();
    if (client.connected()) { 
      int data = client.read();
      Serial.println(data); // Check what input is
      if (data != null) {
        client.println(pose);
      }
    }
  }
  else {
    wifi_counter++;
  }
  
  IMU.readGyroscope(x, y, z);
  input_x = x;
  
  pose += input_x
}
