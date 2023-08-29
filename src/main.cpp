#include "Audio.h"
#include "secrets.h"

#define I2S_DOUT     27
#define I2S_BCLK     26
#define I2S_LRC      25



Audio audio;
unsigned long old_millis = 0;
uint8_t station = 0;


void setup() {
  Serial.begin(115200);
  Serial.print("WiFi ");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(1500);
  Serial.println("connected");
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(5);
  audio.connecttohost("http://stream.lokalradio.nrw/4459m27");
}

void loop() {
  audio.loop();
  if ( ( millis() - old_millis ) > 30000 ) {
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
    if ( station > 1 ) station = 0;
    old_millis = millis();
  }

}