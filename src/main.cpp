#include "secrets.h"
#include <WiFi.h> 
#include "myencoder.h"




MyEncoder encoder;


void setup() {
  Serial.begin(115200);
  Serial.print("WiFi ");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    Serial.print(".");
  }
  Serial.println(" connected");
  encoder.initLev(0,5,0,20);
  encoder.initLev(1,0,0,50);
  encoder.initLev(2,0,0,100);
  encoder.begin();
 }

void loop() {
  encoder.loop();
  if (encoder.newPos()) {
    Serial.print("Level: ");
    Serial.print(encoder.readLev());
    Serial.print(" Position: ");
    Serial.println(encoder.readPos());
  }
  if ( encoder.newLev() ) {
    Serial.print("Level geschaltet: ");
    Serial.print(encoder.readLev());
    Serial.print(" Position: ");
    Serial.println(encoder.readPos());
  }
}