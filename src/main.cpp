#include "Audio.h"
#include "secrets.h"
#include "myencoder.h"

#define I2S_DOUT     27
#define I2S_BCLK     26
#define I2S_LRC      25



Audio audio;
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
  encoder.begin();
  encoder.setPos(5);
  encoder.setLimits(0, 20);
  encoder.setLev(0);
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(encoder.readPos());
  if (audio.connecttohost("http://stream.lokalradio.nrw/4459m27")) {
    Serial.println("Radio Kiepenkerl");
  }
}

void loop() {
  audio.loop();
  encoder.loop();
  if (encoder.newPos()) {
    audio.setVolume(encoder.readPos());
  }
  if ( encoder.newLev() ) {
    switch (encoder.readLev()) {
    case 0:
      if (audio.connecttohost("http://stream.lokalradio.nrw/4459m27")) {
        Serial.println("Radio Kiepenkerl");
      }
      break;
    case 1:
      if (audio.connecttohost("http://streams.radio21.de/nrw/mp3-192/web")) {
        Serial.println("Radio 21");
      }
      break;
    default:
      break;
    }
  }

}