#include "Audio.h"
#include "secrets.h"

#define I2S_DOUT     6
#define I2S_BCLK     5
#define I2S_LRC      4


Audio audio;
unsigned long old_millis = 0;
uint8_t station = 0;


void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(1500);
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(5);
  audio.connecttohost("http://stream.lokalradio.nrw/4459m27");
}

void loop() {
  audio.loop();
  if ( ( millis() - old_millis ) > 10000 ) {
    switch (station)    {
    case 0:
      audio.connecttohost("http://stream.lokalradio.nrw/4459m27");
      Serial.println("Radio Kiepenkerl");
      break;
    case 1:
      audio.connecttohost("http://streams.radio21.de/nrw/mp3-192/web");
      Serial.println("Radio 21");
      break;
    default:
      break;
    }
    station++;
    if ( station > 1 ) station = 1;
    old_millis = millis();
  }

}
